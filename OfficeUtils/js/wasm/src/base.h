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

#include "../../../src/ZipBuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

ZLIB_DECL_EXPORT void* Zlib_Malloc(unsigned int size);
ZLIB_DECL_EXPORT void  Zlib_Free(void* p);

ZLIB_DECL_EXPORT CZipBuffer* Zlib_Create();
ZLIB_DECL_EXPORT CZipBuffer* Zlib_Open(unsigned char* buffer, unsigned long size);
ZLIB_DECL_EXPORT void   Zlib_Close(CZipBuffer* p);

ZLIB_DECL_EXPORT void Zlib_AddFile   (CZipBuffer* p, const char* path, unsigned char* data, unsigned long length);
ZLIB_DECL_EXPORT void Zlib_RemoveFile(CZipBuffer* p, const char* path);

ZLIB_DECL_EXPORT unsigned char* Zlib_GetPaths(CZipBuffer* p);
ZLIB_DECL_EXPORT unsigned char* Zlib_GetFile (CZipBuffer* p, const char* path);
ZLIB_DECL_EXPORT unsigned char* Zlib_Save    (CZipBuffer* p);

#ifdef __cplusplus
}
#endif

#endif // _ZLIB_H
