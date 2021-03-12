#include "ximage.h"

#if CXIMAGE_SUPPORT_SELECTION

bool CxImage::SelectionDelete()
{
    if (pSelection)
    {
		free(pSelection);
        pSelection = NULL;
	}
    info.rSelectionBox.left   = head.biWidth;
	info.rSelectionBox.bottom = head.biHeight;
    info.rSelectionBox.right  = info.rSelectionBox.top = 0;
	return true;
}

#endif //CXIMAGE_SUPPORT_SELECTION
