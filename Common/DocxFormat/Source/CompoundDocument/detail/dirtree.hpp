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

// dirtree header
#pragma once

#include <string>
#include <vector>
#include <cassert>
#include "util.hpp"
#include <cstring>

namespace POLE
{

template<typename _>
class DirEntryT
{
public:
    static const unsigned End;
  
// Construction/destruction  
public:
	DirEntryT():  
		_name("Root Entry"),
		_type(5),
		_size(0),
		_start(End),
		_prev(End),
		_next(End),
		_child(End),
		_index(0),
		_parent(0)
	{}
	DirEntryT(const std::string& name, ULONG8 type, ULONG32 size, ULONG32 start, ULONG32 prev, ULONG32 next, ULONG32 child, ULONG32 index, ULONG32 parent)
	{ 
		set(name, type, size, start, prev, next, child, index, parent);
	}

// Attributes
public:
	bool valid() const;
	const std::string& name() const { return _name; }
	ULONG8 type() const { return _type; }
	bool root() const { return (_type == 5); }
	bool dir() const { return ((_type == 1) || (_type == 5)); }
	bool file() const { return (_type == 2); }
	ULONG32 size() const { return _size; }
	ULONG32 start() const { return _start; }
	ULONG32 prev() const { return _prev; }
	ULONG32 next() const { return _next; }
	ULONG32 child() const { return _child; }
	ULONG32 index() const { return _index; }
	ULONG32 parent() const { return _parent; }

// Operations
public:
	void set(const std::string& name, ULONG8 type, ULONG32 size, ULONG32 start, ULONG32 prev, ULONG32 next, ULONG32 child, ULONG32 index, ULONG32 parent)
	{
		_name = name;
		_type = type;
		_size = size;
		_start = start;
		_prev = prev;
		_next = next;
		_child = child;
		_index = index;
		_parent = parent;
	}
	void set_prev(ULONG32 prev) { _prev = prev; }
	void set_next(ULONG32 next) { _next = next; }
	void set_child(ULONG32 child) { _child = child; }
	void set_parent(ULONG32 parent) { _parent = parent; }
#ifndef NDEBUG
    void debug() const;
#endif

// Implementation
private:
    std::string _name;  // the name, not in unicode anymore 
    ULONG8 _type;       // true if directory   
    ULONG32 _size;		// size (not valid if directory)
    ULONG32 _start;		// starting block
    ULONG32 _prev;      // previous sibling
    ULONG32 _next;      // next sibling
    ULONG32 _child;     // first child
	ULONG32 _index;		// index of the entry in the directory
	ULONG32 _parent;	// parent in the directory structure. Must be a folder. 
};

typedef DirEntryT<void> DirEntry;

template<typename _>
class DirTreeT
{
// Construction/destruction  
public:
	DirTreeT() { clear(); }

// Attributes
public:
    size_t entryCount() const { return _entries.size(); }
    void listAll(std::vector<const DirEntry*>&) const;
    ULONG32 parent( ULONG32 index ) const;
    ULONG32 parentDirectory( ULONG32 index ) const;
    void fullName( ULONG32 index, std::string& ) const;
    void current_path( std::string& result) const {  fullName( _current, result ); }
    const DirEntry* root_entry() const {  return entry( 0 ); }
    const DirEntry* current_entry() const {  return entry( _current ); }
    void children( ULONG32 index, std::vector<ULONG32>& ) const;
    void listDirectory(std::vector<const DirEntry*>&) const;
	const DirEntry* entry( ULONG32 index ) const;
	
// Operations
public:
	void clear();
	const DirEntry* entry( const std::string& name, bool create=false ) { return _entry(name, create); }
    bool enterDirectory( const std::string& dir );
    void leaveDirectory();
    
	bool delete_entry(const std::string& path) { return delete_entry(path, 0); }
	ULONG32 search_prev_link( ULONG32 entry );
	ULONG32 find_rightmost_sibling(ULONG32 left_sib);
	bool set_prev_link(ULONG32 prev_link, ULONG32 entry, ULONG32 value);
	
	bool load( unsigned char* buffer, size_t len );
    bool save( unsigned char* buffer, size_t len );
#ifndef NDEBUG
    void debug() const;
#endif

// Implementation
private:
	DirEntry* entry( ULONG32 index );
	DirEntry* _entry( const std::string& name, bool create = false );
	void find_siblings( std::vector<ULONG32>& result, ULONG32 index ) const;
	bool delete_entry(const std::string& path, int level);
	void set_parents();
	void set_parents(DirEntry * cur_entry, ULONG32 cur_parent);

