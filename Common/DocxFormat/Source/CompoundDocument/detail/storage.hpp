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

#include <fstream>
#include <list>
#include "header.hpp"
#include "dirtree.hpp"
#include <cstring>

#include "../../../../../DesktopEditor/common/File.h"

namespace POLE
{

template<typename _>
class StreamImplT;

template<typename _>
class StorageIOT
{
public:
	enum { Ok, OpenFailed, OpenSmallFatFailed, NotOLE, BadOLE, UnknownError, NewOLE, StupidWorkaroundForBrokenCompiler=255 };

// Construction/destruction  
public:
	StorageIOT( const char* filename, std::ios_base::openmode mode, bool create);
	StorageIOT( const std::wstring& filename, std::ios_base::openmode mode, bool create);
	StorageIOT( std::iostream* stream );
    ~StorageIOT();
    
// Attributes
public:
	int result() const { return _result; }
	const Header* header() const { return _header; }
	const DirEntry* entry(const std::string& path, bool create = false) const { return _dirtree->entry(path, create); }
	void fullName( const DirEntry* entry, std::string& name) const { _dirtree->fullName( entry->index(), name); }
	void current_path( std::string& result) const { _dirtree->current_path(result); }
	const DirEntry* root_entry() const { return _dirtree->root_entry(); }
	const DirEntry* current_entry() const { return _dirtree->current_entry(); }
	void listDirectory(std::list<std::string>&) const;
	ULONG32 small_block_size() const { return (_sbat) ? _sbat->block_size() : 0; }
	ULONG32 big_block_size() const { return (_bbat) ? _bbat->block_size() : 0; }
	void listEntries(std::vector<const DirEntry*>& result) const
	{
	  _dirtree->listDirectory(result);
	}
	void listAll(std::vector<const DirEntry*>& result) const
	{
	  _dirtree->listAll(result);
	}
	bool follow_small_block_table( ULONG32 start, std::vector<ULONG32>& chain ) const 
	{ 
		return (_sbat) ? _sbat->follow(start, chain) : false;
	}
	bool follow_big_block_table( ULONG32 start, std::vector<ULONG32>& chain ) const 
	{ 
		return (_bbat) ? _bbat->follow(start, chain) : false;
	}

	const std::vector<ULONG32>& sb_blocks() const { return _sb_blocks; }

	void get_entry_childrens(size_t index, std::vector<size_t> result) const
	{
        std::vector<ULONG32> result_ulong;
        result_ulong.assign (result.begin(), result.end());

        _dirtree->children(index, result_ulong);
	}

	void children( size_t index, std::vector<size_t>& result ) const
	{
		if (_dirtree)
        {
            std::vector<ULONG32> result_ulong;
            result_ulong.assign (result.begin(), result.end());

            _dirtree->children(index, result_ulong);
        }
	}

// Operations
public:
    bool create( const char* filename );
	bool enterDirectory( const std::string& directory ) { return _dirtree->enterDirectory( directory ); }
	void leaveDirectory() { _dirtree->leaveDirectory(); }
	std::streamsize loadSmallBlocks( const std::vector<ULONG32>& blocks, unsigned char* buffer, std::streamsize maxlen );
	std::streamsize loadBigBlocks( const std::vector<ULONG32>& blocks, unsigned char* buffer, std::streamsize maxlen );
    std::streamsize loadBigBlock(ULONG32 block, unsigned char* buffer, std::streamsize maxlen);
	std::streamsize saveBlock(ULONG32 block, const unsigned char* buffer, std::streamsize maxlen);
	bool delete_entry(const std::string& path);
	bool flush();
	void notify_dirtree_changed() { m_dtmodified = true; }
#ifndef NDEBUG
	void debug() const;
#endif


// Implementation
private:  
    void init();
    bool load();
    void close();

    std::iostream* _stream;
    std::fstream* _file;
	ULONG32 _size;   // size of the storage stream
    int _result;     // result of last operation
    std::vector<ULONG32> _sb_blocks; // blocks for "small" files
	
    Header* _header;           // storage header 
    DirTree* _dirtree;         // directory tree
    AllocTable* _bbat;         // allocation table for big blocks
    AllocTable* _sbat;         // allocation table for small blocks
	bool m_dtmodified;

