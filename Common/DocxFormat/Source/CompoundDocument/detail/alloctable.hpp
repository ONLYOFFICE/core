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

// AllocTableT header
#pragma once

#include <iostream>
#include <vector>
#include "util.hpp"

namespace POLE
{

template<typename _>
class AllocTableT
{
public:
    static const ULONG32 Eof;
    static const ULONG32 Avail;
    static const ULONG32 Bat;    
    static const ULONG32 MetaBat;    

// Construction/destruction  
public:
    AllocTableT(ULONG32 block_size = 4096) { set_block_size(block_size); }

// Attributes
public:
	size_t count() const { return _data.size(); } // number of blocks
	ULONG32 block_size() const { return _block_size; } // block size
    ULONG32 operator[]( size_t index ) const { return _data[index]; }
    bool follow( ULONG32 start, std::vector<ULONG32>& chain ) const;

// Operations
public:
	void set_block_size(ULONG32 size) { _block_size = size; _data.clear(); resize( 128 ); }
    void set_chain( const std::vector<ULONG32>& chain );

    bool load( const unsigned char* buffer, size_t len );
    bool save( unsigned char* buffer, size_t len );
#ifndef NDEBUG
	void debug() const;
#endif

// Implementation
private:
	void resize( size_t newsize ) { _data.resize( newsize,  Avail); }
    size_t unused();
	void preserve( size_t n ) { unused(); }
    void set( size_t index, ULONG32 val );

    std::vector<ULONG32> _data;
    ULONG32 _block_size;
    
	AllocTableT( const AllocTableT& ); // No copy construction
    AllocTableT& operator=( const AllocTableT& ); // No copy operator
};

typedef AllocTableT<void> AllocTable;

// =========== AllocTableT Implementation ==========

template<typename _>
const ULONG32 AllocTableT<_>::Avail = 0xffffffff;
template<typename _>
const ULONG32 AllocTableT<_>::Eof = 0xfffffffe;
template<typename _>
const ULONG32 AllocTableT<_>::Bat = 0xfffffffd;
template<typename _>
const ULONG32 AllocTableT<_>::MetaBat = 0xfffffffc;


template<typename _>
void AllocTableT<_>::set( size_t index, ULONG32 value )
{
  if ( index >= count() )
	  resize( index + 1);
  _data[ index ] = value;
}

template<typename _>
void AllocTableT<_>::set_chain( const std::vector<ULONG32>& chain )
{
  size_t chain_size = chain.size();
  if ( chain_size )
  {
    for( size_t i = 0; i<chain_size-1; i++ )
      set( chain[i], chain[i+1] );
    set( chain[ chain_size-1 ], Eof );
  }
}

// follow 
template<typename _>
bool AllocTableT<_>::follow( ULONG32 start, std::vector<ULONG32>& chain ) const
{
  //assert(chain.size() == 0);

  size_t blocks = count();
  if( start >= blocks ) 
	  return false; 

  ULONG32 p = start;
  chain.reserve(blocks); 
  for (size_t loop_control = 0; p < blocks; ++loop_control)
  {
    if (loop_control >= blocks)
	  return false; 
    chain.push_back( p );
    p = _data[ p ];
  }

  return true;
}

template<typename _>
size_t AllocTableT<_>::unused()
{
  // find first available block
  size_t blocks = count();
  for( size_t i = 0; i < blocks; i++ )
    if( _data[i] == Avail )
      return i;
  
  // completely full, so enlarge the table
  resize( blocks + 10 );
  return count();      
}

template<typename _>
bool AllocTableT<_>::load( const unsigned char* buffer, size_t len )
{
  if (len%4 || !buffer)
    return false;

  resize( len / 4 );
  size_t blocks = count();
  for( size_t i = 0; i < blocks; i++ )
	_data[ i ] = readU32( buffer + i*4 );

  return true;
}

template<typename _>
bool AllocTableT<_>::save( unsigned char* buffer, size_t len )
{
  if (len < 4*count() || !buffer)
	  return false;

  size_t blocks = count();
  for( size_t i = 0; i < blocks; i++ )
    writeU32( buffer + i*4, _data[i] );

  return true;
}

#ifndef NDEBUG
template<typename _>
void AllocTableT<_>::debug() const
{
	std::cout << "block size " << _data.size() << std::endl;
	for( size_t i=0; i< _data.size(); i++ )
	{
		if( _data[i] == Avail ) 
			continue;
        std::cout << i << ": ";
        if( _data[i] == Eof ) 
			std::cout << "[eof]";
        else if( _data[i] == Bat ) 
			std::cout << "[bat]";
        else if( _data[i] == MetaBat ) 
			std::cout << "[metabat]";
        else std::cout << _data[i];
			std::cout << std::endl;
	}
}
#endif

} // namespace POLE
