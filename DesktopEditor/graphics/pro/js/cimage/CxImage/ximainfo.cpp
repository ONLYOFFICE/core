#include "ximage.h"

void CxImage::SetXDPI(int32_t dpi)
{
	if (dpi<=0) dpi = CXIMAGE_DEFAULT_DPI;
	info.xDPI = dpi;
	head.biXPelsPerMeter = (int32_t) floor(dpi * 10000.0 / 254.0 + 0.5);
	if (pDib) ((BITMAPINFOHEADER*)pDib)->biXPelsPerMeter = head.biXPelsPerMeter;
}
void CxImage::SetYDPI(int32_t dpi)
{
	if (dpi<=0) dpi = CXIMAGE_DEFAULT_DPI;
	info.yDPI = dpi;
	head.biYPelsPerMeter = (int32_t) floor(dpi * 10000.0 / 254.0 + 0.5);
	if (pDib) ((BITMAPINFOHEADER*)pDib)->biYPelsPerMeter = head.biYPelsPerMeter;
}
const char* CxImage::GetLastError()
{
    return info.szLastError;
}
