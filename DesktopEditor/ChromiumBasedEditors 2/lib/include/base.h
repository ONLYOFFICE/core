#ifndef APPLICATION_BASE_H
#define APPLICATION_BASE_H

#ifdef WIN32
#ifndef Q_DECL_EXPORT
#define Q_DECL_EXPORT __declspec(dllexport)

#include "windows.h"

#endif

#define WindowHandleId HWND

#include <wchar.h>

#endif

#endif // APPLICATION_BASE_H
