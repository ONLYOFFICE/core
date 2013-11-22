// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

#ifdef _DEBUG
#define _RAW_TRACE(...) AtlTrace(__VA_ARGS__)
#else
#define _RAW_TRACE(...)
#endif

class CAutoHGLOBALFree
{
public:
	
	CAutoHGLOBALFree(HGLOBAL global):m_p(global)
	{
			
	}

	~CAutoHGLOBALFree()
	{
		Free();
	}

	HGLOBAL Detach() throw()
	{
		HGLOBAL p;

		p = m_p;
		m_p = NULL;

		return( p );
	}

	void Free() throw()
	{
		if (m_p)
		{
			_ASSERT( NULL == ::GlobalFree(m_p));
		}
		m_p = NULL;
	}
	HGLOBAL m_p;
};

template< typename T>
class CAutoMemPtr_
{
public:
	CAutoMemPtr_() throw() :
		m_p( NULL )
	{
	}

	template< typename TSrc >
	CAutoMemPtr_( CAutoMemPtr_< TSrc >& p ) throw()
	{
		m_p = p.Detach();  // Transfer ownership
	}
	CAutoMemPtr_( CAutoMemPtr_< T >& p ) throw()
	{
		m_p = p.Detach();  // Transfer ownership
	}
	explicit CAutoMemPtr_( T* p ) throw() :
		m_p( p )
	{
	}
	~CAutoMemPtr_() throw()
	{
		Free();
	}

	// Templated version to allow pBase = pDerived
	template< typename TSrc >
	CAutoMemPtr_< T >& operator=( CAutoMemPtr_< TSrc >& p ) throw()
	{
		if(m_p==p.m_p)
		{
			// This means that two CAutoPtrs of two different types had the same m_p in them
			// which is never correct
			_ASSERT(FALSE);
		}
		else
		{
			Free();
			Attach( p.Detach() );  // Transfer ownership
		}
		return( *this );
	}

	CAutoMemPtr_< T >& operator=( CAutoMemPtr_< T >& p ) throw()
	{
		if(*this==p)
		{
			if(this!=&p)
			{
				// If this assert fires, it means you attempted to assign one CAutoPtr to another when they both contained 
				// a pointer to the same underlying object. This means a bug in your code, since your object will get 
				// double-deleted. 
				_ASSERT(FALSE);

				// For safety, we are going to detach the other CAutoPtr to avoid a double-free. Your code still
				// has a bug, though.
				p.Detach();
			}
			else
			{
				// Alternatively, this branch means that you are assigning a CAutoPtr to itself, which is
				// pointless but permissible

				// nothing to do
			}
		}
		else
		{
			Free();
			Attach( p.Detach() );  // Transfer ownership
		}
		return( *this );
	}

	// basic comparison operators
	bool operator!=(CAutoMemPtr_<T>& p) const
	{
		return !operator==(p);
	}

	bool operator==(CAutoMemPtr_<T>& p) const
	{
		return m_p==p.m_p;
	}

	operator T*() const throw()
	{
		return( m_p );
	}
	T* operator->() const throw()
	{
		ATLASSUME( m_p != NULL );
		return( m_p );
	}	
	T* Detach() throw()
	{
		T* p;

		p = m_p;
		m_p = NULL;

		return( p );
	}
	void Free() throw()
	{
		if (m_p)
			free(m_p);
		m_p = NULL;
	}	
	T* m_p;	
};

using namespace ATL;
//#include "defaultsettings.h"

//#include "ImageRaw3Options.h"


//#include "ImageRaw3Options.h"


//#include "ImageRaw3Options.h"
