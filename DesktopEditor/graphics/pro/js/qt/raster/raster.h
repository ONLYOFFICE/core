#ifndef _RASTER_H
#define _RASTER_H

#ifndef RASTER_USE_DYNAMIC_LIBRARY
#define RASTER_DECL_EXPORT
#else
#include "../../../../../common/base_export.h"
#define RASTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

RASTER_DECL_EXPORT void* Raster_DecodeFile(unsigned char* buffer, int size, bool isRgba = false);
RASTER_DECL_EXPORT void* Raster_GetDecodedBuffer(void* frame);
RASTER_DECL_EXPORT int Raster_GetHeight(void* frame);
RASTER_DECL_EXPORT int Raster_GetWidth(void* frame);
RASTER_DECL_EXPORT int Raster_GetStride(void* frame);
RASTER_DECL_EXPORT void Raster_Destroy(void* frame);

RASTER_DECL_EXPORT void* Raster_EncodeImageData(unsigned char* buffer, int w, int h, int stride, int format, bool isRgba = false);
RASTER_DECL_EXPORT void* Raster_Encode(unsigned char* buffer, int size, int format);
RASTER_DECL_EXPORT void* Raster_GetEncodedBuffer(void* encodedData);
RASTER_DECL_EXPORT int Raster_GetEncodedSize(void* encodedData);
RASTER_DECL_EXPORT void Raster_DestroyEncodedData(void* encodedData);

RASTER_DECL_EXPORT int Image_GetFormat(unsigned char* buffer, int size);

#ifdef __cplusplus
}
#endif

#endif // _RASTER_H
