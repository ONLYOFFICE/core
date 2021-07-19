﻿// ximage.cpp : main implementation file
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 7.0.2 07/Feb/2011
 */

#include "ximage.h"

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

////////////////////////////////////////////////////////////////////////////////
// CxImage 
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the internal structures
 */
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
////////////////////////////////////////////////////////////////////////////////
/**
 * Empty image constructor
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(uint32_t imagetype)
{
	Startup(imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Call this function to destroy image pixels, alpha channel, selection and sub layers.
 * - Attributes are not erased, but IsValid returns false.
 *
 * \return true if everything is freed, false if the image is a Ghost
 */
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
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
/**
 * Sized image constructor
 * \param dwWidth: width
 * \param dwHeight: height
 * \param wBpp: bit per pixel, can be 1, 4, 8, 24
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(uint32_t dwWidth, uint32_t dwHeight, uint32_t wBpp, uint32_t imagetype)
{
	Startup(imagetype);
	Create(dwWidth,dwHeight,wBpp,imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * image constructor from existing source
 * \param src: source image.
 * \param copypixels: copy the pixels from the source image into the new image.
 * \param copyselection: copy the selection from source
 * \param copyalpha: copy the alpha channel from source
 * \sa Copy
 */
CxImage::CxImage(const CxImage &src, bool copypixels, bool copyselection, bool copyalpha)
{
	Startup(src.GetType());
	Copy(src,copypixels,copyselection,copyalpha);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Copies the image from an exsisting source
 * \param src: source image.
 * \param copypixels: copy the pixels from the source image into the new image.
 * \param copyselection: copy the selection from source
 * \param copyalpha: copy the alpha channel from source
 */
void CxImage::Copy(const CxImage &src, bool copypixels, bool copyselection, bool copyalpha)
{
	// if the source is a ghost, the copy is still a ghost
	if (src.info.pGhost){
		Ghost(&src);
		return;
	}
	//copy the attributes
	memcpy(&info,&src.info,sizeof(CXIMAGEINFO));
	memcpy(&head,&src.head,sizeof(BITMAPINFOHEADER)); // [andy] - fix for bitmap header DPI
	//rebuild the image
	Create(src.GetWidth(),src.GetHeight(),src.GetBpp(),src.GetType());
	//copy the pixels and the palette, or at least copy the palette only.
	if (copypixels && pDib && src.pDib) memcpy(pDib,src.pDib,GetSize());
	else SetPalette(src.GetPalette());
	int32_t nSize = head.biWidth * head.biHeight;
	//copy the selection
	if (copyselection && src.pSelection){
		if (pSelection) free(pSelection);
		pSelection = (uint8_t*)malloc(nSize);
		memcpy(pSelection,src.pSelection,nSize);
	}
	//copy the alpha channel
	if (copyalpha && src.pAlpha){
		if (pAlpha) free(pAlpha);
		pAlpha = (uint8_t*)malloc(nSize);
		memcpy(pAlpha,src.pAlpha,nSize);
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Copies the image attributes from an existing image.
 * - Works only on an empty image, and the image will be still empty.
 * - <b> Use it before Create() </b>
 */
void CxImage::CopyInfo(const CxImage &src)
{
	if (pDib==NULL) memcpy(&info,&src.info,sizeof(CXIMAGEINFO));
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \sa Copy
 */
CxImage& CxImage::operator = (const CxImage& isrc)
{
	if (this != &isrc) Copy(isrc);
	return *this;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Initializes or rebuilds the image.
 * \param dwWidth: width
 * \param dwHeight: height
 * \param wBpp: bit per pixel, can be 1, 4, 8, 24
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 * \return pointer to the internal pDib object; NULL if an error occurs.
 */
void* CxImage::Create(uint32_t dwWidth, uint32_t dwHeight, uint32_t wBpp, uint32_t imagetype)
{
	// destroy the existing image (if any)
	if (!Destroy())
		return NULL;

	// prevent further actions if width or height are not vaild <Balabasnia>
	if ((dwWidth == 0) || (dwHeight == 0)){
		strcpy(info.szLastError,"CxImage::Create : width and height must be greater than zero");
		return NULL;
	}

    // Make sure bits per pixel is valid
    if		(wBpp <= 1)	wBpp = 1;
    else if (wBpp <= 4)	wBpp = 4;
    else if (wBpp <= 8)	wBpp = 8;
    else				wBpp = 24;

	// limit memory requirements
	if ((((float)dwWidth*(float)dwHeight*(float)wBpp)/8.0f) > (float)CXIMAGE_MAX_MEMORY)
	{
		strcpy(info.szLastError,"CXIMAGE_MAX_MEMORY exceeded");
		return NULL;
	}

	// set the correct bpp value
    switch (wBpp){
        case 1:
            head.biClrUsed = 2;	break;
        case 4:
            head.biClrUsed = 16; break;
        case 8:
            head.biClrUsed = 256; break;
        default:
            head.biClrUsed = 0;
    }

	//set the common image informations
    info.dwEffWidth = ((((wBpp * dwWidth) + 31) / 32) * 4);
    info.dwType = imagetype;

    // initialize BITMAPINFOHEADER
	head.biSize = sizeof(BITMAPINFOHEADER); //<ralphw>
    head.biWidth = dwWidth;		// fill in width from parameter
    head.biHeight = dwHeight;	// fill in height from parameter
    head.biPlanes = 1;			// must be 1
    head.biBitCount = (uint16_t)wBpp;		// from parameter
    head.biCompression = BI_RGB;    
    head.biSizeImage = info.dwEffWidth * dwHeight;
//    head.biXPelsPerMeter = 0; See SetXDPI
//    head.biYPelsPerMeter = 0; See SetYDPI
//    head.biClrImportant = 0;  See SetClrImportant

	pDib = malloc(GetSize()); // alloc memory block to store our bitmap
    if (!pDib){
		strcpy(info.szLastError,"CxImage::Create can't allocate memory");
		return NULL;
	}
    pDibLimit = (void*)((uint8_t*)pDib + GetSize());

	//clear the palette
	RGBQUAD* pal=GetPalette();
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
////////////////////////////////////////////////////////////////////////////////
/**
 * \return pointer to the image pixels. <b> USE CAREFULLY </b>
 */
uint8_t* CxImage::GetBits(uint32_t row)
{ 
	if (pDib){
		if (row) {
			if (row<(uint32_t)head.biHeight){
				return ((uint8_t*)pDib + *(uint32_t*)pDib + GetPaletteSize() + (info.dwEffWidth * row));
			} else {
				return NULL;
			}
		} else {
			return ((uint8_t*)pDib + *(uint32_t*)pDib + GetPaletteSize());
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return the size in bytes of the internal pDib object
 */
uint32_t CxImage::GetSize()
{
    uint64_t size64 = head.biSize + head.biSizeImage + GetPaletteSize();
    if (size64 > 0xFFFFFFFF)
        return 0xFFFFFFFF;
    return (uint32_t)size64;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the coordinates are inside the image
 * \return true if x and y are both inside the image
 */
bool CxImage::IsInside(int32_t x, int32_t y)
{
  return (0<=y && y<head.biHeight && 0<=x && x<head.biWidth);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the image bits to the specified value
 * - for indexed images, the output color is set by the palette entries.
 * - for RGB images, the output color is a shade of gray.
 */
void CxImage::Clear(uint8_t bval)
{
	if (pDib == 0) return;

	if (GetBpp() == 1){
		if (bval > 0) bval = 255;
	}
	if (GetBpp() == 4){
		bval = (uint8_t)(17*(0x0F & bval));
	}

	memset(info.pImage,bval,head.biSizeImage);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Transfers the image from an existing source image. The source becomes empty.
 * \return true if everything is ok
 */
bool CxImage::Transfer(CxImage &from, bool bTransferFrames /*=true*/)
{
	if (!Destroy())
		return false;

	memcpy(&head,&from.head,sizeof(BITMAPINFOHEADER));
	memcpy(&info,&from.info,sizeof(CXIMAGEINFO));

	pDib = from.pDib;
    pDibLimit = from.pDibLimit;
	pSelection = from.pSelection;
	pAlpha = from.pAlpha;
	ppLayers = from.ppLayers;

	memset(&from.head,0,sizeof(BITMAPINFOHEADER));
	memset(&from.info,0,sizeof(CXIMAGEINFO));
    from.pDib = from.pDibLimit = from.pSelection = from.pAlpha = NULL;
	from.ppLayers = NULL;

	if (bTransferFrames){
		DestroyFrames();
		ppFrames = from.ppFrames;
		from.ppFrames = NULL;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * (this) points to the same pDib owned by (*from), the image remains in (*from)
 * but (this) has the access to the pixels. <b>Use carefully !!!</b>
 */
void CxImage::Ghost(const CxImage *from)
{
	if (from){
		memcpy(&head,&from->head,sizeof(BITMAPINFOHEADER));
		memcpy(&info,&from->info,sizeof(CXIMAGEINFO));
		pDib = from->pDib;
        pDibLimit = from->pDibLimit;
		pSelection = from->pSelection;
		pAlpha = from->pAlpha;
		ppLayers = from->ppLayers;
		ppFrames = from->ppFrames;
		info.pGhost=(CxImage *)from;
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * turns a 16 or 32 bit bitfield image into a RGB image
 */
void CxImage::Bitfield2RGB(uint8_t *src, uint32_t redmask, uint32_t greenmask, uint32_t bluemask, uint8_t bpp)
{
	switch (bpp){
	case 16:
	{
		uint32_t ns[3]={0,0,0};
		// compute the number of shift for each mask
		for (int32_t i=0;i<16;i++){
			if ((redmask>>i)&0x01) ns[0]++;
			if ((greenmask>>i)&0x01) ns[1]++;
			if ((bluemask>>i)&0x01) ns[2]++;
		}
		ns[1]+=ns[0]; ns[2]+=ns[1];	ns[0]=8-ns[0]; ns[1]-=8; ns[2]-=8;
		// dword aligned width for 16 bit image
		int32_t effwidth2=(((head.biWidth + 1) / 2) * 4);
		uint16_t w;
		int32_t y2,y3,x2,x3;
		uint8_t *p=info.pImage;
		// scan the buffer in reverse direction to avoid reallocations
		for (int32_t y=head.biHeight-1; y>=0; y--){
			y2=effwidth2*y;
			y3=info.dwEffWidth*y;
			for (int32_t x=head.biWidth-1; x>=0; x--){
				x2 = 2*x+y2;
				x3 = 3*x+y3;
				w = (uint16_t)(src[x2]+256*src[1+x2]);
				p[  x3]=(uint8_t)((w & bluemask)<<ns[0]);
				p[1+x3]=(uint8_t)((w & greenmask)>>ns[1]);
				p[2+x3]=(uint8_t)((w & redmask)>>ns[2]);
			}
		}
		break;
	}
	case 32:
	{
		uint32_t ns[3]={0,0,0};
		// compute the number of shift for each mask
		for (int32_t i=8;i<32;i+=8){
			if (redmask>>i) ns[0]++;
			if (greenmask>>i) ns[1]++;
			if (bluemask>>i) ns[2]++;
		}
		// dword aligned width for 32 bit image
		int32_t effwidth4 = head.biWidth * 4;
		int32_t y4,y3,x4,x3;
		uint8_t *p=info.pImage;
		// scan the buffer in reverse direction to avoid reallocations
		for (int32_t y=head.biHeight-1; y>=0; y--){
			y4=effwidth4*y;
			y3=info.dwEffWidth*y;
			for (int32_t x=head.biWidth-1; x>=0; x--){
				x4 = 4*x+y4;
				x3 = 3*x+y3;
				p[  x3]=src[ns[2]+x4];
				p[1+x3]=src[ns[1]+x4];
				p[2+x3]=src[ns[0]+x4];
			}
		}
	}

	}
	return;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Creates an image from a generic buffer
 * \param pArray: source memory buffer
 * \param dwWidth: image width
 * \param dwHeight: image height
 * \param dwBitsperpixel: can be 1,4,8,24,32
 * \param dwBytesperline: line alignment, in bytes, for a single row stored in pArray
 * \param bFlipImage: tune this parameter if the image is upsidedown
 * \return true if everything is ok
 */
bool CxImage::CreateFromArray(uint8_t* pArray,uint32_t dwWidth,uint32_t dwHeight,uint32_t dwBitsperpixel, uint32_t dwBytesperline, bool bFlipImage, bool isBGRA)
{
	if (pArray==NULL) return false;
	if (!((dwBitsperpixel==1)||(dwBitsperpixel==4)||(dwBitsperpixel==8)||
		(dwBitsperpixel==24)||(dwBitsperpixel==32))) return false;

	if (!Create(dwWidth,dwHeight,dwBitsperpixel)) return false;

	if (dwBitsperpixel<24) SetGrayPalette();

#if CXIMAGE_SUPPORT_ALPHA
	if (dwBitsperpixel==32) AlphaCreate();
#endif //CXIMAGE_SUPPORT_ALPHA

	uint8_t *dst,*src;

    unsigned char ShiftR = isBGRA ? 2 : 0;
    unsigned char ShiftG = 1;
    unsigned char ShiftB = isBGRA ? 0 : 2;

	for (uint32_t y = 0; y<dwHeight; y++) {
		dst = info.pImage + (bFlipImage?(dwHeight-1-y):y) * info.dwEffWidth;
		src = pArray + y * dwBytesperline;
		if (dwBitsperpixel==32){
			for(uint32_t x=0;x<dwWidth;x++){
                *dst++=src[ShiftB];
                *dst++=src[ShiftG];
                *dst++=src[ShiftR];
#if CXIMAGE_SUPPORT_ALPHA
				AlphaSet(x,(bFlipImage?(dwHeight-1-y):y),src[3]);
#endif //CXIMAGE_SUPPORT_ALPHA
				src+=4;
			}
		} else {
			memcpy(dst,src,min(info.dwEffWidth,dwBytesperline));
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \sa CreateFromArray
 */
bool CxImage::CreateFromMatrix(uint8_t** ppMatrix,uint32_t dwWidth,uint32_t dwHeight,uint32_t dwBitsperpixel, uint32_t dwBytesperline, bool bFlipImage)
{
	if (ppMatrix==NULL) return false;
	if (!((dwBitsperpixel==1)||(dwBitsperpixel==4)||(dwBitsperpixel==8)||
		(dwBitsperpixel==24)||(dwBitsperpixel==32))) return false;

	if (!Create(dwWidth,dwHeight,dwBitsperpixel)) return false;

	if (dwBitsperpixel<24) SetGrayPalette();

#if CXIMAGE_SUPPORT_ALPHA
	if (dwBitsperpixel==32) AlphaCreate();
#endif //CXIMAGE_SUPPORT_ALPHA

	uint8_t *dst,*src;

	for (uint32_t y = 0; y<dwHeight; y++) {
		dst = info.pImage + (bFlipImage?(dwHeight-1-y):y) * info.dwEffWidth;
		src = ppMatrix[y];
		if (src){
			if (dwBitsperpixel==32){
				for(uint32_t x=0;x<dwWidth;x++){
					*dst++=src[0];
					*dst++=src[1];
					*dst++=src[2];
#if CXIMAGE_SUPPORT_ALPHA
					AlphaSet(x,(bFlipImage?(dwHeight-1-y):y),src[3]);
#endif //CXIMAGE_SUPPORT_ALPHA
					src+=4;
				}
			} else {
				memcpy(dst,src,min(info.dwEffWidth,dwBytesperline));
			}
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return lightness difference between elem1 and elem2
 */
int32_t CxImage::CompareColors(const void *elem1, const void *elem2)
{
	RGBQUAD* c1 = (RGBQUAD*)elem1;
	RGBQUAD* c2 = (RGBQUAD*)elem2;

	int32_t g1 = (int32_t)RGB2GRAY(c1->rgbRed,c1->rgbGreen,c1->rgbBlue);
	int32_t g2 = (int32_t)RGB2GRAY(c2->rgbRed,c2->rgbGreen,c2->rgbBlue);
	
	return (g1-g2);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * simply calls "if (memblock) free(memblock);".
 * Useful when calling Encode for a memory buffer,
 * from a DLL compiled with different memory management options.
 * CxImage::FreeMemory will use the same memory environment used by Encode. 
 * \author [livecn]
 */
void CxImage::FreeMemory(void* memblock)
{
	if (memblock)
		free(memblock);
}
////////////////////////////////////////////////////////////////////////////////
//EOF
