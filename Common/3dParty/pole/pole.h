/* POLE - Portable C++ library to access OLE Storage 
   Copyright (C) 2002-2005 Ariya Hidayat <ariya@kde.org>

   Performance optimization, API improvements: Dmitry Fedorov 
   Copyright 2009-2014 <www.bioimage.ucsb.edu> <www.dimin.net> 

   Fix for more than 236 mbat block entries : Michel Boudinot
   Copyright 2010 <Michel.Boudinot@inaf.cnrs-gif.fr>

   Considerable rework to allow for creation and updating of structured storage: Stephen Baum
   Copyright 2013 <srbaum@gmail.com>

   Added GetAllStreams, reworked datatypes
   Copyright 2013 Felix Gorny from Bitplane
   
   More datatype changes to allow for 32 and 64 bit code, some fixes involving incremental updates, flushing
   Copyright 2013 <srbaum@gmail.com>
   
   Version: 0.5.3

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

/*
Unicode notes:

Filenames are considered to be encoded in UTF-8 encoding. On windows they 
can be re-encoded into UTF16 and proper wchar_t APIs will be used to open files.
This is a default behavior for windows and is defined by the macro POLE_USE_UTF16_FILENAMES.

Using a provided function and a modern c++ compiler it's easy to encode a 
wide string into utf8 char*:
    std::string POLE::UTF16toUTF8(const std::wstring &utf16);
*/

#ifndef POLE_H
#define POLE_H

#include <cstdio>
#include <string>
#include <list>

#ifdef __linux__
    #include <string.h>
#endif

#include "../../../OOXML/Base/unicode_util.h"

static std::wstring convertUtf16ToWString(const UTF16 * Data, int nLength)
{
    UTF32 *pStrUtf32 = new UTF32 [nLength + 1];
    memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength + 1));

    // this values will be modificated
    const UTF16 *pStrUtf16_Conv = Data;
    UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult =
            ConvertUTF16toUTF32 (&pStrUtf16_Conv,
                                 &Data[nLength]
            , &pStrUtf32_Conv
            , &pStrUtf32 [nLength]
            , strictConversion);

    if (conversionOK != eUnicodeConversionResult)
    {
        delete [] pStrUtf32;
        return std::wstring();
    }

    std::wstring wstr ((wchar_t *) pStrUtf32);

    delete [] pStrUtf32;
    return wstr;
}
static void convertWStringToUtf16(const std::wstring & str, unsigned char *& Data, int & DataSize)
{			
	size_t nLength = str.length();
			
	DataSize = (int)nLength * 2; 
	Data = new unsigned char [DataSize]; 

	UTF16* pStrUtf16 = (UTF16*)		Data;
	UTF32 *pStrUtf32 = (UTF32 *)	str.c_str();

	UTF16 *pStrUtf16_Conv		= pStrUtf16;
	const UTF32 *pStrUtf32_Conv = pStrUtf32;

    ConversionResult eUnicodeConversionResult = ConvertUTF32toUTF16 (&pStrUtf32_Conv, &pStrUtf32[nLength]
            , &pStrUtf16_Conv, &pStrUtf16 [nLength], strictConversion);

    if (conversionOK != eUnicodeConversionResult)
    {
    }
}
namespace POLE
{

#if defined WIN32 || defined WIN64 || defined _WIN32 || defined _WIN64 || defined _MSVC
	#define POLE_USE_UTF16_FILENAMES
	#define POLE_WIN
	typedef __int32 int32;
	typedef __int64 int64;
	typedef unsigned __int32 uint32;
	typedef unsigned __int64 uint64;
#else
	typedef int int32;
	typedef long long int64;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
#endif

typedef uint64 t_offset;

class StorageIO;
class Stream;
class StreamIO;

class Storage
{
  friend class Stream;
  friend class StreamOut;

public:

  // for Storage::result()
  enum { Ok, OpenFailed, NotOLE, BadOLE, UnknownError };
  
  /**
   * Constructs a storage with name filename.
   **/
  Storage( const wchar_t* filename );

  /**
   * Destroys the storage.
   **/
  ~Storage();
  
  /**
   * Opens the storage. Returns true if no error occurs.
   **/
  bool open(bool bWriteAccess = false, bool bCreate = false);

  /**
   * Closes the storage.
   **/
  void close();
  
  /**
   * Returns the error code of last operation.
   **/
  int result();

  /**
   * Finds all stream and directories in given path.
   **/
  std::list<std::wstring> entries( const std::wstring& path = L"/" );
  std::list<std::wstring> entries_with_prefix( const std::wstring& path = L"/" );
  
  /**
   * Returns true if specified entry name is a directory.
   */
  bool isDirectory( const std::wstring& name );

  /**
   * Returns true if specified entry name exists.
   */
  bool exists( const std::wstring& name );

  /**
   * Returns true if storage can be modified.
   */
  bool isWriteable();

  /**
   * Deletes a specified stream or directory. If directory, it will
   * recursively delete everything underneath said directory.
   * returns true for success
   */
  bool deleteByName( const std::wstring& name );

  /**
   * Returns an accumulation of information, hopefully useful for determining if the storage
   * should be defragmented.
   */

  void GetStats(uint64 *pEntries, uint64 *pUnusedEntries,
      uint64 *pBigBlocks, uint64 *pUnusedBigBlocks,
      uint64 *pSmallBlocks, uint64 *pUnusedSmallBlocks);

  std::list<std::wstring> GetAllStreams( const std::wstring& storageName );

  Storage( const Storage& );
  Storage& operator=( const Storage& );

private:
  StorageIO* io;
  
  // no copy or assign

};

class Stream
{
  friend class Storage;
  friend class StorageIO;
  
public:

  /**
   * Creates a new stream.
   */
  // name must be absolute, e.g "/Workbook"
  Stream( Storage* storage, const std::wstring& name, bool bCreate = false, int64 streamSize = 0);

  /**
   * Destroys the stream.
   */
  ~Stream();

  /**
   * Returns the full stream name.
   */
  std::wstring fullName(); 
  
  /**
   * Returns the stream size.
   **/
  uint64 size();

  /**
   * Changes the stream size (note this is done automatically if you write beyond the old size.
   * Use this primarily as a preamble to rewriting a stream that is already open. Of course, you
   * could simply delete the stream first).
   **/
  void setSize(int64 newSize);

  /**
   * Returns the current read/write position.
   **/
  uint64 tell();

  /**
   * Sets the read/write position.
   **/
  void seek( uint64 pos ); 

  /**
   * Reads a byte.
   **/
  int64 getch();

  /**
   * Reads a block of data.
   **/
  uint64 read( unsigned char* data, uint64 maxlen );
  
  /**
   * Writes a block of data.
   **/
  uint64 write( unsigned char* data, uint64 len );

  /**
   * Makes sure that any changes for the stream (and the structured storage) have been written to disk.
   **/
  void flush();
  
  /**
   * Returns true if the read/write position is past the file.
   **/
  bool eof();
  
  /**
   * Returns true whenever error occurs.
   **/
  bool fail();

private:
  StreamIO* io;

  // no copy or assign
  Stream( const Stream& );
  Stream& operator=( const Stream& );    
};

}

#endif // POLE_H