	// no copy or assign
    StorageIOT( const StorageIOT<_>& );
    StorageIOT<_>& operator=( const StorageIOT<_>& );
};

typedef StorageIOT<void> StorageIO;

// =========== StorageIOT ==========

template<typename _>
StorageIOT<_>::StorageIOT( const char* filename, std::ios_base::openmode mode, bool create)
{
	m_dtmodified = false;
	init();

	// open the file, check for error
	_result = OpenFailed;
	mode |= std::ios_base::in; // we must always read
	if (create)
		mode |= std::ios_base::out | std::ios_base::trunc; // make sure the file will be created if needed
	else
		mode &= ~std::ios_base::trunc; // make sure the file won't be created
	std::fstream* file = new std::fstream( filename, std::ios::binary | mode);
	if( !file)
		return;
	if (file->fail())
	{
		delete file;
		return;
	}
	_file = file;
	_stream = file;
	if (!create)
		load();
}

template<typename _>
StorageIOT<_>::StorageIOT( const std::wstring& filename, std::ios_base::openmode mode, bool create)
{
#ifdef WIN32
	m_dtmodified = false;
	init();

	// open the file, check for error
	_result = OpenFailed;
	mode |= std::ios_base::in; // we must always read
	if (create)
		mode |= std::ios_base::out | std::ios_base::trunc; // make sure the file will be created if needed
	else
		mode &= ~std::ios_base::trunc; // make sure the file won't be created
	std::fstream* file = new std::fstream( filename.c_str(), std::ios::binary | mode);
	if( !file)
		return;
	if (file->fail())
	{
		delete file;
		return;
	}
	_file = file;
	_stream = file;
	if (!create)
		load();
#else
	m_dtmodified = false;
	init();

	// open the file, check for error
	_result = OpenFailed;
	mode |= std::ios_base::in; // we must always read
	if (create)
		mode |= std::ios_base::out | std::ios_base::trunc; // make sure the file will be created if needed
	else
		mode &= ~std::ios_base::trunc; // make sure the file won't be created

	BYTE* pData = NULL;
	LONG lDataLen = 0;
	NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename.c_str(), (LONG)filename.length(), pData, lDataLen);

	std::fstream* file = new std::fstream( (const char*)pData, std::ios::binary | mode);

	if (NULL != pData)
		delete [] pData;

	if( !file)
		return;
	if (file->fail())
	{
		delete file;
		return;
	}
	_file = file;
	_stream = file;
	if (!create)
		load();
#endif
}

template<typename _>
StorageIOT<_>::StorageIOT( std::iostream* stream )
{
	init();
	_result = OpenFailed;
	_stream = stream;
	load();
}

template<typename _>
StorageIOT<_>::~StorageIOT()
{
	flush();
	close();
	if (_sbat) delete _sbat;
	if (_bbat) delete _bbat;
	delete _dirtree;
	delete _header;
}

template<typename _>
void StorageIOT<_>::init()
{
	_result = NewOLE;
	_file = NULL;
	_stream = NULL;

	_header = new Header();
	_dirtree = new DirTree();
	_bbat = new AllocTable(1 << _header->b_shift());
	_sbat = new AllocTable(1 << _header->s_shift());

	_size = 0;
}

