// ASCGraphicsPath.h : Declaration of the AVSGraphicsPath

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "..\Interfaces\IASCGraphics.h"
#include "ASCMatrix.h"
#include "ASCGraphicsPath.h"
#include "ASCSolidPattern.h"
#include "ASCGlyphImage.h"

#include "..\VectorGraphics\STypes.h"
#include "..\Common\Clip.h"
#include "..\Common\Pixels.h"
#include "..\VectorGraphics\SScreen.h"
#include "..\VectorGraphics\SErrorCodes.h"
#include "..\VectorGraphics\SXPath.h"
#include "..\VectorGraphics\SXPathScanner.h"
#include "..\VectorGraphics\SMathExt.h"
#include "ASCFontManager.h"

#include "TimeMeasurer.h"

// Расстояние от центра до контрольных точек Безье для аппроксимации окружности = (4 * (sqrt(2) - 1) / 3) * r
static const double c_dKappa   = ((double)0.55228475);
static const double c_dKappa_2 = ((double)(0.5 * 0.55228475));

// Делим 16-битное значение [0, 255*255] на 255, возвращаем 8-битное значение.
static inline unsigned char Div255(int nValue) 
{
	return (unsigned char)((nValue + (nValue >> 8) + 0x80) >> 8);
}

//-------------------------------------------------------------------------------------------------------------------------------
// Blend functions
//-------------------------------------------------------------------------------------------------------------------------------

static void RGBToHSV(unsigned char unR, unsigned char unG, unsigned char unB, int *pnH, int *pnS, int *pnV) 
{
	int nCmax, nCmid, nCmin, nValue;

	if ( unR >= unG ) 
	{
		if      ( unG >= unB ) { nValue = 0; nCmax = unR; nCmid = unG; nCmin = unB; }
		else if ( unB >= unR ) { nValue = 4; nCmax = unB; nCmid = unR; nCmin = unG; }
		else                   { nValue = 5; nCmax = unR; nCmid = unB; nCmin = unG; }
	} 
	else 
	{
		if      ( unR >= unB ) { nValue = 1; nCmax = unG; nCmid = unR; nCmin = unB; }
		else if ( unG >= unB ) { nValue = 2; nCmax = unG; nCmid = unB; nCmin = unR; }
		else                   { nValue = 3; nCmax = unB; nCmid = unG; nCmin = unR; }
	}
	if ( nCmax == nCmin ) 
	{
		*pnH = *pnS = 0;
	} 
	else 
	{
		*pnH = nValue * 60;
		if ( nValue & 1 ) 
		{
			*pnH += ( ( nCmax - nCmid ) * 60 ) / ( nCmax - nCmin );
		} 
		else 
		{
			*pnH += ( ( nCmid - nCmin ) * 60 ) / ( nCmax - nCmin );
		}
		*pnS = ( 255 * ( nCmax - nCmin ) ) / nCmax;
	}
	*pnV = nCmax;
}

static void HSVToRGB(int nH, int nS, int nV, unsigned char *punR, unsigned char *punG, unsigned char *punB) 
{
	if ( nS == 0 ) 
	{
		*punR = *punG = *punB = nV;
	} 
	else 
	{
		int nInt   = nH / 60;
		int nFloat = nH % 60;
		int nCmax = nV, nCmid;
		if ( nInt & 1 ) 
		{
			nCmid = Div255( nV * 255 - ( ( nS * nFloat ) / 60 ) );
		} 
		else 
		{
			nCmid = Div255( nV * ( 255 - ( ( nS * ( 60 - nFloat ) ) / 60 ) ) );
		}
		int nCmin = Div255( nV * ( 255 - nS ) );
		switch (nInt) 
		{
		case 0: *punR = nCmax; *punG = nCmid; *punB = nCmin; break;
		case 1: *punG = nCmax; *punR = nCmid; *punB = nCmin; break;
		case 2: *punG = nCmax; *punB = nCmid; *punR = nCmin; break;
		case 3: *punB = nCmax; *punG = nCmid; *punR = nCmin; break;
		case 4: *punB = nCmax; *punR = nCmid; *punG = nCmin; break;
		case 5: *punR = nCmax; *punB = nCmid; *punG = nCmin; break;
		}
	}
}


