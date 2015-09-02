#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

#include "../../../common/base_export.h"

#ifdef WIN32

#include "windows.h"
#define WindowHandleId HWND
#include <wchar.h>

#endif

#if defined(_LINUX) && !defined(_MAC)

//#include <X11/X.h>
//typedef unsigned long XID;
#define WindowHandleId unsigned long

#endif

#ifdef _MAC

#define WindowHandleId void*

#endif

#endif // APPLICATION_BASE_H