template<typename _>
bool StorageIOT<_>::load()
{
	if (!_stream) return false;

	// find size of input file
	_stream->seekg( 0, std::ios::end );
	_size = _stream->tellg();
	_stream->seekg( 0 ); 

	// load header
	unsigned char buf_header[512];
	_stream->read( (char*)buf_header, 512 );
	bool res = _header->load( buf_header, 512 );
	if (!res)
		return false;

	// check OLE magic id
	_result = NotOLE;
	if (!_header->is_ole())
		return false;

	// sanity checks
	_result = BadOLE;
	if (!_header->valid())
		return false;

	// important block size
	_bbat->set_block_size(1 << _header->b_shift());
	_sbat->set_block_size(1 << _header->s_shift());

	// find blocks allocated to store big bat
	// the first 109 blocks are in header, the rest in meta bat
	std::vector<ULONG32> blocks;
	blocks.resize( _header->num_bat() );
	for( unsigned i = 0; i < 109; i++ )
	{
		if( i >= _header->num_bat() ) 
			break;
		else 
			blocks[i] = _header->bb_blocks()[i];
	}
	if( (_header->num_bat() > 109) && (_header->num_mbat() > 0) )
	{
		unsigned char* buffer = new unsigned char[ _bbat->block_size() ];
		if (!buffer)
			return false;
		unsigned k = 109;
		for( unsigned r = 0; r < _header->num_mbat(); r++ )
		{
			size_t bytes = loadBigBlock(_header->mbat_start()+r+1, buffer, _bbat->block_size() );
			if (bytes != _bbat->block_size())
				break;
			
			for( unsigned s=0; s < _bbat->block_size(); s+=4 )
			{
				if( k >= _header->num_bat() ) 
					break;
				else  
					blocks[k++] = readU32( buffer + s );
			}  
		}    
		delete[] buffer;
	}

	// load big bat
	std::streamsize buflen = _bbat->block_size()*(std::streamsize)blocks.size();
	if( buflen > 0 )
	{
		unsigned char* buffer = new unsigned char[ buflen ]; 
		if (!buffer)
			return false;
		loadBigBlocks( blocks, buffer, buflen );
		bool res = _bbat->load( buffer, buflen );
		delete[] buffer;
		if (!res)
			return false;
	}  

	// load directory tree
	blocks.clear();
	if (!_bbat->follow( _header->dirent_start(), blocks ))
		return false;
	buflen = _bbat->block_size()*(std::streamsize)blocks.size();
	unsigned char* buffer = new unsigned char[ buflen ];  
	if (!buffer)
		return false;
	loadBigBlocks( blocks, buffer, buflen );
	if (!_dirtree->load( buffer, buflen ))
	{
		delete[] buffer;
		return false;
	}
	unsigned sb_start = readU32( buffer + 0x74 );
	delete[] buffer;
		
	_result = OpenSmallFatFailed;
	// fetch block chain as data for small-files
	if (!_bbat->follow( sb_start, _sb_blocks ))// small files
		return false;

	// load small bat
	blocks.clear();
	if (!_bbat->follow( _header->sbat_start(), blocks ))
		return false;
	buflen = _bbat->block_size()*(std::streamsize)blocks.size();
	if( buflen > 0 )
	{
		buffer = new unsigned char[ buflen ];  
		if (!buffer)
			return false;
		loadBigBlocks( blocks, buffer, buflen );
		bool res = _sbat->load( buffer, buflen );
		delete[] buffer;
		if (!res)
			return false;
	}  

// for troubleshooting, just enable this block
#if 0
	debug();
#endif

	// so far so good
	_result = Ok;
	return true;
}

template<typename _>
bool StorageIOT<_>::create( const char* filename )
{
  // std::cout << "Creating " << filename << std::endl; 
  
  std::fstream* file = new std::fstream(filename, std::ios::out|std::ios::binary);
  if( !file || file->fail() )
  {
    std::cerr << "Can't create " << filename << std::endl;
    _result = OpenFailed;
	if (file)
		delete file;
    return false;
  }
  
  // so far so good
  _result = Ok;
  _file = file;
  _stream = file;
  return true;
}

template<typename _>
void StorageIOT<_>::close()
{
	flush();
	if (_file)
	{
		_file->close();
		delete _file;
		_file = NULL;
	}
}

template<typename _>
std::streamsize StorageIOT<_>::loadBigBlocks( const std::vector<ULONG32>& blocks, unsigned char* data, std::streamsize maxlen )
{
  // sentinel
  if( !_stream ) return 0; 
  if( !data ) return 0;
  if( !_stream->good() ) return 0;
  if( maxlen == 0 ) return 0;
  size_t block_num = blocks.size();
  if( block_num < 1 ) return 0;

  // read block one by one, seems fast enough
  std::streamsize totalbytes = 0;
  for( ULONG32 i=0; (i < block_num ) && ( totalbytes < maxlen ); i++ )
  {
    ULONG32 block = blocks[i];
    std::streamsize p = (_bbat->block_size() < (unsigned)(maxlen-totalbytes)) ? _bbat->block_size() : maxlen-totalbytes;
	std::streamsize bytes = loadBigBlock(block+1, data+totalbytes, p );
    totalbytes += bytes;
  }

  return totalbytes;
}

