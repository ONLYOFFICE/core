/* POLE - Portable C++ library to access OLE Storage 
   Copyright (C) 2005-2006 Jorge Lodos Vigil
   Copyright (C) 2002-2005 Ariya Hidayat <ariya@kde.org>

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted provided that the following conditions 
   are met:
   * Redistributions of source code must retain the above copyright notice, 
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution.
   * Neither the name of the authors nor the names of its contributors may be 
     used to endorse or promote products derived from this software without 
     specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
   THE POSSIBILITY OF SUCH DAMAGE.
*/

// storage header
#pragma once

#include "storage.hpp"

namespace POLE
{

template<typename _>
class StreamImplT
{
public:
	enum {Ok = 0, Eof = 1, Bad = 2, Corrupted = 3};
	static const std::string null_path;

// Construction/destruction  
public:
    StreamImplT( const StreamImplT<_>& );
	StreamImplT(StorageIO* io, const DirEntry* e): _entry(e), _io(io) { init(); }
	StreamImplT(StorageIO* io, const std::string& path): _entry(io->entry(path)), _io(io) { init(); }
    ~StreamImplT() { delete[] _cache_data; }

// Attributes
public:
	const std::string& path() const { return _entry ? _entry->name() : null_path; }
	std::streamsize size() const { return (_entry) ? _entry->size() : 0; }
    std::streampos tellg() const { return _gpos; }
    std::streampos tellp() const { return _ppos; }
	bool fail() const { return (_state & Bad) != 0; }
	bool eof() const { return (_state & Eof) != 0; }

// Operations
public:
	std::streampos seek(std::streampos pos, std::ios::seekdir origin, std::ios::openmode mode)
	{
		if (mode == std::ios::in)
		{
			seekg(pos, origin);
			return tellg();
		}
		if (mode == std::ios::out)
		{
			seekp(pos, origin);
			return tellp();
		}
		
		// Bad seek
		assert(false);
		return 0;
	}
	void seekg(std::streampos pos, std::ios_base::seekdir origin); 
	void seekp(std::streampos pos, std::ios_base::seekdir origin);

    int getch();
    std::streamsize read( unsigned char* data, std::streamsize maxlen );
	std::streamsize write(const unsigned char* data, std::streamsize maxlen);
	bool reserve(std::streamsize size);
	bool resize(std::streamsize size, char val);

// Implementation
private:
	void init();
	std::streamsize read( std::streampos pos, unsigned char* data, std::streamsize maxlen );
	void update_cache();

	StorageIO* _io; 
    const DirEntry* _entry; 
    std::vector<ULONG32> _blocks;
    std::streampos _gpos; // pointer for read
    std::streampos _ppos; // pointer for write

	// simple cache system to speed-up getch()
	unsigned char* _cache_data; 
    std::streamsize _cache_size;
    std::streampos _cache_pos;
	int _state;

