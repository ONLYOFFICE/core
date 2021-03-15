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
RGBQUAD CxImage::GetPaletteColor(uint8_t idx)
{
    RGBQUAD rgb = {0, 0, 0, 0};
    if ((pDib) && (head.biClrUsed))
    {
        uint8_t* iDst = (uint8_t*)(pDib) + sizeof(BITMAPINFOHEADER);
        if (idx < head.biClrUsed)
        {
            int32_t ldx  = idx * sizeof(RGBQUAD);
            rgb.rgbBlue  = iDst[ldx++];
            rgb.rgbGreen = iDst[ldx++];
            rgb.rgbRed   = iDst[ldx++];
            rgb.rgbReserved = iDst[ldx];
        }
    }
    return rgb;
}
uint8_t CxImage::BlindGetPixelIndex(const int32_t x, const int32_t y)
{
#ifdef _DEBUG
    if ((pDib == NULL) || (head.biClrUsed == 0) || !IsInside(x,y))
    #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
        throw 0;
    #else
        return 0;
    #endif // CXIMAGE_SUPPORT_EXCEPTION_HANDLING
#endif // _DEBUG

    if (head.biBitCount == 8)
        return info.pImage[y * info.dwEffWidth + x];

    uint8_t pos;
    uint8_t iDst = info.pImage[y * info.dwEffWidth + (x * head.biBitCount >> 3)];
    if (head.biBitCount == 4)
    {
        pos = (uint8_t)(4 * (1 - x % 2));
        iDst &= (0x0F << pos);
        return (uint8_t)(iDst >> pos);
    }
    else if (head.biBitCount==1)
    {
        pos = (uint8_t)(7 - x % 8);
        iDst &= (0x01 << pos);
        return (uint8_t)(iDst >> pos);
    }
    return 0;
}
RGBQUAD CxImage::BlindGetPixelColor(const int32_t x, const int32_t y, bool bGetAlpha)
{
    RGBQUAD rgb;
#ifdef _DEBUG
    if ((pDib == NULL) || !IsInside(x,y))
    #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
        throw 0;
    #else
        { rgb.rgbReserved = 0; return rgb; }
    #endif // CXIMAGE_SUPPORT_EXCEPTION_HANDLING
#endif // _DEBUG

    if (head.biClrUsed)
        rgb = GetPaletteColor(BlindGetPixelIndex(x,y));
    else
    {
        uint8_t* iDst = info.pImage + y * info.dwEffWidth + x * 3;
        rgb.rgbBlue  = *iDst++;
        rgb.rgbGreen = *iDst++;
        rgb.rgbRed   = *iDst;
        rgb.rgbReserved = 0; //needed for images without alpha layer
    }
#if CXIMAGE_SUPPORT_ALPHA
    if (pAlpha && bGetAlpha) rgb.rgbReserved = BlindAlphaGet(x,y);
#else
    rgb.rgbReserved = 0;
#endif //CXIMAGE_SUPPORT_ALPHA
    return rgb;
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
bool CxImage::IsGrayScale()
{
    RGBQUAD* ppal = GetPalette();
    if (!(pDib && ppal && head.biClrUsed)) return false;
    for (uint32_t i = 0; i < head.biClrUsed; i++)
        if (ppal[i].rgbBlue != i || ppal[i].rgbGreen != i || ppal[i].rgbRed != i)
            return false;
    return true;
}
