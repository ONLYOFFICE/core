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

#ifndef POLE2_H
#define POLE2_H

#pragma once

#include "./detail/stream.hpp"

namespace POLE2
{

class Stream;

template<typename _>
class StorageT
{
public:
  enum { Ok, OpenFailed, OpenSmallFatFailed, NotOLE, BadOLE, UnknownError, StupidWorkaroundForBrokenCompiler=255 };

  // Constructs a storage with name filename.
  StorageT( const char* filename, std::ios_base::openmode mode = std::ios_base::in, bool create = false )
  {
    io = new StorageIO( filename, mode, create );
  }
  StorageT( const std::wstring& filename, std::ios_base::openmode mode = std::ios_base::in, bool create = false )
  {
    io = new StorageIO( filename, mode, create );
  }

  // Constructs a storage from a stream.
  StorageT( std::iostream& stream )
  {
    io = new StorageIO( &stream );
  }

  // Destroys the storage.
  ~StorageT()
  {
    delete io;
    std::list<Stream*>::iterator it;
    for( it = streams.begin(); it != streams.end(); ++it )
      delete *it;
  }

// Attributes
public:
  
  // Returns the error code of last operation.
  int result() const
  {
    return io->result();
  }

  void fullName( const DirEntry* entry, std::string& name) const
  {
    io->fullName( entry, name);
  }

  // Returns the current path.
  void current_path( std::string& result) const
  {
    io->current_path(result);
  }

  // Returns the root entry.
  const DirEntry* root_entry() const
  {
	  return io->root_entry();
  }

  // Returns the current entry.
  const DirEntry* current_entry() const
  {
	  return io->current_entry();
  }

  const DirEntry* getEntry(const std::string& name) const
  {
	return io->entry( name );
  } 

  void listEntries(std::vector<const DirEntry*>& result) const
  {
	io->listEntries(result);
  } 

  void listAll(std::vector<const DirEntry*>& result) const
  {
	io->listAll(result);
  } 

// Operations
public:
  // Changes path to directory. Returns true if no error occurs.
  bool enterDirectory( const std::string& directory )
  {
	  	return io->enterDirectory( directory );
  }

  // Goes to one directory up.
  void leaveDirectory()
  {
    io->leaveDirectory();
  }

  // Finds and returns a stream with the specified name.
  Stream* stream( const std::string& name, bool reuse = false );

  // If there is a stream using this entry it will be corrupted.
  // Any existent entry list is not valid anymore
  bool delete_entry(const std::string& path)
  {
    return io->delete_entry(path);
  }

  bool flush()
  {
    return io->flush();
  }

#ifndef NDEBUG
  void debug() const
  {
	io->debug();
  }
#endif

private:
  StorageIO* io;
  std::list<Stream*> streams;
  
  // no copy or assign
  StorageT( const StorageT<_>& );
  StorageT<_>& operator=( const StorageT<_>& );
};

typedef StorageT<void> Storage;

class Stream
{
  friend class StorageT<void>;

public: // Attributes

  // Returns the path for this stream.
  const std::string& path() const
  {
	return impl ? impl->path() : StreamImpl::null_path;
  }

  // Returns the stream size.
  std::streamsize size() const
  {
    return impl ? impl->size() : 0;
  }

  // Returns the read pointer.
  std::streampos tellg() const
  {
      return impl ? impl->tellg() : std::streampos();
  }

  // Returns the write pointer.
  std::streampos tellp() const
  {
      return impl ? impl->tellp() : std::streampos();
  }

  // Return the Eof state of the stream
  bool eof() const
  { 
	return impl ? impl->eof() : true; 
  }

  // Return the fail state of the Stream
  bool fail() const
  { 
	return impl ? impl->fail() : true; 
  }

public: // Operations

  // Sets the read position.
  std::streampos seek( std::streampos pos, std::ios::seekdir origin = std::ios_base::beg, std::ios::openmode mode = std::ios::in)
  {
    if( impl ) return impl->seek( pos, origin, mode );
	return 0;
  }

  // Sets the read position.
  void seekg( std::streampos pos, std::ios_base::seekdir origin = std::ios_base::beg )
  {
    if( impl ) impl->seekg( pos, origin );
  }

  // Sets the write position.
  void seekp( std::streampos pos, std::ios_base::seekdir origin = std::ios_base::beg )
  {
    if( impl ) impl->seekp( pos, origin );
  }

  // Reads a byte.
  int getch()
  {
    return impl ? impl->getch() : 0;
  }

  // Reads a block of data.
  std::streamsize read( unsigned char* data, std::streamsize maxlen )
  {
    return impl ? impl->read( data, maxlen ) : 0;
  }

  // Write a block of data
  std::streamsize write(const unsigned char* data, std::streamsize len)
  {
	  return impl ? impl->write( data, len ) : 0;
  }

  // Make the size grow to size. If size is less than current size no
  // action will be taken.
  bool reserve(std::streamsize size)
  {
	  return impl ? impl->reserve( size ) : false;
  }

  // Make the size exactly size. If size is less than current size the
  // extra bytes will be truncated. If it is greater the new data will
  // be filled with val.
  bool resize(std::streamsize size, char val = 0)
  {
	  return impl ? impl->resize( size, val ) : false;
  }

private:
	Stream(StreamImpl* i) { impl = i; }
	~Stream() { delete impl; }

  // no default, copy or assign
  Stream();
  Stream( const Stream& );
  Stream& operator=( const Stream& );
    
  StreamImpl* impl;
};

// =========== StorageT ==========

template<typename _>
Stream* StorageT<_>::stream( const std::string& name, bool reuse )
{
  // sanity check
  if( !name.length() ) return (Stream*)0;
  if( !io ) return (Stream*)0;

  // make absolute if necesary
  std::string fullName = name;
  std::string path_;
  current_path(path_);
  if( name[0] != '/' ) fullName.insert( 0, path_ + "/" );
  
  // If a stream for this path already exists return it
  if (reuse)
  {
	std::list<Stream*>::iterator it;
	for( it = streams.begin(); it != streams.end(); ++it )
		if ((*it)->path() == name)
			return *it;
  }

  const DirEntry* entry = io->entry( name );
  if( !entry ) return (Stream*)0;

  // We create the StreamImpl here instead of in the constructor to avoid 
  // passing implementation parameters in the constructor.
  // The created StreamImpl will be deleted in the Stream destructor.
  StreamImpl* i = new StreamImpl( io, entry );
  Stream* s = new Stream(i); 
  streams.push_back( s );
  
  return s;
}

}

#endif // POLE_H
