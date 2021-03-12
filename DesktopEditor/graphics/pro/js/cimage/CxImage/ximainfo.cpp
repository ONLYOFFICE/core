#include "ximage.h"

int32_t CxImage::GetXDPI() const
{
    return info.xDPI;
}
int32_t CxImage::GetYDPI() const
{
    return info.yDPI;
}
void CxImage::SetXDPI(int32_t dpi)
{
    if (dpi <= 0) dpi = CXIMAGE_DEFAULT_DPI;
    info.xDPI = dpi;
    head.biXPelsPerMeter = (int32_t) floor(dpi * 10000.0 / 254.0 + 0.5);
    if (pDib) ((BITMAPINFOHEADER*)pDib)->biXPelsPerMeter = head.biXPelsPerMeter;
}
void CxImage::SetYDPI(int32_t dpi)
{
    if (dpi <= 0) dpi = CXIMAGE_DEFAULT_DPI;
    info.yDPI = dpi;
    head.biYPelsPerMeter = (int32_t) floor(dpi * 10000.0 / 254.0 + 0.5);
    if (pDib) ((BITMAPINFOHEADER*)pDib)->biYPelsPerMeter = head.biYPelsPerMeter;
}
uint32_t CxImage::GetCodecOption(uint32_t imagetype)
{
    imagetype = GetTypeIndexFromId(imagetype);
    if (imagetype == 0)
        imagetype = GetTypeIndexFromId(GetType());
    return info.dwCodecOpt[imagetype];
}
uint32_t CxImage::GetHeight() const
{
    return head.biHeight;
}
uint32_t CxImage::GetWidth() const
{
    return head.biWidth;
}
uint32_t CxImage::GetEffWidth() const
{
    return info.dwEffWidth;
}
uint32_t CxImage::GetType() const
{
    return info.dwType;
}
uint32_t CxImage::GetTypeIndexFromId(const uint32_t id)
{
    uint32_t n;
    n=0; if (id == CXIMAGE_FORMAT_UNKNOWN) return n;
#if CXIMAGE_SUPPORT_BMP
    n++; if (id == CXIMAGE_FORMAT_BMP) return n;
#endif
#if CXIMAGE_SUPPORT_GIF
    n++; if (id == CXIMAGE_FORMAT_GIF) return n;
#endif
#if CXIMAGE_SUPPORT_JPG
    n++; if (id == CXIMAGE_FORMAT_JPG) return n;
#endif
#if CXIMAGE_SUPPORT_PNG
    n++; if (id == CXIMAGE_FORMAT_PNG) return n;
#endif
#if CXIMAGE_SUPPORT_ICO
    n++; if (id == CXIMAGE_FORMAT_ICO) return n;
#endif
#if CXIMAGE_SUPPORT_TIF
    n++; if (id == CXIMAGE_FORMAT_TIF) return n;
#endif
#if CXIMAGE_SUPPORT_TGA
    n++; if (id == CXIMAGE_FORMAT_TGA) return n;
#endif
#if CXIMAGE_SUPPORT_PCX
    n++; if (id == CXIMAGE_FORMAT_PCX) return n;
#endif
#if CXIMAGE_SUPPORT_WBMP
    n++; if (id == CXIMAGE_FORMAT_WBMP) return n;
#endif
#if CXIMAGE_SUPPORT_WMF
    n++; if (id == CXIMAGE_FORMAT_WMF) return n;
#endif
#if CXIMAGE_SUPPORT_JP2
    n++; if (id == CXIMAGE_FORMAT_JP2) return n;
#endif
#if CXIMAGE_SUPPORT_JPC
    n++; if (id == CXIMAGE_FORMAT_JPC) return n;
#endif
#if CXIMAGE_SUPPORT_PGX
    n++; if (id == CXIMAGE_FORMAT_PGX) return n;
#endif
#if CXIMAGE_SUPPORT_PNM
    n++; if (id == CXIMAGE_FORMAT_PNM) return n;
#endif
#if CXIMAGE_SUPPORT_RAS
    n++; if (id == CXIMAGE_FORMAT_RAS) return n;
#endif
#if CXIMAGE_SUPPORT_JBG
    n++; if (id == CXIMAGE_FORMAT_JBG) return n;
#endif
#if CXIMAGE_SUPPORT_MNG
    n++; if (id == CXIMAGE_FORMAT_MNG) return n;
#endif
#if CXIMAGE_SUPPORT_SKA
    n++; if (id == CXIMAGE_FORMAT_SKA) return n;
#endif
#if CXIMAGE_SUPPORT_RAW
    n++; if (id == CXIMAGE_FORMAT_RAW) return n;
#endif
#if CXIMAGE_SUPPORT_PSD
    n++; if (id == CXIMAGE_FORMAT_PSD) return n;
#endif

    return 0;
}
uint8_t CxImage::GetJpegQuality() const
{
    return (uint8_t)(info.fQuality + 0.5f);
}
uint8_t CxImage::GetJpegScale() const
{
    return info.nJpegScale;
}
const char* CxImage::GetLastError()
{
    return info.szLastError;
}
