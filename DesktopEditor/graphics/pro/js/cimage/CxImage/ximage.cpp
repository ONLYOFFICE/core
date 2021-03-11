#include "ximage.h"
void CxImage::Startup(uint32_t imagetype)
{
    //init pointers
    pDib = pDibLimit = pSelection = pAlpha = NULL;
    ppLayers = ppFrames = NULL;
    //init structures
    memset(&head,0,sizeof(BITMAPINFOHEADER));
    memset(&info,0,sizeof(CXIMAGEINFO));
    //init default attributes
    info.dwType = imagetype;
    info.fQuality = 90.0f;
    info.nAlphaMax = 255;
    info.nBkgndIndex = -1;
    info.bEnabled = true;
    info.nJpegScale = 1;
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
	if (info.pGhost==NULL){
		if (ppLayers) { 
			for(int32_t n=0; n<info.nNumLayers;n++){ delete ppLayers[n]; }
			delete [] ppLayers; ppLayers=0; info.nNumLayers = 0;
		}
		if (pSelection) {free(pSelection); pSelection=0;}
		if (pAlpha) {free(pAlpha); pAlpha=0;}
		if (pDib) {free(pDib); pDib=0;}
		return true;
	}
	return false;
}
bool CxImage::DestroyFrames()
{
	if (info.pGhost==NULL) {
		if (ppFrames) {
			for (int32_t n=0; n<info.nNumFrames; n++) { delete ppFrames[n]; }
			delete [] ppFrames; ppFrames = NULL; info.nNumFrames = 0;
		}
		return true;
	}
	return false;
}
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