template<typename _>
std::streamsize StorageIOT<_>::loadBigBlock( ULONG32 block, unsigned char* data, std::streamsize maxlen )
{
	assert(_stream);
	assert((unsigned)maxlen <= big_block_size());

	ULONG32 block_pos = block * _bbat->block_size();
	if (block_pos > _size)
		return 0;
	if (block_pos + maxlen > _size)
		maxlen = _size - block_pos;

	_stream->seekg( block_pos );
	_stream->read( (char*)data, (std::streamsize)maxlen );

	assert(!_stream->fail());
	return _stream->gcount();
}

// return number of bytes which has been read
template<typename _>
std::streamsize StorageIOT<_>::loadSmallBlocks( const std::vector<ULONG32>& blocks, unsigned char* data, std::streamsize maxlen )
{
  // sentinel
  if( !data ) return 0;
  if( !_stream || !_stream->good() ) return 0;
  if( maxlen == 0 ) return 0;
  size_t block_num = blocks.size();
  if( block_num < 1 ) return 0;

  // our own local buffer
  unsigned char* buf = new unsigned char[ _bbat->block_size() ];
  if (!buf) return 0;

  ULONG32 loaded_bblock = 0;
  bool bblock_loaded = false;

  // read small block one by one
  std::streamsize totalbytes = 0;
  for( ULONG32 i=0; ( i<block_num ) && ( totalbytes<maxlen ); i++ )
  {
    // find where the small-block exactly is
    ULONG32 pos = blocks[i] * _sbat->block_size();
    ULONG32 bbindex = pos / _bbat->block_size();
    if( bbindex >= _sb_blocks.size() ) break;

    ULONG32 bblock = _sb_blocks[ bbindex ] + 1;
	if (bblock != loaded_bblock || !bblock_loaded)
	{
		size_t read = loadBigBlock( bblock, buf, _bbat->block_size());
		if (read != _bbat->block_size())
			break;
		bblock_loaded = true;
		loaded_bblock = bblock;
	}

    // copy the data
    ULONG32 offset = pos % _bbat->block_size();
	std::streamsize p = _sbat->block_size();
	if (p > maxlen-totalbytes)
		p = maxlen-totalbytes;
	if ((unsigned)p > _bbat->block_size()-offset)
		p = _bbat->block_size()-offset;
    memcpy( data + totalbytes, buf + offset, p );
    totalbytes += p;
  }
  
  delete[] buf;
  return totalbytes;
}

// list all files and subdirs in current path
template<typename _>
void StorageIOT<_>::listDirectory(std::list<std::string>& result) const
{
  std::vector<const DirEntry*> entries;
  _dirtree->listDirectory(entries);
  result.resize(entries.size());
  for( unsigned i = 0; i < entries.size(); i++ )
    result.push_back( entries[i]->name() );
}

// Write a bigblock
template<typename _>
std::streamsize StorageIOT<_>::saveBlock(ULONG32 fisical_offset, const unsigned char* data, std::streamsize len)
{
	assert((unsigned)len <= big_block_size());

	_file->seekp(fisical_offset);
	_file->write((const char*)data, (std::streamsize)len);
	return len;
}

template<typename _>
bool StorageIOT<_>::delete_entry(const std::string& path)
{
	if (!_dirtree || !_dirtree->delete_entry(path))
		return false;

	m_dtmodified = true;

// todo: delete blocks

	return true;
}

template<typename _>
bool StorageIOT<_>::flush()
{
// for troubleshooting, just enable this block
#if 0
	debug();
#endif

	if (m_dtmodified && _bbat && _header)
	{
		std::vector<ULONG32> blocks;
		if (!_bbat->follow( _header->dirent_start(), blocks ))
			return false;
		size_t bufflen = blocks.size() * _bbat->block_size();
		unsigned char *buffer = new unsigned char[bufflen];
		if (!_dirtree->save(buffer, bufflen))
			return false;
		for (ULONG32 ndx = 0; ndx < blocks.size(); ++ndx)
		{
			ULONG32 fisical_offset = (blocks[ndx] * big_block_size()) + big_block_size();
			saveBlock(fisical_offset, buffer, big_block_size());
			buffer += big_block_size();
		}
		m_dtmodified = false;
	}

	return !m_dtmodified;
}

#ifndef NDEBUG
template<typename _>
void StorageIOT<_>::debug() const
{
	_header->debug();
	_sbat->debug();
	_bbat->debug();
	_dirtree->debug();
}
#endif

}
