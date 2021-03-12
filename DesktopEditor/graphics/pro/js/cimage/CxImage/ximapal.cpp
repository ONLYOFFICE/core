#include "ximage.h"

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

void CxImage::RGBtoBGR(uint8_t* buffer, int32_t length)
{
    if (buffer && (head.biClrUsed == 0))
    {
        uint8_t temp;
        length = min(length,(int32_t)info.dwEffWidth);
        length = min(length,(int32_t)(3 * head.biWidth));
        for (int32_t i = 0; i < length; i += 3)
        {
            temp = buffer[i];
            buffer[i] = buffer[i + 2];
            buffer[i + 2] = temp;
        }
    }
}
uint32_t CxImage::GetPaletteSize()
{
	return (head.biClrUsed * sizeof(RGBQUAD));
}
RGBQUAD* CxImage::GetPalette() const
{
    if ((pDib) && (head.biClrUsed))
        return (RGBQUAD*)((uint8_t*)pDib + sizeof(BITMAPINFOHEADER));
    return NULL;
}
void CxImage::SetGrayPalette()
{
    if ((pDib == NULL) || (head.biClrUsed == 0)) return;
    RGBQUAD* pal = GetPalette();
    for (uint32_t ni = 0; ni < head.biClrUsed; ni++)
        pal[ni].rgbBlue = pal[ni].rgbGreen = pal[ni].rgbRed = (uint8_t)(ni * (255 / (head.biClrUsed - 1)));
}
void CxImage::SetPalette(uint32_t n, uint8_t* r, uint8_t* g, uint8_t* b)
{
    if ((!r) || (pDib == NULL) || (head.biClrUsed == 0)) return;
    if (!g) g = r;
    if (!b) b = g;
    RGBQUAD* ppal = GetPalette();
    uint32_t m = min(n,head.biClrUsed);
    for (uint32_t i = 0; i < m; i++)
    {
        ppal[i].rgbRed   = r[i];
        ppal[i].rgbGreen = g[i];
        ppal[i].rgbBlue  = b[i];
    }
    info.last_c_isvalid = false;
}