	ULONG32 _current;
    std::vector<DirEntry> _entries;
    
	DirTreeT( const DirTreeT<_>& );
    DirTreeT<_>& operator=( const DirTreeT<_>& );
};

typedef DirTreeT<void> DirTree;

// =========== DirEntryT ==========

template<typename _>
const unsigned DirEntryT<_>::End = 0xffffffff;

template<typename _>
bool DirEntryT<_>::valid() const
{ 
	//if ((_type == 1 ) && (_size != 0 ))
	//	return false;
	if ((_type != 1 ) && (_type != 2 ) && (_type != 5 ))
		return false;
	if (_name.empty())
		return false;

	return true;
}

#ifndef NDEBUG
template<typename _>
void DirEntryT<_>::debug() const
{ 
    if( !valid() ) std::cout << "INVALID ";
    std::cout << name() << " ";
    if( valid() ) std::cout << "(Valid) ";
	else std::cout << "(Invalid) ";
    if( dir() ) std::cout << "(Dir) ";
    else std::cout << "(File) ";
    std::cout << index() << " ";
    std::cout << size() << " ";
    std::cout << "s:" << start() << " ";
    std::cout << "(";
	if( child() == DirEntry::End ) std::cout << "-"; else std::cout << child();
    std::cout << " ";
	if( prev() == DirEntry::End ) std::cout << "-"; else std::cout << prev();
    std::cout << ":";
	if( next() == DirEntry::End ) std::cout << "-"; else std::cout << next();
    std::cout << ") ";    
    std::cout << parent();
    std::cout << std::endl;
}
#endif

// =========== DirTree ==========

template<typename _>
void DirTreeT<_>::clear()
{
  // leave only the root entry
  _entries.resize( 1 );
  _current = 0;
}

template<typename _>
const DirEntry* DirTreeT<_>::entry( ULONG32 index ) const
{
  if( index >= entryCount() ) return NULL;
  return &_entries[ index ];
}

template<typename _>
DirEntry* DirTreeT<_>::entry( ULONG32 index )
{
  if( index >= entryCount() ) return NULL;
  return &_entries[ index ];
}

template<typename _>
ULONG32 DirTreeT<_>::parentDirectory( ULONG32 index ) const
{
	const DirEntry * e = entry(index);
	if (!e) return DirEntry::End;
	assert(e->valid());

#ifndef NDEBUG
	const DirEntry * test = entry(e->parent());
	assert(test);
	assert(test->valid());
	assert(test->dir());
#endif

	return e->parent();
}

template<typename _>
ULONG32 DirTreeT<_>::parent( ULONG32 index ) const
{
	// brute-force, basically we iterate for all entries, find its children
	// and check if one of the children is 'index'
	std::vector<DirEntry>::const_iterator it;
	for (it = _entries.begin(); it != _entries.end(); ++it)
		if (it->valid() && it->child() == index)
			return it->index();
	        
	return DirEntry::End;
}

template<typename _>
void DirTreeT<_>::fullName( ULONG32 index, std::string& result ) const
{
	const DirEntry* e = entry( index );
	if( !e )
		return;
	
	// don't use root name ("Root Entry"), just give "/"
	if( index == 0 )
	{
		result = "/";
		return;
	}

	result = e->name();
	result.insert( 0,  "/" );
	ULONG32 p = parentDirectory( index );
	if (p == -1)
		return;
	const DirEntry* _entry = 0;
	while( p > 0 )
	{
		_entry = entry( p );
		if (_entry && _entry->dir() && _entry->valid())
		{
			result.insert( 0, _entry->name());
			result.insert( 0,  "/" );
		}
		p = parentDirectory(p);
		if (p == DirEntry::End)
			break;
	}
}

// Given a fullname (e.g "/ObjectPool/_1020961869"), find the entry.
// If not found and create is false, return 0.
// If create is true, and name ends with '/' a new dir entry is returned.
// If create is true, and name does not end with '/' a new file entry is returned.
template<typename _>
DirEntry* DirTreeT<_>::_entry( const std::string& name, bool create )
{
	if( name.empty() ) 
		return NULL;
   
   // quick check for "/" (that's root)
   if( name == "/" ) return &_entries[0];
   
   // get the parent and child, e.g  "/ObjectPool/_1020961869" will become:
   // "/ObjectPool" and "_1020961869". "/Data" will become "/" and "Data". 

   std::string parent;
   std::string child;
   std::string::size_type name_pos = name.rfind('/');
   if ( name_pos == name.length()-1)
	   name_pos = name.rfind('/', name_pos);
   if (name_pos != std::string::npos && name_pos != 0)
   {
	   parent = name.substr( 0, name_pos );
	   child = name.substr(name_pos+1);
   }
   else if (name_pos == 0)
   {
	   parent = "/";
	   child = name.substr(1);
   }
   else
      child = name;
   if (child[child.length()-1] == '/')
	   child.erase(child.length()-1);
	
   // start from root when name is absolute
   // or current directory when name is relative
   ULONG32 index = (name[0] == '/' ) ? 0 : _current;
   
   const DirEntry* pe = (parent.empty()) ? entry( index ) : entry( parent, create );
   if (!pe) return NULL;
   if (!pe->valid()) return NULL;
   assert(pe->dir());

  // search entries with the same parent
  std::vector<DirEntry>::iterator it;
  for (it = _entries.begin(); it != _entries.end(); ++it)
	  if (pe->index() == it->parent() && it->valid()) 
           if (it->name().length() == child.length())
		   {
			   size_t j;
			   for (j = 0; j<it->name().length(); ++j)
			     if( tolower(child[j]) != tolower(it->name()[j]) )
					 break;
		       if (j == it->name().length())
				   return &(*it); // found
		   }

  if (!create)
    return NULL; // not found
        
  // create a new entry
  assert(parent.empty());
  DirEntry* ep = &_entries[index];
  ULONG8 type = (name[name.length()-1] == '/') ? 1 : 2;
  index = (ULONG32)entryCount();
  DirEntry ne(child, type, 0, 0, DirEntry::End, DirEntry::End, ep->child(), index, ep->index());
  _entries.push_back( ne );
  ep->set_child(index);
  return entry( index );
}

template<typename _>
void DirTreeT<_>::children( ULONG32 index, std::vector<ULONG32>& result ) const
{ 
  const DirEntry* e = entry( index );
  if( e && e->valid() && e->dir() )
    find_siblings( result, e->child() );
}

template<typename _>
void DirTreeT<_>::listAll(std::vector<const DirEntry*>& result) const
{
  size_t size = _entries.size();
  result.reserve(size);
  for( size_t i = 0; i < size; i++ )
	  if (_entries[i].valid())
		result.push_back( &_entries[i] );
}

template<typename _>
void DirTreeT<_>::listDirectory(std::vector<const DirEntry*>& result) const
{
  std::vector<ULONG32> chi;
  children( _current, chi );
  size_t size = chi.size();
  result.reserve(size);
  for( size_t i = 0; i < size; i++ )
    result.push_back( entry( chi[i] ) );
}

template<typename _>
bool DirTreeT<_>::enterDirectory( const std::string& dir )
{
  const DirEntry* e = entry( dir );
  if( !e ) 
	  return false;
  if( !e->valid() ) 
	  return false;
  if( !e->dir() ) 
	  return false;
  
  ULONG32 index = e->index();
  if( index == DirEntry::End) return false;
    
  _current = index;
  return true;
}

template<typename _>
void DirTreeT<_>::leaveDirectory()
{
  // already at root ?
  if( _current == 0 ) 
	  return;

  ULONG32 p = parentDirectory( _current );
  if( p != DirEntry::End) 
	  _current = p;
#ifndef NDEBUG
  else
	  assert(false);
#endif
 }

template<typename _>
bool DirTreeT<_>::load( unsigned char* buffer, size_t size )
{
  _entries.clear();
  _current = 0;
  size_t init_count = size / 128;
  
  _entries.reserve(init_count);
  for( unsigned i = 0; i < init_count; i++ )
  {
    unsigned p = i * 128;
      
	// Name's length 
    int name_len = readU16( buffer + 0x40+p );
    // parse name of this entry, which stored as Unicode 16-bit
    std::string name;
	name.reserve(name_len);
	// Read name char by char
    if( name_len > 64 ) name_len = 64;
    for( int j=0; ( buffer[j + p]) && (j < name_len); j+= 2 )
      name.append( 1, buffer[j + p] );
      
    // 1 = directory (aka storage), 2 = file (aka stream),  5 = root
    ULONG8 type = buffer[ 0x42 + p];
    ULONG32 start = readU32( buffer + 0x74+p );
    ULONG32 size = readU32( buffer + 0x78+p );
    ULONG32 prev = readU32( buffer + 0x44+p );
    ULONG32 next = readU32( buffer + 0x48+p );
    ULONG32 child = readU32( buffer + 0x4C+p );
    
	DirEntry e(name, type, size, start, prev, next, child, i, 0);
	_entries.push_back( e );
  }
  set_parents();

  return true;
}

template<typename _>
bool DirTreeT<_>::save( unsigned char* buffer, size_t len )
{
  size_t size = 128*entryCount();
  if (len < size)
    return false;
  memset( buffer, 0, size );

  for( unsigned i = 0; i < entryCount(); i++ )
  {
    const DirEntry* e = entry( i );
    if( !e ) return false;
    
    // max length for name is 32 chars
    std::string name = e->name();
    if( name.length() > 32 )
      name.erase( 32, name.length() );
      
    // write name as Unicode 16-bit
    for( unsigned j = 0; j < name.length(); j++ )
      buffer[ i*128 + j*2 ] = name[j];

    writeU16( buffer + i*128 + 0x40, (ULONG16)(name.length()*2 + 2) );    
    writeU32( buffer + i*128 + 0x74, e->start() );
    writeU32( buffer + i*128 + 0x78, e->size() );
    writeU32( buffer + i*128 + 0x44, e->prev() );
    writeU32( buffer + i*128 + 0x48, e->next() );
    writeU32( buffer + i*128 + 0x4c, e->child() );
    buffer[ i*128 + 0x42 ] = e->type();
    buffer[ i*128 + 0x43 ] = 1; // always black
  }  
  return true;
}

#ifndef NDEBUG
template<typename _>
void DirTreeT<_>::debug() const
{
  for( unsigned i = 0; i < entryCount(); i++ )
  {
    const DirEntry* e = entry( i );
    if( !e ) continue;
    std::cout << i << ": ";
    e->debug();
  }
  std::vector<size_t> res;
  children(0, res);
  std::cout << std::endl << std::endl << "--------------------------" << std::endl;
  for (unsigned int i = 0; i < res.size(); ++i)
  {
	  std::cout << res[i] << std::endl;
  }
}
#endif

// helper function: recursively find siblings of index
template<typename _>
void DirTreeT<_>::find_siblings( std::vector<ULONG32>& result, ULONG32 index ) const
{
  const DirEntry* e = entry( index );
  if( !e ) return;
  if( !e->valid() ) return;

  // search entries with the same parent
  result.reserve(_entries.size()); // for efficiency
  std::vector<DirEntry>::const_iterator it;
  for (it = _entries.begin(); it != _entries.end(); ++it)
	  if (e->parent() == it->parent() && it->valid() && !it->root())
	  {
		result.push_back(it->index());
	  }
}

template<typename _>
bool DirTreeT<_>::delete_entry(const std::string& path, int level)
{
	// Deletion is not posible over Root Entry
	if (path == "/")
		return false;
	DirEntry *e = _entry(path);
	// Hack: Should an invalid path throw and exception?
	if (!e)
		return false;
	// Is a Storage?
 	if (e->type() == 1)
	{
		// Has Child?
		if (e->child() != DirEntry::End)
		{
			DirEntry *child = entry(e->child());
			std::string _child_path = path + ((path[path.size()] == '/') ? child->name() : std::string("/") + child->name());
			level++;
			if (!delete_entry(_child_path, level))
				return false;
			e->set_child(DirEntry::End);
			level--;
		}
	}
	// This is not the first call. Searching for brothers
	if (level)
	{
		// Has right sibling?
		if (e->prev() != DirEntry::End)
		{
			// Find the previous entry
			DirEntry *pe = entry(e->prev());
			if (!pe)
				return false;
			std::string _child_path;
			std::string::size_type _pos = path.find_last_of("/");

			if (_pos != std::string::npos)
				_child_path = path.substr(0, path.size() - ((path.size() - _pos) - 1)) + pe->name();
			else
				return false;

			level++;
			if (!delete_entry(_child_path, level))
				return false;
			e->set_prev(DirEntry::End);
			level--;
		}
		// Has left sibling?
		if (e->next() != DirEntry::End)
		{
			// Find the previous entry
			DirEntry *pe = entry(e->next());
			if (!pe)
				return false;
			std::string _child_path;
			std::string::size_type _pos = path.find_last_of("/");

			if (_pos != std::string::npos)
				_child_path = path.substr(0, path.size() - ((path.size() - _pos) - 1)) + pe->name();
			else
				return false;

			level++;
			if (!delete_entry(_child_path, level))
				return false;
			e->set_next(DirEntry::End);
			level--;
		}
	}

	// Delete the entry
	if (level == 0)
	{
		// This is not an recursive call
		// Find the entry that points to the entry is being deleted
		size_t prev_link = search_prev_link(e->index());
		if (prev_link == -1)
			return false;
		// Last entry?
		if (e->next() == DirEntry::End &&
			e->prev() == DirEntry::End)
		{
			// Set the end of chain mark in the entry that points to the entry
			// being deleted
			if (!set_prev_link(prev_link, e->index(), DirEntry::End))
				return false;
		}
		else 
		{
			// It is not the last one, but has a sibling
			if(e->next() == DirEntry::End ||
			   e->prev() == DirEntry::End)
			{
				// If hasn't a previous sibling, set previous link to point
				// to the next field of the entry being deleted
				if (e->prev() == DirEntry::End)
				{
					if (!set_prev_link(prev_link, e->index(), e->next()))
						return false;
				}
				else
					// If hasn't a next sibling, set previous link to point
					// to the prev field of the entry being deleted
					if (!set_prev_link(prev_link, e->index(), e->prev()))
						return false;
			}
			else
			{
				// If has a previous and a next sibling, find the right most sibling
				// pointed by the next field of the entry being deleted, set this entry's previous field
				// to point to the prev field of the entry being deleted.
				// Then set previous link to point to the next field of the entry being deleted.
				size_t right_most = find_rightmost_sibling(e->next());
				if (right_most == -1)
					return false;
				DirEntry *_right = entry(right_most);
				if (!_right) return false;
				_right->set_prev(e->prev());

				if (!set_prev_link(prev_link, e->index(), e->next()))
					return false;
			}
		}
	}
	e->set("", 0, 0, 0, 0, DirEntry::End, DirEntry::End, DirEntry::End, 0);

	return true;
}

template<typename _>
ULONG32 DirTreeT<_>::search_prev_link( ULONG32 _entry )
{
	// Find parent
	size_t par_index = parent(_entry);
	if (par_index == -1)
		return false;
	if (_entries[par_index].child() == _entry)
		return par_index;
	else
	{
		std::vector<ULONG32> brothers;
		children(par_index, brothers);
		if (brothers.size() == 0)
			return false;
		for (size_t ndx = 0; ndx < brothers.size(); ++ndx)
		{
			if (_entries[brothers[ndx]].next() == _entry || 
				_entries[brothers[ndx]].prev() == _entry)
			{
				return brothers[ndx];
			}
		}
	}

	return DirEntry::End;
}

template<typename _>
bool DirTreeT<_>::set_prev_link(ULONG32 prev_link, ULONG32 index, ULONG32 value)
{
	DirEntry * pl = entry(prev_link);
	if (!pl) return false;

	if (pl->prev() == index)
		pl->set_prev(value);
	if (pl->next() == index)
		pl->set_next(value);
	if (pl->child() == index)
		pl->set_child(value);

	return true;
}

template<typename _>
ULONG32 DirTreeT<_>::find_rightmost_sibling(ULONG32 sib)
{
	DirEntry * _ent = entry(sib);
	if (!_ent)
		return DirEntry::End;
	if (_ent->prev() == DirEntry::End)
		return _ent->index();
	return find_rightmost_sibling(_ent->prev());
}

template<typename _>
void DirTreeT<_>::set_parents()
{
	if (_entries.size()) 
	{ 
		// The parent is already set to 0
		if (_entries[0].valid())
		{
			DirEntry * _child = entry(_entries[0].child());
			if (_child && _child->valid())
			{
				_child->set_parent(0);
				set_parents(_child, 0); 
			}
		}
	} 
}

template<typename _>
void DirTreeT<_>::set_parents(DirEntry * cur_entry, ULONG32 cur_parent)
{
	assert(cur_entry);
	assert(cur_entry->valid());

	if (cur_entry->dir())
	{
		DirEntry * _child = entry(cur_entry->child());
		// the last condition prevents loops
		if (_child && _child->valid() && _child->parent() == 0)
		{
			_child->set_parent(cur_entry->index());
			set_parents(_child, cur_entry->index());
		}
	}
	
	DirEntry * _prev = entry(cur_entry->prev());
	// the last condition prevents loops
	if (_prev && _prev->valid() && _prev->parent() == 0)
	{
		_prev->set_parent(cur_parent);
		set_parents(_prev, cur_parent);
	}

	DirEntry * _next = entry(cur_entry->next());
	// the last condition prevents loops
	if (_next && _next->valid() && _next->parent() == 0)
	{
		_next->set_parent(cur_parent);
		set_parents(_next, cur_parent);
	}
}

}
