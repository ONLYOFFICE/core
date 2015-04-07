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

// util header
#pragma once

namespace POLE
{

typedef unsigned char ULONG8;
typedef unsigned short ULONG16;
typedef unsigned long  ULONG32;

inline ULONG32 readU32( const unsigned char* ptr )
{
  return ptr[0]+(ptr[1]<<8)+(ptr[2]<<16)+(ptr[3]<<24);
}

inline void writeU32( unsigned char* ptr, ULONG32 data )
{
  ptr[0] = (unsigned char)(data & 0xff);
  ptr[1] = (unsigned char)((data >> 8) & 0xff);
  ptr[2] = (unsigned char)((data >> 16) & 0xff);
  ptr[3] = (unsigned char)((data >> 24) & 0xff);
}

inline ULONG16 readU16( const unsigned char* ptr )
{
  return ptr[0]+(ptr[1]<<8);
}

inline void writeU16( unsigned char* ptr, ULONG16 data )
{
  ptr[0] = (unsigned char)(data & 0xff);
  ptr[1] = (unsigned char)((data >> 8) & 0xff);
}

}
