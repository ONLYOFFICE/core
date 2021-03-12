#include "ximage.h"

#if CXIMAGE_SUPPORT_ALPHA

void CxImage::AlphaDelete()
{
	if (pAlpha) { free(pAlpha); pAlpha=0; }
}

#endif //CXIMAGE_SUPPORT_ALPHA
