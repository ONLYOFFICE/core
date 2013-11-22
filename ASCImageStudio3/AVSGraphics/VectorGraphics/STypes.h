#ifndef _STYPES_H
#define _STYPES_H

//-------------------------------------------------------------------------------------------------------------------------------
// Antialiasing
//-------------------------------------------------------------------------------------------------------------------------------

#define AntiAliasingSize 4 

//-------------------------------------------------------------------------------------------------------------------------------
// Цвета
//-------------------------------------------------------------------------------------------------------------------------------

enum SColorMode 
{
	colorModeMono1, // 1 bit per component,  1 bit   per pixel
	colorModeMono8, // 1 byte per component, 1 byte  per pixel
	colorModeRGB8,  // 1 byte per component, 3 bytes per pixel: RGBRGB...
	colorModeBGR8,  // 1 byte per component, 3 bytes per pixel: BGRBGR...
	colorModeBGRA8,	// 1 byte per component + alpha, 4 bytes per pixel: BGRABGRA...
	colorModeRGBA8	// 1 byte per component + alpha, 4 bytes per pixel: RGBARGBA...
};

static int SColorModeNComps[] = 
{
	1, // 1 bits per pixel
	1, // 8 bits per pixel
	3, // 24 bits per pixel RGBRGBRGB...
	3, // 24 bits per pixel BGRBGRBGR...
	4  // 32 bits per pixel CMYK
};


#define MaxColorComps 4

typedef unsigned char SColor[MaxColorComps];
typedef unsigned char *SColorPointer;

static inline void ColorCopy(SColorPointer pDst, SColorPointer pSrc) 
{
	pDst[0] = pSrc[0];
	pDst[1] = pSrc[1];
	pDst[2] = pSrc[2];
}

static inline void ColorXor(SColorPointer pDst, SColorPointer pSrc) 
{
	pDst[0] ^= pSrc[0];
	pDst[1] ^= pSrc[1];
	pDst[2] ^= pSrc[2];
}

//-------------------------------------------------------------------------------------------------------------------------------
// blend functions
//-------------------------------------------------------------------------------------------------------------------------------

typedef void (*SBlendFunc)(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode);

//-------------------------------------------------------------------------------------------------------------------------------
// Параметры экрана
//-------------------------------------------------------------------------------------------------------------------------------

enum SScreenType 
{
	sscreenDispersed,
	sscreenClustered,
	sscreenStochasticClustered
};

struct SScreenParams 
{
	SScreenType eType;
	int         nSize;
	int         nDotRadius;
	double      dGamma;
	double      dBlackThreshold;
	double      dWhiteThreshold;
};

enum SPipeResultColorCtrl 
{
	PipeResultColorNoAlphaBlendRGB,
	PipeResultColorNoAlphaBlendMono,
	PipeResultColorAlphaNoBlendMono,
	PipeResultColorAlphaNoBlendRGB,
	PipeResultColorAlphaBlendMono,
	PipeResultColorAlphaBlendRGB,
	PipeResultColorAlphaBlendRGBA
};

#endif /* _STYPES_H */
