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

#include "../../../../DesktopEditor/common/Types.h"
#include "../../../src/zlib-1.2.11/contrib/minizip/unzip.h"
#include "../../../src/ZipUtilsCP.h"
#include "ioapibuf.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Zlib
{
    unsigned char* buffer;
    unsigned long  size;

    std::wstring* paths;
    int pathsSize;

    bool fileIsIn;
    BYTE** file;
    ULONG fileSize;
};

ZLIB_DECL_EXPORT void* Zlib_Malloc(unsigned int size);
ZLIB_DECL_EXPORT void  Zlib_Free(void* p);

ZLIB_DECL_EXPORT Zlib* Zlib_Create();
ZLIB_DECL_EXPORT Zlib* Zlib_Load(unsigned char* buffer, unsigned long size);
ZLIB_DECL_EXPORT void  Zlib_Destroy(Zlib* p);

ZLIB_DECL_EXPORT int Zlib_GetNumberPaths(Zlib* p);
ZLIB_DECL_EXPORT std::wstring* Zlib_GetPaths(Zlib* p);
ZLIB_DECL_EXPORT int Zlib_GetSizeFileByPath(Zlib* p, const wchar_t* path);
ZLIB_DECL_EXPORT unsigned char* Zlib_GetLastFileByPath(Zlib* p);

#ifdef __cplusplus
}
#endif

#endif // _ZLIB_H
