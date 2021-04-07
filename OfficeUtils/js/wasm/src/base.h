#ifndef _ZLIB_H
#define _ZLIB_H

#ifndef ZLIB_USE_DYNAMIC_LIBRARY
#define ZLIB_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define ZLIB_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>
#include <string>
#include <deque>
#include <fstream>
#include <vector>
#include <list>

#if defined(_WIN32) || defined (_WIN64)
    #include <direct.h>
    #include <windows.h>
#endif

#include "../../../src/OfficeUtilsCommon.h"
#include "../../../../DesktopEditor/common/Types.h"

// using namespace std;

#include "../../../src/zlib-1.2.11/contrib/minizip/unzip.h"
#include "../../../src/zlib-1.2.11/contrib/minizip/zip.h"
#if defined(_WIN32) || defined (_WIN64)
    #include "../../../src/zlib-1.2.11/contrib/minizip/iowin32.h"
#endif
#include "../../../src/zlib-1.2.11/zlib.h"
#include "../../../src/ZipUtilsCP.h"

#include <fstream>

#ifdef __cplusplus
extern "C" {
#endif

ZLIB_DECL_EXPORT void* Zlib_Malloc(unsigned int size);
ZLIB_DECL_EXPORT void  Zlib_Free(void* p);

ZLIB_DECL_EXPORT unsigned char** Zlib_GetPaths(unsigned char* buffer, int size);
ZLIB_DECL_EXPORT unsigned char*  Zlib_GetFile (std::string buffer, const wchar_t* path);

#ifdef __cplusplus
}
#endif

#endif // _ZLIB_H
