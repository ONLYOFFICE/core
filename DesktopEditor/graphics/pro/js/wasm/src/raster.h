#ifndef _RASTER_H
#define _RASTER_H

#ifndef RASTER_USE_DYNAMIC_LIBRARY
#define RASTER_DECL_EXPORT
#else
#include "../../../../../common/base_export.h"
#define RASTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <malloc.h>
#include "../../../../../raster/BgraFrame.h"

#ifdef __cplusplus
extern "C" {
#endif

RASTER_DECL_EXPORT void* Raster_Malloc(unsigned int size);
RASTER_DECL_EXPORT void Raster_Free(void* p);

RASTER_DECL_EXPORT CBgraFrame* Raster_Create();
RASTER_DECL_EXPORT CBgraFrame* Raster_Load(unsigned char* buffer, int size);
RASTER_DECL_EXPORT CBgraFrame* Raster_Init(double width_px, double height_px);
RASTER_DECL_EXPORT void Raster_Destroy(CBgraFrame* p);

RASTER_DECL_EXPORT int Raster_GetHeight(CBgraFrame* p);
RASTER_DECL_EXPORT int Raster_GetWidth (CBgraFrame* p);

RASTER_DECL_EXPORT bool Raster_Decode(CBgraFrame* p, unsigned char* buffer, int size);
RASTER_DECL_EXPORT unsigned char* Raster_GetRGBA(CBgraFrame* p);

#ifdef __cplusplus
}
#endif

#endif // _RASTER_H
