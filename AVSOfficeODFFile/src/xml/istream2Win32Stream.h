
#ifndef _CPDOCCORE_ISTREAM2WIN32STREAM_H_
#define _CPDOCCORE_ISTREAM2WIN32STREAM_H_

#ifdef _MSC_VER
#pragma once
#endif

#include "win32ver.h"
#include <windows.h>
#include <objidl.h>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcomcli.h>

#include <iosfwd>

namespace cpdoccore {
namespace xml {

::ATL::CComPtr< IStream > istreamToWin32IStream(shared_ptr< std::istream >::Type istreamPtr);
::ATL::CComPtr< IStream > istreamToWin32IStream(std::istream & istreamRef);

}
}

#endif