    // no default, copy or assign
    StreamImplT( );
    StreamImplT<_>& operator=( const StreamImplT<_>& );
};

typedef StreamImplT<void> StreamImpl;

// =========== StreamImplT ==========

template<typename _>
const std::string StreamImplT<_>::null_path;

template<typename _>
StreamImplT<_>::StreamImplT( const StreamImplT<_>& stream)
{
	_state = stream._state; 
	_io = stream._io; 
    _entry = stream._entry; 
	_blocks = stream._blocks;
	_gpos = stream._gpos;
	_ppos = stream._ppos;

	_cache_size = stream._cache_size;
    _cache_pos = stream._cache_pos;
	_cache_data = new unsigned char[4096];
	for (std::streamsize i = 0; i<_cache_size; i++)
		_cache_data[i] = stream._cache_data[i];
}

template<typename _>
void StreamImplT<_>::init()
{
  _state = StreamImpl::Ok;
  _gpos = 0;
  _ppos = 0;
  // prepare cache
  _cache_pos = 0;
  _cache_size = 4096; // optimal ?
  _cache_data = new unsigned char[_cache_size];

  // sanity check
  if (!_entry) 
  {
	  _state = StreamImpl::Bad;
	  return;
  }

  if( _entry->size() >= _io->header()->threshold() ) 
  {
	  if (!_io->follow_big_block_table( _entry->start(), _blocks ))
		  _state = StreamImpl::Bad;
  }
  else
    if (!_io->follow_small_block_table( _entry->start(), _blocks ))
		_state = StreamImpl::Bad;

  //update_cache();
}

template<typename _>
void StreamImplT<_>::seekg( std::streampos pos, std::ios_base::seekdir origin)
{
	switch (origin)
	{
	case std::ios_base::beg:
		if ((unsigned)pos > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_gpos = _entry->size();
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_gpos = pos;
		break;
	case std::ios_base::cur:
		if ((unsigned)(_gpos + pos) > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_gpos = _entry->size();
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_gpos += pos;
		break;
	case std::ios_base::end:
		if ((unsigned)pos > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_gpos = 0;
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_gpos = _entry->size() - pos;
		break;
	}
}

template<typename _>
void StreamImplT<_>::seekp( std::streampos pos, std::ios_base::seekdir origin)
{
	switch (origin)
	{
	case std::ios_base::beg:
		if ((unsigned)pos > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_ppos = _entry->size();
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_ppos = pos;
		break;
	case std::ios_base::cur:
		if ((unsigned)(_ppos + pos) > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_ppos = _entry->size();
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_ppos += pos;
		break;
	case std::ios_base::end:
		if ((unsigned)pos > _entry->size())
		{
			_state |= StreamImpl::Eof;
			_ppos = 0;
			return;
		}
		else
		{
			_state &= ~StreamImpl::Eof;
		}
		_ppos = _entry->size() - pos;
		break;
	}
}

template<typename _>
int StreamImplT<_>::getch()
{
  // sanity check
  if (!_entry) 
	  return 0;

  // past end-of-file ?
  if( _gpos > static_cast<std::streamsize>(_entry->size()) ) 
	  return -1;

  // need to update cache ?
  if( !_cache_size || ( _gpos < _cache_pos ) ||
    ( (unsigned)_gpos >= (unsigned)_cache_pos + _cache_size ) )
      update_cache();

  // something bad if we don't get good cache
  if( !_cache_size ) 
	  return -1;

  int data = _cache_data[_gpos - _cache_pos];
  _gpos += 1;

  return data;
}

template<typename _>
std::streamsize StreamImplT<_>::read( std::streampos pos, unsigned char* data, std::streamsize maxlen )
{
  // sanity checks
  if (!_entry) 
	  return 0;
  if( !data ) 
	  return 0;
  if ((maxlen + (unsigned)pos) > _entry->size())
  {
	  maxlen = _entry->size() - pos;
	  _state |= StreamImpl::Eof;
  }
  else
  {
	  _state &= ~StreamImpl::Eof;
  }
  if( maxlen == 0 ) 
	  return 0;

  std::streamsize totalbytes = 0;
  std::streamsize max_block_num = (std::streamsize)_blocks.size(); 
  
  if ( _entry->size() < _io->header()->threshold() )
  {
    // small file
    std::streamsize index = pos / _io->small_block_size();
    if( index >= max_block_num ) 
		return 0;

    // To read small blocks we have to read at least a big block,
	// so read as many small blocks as fit in the big block every
	// time. If we don't do this the same big block may be loaded
	// several times.

	unsigned char* buf = new unsigned char[ _io->big_block_size() ];
	if (!buf)
		return 0;
	std::streamsize max_blocks_to_read = maxlen / _io->small_block_size();
	if (maxlen % _io->small_block_size())
		max_blocks_to_read++;
    std::streamsize small_blocks_in_big_blocks = _io->big_block_size() / _io->small_block_size();
	std::vector<ULONG32> blocks;
	blocks.reserve(small_blocks_in_big_blocks);

	// Read the small blocks that start not at the beginning of the
	// big block.
	std::streamsize read_blocks = 0;
	std::streamsize offset = pos % _io->small_block_size();
	std::streamsize unaligned_blocks = small_blocks_in_big_blocks - index % small_blocks_in_big_blocks;
	unaligned_blocks %= small_blocks_in_big_blocks;
	std::streamsize j = 0;
    if (unaligned_blocks)
	{
		for (; j < unaligned_blocks && j < max_block_num && j < max_blocks_to_read; ++j)
			if (index+j < max_block_num)
				blocks.push_back(_blocks[index+j]);
			else
				break;
		index += j;
		std::streamsize bytes = j * _io->small_block_size();
		std::streamsize read = _io->loadSmallBlocks( blocks, buf, bytes );
		if (read != bytes)
		{
			memcpy(data, buf + offset, read);
			delete[] buf;
			return totalbytes;
		}
		read_blocks += j;
		std::streamsize count = bytes - offset;
		if(count > maxlen) 
			count = maxlen;
		memcpy(data, buf + offset, count);
		totalbytes += count;
		if (totalbytes == maxlen)
		{
			delete[] buf;
			return totalbytes;
		}
		offset = 0;
	}

	// Read the remaining small blocks. These start at the beginning of a 
	// big block.
	size_t max_bblocks = max_blocks_to_read / small_blocks_in_big_blocks + 1;
    for (size_t i = 0; i<max_bblocks && totalbytes < maxlen; ++i)
	{
		j = 0;
		blocks.clear();
		for (; j < small_blocks_in_big_blocks && j < max_block_num && (j+read_blocks) <= max_blocks_to_read; ++j)
			if (index+j < max_block_num)
				blocks.push_back(_blocks[index+j]);
			else
				break;
		index += j;
		std::streamsize bytes = j * _io->small_block_size();
		std::streamsize read = _io->loadSmallBlocks( blocks, buf, bytes );
		read_blocks += j;
		std::streamsize count = read - offset;
		if(count > maxlen-totalbytes) 
			count = maxlen-totalbytes;
		memcpy(data+totalbytes, buf + offset, count);
		totalbytes += count;
		offset = 0;
		if (totalbytes == maxlen)
			break;
	}
	delete[] buf;
	return totalbytes;
  }

  // big file
  std::streamsize index = pos / _io->big_block_size();
    
  if( index >= max_block_num ) 
	return 0;
    
  unsigned char* buf = new unsigned char[ _io->big_block_size() ];
  size_t offset = pos % _io->big_block_size();
  for (; index < max_block_num && totalbytes < maxlen; ++index )
  {
    ULONG32 block = _blocks[index];
	size_t read = _io->loadBigBlock(block+1, buf, _io->big_block_size());
    if (read != _io->big_block_size())
 	  break;
    std::streamsize count = _io->big_block_size() - offset;
    if( count > maxlen-totalbytes ) count = maxlen-totalbytes;
    memcpy( data+totalbytes, buf + offset, count );
    totalbytes += count;
    offset = 0;
  }
    
  delete [] buf;
  return totalbytes;
}

template<typename _>
std::streamsize StreamImplT<_>::read( unsigned char* data, std::streamsize maxlen )
{
  std::streamsize bytes = read( tellg(), data, maxlen );
  _gpos += bytes;

  if ((unsigned)_gpos == _entry->size())
	  _state |= StreamImpl::Eof;

  return bytes;
}

template<typename _>
void StreamImplT<_>::update_cache()
{
  // sanity checks
  if (!_entry) 
	  return;
  if( !_cache_data ) 
	  return;

  _cache_pos = _gpos - ( _gpos % _cache_size );
  std::streamsize bytes = _cache_size;
  if( (unsigned)_cache_pos + bytes > _entry->size() ) 
	  bytes = _entry->size() - _cache_pos;
  _cache_size = read( _cache_pos, _cache_data, bytes );
}

/*
para escribir:
1. El entry de cada stream contiene el bloque en que comienza, hay que cargarlos todos en un vector utilizando el follow de la clase AllocTable.
2. Para conocer en que bloque del stream se debe comenzar a escribir, esto se hace dividiendo el offset con respecto al inicio del stream entre el tamaño de cada bloque.
3. Para calcular el offset dentro de ese bloque se toma el resto de la division anterior.
4. Para conocer el desplazamiento fisico donde hay que escribir en el fichero se suma el offset dentro del bloque y se suma con el offset del bloque en el que se va a escribir, este ltimo se calcula como el (bloque*512)+512.
5. Para saber la cantidad de bytes afectados en un bloque dado se resta el tamaño del bloque entre el offset dentro del bloque. Si es menos, entonces todo pertenece a ese bloque, si es mayor, entonces se escriben los bytes desde el desplazamiento inicial hasta el final del bloque y el resto pertenece al bloque adyacente.
6. Es necesario incrementar el buffer de datos en la cantidad de bytes copiados y restar de la cantidad total de bytes lo que se copio
7. Repetir la operacion con otro bloque si es necesario.
*/
template<typename _>
std::streamsize StreamImplT<_>::write(const unsigned char* data, std::streamsize maxlen)
{
	// Sanity checks
	if(! _entry) 
		return 0;
	if(!data) 
		return 0;
	if((maxlen + (unsigned)_ppos) > _entry->size())
	{
		maxlen = _entry->size() - _ppos;
		_state |= StreamImpl::Eof;
	}
	else
	{
		_state &= ~StreamImpl::Eof;
	}
	if(maxlen == 0) 
		return 0;

    size_t max_block_num = _blocks.size(); 

    // Amount of written byes
	std::streamsize written = 0;
	std::streamsize count = 0;
	std::streamsize data_len = maxlen;
	
	if (_entry->size() < _io->header()->threshold())
	{
		// small file
		size_t index = _ppos / _io->small_block_size();
	    if( index >= max_block_num ) 
			return 0;
		
		size_t offset = _ppos % _io->small_block_size();
		std::vector<ULONG32> _sbroot_entry = _io->sb_blocks();
		for (; index < max_block_num && count < maxlen; ++index)
		{
			// Take the minifat sector index
			ULONG32 minifat_index = _blocks[index];
			ULONG32 sbindex_offset = minifat_index % 8;
			// Calculate the the root entry's big block index
			ULONG32 position = minifat_index * _io->small_block_size();
			ULONG32 bbindex = position / _io->big_block_size();
			// Fisical offset inside the file
			ULONG32 bbindice = _sbroot_entry[bbindex];
			ULONG32 fisical_offset = (((bbindice * _io->big_block_size()) + _io->big_block_size()) + 
								     (sbindex_offset * _io->small_block_size())) + offset;

			// Amount of bytes that can actually be written
			std::streamsize canwrite = _io->small_block_size() - offset;
			if (canwrite > data_len )
				canwrite = data_len;

			written = _io->saveBlock(fisical_offset, data, canwrite);
			count += written;
			if (written < canwrite)
			{
				_state |= StreamImpl::Bad;
				return count;
			}
			data += written;
			data_len -= written;
			offset = 0;
		}
	}
	else
	{
		// big file
		// Ordinal of the first block for writing
		size_t index = _ppos / _io->big_block_size();
		if(index >= max_block_num) 
			return 0;
		
		// Offset inside this block
		size_t offset = _ppos % _io->big_block_size();

		for (; index < max_block_num && count < maxlen; ++index)
		{
			// Fisical offset inside the file
			ULONG32 fisical_offset = ((_blocks[index] * _io->big_block_size()) + _io->big_block_size() + offset);

			// Amount of bytes that can actually be written
			std::streamsize canwrite = _io->big_block_size() - offset;
			if (canwrite > data_len )
				canwrite = data_len;

			written = _io->saveBlock(fisical_offset, data, canwrite);
			count += written;
			if (written < canwrite)
			{
				_state |= StreamImpl::Bad;
				return count;
			}
			data += written;
			data_len -= written;
			offset = 0;
		}
	}
	return count;
}

template<typename _>
bool StreamImplT<_>::reserve(std::streamsize size)
{
// todo: implement
	return false;
}

template<typename _>
bool StreamImplT<_>::resize(std::streamsize size, char val)
{
// todo: implement
	return false;
}

}
