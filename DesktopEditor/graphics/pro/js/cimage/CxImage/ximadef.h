#ifndef __ximadef_h
#define __ximadef_h

#include "ximacfg.h"
#include <string>

#if defined(WIN32) || defined(_WIN32_WCE)
#include "stdint.h"
#endif

#ifndef BOOL
typedef int BOOL;
#endif

typedef struct tagRECT
{
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} RECT;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

#define BI_RGB 0L

#endif //__ximadef_h
