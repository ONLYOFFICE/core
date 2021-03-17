#ifndef _RASTER_H
#define _RASTER_H

#ifndef RASTER_USE_DYNAMIC_LIBRARY
#define RASTER_DECL_EXPORT
#else
#include "../../../../../common/base_export.h"
#define RASTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>
#include "../../cimage/CxImage/ximage.h"

#ifdef __cplusplus
extern "C" {
#endif

RASTER_DECL_EXPORT void* CxImage_Malloc(unsigned int size);
RASTER_DECL_EXPORT void CxImage_Free(void* p);

RASTER_DECL_EXPORT CxImage* CxImage_Create();
RASTER_DECL_EXPORT CxImage* CxImage_Load(unsigned char* buffer, unsigned int size, unsigned int imagetype = 0);
RASTER_DECL_EXPORT void CxImage_Destroy(CxImage* p);

RASTER_DECL_EXPORT unsigned int CxImage_GetHeight(CxImage* p);
RASTER_DECL_EXPORT unsigned int CxImage_GetWidth (CxImage* p);

RASTER_DECL_EXPORT bool CxImage_Decode(CxImage* p, unsigned char* buffer, unsigned int size, unsigned int imagetype);
RASTER_DECL_EXPORT bool CxImage_Encode(CxImage* p, unsigned char* &buffer, int &size, unsigned int imagetype);
RASTER_DECL_EXPORT unsigned char* CxImage_GetRGBA(CxImage* p);

#ifdef __cplusplus
}
#endif

#endif // _RASTER_H
