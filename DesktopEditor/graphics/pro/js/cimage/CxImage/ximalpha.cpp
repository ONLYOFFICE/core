#include "ximage.h"

#if CXIMAGE_SUPPORT_ALPHA
void CxImage::AlphaDelete()
{
    if (pAlpha) { free(pAlpha); pAlpha=0; }
}
uint8_t CxImage::BlindAlphaGet(const int32_t x, const int32_t y)
{
#ifdef _DEBUG
    if (!IsInside(x,y) || (pAlpha == 0))
    #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
        throw 0;
    #else
        return 0;
    #endif
#endif
    return pAlpha[x + y * head.biWidth];
}
#endif //CXIMAGE_SUPPORT_ALPHA
