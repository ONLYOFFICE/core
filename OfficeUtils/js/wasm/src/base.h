#ifndef _ZLIB_H
#define _ZLIB_H

#ifndef ZLIB_USE_DYNAMIC_LIBRARY
#define ZLIB_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define ZLIB_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>

#include "../../../../DesktopEditor/common/Types.h"
#include "../../../src/zlib-1.2.11/contrib/minizip/unzip.h"
#include "../../../src/ZipUtilsCP.h"

#ifdef __cplusplus
extern "C" {
#endif

struct myFile
{
    unsigned char* data;
    unsigned long size;
};

ZLIB_DECL_EXPORT void* Zlib_Malloc(unsigned int size);
ZLIB_DECL_EXPORT void  Zlib_Free(void* p);

ZLIB_DECL_EXPORT unsigned char** Zlib_GetPaths(unsigned char* buffer, int size);
ZLIB_DECL_EXPORT myFile*         Zlib_GetFile (char* buffer, int size, const wchar_t* path);

#ifdef __cplusplus
}
#endif

#endif // _ZLIB_H
