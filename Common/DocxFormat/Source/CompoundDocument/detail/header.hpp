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

// header.hpp
#pragma once
#include "alloctable.hpp"

namespace POLE
{

template<typename _>
class HeaderT
{
public:
	static const unsigned char pole_magic[];

// Construction/destruction
public:
     HeaderT();

// Attributes
public:
	const unsigned char* id() const { return _id; }
	unsigned b_shift() const { return _b_shift; }
	unsigned s_shift() const { return _s_shift; }
	unsigned num_bat() const { return _num_bat; }
	unsigned dirent_start() const { return _dirent_start; }
	unsigned threshold() const { return _threshold; }
	unsigned sbat_start() const { return _sbat_start; }
	unsigned num_sbat() const { return _num_sbat; }
	unsigned mbat_start() const { return _mbat_start; }
	unsigned num_mbat() const { return _num_mbat; }
	const ULONG32* bb_blocks() const { return _bb_blocks; }
	bool valid() const;
	bool is_ole() const;
    
// Operations
public:
    bool load( const unsigned char* buffer, size_t len );
    bool save( unsigned char* buffer, size_t len );
#ifndef NDEBUG
    void debug() const;
#endif

// Implementation
private:
    unsigned char _id[8];     // signature, or magic identifier
    unsigned _b_shift;        // bbat->blockSize = 1 << b_shift [_uSectorShift]
    unsigned _s_shift;        // sbat->blockSize = 1 << s_shift [_uMiniSectorShift]
    unsigned _num_bat;        // blocks allocated for big bat   [_csectFat]
    unsigned _dirent_start;   // starting block for directory info  [_secDirStart]
    unsigned _threshold;      // switch from small to big file (usually 4K)  [_ulMiniSectorCutoff]
    unsigned _sbat_start;     // starting block index to store small bat  [_sectMiniFatStart]
    unsigned _num_sbat;       // blocks allocated for small bat  [_csectMiniFat]
    unsigned _mbat_start;     // starting block to store meta bat  [_sectDifStart]
    unsigned _num_mbat;       // blocks allocated for meta bat  [_csectDif]
    ULONG32 _bb_blocks[109];  //                                [_sectFat]
};

typedef HeaderT<void> Header;

// =========== HeaderT ==========

template<typename _>
const unsigned char HeaderT<_>::pole_magic[] = { 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };

template<typename _>
HeaderT<_>::HeaderT()
{
  _b_shift = 9;
  _s_shift = 6;
  _num_bat = 0;
  _dirent_start = 0;
  _threshold = 4096;
  _sbat_start = 0;
  _num_sbat = 0;
  _mbat_start = 0;
  _num_mbat = 0;

  unsigned i = 0;
  for(; i < 8; i++ )
    _id[i] = pole_magic[i];  
  for( i=0; i<109; i++ )
    _bb_blocks[i] = AllocTable::Avail;
}

template<typename _>
bool HeaderT<_>::valid() const
{
  if( _threshold != 4096 ) return false;
  if( _num_bat == 0 ) return false; //ok
  if( (_num_bat > 109) && (_num_bat > ((_num_mbat * 127) + 109))) return false; //ok
  if( (_num_bat < 109) && (_num_mbat != 0) ) return false; //ok
  if( _s_shift > _b_shift ) return false;
  if( _b_shift <= 6 ) return false;
  if( _b_shift >=31 ) return false;
  return true;
}

template<typename _>
bool HeaderT<_>::is_ole() const
{
  for( unsigned i=0; i<8; i++ )
    if( _id[i] != pole_magic[i] )
      return false;
  return true;
}

template<typename _>
bool HeaderT<_>::load( const unsigned char* buffer, size_t len )
{
  if (len < 0x4C+109 * 4 || !buffer)
	  return false;

  _b_shift     = readU16( buffer + 0x1e );
  _s_shift     = readU16( buffer + 0x20 );
  _num_bat      = readU32( buffer + 0x2c );
  _dirent_start = readU32( buffer + 0x30 );
  _threshold    = readU32( buffer + 0x38 );
  _sbat_start   = readU32( buffer + 0x3c );
  _num_sbat     = readU32( buffer + 0x40 );
  _mbat_start   = readU32( buffer + 0x44 );
  _num_mbat     = readU32( buffer + 0x48 );
  
  unsigned int i = 0;
  for( ; i < 8; i++ )
    _id[i] = buffer[i];  
  for( i=0; i<109; i++ )
    _bb_blocks[i] = readU32( buffer + 0x4C+i*4 );

  return true;
}

template<typename _>
bool HeaderT<_>::save( unsigned char* buffer, size_t len )
{
  if (len<0x4C+109*4 || !buffer)
	  return false;

  memset( buffer, 0, 0x4c );
  memcpy( buffer, pole_magic, 8 );        // ole signature
  writeU32( buffer + 8, 0 );              // unknown 
  writeU32( buffer + 12, 0 );             // unknown
  writeU32( buffer + 16, 0 );             // unknown
  writeU16( buffer + 24, 0x003e );        // revision ?
  writeU16( buffer + 26, 3 );             // version ?
  writeU16( buffer + 28, 0xfffe );        // unknown
  writeU16( buffer + 0x1e, _b_shift );
  writeU16( buffer + 0x20, _s_shift );
  writeU32( buffer + 0x2c, _num_bat );
  writeU32( buffer + 0x30, _dirent_start );
  writeU32( buffer + 0x38, _threshold );
  writeU32( buffer + 0x3c, _sbat_start );
  writeU32( buffer + 0x40, _num_sbat );
  writeU32( buffer + 0x44, _mbat_start );
  writeU32( buffer + 0x48, _num_mbat );
  
  for( unsigned i=0; i<109; i++ )
    writeU32( buffer + 0x4C+i*4, _bb_blocks[i] );

  return true;
}

#ifndef NDEBUG
template<typename _>
void HeaderT<_>::debug() const
{
  std::cout << std::endl;
  std::cout << "b_shift " << _b_shift << std::endl;
  std::cout << "s_shift " << _s_shift << std::endl;
  std::cout << "num_bat " << _num_bat << std::endl;
  std::cout << "dirent_start " << _dirent_start << std::endl;
  std::cout << "threshold " << _threshold << std::endl;
  std::cout << "sbat_start " << _sbat_start << std::endl;
  std::cout << "num_sbat " << _num_sbat << std::endl;
  std::cout << "mbat_start " << _mbat_start << std::endl;
  std::cout << "num_mbat " << _num_mbat << std::endl;
  
  unsigned s = (_num_bat<=109) ? _num_bat : 109;
  std::cout << "bat blocks: ";
  for( unsigned i = 0; i < s; i++ )
    std::cout << _bb_blocks[i] << " ";
  std::cout << std::endl;
}
#endif

}
