#include "ximage.h"

void CxImage::Startup(uint32_t imagetype)
{
    //init pointers
    pDib = pDibLimit = pSelection = pAlpha = NULL;
    ppLayers = ppFrames = NULL;
    //init structures
    memset(&head, 0, sizeof(BITMAPINFOHEADER));
    memset(&info, 0, sizeof(CXIMAGEINFO));
    //init default attributes
    info.dwType = imagetype;
    info.fQuality  = 90.0f;
    info.nAlphaMax = 255;
    info.nBkgndIndex = -1;
    info.bEnabled    = true;
    info.nJpegScale  = 1;
    SetXDPI(CXIMAGE_DEFAULT_DPI);
    SetYDPI(CXIMAGE_DEFAULT_DPI);

    int16_t test = 1;
    info.bLittleEndianHost = (*((char *) &test) == 1);
}
CxImage::CxImage(uint32_t imagetype)
{
    Startup(imagetype);
}
bool CxImage::Destroy()
{
    //free this only if it's valid and it's not a ghost
    if (info.pGhost==NULL)
    {
        if (ppLayers)
        {
            for (int32_t n = 0; n < info.nNumLayers; n++) delete ppLayers[n];
            delete[] ppLayers; ppLayers = 0; info.nNumLayers = 0;
        }
        if (pSelection) { free(pSelection); pSelection=0; }
        if (pAlpha) { free(pAlpha); pAlpha=0; }
        if (pDib) { free(pDib); pDib=0; }
        return true;
    }
    return false;
}
bool CxImage::DestroyFrames()
{
    if (info.pGhost==NULL)
    {
        if (ppFrames)
        {
            for (int32_t n = 0; n < info.nNumFrames; n++) delete ppFrames[n];
            delete[] ppFrames; ppFrames = NULL; info.nNumFrames = 0;
        }
        return true;
    }
    return false;
}
void CxImage::CopyInfo(const CxImage &src)
{
    if (pDib == NULL) memcpy(&info, &src.info, sizeof(CXIMAGEINFO));
}
void* CxImage::Create(uint32_t dwWidth, uint32_t dwHeight, uint32_t wBpp, uint32_t imagetype)
{
    // destroy the existing image (if any)
    if (!Destroy())
        return NULL;

    // prevent further actions if width or height are not vaild <Balabasnia>
    if ((dwWidth == 0) || (dwHeight == 0))
    {
        strcpy(info.szLastError, "CxImage::Create : width and height must be greater than zero");
        return NULL;
    }

    // Make sure bits per pixel is valid
    if      (wBpp <= 1) wBpp = 1;
    else if (wBpp <= 4) wBpp = 4;
    else if (wBpp <= 8) wBpp = 8;
    else                wBpp = 24;

    // limit memory requirements
    if ((((float)dwWidth * (float)dwHeight * (float)wBpp) / 8.0f) > (float)CXIMAGE_MAX_MEMORY)
    {
        strcpy(info.szLastError, "CXIMAGE_MAX_MEMORY exceeded");
        return NULL;
    }

    // set the correct bpp value
    switch (wBpp)
    {
        case 1: head.biClrUsed = 2; break;
        case 4: head.biClrUsed = 16; break;
        case 8: head.biClrUsed = 256; break;
        default: head.biClrUsed = 0;
    }

    //set the common image informations
    info.dwEffWidth = ((((wBpp * dwWidth) + 31) / 32) * 4);
    info.dwType = imagetype;

    // initialize BITMAPINFOHEADER
    head.biSize = sizeof(BITMAPINFOHEADER); //<ralphw>
    head.biWidth = dwWidth;     // fill in width from parameter
    head.biHeight = dwHeight;   // fill in height from parameter
    head.biPlanes = 1;          // must be 1
    head.biBitCount = (uint16_t)wBpp;       // from parameter
    head.biCompression = BI_RGB;
    head.biSizeImage = info.dwEffWidth * dwHeight;

    pDib = malloc(GetSize()); // alloc memory block to store our bitmap
    if (!pDib)
    {
        strcpy(info.szLastError, "CxImage::Create can't allocate memory");
        return NULL;
    }
    pDibLimit = (void*)((uint8_t*)pDib + GetSize());

    //clear the palette
    RGBQUAD* pal = GetPalette();
    if (pal) memset(pal,0,GetPaletteSize());
    //Destroy the existing selection
#if CXIMAGE_SUPPORT_SELECTION
    if (pSelection) SelectionDelete();
#endif //CXIMAGE_SUPPORT_SELECTION
    //Destroy the existing alpha channel
#if CXIMAGE_SUPPORT_ALPHA
    if (pAlpha) AlphaDelete();
#endif //CXIMAGE_SUPPORT_ALPHA

    // use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
    BITMAPINFOHEADER*  lpbi;
    lpbi = (BITMAPINFOHEADER*)(pDib);
    *lpbi = head;

    info.pImage=GetBits();

    return pDib; //return handle to the DIB
}
uint8_t* CxImage::GetBits(uint32_t row)
{
    if (pDib)
    {
        if (row)
        {
            if (row < (uint32_t)head.biHeight)
                return ((uint8_t*)pDib + *(uint32_t*)pDib + GetPaletteSize() + (info.dwEffWidth * row));
            return NULL;
        }
        return ((uint8_t*)pDib + *(uint32_t*)pDib + GetPaletteSize());
    }
    return NULL;
}
uint32_t CxImage::GetSize()
{
    uint64_t size64 = head.biSize + head.biSizeImage + GetPaletteSize();
    if (size64 > 0xFFFFFFFF)
        return 0xFFFFFFFF;
    return (uint32_t)size64;
}
bool CxImage::IsInside(int32_t x, int32_t y)
{
  return (0 <= y && y < head.biHeight && 0 <= x && x < head.biWidth);
}
bool CxImage::Transfer(CxImage &from, bool bTransferFrames)
{
    if (!Destroy())
        return false;

    memcpy(&head, &from.head, sizeof(BITMAPINFOHEADER));
    memcpy(&info, &from.info, sizeof(CXIMAGEINFO));

    pDib       = from.pDib;
    pDibLimit  = from.pDibLimit;
    pSelection = from.pSelection;
    pAlpha     = from.pAlpha;
    ppLayers   = from.ppLayers;

    memset(&from.head, 0, sizeof(BITMAPINFOHEADER));
    memset(&from.info, 0, sizeof(CXIMAGEINFO));
    from.pDib = from.pDibLimit = from.pSelection = from.pAlpha = NULL;
    from.ppLayers = NULL;

    if (bTransferFrames)
    {
        DestroyFrames();
        ppFrames = from.ppFrames;
        from.ppFrames = NULL;
    }

    return true;
}
void CxImage::Ghost(const CxImage* from)
{
    if (from)
    {
        memcpy(&head, &from->head, sizeof(BITMAPINFOHEADER));
        memcpy(&info, &from->info, sizeof(CXIMAGEINFO));
        pDib = from->pDib;
        pDibLimit  = from->pDibLimit;
        pSelection = from->pSelection;
        pAlpha   = from->pAlpha;
        ppLayers = from->ppLayers;
        ppFrames = from->ppFrames;
        info.pGhost = (CxImage*)from;
    }
}