static void BlendMultiply  (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = ( pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendScreen    (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] + pSrc[nIndex] - ( pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendOverlay   (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < 0x80 ? ( pSrc[nIndex] * 2 * pDst[nIndex] ) / 255 : 255 - 2 * ( ( 255 - pSrc[nIndex] ) * ( 255 - pDst[nIndex] ) ) / 255;
	}
}

static void BlendDarken    (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < pSrc[nIndex] ? pDst[nIndex] : pSrc[nIndex];
	}
}

static void BlendLighten   (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] > pSrc[nIndex] ? pDst[nIndex] : pSrc[nIndex];
	}
}

static void BlendColorDodge(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] == 255 ) 
		{
			pBlend[nIndex] = 255;
		} 
		else 
		{
			int nValue = ( pDst[nIndex] * 255 ) / ( 255 - pSrc[nIndex] );
			pBlend[nIndex] = nValue <= 255 ? nValue : 255;
		}
	}
}

static void BlendColorBurn (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] == 0 ) 
		{
			pBlend[nIndex] = 0;
		} 
		else 
		{
			int nValue = ( ( 255 - pDst[nIndex] ) * 255 ) / pSrc[nIndex];
			pBlend[nIndex] = nValue <= 255 ? 255 - nValue : 0;
		}
	}
}

static void BlendHardLight (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pSrc[nIndex] < 0x80 ? ( pDst[nIndex] * 2 * pSrc[nIndex] ) / 255 : 255 - 2 * ( ( 255 - pDst[nIndex] ) * ( 255 - pSrc[nIndex] ) ) / 255;
	}
}

static void BlendSoftLight (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		if ( pSrc[nIndex] < 0x80 ) 
		{
			pBlend[nIndex] = pDst[nIndex] - (255 - 2 * pSrc[nIndex]) * pDst[nIndex] * (255 - pDst[nIndex]) / ( 255 * 255 );
		} 
		else 
		{
			int nValue;
			if ( pDst[nIndex] < 0x40 ) 
			{
				nValue = ( ( ( ( ( 16 * pDst[nIndex] - 12 * 255 ) * pDst[nIndex] ) / 255 ) + 4 * 255 ) * pDst[nIndex] ) / 255;
			} 
			else 
			{
				nValue = (int)sqrt( 255.0 * pDst[nIndex] );
			}
			pBlend[nIndex] = pDst[nIndex] + ( 2 * pSrc[nIndex] - 255 ) * ( nValue - pDst[nIndex] ) / 255;
		}
	}
}

static void BlendDifference(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] < pSrc[nIndex] ? pSrc[nIndex] - pDst[nIndex] : pDst[nIndex] - pSrc[nIndex];
	}
}

static void BlendExclusion (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pDst[nIndex] + pSrc[nIndex] - ( 2 * pDst[nIndex] * pSrc[nIndex] ) / 255;
	}
}

static void BlendHue       (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nSrcH, nDstS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendSaturation(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nDstH, nSrcS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendColor     (SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nSrcH, nSrcS, nDstV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}

static void BlendLuminosity(SColorPointer pSrc, SColorPointer pDst, SColorPointer pBlend, SColorMode eColorMode) 
{
	int nSrcH, nSrcS, nSrcV, nDstH, nDstS, nDstV;

	switch (eColorMode) 
	{
	case colorModeMono1:
	case colorModeMono8:
		pBlend[0] = pDst[0];
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		RGBToHSV( pSrc[0], pSrc[1], pSrc[2], &nSrcH, &nSrcS, &nSrcV );
		RGBToHSV( pDst[0], pDst[1], pDst[2], &nDstH, &nDstS, &nDstV );
		HSVToRGB( nDstH, nDstS, nSrcV, &pBlend[0], &pBlend[1], &pBlend[2] );
		break;
	}
}