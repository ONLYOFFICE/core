#include "stdafx.h"

#include "../Common/PaintStruct.h"

#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "SErrorCodes.h"
#include "SMathExt.h"
#include "SBitmap.h"
#include "SState.h"
#include "SPath.h"
#include "SXPath.h"
#include "SXPathScanner.h"
#include "SPattern.h"
#include "SScreen.h"
#include "SFont.h"
#include "SGlyphBitmap.h"
#include "SImage.h"

//-------------------------------------------------------------------------------------------------------------------------------

// Расстояние от центра до контрольных точек Безье для аппроксимации окружности = (4 * (sqrt(2) - 1) / 3) * r
static const double c_dKappa   = ((double)0.55228475);
static const double c_dKappa_2 = ((double)(0.5 * 0.55228475));

// Делим 16-битное значение [0, 255*255] на 255, возвращаем 8-битное значение.
static inline unsigned char Div255(int nValue) 
{
	return (unsigned char)((nValue + (nValue >> 8) + 0x80) >> 8);
}

//-------------------------------------------------------------------------------------------------------------------------------
// SPipe
//-------------------------------------------------------------------------------------------------------------------------------

#define PipeMaxStages 9

struct SPipe 
{
	int            nX;               // Координаты
	int            nY;               //
	SPattern      *pPattern;         // Source Pattern
	double         dAlphaInput;      //
	BOOL           bUsesShape;       //
	unsigned char  unAlphaSrc;       // Source Alpha
	SColorPointer  pColorSrc;        // Source Color
	SColor         arrColorSrcVal;   //
	unsigned char *pAlpha0;          // Альфа-канал для Non-isolated групп
	SColorPointer  pSMask;           // Soft Mask 
 
  	SColorPointer  pColorDst;        // Destination Color
	int            nColorMaskDst;    // Destination Color Mask
	unsigned char *pAlphaDst;        // Destination Alpha 

	double         dShape;           // Shape

	BOOL           bNoTransparency;  //
	int            nNonIsolatedGroup;// Non-isolated group correction

	SPipeResultColorCtrl eResultColorCtrl;

};

SPipeResultColorCtrl SImage::m_arrePipeResultColorNoAlphaBlend[] = 
{
	PipeResultColorNoAlphaBlendMono,  // 1 bit
	PipeResultColorNoAlphaBlendMono,  // 8 bit
	PipeResultColorNoAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorNoAlphaBlendRGB    // 24 bit BGR
};

SPipeResultColorCtrl SImage::m_arrePipeResultColorAlphaNoBlend[] = 
{
	PipeResultColorAlphaNoBlendMono,  // 1 bit
	PipeResultColorAlphaNoBlendMono,  // 8 bit
	PipeResultColorAlphaNoBlendRGB,   // 24 bit RGB
	PipeResultColorAlphaNoBlendRGB
};

SPipeResultColorCtrl SImage::m_arrePipeResultColorAlphaBlend[] = 
{
	PipeResultColorAlphaBlendMono,  // 1 bit
	PipeResultColorAlphaBlendMono,  // 8 bit
	PipeResultColorAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorAlphaBlendRGB    // 24 bit BGR
};

//-------------------------------------------------------------------------------------------------------------------------------

static void BlendXor(SColorPointer pSrc, SColorPointer pDest, SColorPointer pBlend, SColorMode eColorMode) 
{
	for ( int nIndex = 0; nIndex < SColorModeNComps[eColorMode]; ++nIndex ) 
	{
		pBlend[nIndex] = pSrc[nIndex] ^ pDest[nIndex];
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// Modified Region
//-------------------------------------------------------------------------------------------------------------------------------

void SImage::ClearModRegion() 
{
	m_nModRegMinX = m_pBitmap->GetWidth();
	m_nModRegMinY = m_pBitmap->GetHeight();
	m_nModRegMaxX = -1;
	m_nModRegMaxY = -1;
}

inline void SImage::UpdateModX(int nX) 
{
	if ( nX < m_nModRegMinX ) 
	{
		m_nModRegMinX = nX;
	}
	if ( nX > m_nModRegMaxX ) 
	{
		m_nModRegMaxX = nX;
	}
}

inline void SImage::UpdateModY(int nY) 
{
	if ( nY < m_nModRegMinY ) 
	{
		m_nModRegMinY = nY;
	}
	if ( nY > m_nModRegMaxY ) 
	{
		m_nModRegMaxY = nY;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// Pipeline
//-------------------------------------------------------------------------------------------------------------------------------

inline void SImage::PipeInit(SPipe *pPipe, int nX, int nY, SPattern *pPattern, SColorPointer pColorSrc, double dAlphaInput, BOOL bUsesShape, BOOL bNonIsolatedGroup) 
{
	PipeSetXY( pPipe, nX, nY );
	pPipe->pPattern = NULL;

	// Source color
	if ( pPattern ) 
	{
		if ( pPattern->IsStatic() ) 
		{
			pPattern->GetColor( nX, nY, pPipe->arrColorSrcVal );
		} 
		else 
		{
			pPipe->pPattern = pPattern;
		}
		pPipe->pColorSrc = pPipe->arrColorSrcVal;
	} 
	else 
	{
		pPipe->pColorSrc = pColorSrc;
	}

	// Source alpha
	pPipe->dAlphaInput = dAlphaInput;
	if ( !m_pState->m_pSoftMask ) 
	{
		if ( bUsesShape ) 
		{
			pPipe->dAlphaInput *= 255;
		} 
		else 
		{
			pPipe->unAlphaSrc = (unsigned char)round( pPipe->dAlphaInput * 255 );
		}
	}
	pPipe->bUsesShape = bUsesShape;

	// Result Alpha
	if ( dAlphaInput == 1 && !m_pState->m_pSoftMask && !bUsesShape && !m_pState->m_bInNonIsolatedGroup ) 
	{
		pPipe->bNoTransparency = TRUE;
	} 
	else 
	{
		pPipe->bNoTransparency = FALSE;
	}

	// Result Color
	if ( pPipe->bNoTransparency ) 
	{
		pPipe->eResultColorCtrl = m_arrePipeResultColorNoAlphaBlend[m_pBitmap->m_eMode];
	} 
	else if ( !m_pState->m_pBlendFunction ) 
	{
		pPipe->eResultColorCtrl = m_arrePipeResultColorAlphaNoBlend[m_pBitmap->m_eMode];
	} 
	else 
	{
		pPipe->eResultColorCtrl = m_arrePipeResultColorAlphaBlend[m_pBitmap->m_eMode];
	}

	// Non-isolated group correction
	if ( bNonIsolatedGroup ) 
	{
		pPipe->nNonIsolatedGroup = SColorModeNComps[m_pBitmap->m_eMode];
	} 
	else 
	{
		pPipe->nNonIsolatedGroup = 0;
	}
}

inline void SImage::PipeRun(SPipe *pPipe) 
{
	unsigned char aSrc, aDest, alpha2, alpha0, aResult;
	SColor cDest, cBlend;
	unsigned char cResult0, cResult1, cResult2, cResult3;

	//----- source color

	// static pattern: handled in pipeInit

	// dynamic pattern
	if ( pPipe->pPattern ) 
	{
		pPipe->pPattern->GetColor( pPipe->nX, pPipe->nY, pPipe->arrColorSrcVal );
	}

	if ( pPipe->bNoTransparency && !m_pState->m_pBlendFunction ) 
	{

		//----- write destination pixel

		switch ( m_pBitmap->m_eMode ) 
		{
		case colorModeMono1:
			cResult0 = pPipe->pColorSrc[0];
			if ( m_pState->m_pScreen->GetGrayPixel( pPipe->nX, pPipe->nY, cResult0 ) ) 
			{
				*pPipe->pColorDst |= pPipe->nColorMaskDst;
			} 
			else 
			{
				*pPipe->pColorDst &= ~pPipe->nColorMaskDst;
			}
			if ( !(pPipe->nColorMaskDst >>= 1) ) 
			{
				pPipe->nColorMaskDst = 0x80;
				++pPipe->pColorDst;
			}
			break;
		case colorModeMono8:
			*pPipe->pColorDst++ = pPipe->pColorSrc[0];
			break;
		case colorModeRGB8:
			*pPipe->pColorDst++ = pPipe->pColorSrc[0];
			*pPipe->pColorDst++ = pPipe->pColorSrc[1];
			*pPipe->pColorDst++ = pPipe->pColorSrc[2];
			break;
		case colorModeBGR8:
			*pPipe->pColorDst++ = pPipe->pColorSrc[2];
			*pPipe->pColorDst++ = pPipe->pColorSrc[1];
			*pPipe->pColorDst++ = pPipe->pColorSrc[0];
			break;
		}
		if ( pPipe->pAlphaDst ) 
		{
			*pPipe->pAlphaDst++ = 255;
		}
	} 
	else 
	{

		//----- read destination pixel

		switch ( m_pBitmap->m_eMode ) 
		{
		case colorModeMono1:
			cDest[0] = (*pPipe->pColorDst & pPipe->nColorMaskDst) ? 0xff : 0x00;
			break;
		case colorModeMono8:
			cDest[0] = *pPipe->pColorDst;
			break;
		case colorModeRGB8:
			cDest[0] = pPipe->pColorDst[0];
			cDest[1] = pPipe->pColorDst[1];
			cDest[2] = pPipe->pColorDst[2];
			break;
		case colorModeBGR8:
			cDest[0] = pPipe->pColorDst[2];
			cDest[1] = pPipe->pColorDst[1];
			cDest[2] = pPipe->pColorDst[0];
			break;
		}
		if ( pPipe->pAlphaDst )
		{
			aDest = *pPipe->pAlphaDst;
		} 
		else 
		{
			aDest = 0xff;
		}

		//----- blend function

		if ( m_pState->m_pBlendFunction ) 
		{
			(*m_pState->m_pBlendFunction)( pPipe->pColorSrc, cDest, cBlend, m_pBitmap->m_eMode );
		}

		//----- source alpha

		if ( m_pState->m_pSoftMask ) 
		{
			if ( pPipe->bUsesShape ) 
			{
				aSrc = (unsigned char)round( pPipe->dAlphaInput * *pPipe->pSMask++ * pPipe->dShape );
			} 
			else 
			{
				aSrc = (unsigned char)round( pPipe->dAlphaInput * *pPipe->pSMask++ ); 
			}
		} 
		else if ( pPipe->bUsesShape ) 
		{
			aSrc = (unsigned char)round( pPipe->dAlphaInput * pPipe->dShape );
		} 
		else 
		{
			aSrc = pPipe->unAlphaSrc;
		}

		//----- result alpha and non-isolated group element correction

		if ( pPipe->bNoTransparency ) 
		{
			alpha2 = aResult = 255;
		} 
		else 
		{
			aResult = aSrc + aDest - Div255(aSrc * aDest);

			if ( pPipe->pAlpha0 ) 
			{
				alpha0 = *pPipe->pAlpha0++;
				alpha2 = aResult + alpha0 - Div255(aResult * alpha0);
			} 
			else 
			{
				alpha2 = aResult;
			}
		}

		//----- result color

		cResult0 = cResult1 = cResult2 = cResult3 = 0;

		switch ( pPipe->eResultColorCtrl ) 
		{
		case PipeResultColorNoAlphaBlendRGB:
			cResult2 = Div255((255 - aDest) * pPipe->pColorSrc[2] + aDest * cBlend[2]);
			cResult1 = Div255((255 - aDest) * pPipe->pColorSrc[1] + aDest * cBlend[1]);
		case PipeResultColorNoAlphaBlendMono:
			cResult0 = Div255((255 - aDest) * pPipe->pColorSrc[0] + aDest * cBlend[0]);
			break;
		case PipeResultColorAlphaNoBlendMono:
			if ( alpha2 == 0 ) 
			{
				cResult0 = 0;
			} 
			else 
			{
				cResult0 = (unsigned char)(((alpha2 - aSrc) * cDest[0] + aSrc * pPipe->pColorSrc[0]) / alpha2);
			}
			break;
		case PipeResultColorAlphaNoBlendRGB:
			if ( alpha2 == 0 ) 
			{
				cResult0 = 0;
				cResult1 = 0;
				cResult2 = 0;
			} 
			else 
			{
				cResult0 = (unsigned char)(((alpha2 - aSrc) * cDest[0] + aSrc * pPipe->pColorSrc[0]) / alpha2);
				cResult1 = (unsigned char)(((alpha2 - aSrc) * cDest[1] + aSrc * pPipe->pColorSrc[1]) / alpha2);
				cResult2 = (unsigned char)(((alpha2 - aSrc) * cDest[2] + aSrc * pPipe->pColorSrc[2]) / alpha2);
			}
			break;
		case PipeResultColorAlphaBlendMono:
			if ( alpha2 == 0 ) 
			{
				cResult0 = 0;
			} 
			else 
			{
				cResult0 = (unsigned char)(((alpha2 - aSrc) * cDest[0] + aSrc * ((255 - aDest) * pPipe->pColorSrc[0] + aDest * cBlend[0]) / 255) / alpha2);
			}
			break;
		case PipeResultColorAlphaBlendRGB:
			if ( alpha2 == 0 ) 
			{
				cResult0 = 0;
				cResult1 = 0;
				cResult2 = 0;
			} 
			else 
			{
				cResult0 = (unsigned char)(((alpha2 - aSrc) * cDest[0] + aSrc * ((255 - aDest) * pPipe->pColorSrc[0] + aDest * cBlend[0]) / 255) / alpha2);
				cResult1 = (unsigned char)(((alpha2 - aSrc) * cDest[1] + aSrc * ((255 - aDest) * pPipe->pColorSrc[1] + aDest * cBlend[1]) / 255) / alpha2);
				cResult2 = (unsigned char)(((alpha2 - aSrc) * cDest[2] + aSrc * ((255 - aDest) * pPipe->pColorSrc[2] + aDest * cBlend[2]) / 255) / alpha2);
			}
			break;
		}

		//----- non-isolated group correction

		if ( aResult != 0 ) 
		{
			switch ( pPipe->nNonIsolatedGroup ) 
			{
			case 3:
				cResult2 += (cResult2 - cDest[2]) * aDest * (255 - aResult) / (255 * aResult);
				cResult1 += (cResult1 - cDest[1]) * aDest * (255 - aResult) / (255 * aResult);
			case 1:
				cResult0 += (cResult0 - cDest[0]) * aDest * (255 - aResult) / (255 * aResult);
			case 0:
				break;
			}
		}

		//----- write destination pixel

		switch ( m_pBitmap->m_eMode ) 
		{
		case colorModeMono1:
			if ( m_pState->m_pScreen->GetGrayPixel(pPipe->nX, pPipe->nY, cResult0) ) 
			{
				*pPipe->pColorDst |= pPipe->nColorMaskDst;
			} 
			else 
			{
				*pPipe->pColorDst &= ~pPipe->nColorMaskDst;
			}

			if ( !( pPipe->nColorMaskDst >>= 1 ) ) 
			{
				pPipe->nColorMaskDst = 0x80;
				++pPipe->pColorDst;
			}
			break;
		case colorModeMono8:
			*pPipe->pColorDst++ = cResult0;
			break;
		case colorModeRGB8:
			*pPipe->pColorDst++ = cResult0;
			*pPipe->pColorDst++ = cResult1;
			*pPipe->pColorDst++ = cResult2;
			break;
		case colorModeBGR8:
			*pPipe->pColorDst++ = cResult2;
			*pPipe->pColorDst++ = cResult1;
			*pPipe->pColorDst++ = cResult0;
			break;
		}
		if ( pPipe->pAlphaDst ) 
		{
			*pPipe->pAlphaDst++ = aResult;
		}
	}
	++pPipe->nX;
}

inline void SImage::PipeSetXY(SPipe *pPipe, int nX, int nY) 
{
	pPipe->nX = nX;
	pPipe->nY = nY;

	if ( m_pState->m_pSoftMask ) 
	{
		pPipe->pSMask = &m_pState->m_pSoftMask->m_pData[nY * m_pState->m_pSoftMask->m_nStride + nX];
	}

	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
		pPipe->pColorDst = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride + (nX >> 3)];
		pPipe->nColorMaskDst = 0x80 >> (nX & 7);
		break;
	case colorModeMono8:
		pPipe->pColorDst = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride + nX];
	break;
	case colorModeRGB8:
	case colorModeBGR8:
		pPipe->pColorDst = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride + 3 * nX];
		break;
	}

	if ( m_pBitmap->m_pAlpha ) 
	{
		pPipe->pAlphaDst = &m_pBitmap->m_pAlpha[nY * m_pBitmap->m_nWidth + nX];
	} 
	else 
	{
		pPipe->pAlphaDst = NULL;
	}
	if ( m_pState->m_bInNonIsolatedGroup && m_pAlpha0Bitmap->m_pAlpha ) 
	{
		pPipe->pAlpha0 = &m_pAlpha0Bitmap->m_pAlpha[( m_nAlpha0Y + nY ) * m_pAlpha0Bitmap->m_nWidth + ( m_nAlpha0X + nX )];
	} 
	else 
	{
		pPipe->pAlpha0 = NULL;
	}
}

inline void SImage::PipeIncreaseX(SPipe *pPipe) 
{
	++pPipe->nX;
	if ( m_pState->m_pSoftMask ) 
	{
		++pPipe->pSMask;
	}

	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
		if ( !( pPipe->nColorMaskDst >>= 1 ) ) 
		{
			pPipe->nColorMaskDst = 0x80;
			++pPipe->pColorDst;
		}
		break;
	case colorModeMono8:
		++pPipe->pColorDst;
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		pPipe->pColorDst += 3;
		break;
	}

	if ( pPipe->pAlphaDst ) 
	{
		++pPipe->pAlphaDst;
	}

	if ( pPipe->pAlpha0 ) 
	{
		++pPipe->pAlpha0;
	}
}

inline void SImage::DrawPixel(SPipe *pPipe, int nX, int nY, BOOL bNoClip) 
{
	if ( bNoClip || m_pState->m_pClip->IsInsideClip( nX, nY ) ) 
	{
		PipeSetXY( pPipe, nX, nY );
		PipeRun( pPipe );
		UpdateModX( nX );
		UpdateModY( nY );
	}
}

inline void SImage::DrawAAPixelInit() 
{
	m_nAABufferY = -1;
}

inline void SImage::DrawAAPixel(SPipe *pPipe, int nX, int nY) 
{
	static int arrnBitCount4[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

	if ( nX < 0 || nX >= m_pBitmap->m_nWidth || nY < m_pState->m_pClip->GetMinY() || nY > m_pState->m_pClip->GetMaxY() ) 
	{
		return;
	}

	// Пересчитываем m_nAABufferY
	if ( nY != m_nAABufferY ) 
	{
		memset( m_pAABuffer->GetData(), 0xff, m_pAABuffer->GetStride() * m_pAABuffer->GetHeight() );

		int nX0 = 0;
		int nX1 = m_pBitmap->m_nWidth - 1;
		
		m_pState->m_pClip->ClipAALine( m_pAABuffer, &nX0, &nX1, nY );
		m_nAABufferY = nY;
	}

	SColorPointer pColor = m_pAABuffer->GetData() + (nX >> 1);
	int nStride = m_pAABuffer->GetStride();

	int nTemp = 0;
	if ( nX & 1 ) 
	{
		nTemp = arrnBitCount4[*pColor & 0x0f] + arrnBitCount4[pColor[nStride] & 0x0f] + arrnBitCount4[pColor[2 * nStride] & 0x0f] + arrnBitCount4[pColor[3 * nStride] & 0x0f];
	} 
	else 
	{
		nTemp = arrnBitCount4[*pColor >> 4] + arrnBitCount4[pColor[nStride] >> 4] + arrnBitCount4[pColor[2 * nStride] >> 4] + arrnBitCount4[pColor[3 * nStride] >> 4];
	}

	// Рисуем данный пиксель
	if ( nTemp != 0 ) 
	{
		PipeSetXY( pPipe, nX, nY );
		pPipe->dShape *= m_arrdAAGamma[nTemp];
		PipeRun( pPipe );
		UpdateModX( nX );
		UpdateModY( nY );
	}
}

inline void SImage::DrawSpan(SPipe *pPipe, int nX0, int nX1, int nY, BOOL bNoClip) 
{
	PipeSetXY( pPipe, nX0, nY );
	if ( bNoClip ) 
	{
		for ( int nX = nX0; nX <= nX1; ++nX ) 
		{
			PipeRun( pPipe );
		}
		UpdateModX( nX0 );
		UpdateModX( nX1 );
		UpdateModY( nY );
	} 
	else 
	{
		for ( int nX = nX0; nX <= nX1; ++nX ) 
		{
			if ( m_pState->m_pClip->IsInsideClip( nX, nY ) ) 
			{
				PipeRun( pPipe );
				UpdateModX( nX );
				UpdateModY( nY );
			} 
			else 
			{
				PipeIncreaseX( pPipe );
			}
		}
	}
}

inline void SImage::DrawAALine(SPipe *pPipe, int nX0, int nX1, int nY) 
{
	static int arrnBitCount4[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

	SColorPointer pLine0 = m_pAABuffer->GetData() + (nX0 >> 1);
	SColorPointer pLine1 = pLine0 + m_pAABuffer->GetStride();
	SColorPointer pLine2 = pLine1 + m_pAABuffer->GetStride();
	SColorPointer pLine3 = pLine2 + m_pAABuffer->GetStride();

	PipeSetXY( pPipe, nX0, nY );

	for ( int nX = nX0; nX <= nX1; ++nX ) 
	{
		int nTemp = 0;
		if ( nX & 1 ) 
		{
			nTemp = arrnBitCount4[*pLine0 & 0x0f] + arrnBitCount4[*pLine1 & 0x0f] + arrnBitCount4[*pLine2 & 0x0f] + arrnBitCount4[*pLine3 & 0x0f];
			++pLine0; ++pLine1; ++pLine2; ++pLine3;
		} 
		else 
		{
			nTemp = arrnBitCount4[*pLine0 >> 4] + arrnBitCount4[*pLine1 >> 4] + arrnBitCount4[*pLine2 >> 4] + arrnBitCount4[*pLine3 >> 4];
		}

		if ( nTemp != 0 ) 
		{
			pPipe->dShape = m_arrdAAGamma[nTemp];
			PipeRun( pPipe );
			UpdateModX( nX );
			UpdateModY( nY );
		} 
		else 
		{
			PipeIncreaseX( pPipe );
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------

// Пользовательские координаты -> координаты устройства.
inline void SImage::Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY) 
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}

//-------------------------------------------------------------------------------------------------------------------------------
// SImage
//-------------------------------------------------------------------------------------------------------------------------------

SImage::SImage(SBitmap *pBitmap, BOOL bVectorAA, SScreenParams *pScreenParams) 
{
	m_pBitmap   = pBitmap;
	m_bVectorAA = bVectorAA;
	m_pState = new SState( m_pBitmap->m_nWidth, m_pBitmap->m_nHeight, m_bVectorAA, pScreenParams );

	if ( m_bVectorAA ) 
	{
		m_pAABuffer = new SBitmap( AntiAliasingSize * m_pBitmap->m_nWidth, AntiAliasingSize, 1, colorModeMono1, FALSE );

		for ( int nIndex = 0; nIndex <= AntiAliasingSize * AntiAliasingSize; ++nIndex ) 
		{
			m_arrdAAGamma[nIndex] = pow((double)nIndex / (double)(AntiAliasingSize * AntiAliasingSize), 1.5);
		}
	} 
	else 
	{
		m_pAABuffer = NULL;
	}
	ClearModRegion();
}

SImage::SImage(SBitmap *pBitmap, BOOL bVectorAA, SScreen *pScreen) 
{
	m_pBitmap   = pBitmap;
	m_bVectorAA = bVectorAA;
	m_pState = new SState( m_pBitmap->m_nWidth, m_pBitmap->m_nHeight, m_bVectorAA, pScreen );

	if ( m_bVectorAA ) 
	{
		m_pAABuffer = new SBitmap( AntiAliasingSize * m_pBitmap->m_nWidth, AntiAliasingSize, 1, colorModeMono1, FALSE );

		for ( int nIndex = 0; nIndex <= AntiAliasingSize * AntiAliasingSize; ++nIndex ) 
		{
			m_arrdAAGamma[nIndex] = pow( (double)nIndex / (double)(AntiAliasingSize * AntiAliasingSize), 1.5);
		}
	} 
	else 
	{
		m_pAABuffer = NULL;
	}
	ClearModRegion();
}

SImage::~SImage() 
{
	while ( m_pState->m_pNext ) 
	{
		RestoreState();
	}
	if ( m_pState )
		delete m_pState;

	if ( m_bVectorAA && m_pAABuffer ) 
		delete m_pAABuffer;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Считываем данные из текущего SState
//-------------------------------------------------------------------------------------------------------------------------------

double    *SImage::GetMatrix() 
{
	return m_pState->m_arrdMatrix;
}

SPattern  *SImage::GetStrokePattern() 
{
	return m_pState->m_pStrokePattern;
}

SPattern  *SImage::GetFillPattern() 
{
	return m_pState->m_pFillPattern;
}

SScreen   *SImage::GetScreen()
{
	return m_pState->m_pScreen;
}

SBlendFunc SImage::GetBlendFunc() 
{
	return m_pState->m_pBlendFunction;
}

double     SImage::GetStrokeAlpha() 
{
	return m_pState->m_dStrokeAlpha;
}

double     SImage::GetFillAlpha() 
{
	return m_pState->m_dFillAlpha;
}

double     SImage::GetLineWidth() 
{
	return m_pState->m_dLineWidth;
}

int        SImage::GetLineCap() 
{
	return m_pState->m_nLineCap;
}

int        SImage::GetLineJoin() 
{
	return m_pState->m_nLineJoin;
}

double     SImage::GetMiterLimit() 
{
	return m_pState->m_dMiterLimit;
}

double     SImage::GetFlatness() 
{
	return m_pState->m_dFlatness;
}

double    *SImage::GetLineDash() 
{
	return m_pState->m_pdLineDash;
}

int        SImage::GetLineDashLength() 
{
	return m_pState->m_nLineDashCount;
}

double     SImage::GetLineDashPhase() 
{
	return m_pState->m_dLineDashPhase;
}

SClip     *SImage::GetClip() 
{
	return m_pState->m_pClip;
}

SBitmap   *SImage::GetSoftMask() 
{
	return m_pState->m_pSoftMask;
}

BOOL       SImage::GetInNonIsolatedGroup() 
{
	return m_pState->m_bInNonIsolatedGroup;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Изменяем параметры текущего SState
//-------------------------------------------------------------------------------------------------------------------------------

void SImage::SetMatrix(double *pMatrix) 
{
	memcpy( m_pState->m_arrdMatrix, pMatrix, 6 * sizeof(double) );
}

void SImage::SetStrokePattern(SPattern *pPattern) 
{
	m_pState->SetStrokePattern( pPattern );
}

void SImage::SetFillPattern(SPattern *pPattern)
{
	m_pState->SetFillPattern( pPattern );
}

void SImage::SetScreen(SScreen *pScreen) 
{
	m_pState->SetScreen( pScreen );
}

void SImage::SetBlendFunc(SBlendFunc pFunction) 
{
	m_pState->m_pBlendFunction = pFunction;
}

void SImage::SetStrokeAlpha(double dAlpha) 
{
	m_pState->m_dStrokeAlpha = dAlpha;
}

void SImage::SetFillAlpha(double dAlpha) 
{
	m_pState->m_dFillAlpha = dAlpha;
}

void SImage::SetLineWidth(double dLineWidth) 
{
	m_pState->m_dLineWidth = dLineWidth;
}

void SImage::SetLineCap(int nLineCap) 
{
	m_pState->m_nLineCap = nLineCap;
}

void SImage::SetLineJoin(int nLineJoin) 
{
	m_pState->m_nLineJoin = nLineJoin;
}

void SImage::SetMiterLimit(double dMiterLimit) 
{
	m_pState->m_dMiterLimit = dMiterLimit;
}

void SImage::SetFlatness(double dFlatness) 
{
	if ( dFlatness < 1 ) 
	{
		m_pState->m_dFlatness = 1;
	} 
	else 
	{
		m_pState->m_dFlatness = dFlatness;
	}
}

void SImage::SetLineDash(double *pLineDash, int nLineDashLength, double dLineDashPhase) 
{
	m_pState->SetLineDash( pLineDash, nLineDashLength, dLineDashPhase );
}

void SImage::SetStrokeAdjust(BOOL bStrokeAdjust) 
{
	m_pState->m_bStrokeAdjust = bStrokeAdjust;
}

void SImage::ClipResetToRect(double dX0, double dY0, double dX1, double dY1) 
{
	m_pState->m_pClip->ResetToRect( dX0, dY0, dX1, dY1 );
}

int  SImage::ClipToRect(double dX0, double dY0, double dX1, double dY1) 
{
	return m_pState->m_pClip->ClipToRect( dX0, dY0, dX1, dY1 );
}

int  SImage::ClipToPath(SPath *pPath, BOOL bEo) 
{
	return m_pState->m_pClip->ClipToPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness, bEo );
}

void SImage::SetSoftMask(SBitmap *pSMask) 
{
	m_pState->SetSoftMask( pSMask );
}

void SImage::SetInNonIsolatedGroup(SBitmap *pAlpha0Bitmap, int nAlpha0X, int nAlpha0Y) 
{
	m_pAlpha0Bitmap = pAlpha0Bitmap;
	m_nAlpha0X = nAlpha0X;
	m_nAlpha0Y = nAlpha0Y;
	m_pState->m_bInNonIsolatedGroup = TRUE;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Сохраняем/восстанавливаем SState (имеется ввиду стек объектов SState)
//-------------------------------------------------------------------------------------------------------------------------------

void SImage::SaveState() 
{
	SState *pNewState = m_pState->Copy();
	pNewState->m_pNext = m_pState;
	m_pState = pNewState;
}

int  SImage::RestoreState() 
{
	if ( !m_pState->m_pNext ) 
	{
		return SErrorNoSaveState;
	}

	SState *pOldState = m_pState;
	m_pState = m_pState->m_pNext;
	delete pOldState;
	return SNoError;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Функции рисования
//-------------------------------------------------------------------------------------------------------------------------------

void SImage::Clear(SColorPointer pColor, unsigned char unAlpha) 
{
	unsigned char unMono;

	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
		unMono = (pColor[0] & 0x80) ? 0xff : 0x00;
		if ( m_pBitmap->m_nStride < 0 ) 
		{
			memset( m_pBitmap->m_pData + m_pBitmap->m_nStride * (m_pBitmap->m_nHeight - 1), unMono, -m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
		} 
		else 
		{
			memset( m_pBitmap->m_pData, unMono, m_pBitmap->m_nStride * m_pBitmap->m_nHeight); 
		}
		break;
	case colorModeMono8:
		if ( m_pBitmap->m_nStride < 0 ) 
		{
			memset( m_pBitmap->m_pData + m_pBitmap->m_nStride * (m_pBitmap->m_nHeight - 1), pColor[0], -m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
		} 
		else 
		{
			memset( m_pBitmap->m_pData, pColor[0], m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
		}
		break;
	case colorModeRGB8:
		if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
		{
			if ( m_pBitmap->m_nStride < 0 ) 
			{
				memset( m_pBitmap->m_pData + m_pBitmap->m_nStride * (m_pBitmap->m_nHeight - 1), pColor[0], -m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
			} 
			else 
			{
				memset( m_pBitmap->m_pData, pColor[0], m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
			}
		} 
		else 
		{
			SColorPointer pLine = m_pBitmap->m_pData;
			for ( int nY = 0; nY < m_pBitmap->m_nHeight; ++nY ) 
			{
				SColorPointer pCur = pLine;
				for ( int nX = 0; nX < m_pBitmap->m_nWidth; ++nX ) 
				{
					*pCur++ = pColor[2];
					*pCur++ = pColor[1];
					*pCur++ = pColor[0];
				}
				pLine += m_pBitmap->m_nStride;
			}
		}
		break;
	case colorModeBGR8:
		if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
		{
			if ( m_pBitmap->m_nStride < 0 ) 
			{
				memset( m_pBitmap->m_pData + m_pBitmap->m_nStride * (m_pBitmap->m_nHeight - 1), pColor[0], -m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
			} 
			else 
			{
				memset( m_pBitmap->m_pData, pColor[0], m_pBitmap->m_nStride * m_pBitmap->m_nHeight );
			}
		} 
		else 
		{
			SColorPointer pLine = m_pBitmap->m_pData;
			for ( int nY = 0; nY < m_pBitmap->m_nHeight; ++nY ) 
			{
				SColorPointer pCur = pLine;
				for ( int nX = 0; nX < m_pBitmap->m_nWidth; ++nX ) 
				{
					*pCur++ = pColor[0];
					*pCur++ = pColor[1];
					*pCur++ = pColor[2];
				}
				pLine += m_pBitmap->m_nStride;
			}
		}
		break;
	}

	if ( m_pBitmap->m_pAlpha ) 
	{
		memset( m_pBitmap->m_pAlpha, unAlpha, m_pBitmap->m_nWidth * m_pBitmap->m_nHeight );
	}

	UpdateModX( 0 );
	UpdateModY( 0 );
	UpdateModX( m_pBitmap->m_nWidth - 1 );
	UpdateModY( m_pBitmap->m_nHeight - 1 );
}

int  SImage::Stroke(SPath *pPath) 
{
	m_pOpClipRes = clipAllOutside;
	if ( pPath->m_nPointsCount == 0 ) 
	{
		return SErrorEmptyPath;
	}
	SPath *pResultPath = FlattenPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness );
	if ( !pResultPath )
		return SErrorMemory;

	if ( m_pState->m_nLineDashCount > 0 ) 
	{
		SPath *pDashPath = MakeDashedPath( pResultPath );
		delete pResultPath;
		pResultPath = pDashPath;
	}

	if ( m_pState->m_dLineWidth == 0 ) 
	{
		StrokeNarrow( pResultPath );
	} 
	else 
	{
		StrokeWide( pResultPath );
	}
	delete pResultPath;
	return SNoError;
}

void SImage::StrokeNarrow(SPath *pPath) 
{
	SClipResult eClipResult;
	int arrnClipRes[3];

	arrnClipRes[0] = arrnClipRes[1] = arrnClipRes[2] = 0;

	SXPath *pXPath = new SXPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness, FALSE );
	if ( !pXPath )
		return;

	SPipe oPipe;
	PipeInit( &oPipe, 0, 0, m_pState->m_pStrokePattern, NULL, m_pState->m_dStrokeAlpha, FALSE, FALSE );

	int nSegmentIndex;
	SXPathSegment *pSegment = NULL;
	for ( nSegmentIndex = 0, pSegment = pXPath->m_pSegments; nSegmentIndex < pXPath->m_nSegmentsCount; ++nSegmentIndex, ++pSegment ) 
	{
		int nX0 = (int)floor( pSegment->dFirstX );
		int nX1 = (int)floor( pSegment->dSecondX );
		int nY0 = (int)floor( pSegment->dFirstY );
		int nY1 = (int)floor( pSegment->dSecondY );

		// горизонтальный сегмент
		if ( nY0 == nY1 ) 
		{
			if ( nX0 > nX1 ) 
			{
				int nTemp = nX0; 
				nX0 = nX1; 
				nX1 = nTemp;
			}

			if ( ( eClipResult = m_pState->m_pClip->CheckSpan( nX0, nX1, nY0 ) ) != clipAllOutside ) 
			{
				DrawSpan( &oPipe, nX0, nX1, nY0, eClipResult == clipAllInside );
			}
		} 
		else if ( fabs(pSegment->dDxDy) > 1) // |dDx| > |dDy|
		{
			double dDx = pSegment->dSecondX - pSegment->dFirstX;
			double dDy = pSegment->dSecondY - pSegment->dFirstY;
			double dDxDy = pSegment->dDxDy;
			
			if ( nY0 > nY1 ) 
			{
				int nTemp = nY0; 
				nY0 = nY1; 
				nY1 = nTemp;

				nTemp = nX0; 
				nX0 = nX1; 
				nX1 = nTemp;

				dDx = -dDx;
				dDy = -dDy;
			}
			
			if ( ( eClipResult = m_pState->m_pClip->CheckRectangle( nX0 <= nX1 ? nX0 : nX1, nY0, nX0 <= nX1 ? nX1 : nX0, nY1) ) != clipAllOutside ) 
			{
				if ( dDx > 0 ) 
				{
					int nX2 = nX0;
					int nX3 = (int)floor( pSegment->dFirstX + ((double)nY0 + 1 - pSegment->dFirstY) * dDxDy );
					DrawSpan( &oPipe, nX2, (nX2 <= nX3 - 1) ? nX3 - 1 : nX2, nY0, eClipResult == clipAllInside );
					nX2 = nX3;
					for ( int nY = nY0 + 1; nY <= nY1 - 1; ++nY ) 
					{
						nX3 = (int)floor( pSegment->dFirstX + ((double)nY + 1 - pSegment->dFirstY) * dDxDy );
						DrawSpan( &oPipe, nX2, nX3 - 1, nY, eClipResult == clipAllInside );
						nX2 = nX3;
					}
					DrawSpan( &oPipe, nX2, nX2 <= nX1 ? nX1 : nX2, nY1, eClipResult == clipAllInside );
				} 
				else 
				{
					int nX2 = nX0;
					int nX3 = (int)floor( pSegment->dFirstX + ((double)nY0 + 1 - pSegment->dFirstY) * dDxDy );
					DrawSpan( &oPipe, (nX3 + 1 <= nX2) ? nX3 + 1 : nX2, nX2, nY0, eClipResult == clipAllInside );
					nX2 = nX3;
					for ( int nY = nY0 + 1; nY <= nY1 - 1; ++nY ) 
					{
						nX3 = (int)floor( pSegment->dFirstX + ((double)nY + 1 - pSegment->dFirstY) * dDxDy );
						DrawSpan( &oPipe, nX3 + 1, nX2, nY, eClipResult == clipAllInside );
						nX2 = nX3;
					}
					DrawSpan( &oPipe, nX1, (nX1 <= nX2) ? nX2 : nX1, nY1, eClipResult == clipAllInside );
				}
			}
		} 
		else // |dDy| > |dDx|
		{
			double dDxDy = pSegment->dDxDy;
			if ( nY0 > nY1 ) 
			{
				int nTemp = nX0; 
				nX0 = nX1; 
				nX1 = nTemp;

				nTemp = nY0; 
				nY0 = nY1; 
				nY1 = nTemp;
			}

			if ( ( eClipResult = m_pState->m_pClip->CheckRectangle( nX0 <= nX1 ? nX0 : nX1, nY0, nX0 <= nX1 ? nX1 : nX0, nY1) ) != clipAllOutside ) 
			{
				DrawPixel( &oPipe, nX0, nY0, eClipResult == clipAllInside );
				for ( int nY = nY0 + 1; nY <= nY1 - 1; ++nY ) 
				{
					int nX = (int)floor(pSegment->dFirstX + ((double)nY - pSegment->dFirstY) * dDxDy);
					DrawPixel( &oPipe, nX, nY, eClipResult == clipAllInside );
				}
				DrawPixel( &oPipe, nX1, nY1, eClipResult == clipAllInside );
			}
		}
		++arrnClipRes[eClipResult];
	}

	if ( arrnClipRes[clipPartial] || ( arrnClipRes[clipAllInside] && arrnClipRes[clipAllOutside] ) ) 
	{
		m_pOpClipRes = clipPartial;
	} 
	else if ( arrnClipRes[clipAllInside] ) 
	{
		m_pOpClipRes = clipAllInside;
	} 
	else 
	{
		m_pOpClipRes = clipAllOutside;
	}

	delete pXPath;
}

void SImage::StrokeWide(SPath *pPath) 
{
	SPath *pStrokePath = MakeStrokePath( pPath, FALSE );
	if ( !pStrokePath )
		return;
	FillWithPattern( pStrokePath, FALSE, m_pState->m_pStrokePattern, m_pState->m_dStrokeAlpha );
	delete pStrokePath;
}

SPath *SImage::FlattenPath(SPath *pPath, double *pMatrix, double dFlatness) 
{
	SPath *pFlatPath = new SPath();
	double dFlatness_2 = dFlatness * dFlatness;
	int nPointIndex = 0;
	while ( nPointIndex < pPath->m_nPointsCount ) 
	{
		unsigned char unFlag = pPath->m_pFlags[nPointIndex];
		if ( unFlag & SPathFirst ) 
		{
			pFlatPath->MoveTo( pPath->m_pPoints[nPointIndex].dX, pPath->m_pPoints[nPointIndex].dY );
			++nPointIndex;
		} 
		else 
		{
			if ( unFlag & SPathCurve ) 
			{
				FlattenCurve( pPath->m_pPoints[nPointIndex - 1].dX, pPath->m_pPoints[nPointIndex - 1].dY, pPath->m_pPoints[nPointIndex].dX, pPath->m_pPoints[nPointIndex].dY, pPath->m_pPoints[nPointIndex + 1].dX, pPath->m_pPoints[nPointIndex + 1].dY, pPath->m_pPoints[nPointIndex + 2].dX, pPath->m_pPoints[nPointIndex + 2].dY, pMatrix, dFlatness_2, pFlatPath );
				nPointIndex += 3;
			} 
			else 
			{
				pFlatPath->LineTo( pPath->m_pPoints[nPointIndex].dX, pPath->m_pPoints[nPointIndex].dY );
				++nPointIndex;
			}
			if ( pPath->m_pFlags[nPointIndex - 1] & SPathClosed ) 
			{
				pFlatPath->Close();
			}
		}
	}
	return pFlatPath;
}

void SImage::FlattenCurve(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, double *matrix, double dFlatness_2, SPath *fPath) 
{
	double arrSegX[MaxCurveSplits + 1][3];
	double arrSegY[MaxCurveSplits + 1][3];
	int arrNext[MaxCurveSplits + 1];
	double xl0, xl1, xl2, xr0, xr1, xr2, xr3, xx1, xx2, xh;
	double yl0, yl1, yl2, yr0, yr1, yr2, yr3, yy1, yy2, yh;
	double dx, dy, mx, my, tx, ty, d1, d2;

	// Начальный сегмент
	int nPart1 = 0, nPart2 = MaxCurveSplits;
	arrSegX[nPart1][0] = x0;  arrSegY[nPart1][0] = y0;
	arrSegX[nPart1][1] = x1;  arrSegY[nPart1][1] = y1;
	arrSegX[nPart1][2] = x2;  arrSegY[nPart1][2] = y2;
	arrSegX[nPart2][0] = x3;  arrSegY[nPart2][0] = y3;
	arrNext[nPart1] = nPart2;

	while ( nPart1 < MaxCurveSplits ) 
	{
		// Следующий сегмент
		xl0 = arrSegX[nPart1][0];  yl0 = arrSegY[nPart1][0];
		xx1 = arrSegX[nPart1][1];  yy1 = arrSegY[nPart1][1];
		xx2 = arrSegX[nPart1][2];  yy2 = arrSegY[nPart1][2];
		nPart2 = arrNext[nPart1];
		xr3 = arrSegX[nPart2][0];  yr3 = arrSegY[nPart2][0];

		// Вычисляем расстояние от контрольных точек до средних точек прямой линии. (Вычисление не совсем точное, но быстрое)
		Transform( matrix, (xl0 + xr3) * 0.5, (yl0 + yr3) * 0.5, &mx, &my );
		Transform( matrix, xx1, yy1, &tx, &ty );
		dx = tx - mx;
		dy = ty - my;
		d1 = dx*dx + dy*dy;
		Transform(matrix, xx2, yy2, &tx, &ty);
		dx = tx - mx;
		dy = ty - my;
		d2 = dx*dx + dy*dy;

		// Если сегмент уже достаточно плоский или больше делений невозомжно сделать, добавляем прямую линию
		if ( nPart2 - nPart1 == 1 || ( d1 <= dFlatness_2 && d2 <= dFlatness_2 ) ) 
		{
			fPath->LineTo( xr3, yr3 );
			nPart1 = nPart2;
		} 
		else // otherwise, subdivide the curve
		{
			xl1 = (xl0 + xx1) * 0.5;
			yl1 = (yl0 + yy1) * 0.5;
			xh = (xx1 + xx2) * 0.5;
			yh = (yy1 + yy2) * 0.5;
			xl2 = (xl1 + xh) * 0.5;
			yl2 = (yl1 + yh) * 0.5;
			xr2 = (xx2 + xr3) * 0.5;
			yr2 = (yy2 + yr3) * 0.5;
			xr1 = (xh + xr2) * 0.5;
			yr1 = (yh + yr2) * 0.5;
			xr0 = (xl2 + xr1) * 0.5;
			yr0 = (yl2 + yr1) * 0.5;

			// add the new subdivision points
			int nPart3 = (nPart1 + nPart2) / 2;
			arrSegX[nPart1][1] = xl1;  arrSegY[nPart1][1] = yl1;
			arrSegX[nPart1][2] = xl2;  arrSegY[nPart1][2] = yl2;
			arrNext[nPart1] = nPart3;

			arrSegX[nPart3][0] = xr0;  arrSegY[nPart3][0] = yr0;
			arrSegX[nPart3][1] = xr1;  arrSegY[nPart3][1] = yr1;
			arrSegX[nPart3][2] = xr2;  arrSegY[nPart3][2] = yr2;
			arrNext[nPart3] = nPart2;
		}
	}
}

SPath *SImage::MakeDashedPath(SPath *pPath) 
{
	double dLineDashTotal = 0;
	int nIndex = 0;
	for ( nIndex = 0; nIndex < m_pState->m_nLineDashCount; ++nIndex ) 
	{
		dLineDashTotal += m_pState->m_pdLineDash[nIndex];
	}

	double dLineDashStartPhase = m_pState->m_dLineDashPhase;
	nIndex = (int)floor( dLineDashStartPhase / dLineDashTotal );
	dLineDashStartPhase -= (double)nIndex * dLineDashTotal;

	BOOL bLineDashStartOn = TRUE;
	int  nLineDashStartIndex = 0;

	while ( dLineDashStartPhase >= m_pState->m_pdLineDash[nLineDashStartIndex] ) 
	{
		bLineDashStartOn = !bLineDashStartOn;
		dLineDashStartPhase -= m_pState->m_pdLineDash[nLineDashStartIndex];
		++nLineDashStartIndex;
	}

	SPath *pDashPath = new SPath();

	nIndex = 0;
	while ( nIndex < pPath->m_nPointsCount ) 
	{
		// Ищем окончание данного SupPath
		int nEndIndex;
		for ( nEndIndex = nIndex; nEndIndex < pPath->m_nPointsCount - 1 && !(pPath->m_pFlags[nEndIndex] & SPathLast); ++nEndIndex) ;

		// Обнуляем параметры
		BOOL   bLineDashOn    = bLineDashStartOn;
		int    nLineDashIndex = nLineDashStartIndex;
		double dLineDashDist  = m_pState->m_pdLineDash[nLineDashIndex] - dLineDashStartPhase;

		// Работаем с частями SupPath
		BOOL bNewPath = TRUE;
		for ( int nK = nIndex; nK < nEndIndex; ++nK ) 
		{
			// Забираем сегмент
			double dX0 = pPath->m_pPoints[nK].dX;
			double dY0 = pPath->m_pPoints[nK].dY;
			double dX1 = pPath->m_pPoints[nK + 1].dX;
			double dY1 = pPath->m_pPoints[nK + 1].dY;
			double dSegLen = distance( dX0, dY0, dX1, dY1 );

			while ( dSegLen > 0 ) 
			{
				if ( dLineDashDist >= dSegLen ) 
				{
					if ( bLineDashOn ) 
					{
						if ( bNewPath ) 
						{
							pDashPath->MoveTo( dX0, dY0 );
							bNewPath = FALSE;
						}
						pDashPath->LineTo( dX1, dY1 );
					}
					dLineDashDist -= dSegLen;
					dSegLen = 0;
				} 
				else 
				{
					double dTempX = dX0 + (dLineDashDist / dSegLen) * (dX1 - dX0);
					double dTempY = dY0 + (dLineDashDist / dSegLen) * (dY1 - dY0);
					if ( bLineDashOn ) 
					{
						if ( bNewPath ) 
						{
							pDashPath->MoveTo( dX0, dY0 );
							bNewPath = FALSE;
						}
						pDashPath->LineTo( dTempX, dTempY );
					}
					dX0 = dTempX;
					dY0 = dTempY;
					dSegLen -= dLineDashDist;
					dLineDashDist = 0;
				}

				if ( dLineDashDist <= 0 )
				{
					bLineDashOn = !bLineDashOn;
					if ( ++nLineDashIndex == m_pState->m_nLineDashCount ) 
					{
						nLineDashIndex = 0;
					}
					dLineDashDist = m_pState->m_pdLineDash[nLineDashIndex];
					bNewPath = TRUE;
				}
			}
		}
		nIndex = nEndIndex + 1;
	}

	return pDashPath;
}

int SImage::Fill(SPath *pPath, BOOL bEO) 
{
	return FillWithPattern( pPath, bEO, m_pState->m_pFillPattern, m_pState->m_dFillAlpha );
}

int SImage::FillWithPattern(SPath *pPath, BOOL bEO, SPattern *pPattern, double dAlpha) 
{
	int nX0, nX1;

	if ( pPath->m_nPointsCount == 0 ) 
	{
		return SErrorEmptyPath;
	}
	
	SXPath *pXPath = new SXPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness, TRUE );
	if ( !pXPath )
		return SErrorMemory;
	if ( m_bVectorAA ) 
	{
		pXPath->AntiAliasingScale();
	}
	pXPath->Sort();

	SXPathScanner *pScanner = new SXPathScanner( pXPath, bEO );
	if ( !pScanner )
		return SErrorMemory;

	int nMinX, nMinY, nMaxX, nMaxY;
	if ( m_bVectorAA )
	{
		pScanner->GetBBoxAA( &nMinX, &nMinY, &nMaxX, &nMaxY );
	} 
	else 
	{
		pScanner->GetBBox( &nMinX, &nMinY, &nMaxX, &nMaxY );
	}
	
	SClipResult eClipResult;
	if ( ( eClipResult = m_pState->m_pClip->CheckRectangle( nMinX, nMinY, nMaxX, nMaxY) ) != clipAllOutside ) 
	{
		if ( nMinY < m_pState->m_pClip->GetMinY() ) 
		{
			nMinY = m_pState->m_pClip->GetMinY();
		}
		if ( nMaxY > m_pState->m_pClip->GetMaxY() ) 
		{
			nMaxY = m_pState->m_pClip->GetMaxY();
		}
		SPipe oPipe;
		PipeInit( &oPipe, 0, nMinY, pPattern, NULL, dAlpha, m_bVectorAA, FALSE );

		if ( m_bVectorAA ) 
		{
			for ( int nY = nMinY; nY <= nMaxY; ++nY ) 
			{
				pScanner->RenderAALine( m_pAABuffer, &nX0, &nX1, nY );
				if ( eClipResult != clipAllInside ) 
				{
					m_pState->m_pClip->ClipAALine( m_pAABuffer, &nX0, &nX1, nY );
				}
				DrawAALine( &oPipe, nX0, nX1, nY );
			}
		} 
		else 
		{
			for ( int nY = nMinY; nY <= nMaxY; ++nY ) 
			{	
				while ( pScanner->GetNextSpan( nY, &nX0, &nX1) ) 
				{
					if ( eClipResult == clipAllInside ) 
					{
						DrawSpan( &oPipe, nX0, nX1, nY, TRUE );
					} 
					else 
					{
						// limit the x range
						if ( nX0 < m_pState->m_pClip->GetMinX() ) 
						{
							nX0 = m_pState->m_pClip->GetMinX();
						}
						if ( nX1 > m_pState->m_pClip->GetMaxX() ) 
						{
							nX1 = m_pState->m_pClip->GetMaxX();
						}
						SClipResult eTempClipRes = m_pState->m_pClip->CheckSpan( nX0, nX1, nY );
						DrawSpan( &oPipe, nX0, nX1, nY, eTempClipRes == clipAllInside );
					}
				}
			}
		}
	}
	m_pOpClipRes = eClipResult;

	delete pScanner;
	delete pXPath;
	return SNoError;
}

int SImage::XorFill(SPath *pPath, BOOL bEO) 
{
	int nX0, nX1;

	if ( pPath->m_nPointsCount == 0 ) 
	{
		return SErrorEmptyPath;
	}
	SXPath *pXPath = new SXPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness, TRUE);
	if ( !pXPath )
		return SErrorMemory;
	pXPath->Sort();

	SXPathScanner *pScanner = new SXPathScanner( pXPath, bEO );
	if ( !pScanner )
		return SErrorMemory;

	int nMinX, nMinY, nMaxX, nMaxY;
	pScanner->GetBBox( &nMinX, &nMinY, &nMaxX, &nMaxY );

	SClipResult eClipResult;
	if ( ( eClipResult = m_pState->m_pClip->CheckRectangle( nMinX, nMinY, nMaxX, nMaxY) ) != clipAllOutside ) 
	{
		if ( nMinY < m_pState->m_pClip->GetMinY() ) 
		{
			nMinY = m_pState->m_pClip->GetMinY();
		}
		if ( nMaxY > m_pState->m_pClip->GetMaxY() ) 
		{
			nMaxY = m_pState->m_pClip->GetMaxY();
		}

		SBlendFunc pOrigBlendFunc = m_pState->m_pBlendFunction;
		m_pState->m_pBlendFunction = &BlendXor;

		SPipe oPipe;
		PipeInit( &oPipe, 0, nMinY, m_pState->m_pFillPattern, NULL, 1, FALSE, FALSE );

		for ( int nY = nMinY; nY <= nMaxY; ++nY ) 
		{
			while ( pScanner->GetNextSpan( nY, &nX0, &nX1 ) ) 
			{
				if ( eClipResult == clipAllInside ) 
				{
					DrawSpan( &oPipe, nX0, nX1, nY, TRUE );
				} 
				else 
				{
					if ( nX0 < m_pState->m_pClip->GetMinX() ) 
					{
						nX0 = m_pState->m_pClip->GetMinX();
					}
					if ( nX1 > m_pState->m_pClip->GetMaxX() ) 
					{
						nX1 = m_pState->m_pClip->GetMaxX();
					}
					SClipResult eTempClipRes = m_pState->m_pClip->CheckSpan( nX0, nX1, nY );
					DrawSpan( &oPipe, nX0, nX1, nY, eTempClipRes == clipAllInside);
				}
			}
		}
		m_pState->m_pBlendFunction = pOrigBlendFunc;
	}
	m_pOpClipRes = eClipResult;

	delete pScanner;
	delete pXPath;
	return SNoError;
}

int SImage::FillChar(double dX, double dY, int nCode, SFont *pFont) 
{
	double dTransformX, dTransformY;

	Transform( m_pState->m_arrdMatrix, dX, dY, &dTransformX, &dTransformY );

	int nX     = (int)floor( dTransformX );
	int nFracX = (int)floor( (dTransformX - nX) * sfontFraction );
	int nY     = (int)floor( dTransformY );
	int nFracY = (int)floor( (dTransformY - nY) * sfontFraction );

	SGlyphBitmap oGlyph;
	if ( !pFont->GetGlyph( nCode, nFracX, nFracY, &oGlyph ) ) 
	{
		return SErrorNoGlyph;
	}
	int nError = FillGlyph2( nX, nY, &oGlyph );
	if ( oGlyph.bFreeData ) 
	{
		MemUtilsFree( oGlyph.pData );
	}
	return nError;
}

int SImage::FillGlyph(double dX, double dY, SGlyphBitmap *pGlyph) 
{
	double dTransformX, dTransformY;
	Transform( m_pState->m_arrdMatrix, dX, dY, &dTransformX, &dTransformY );

	int nX = (int)floor( dTransformX );
	int nY = (int)floor( dTransformY );

	return FillGlyph2( nX, nY, pGlyph);
}

int SImage::FillGlyph2(int nX, int nY, SGlyphBitmap *pGlyph) 
{
	SPipe oPipe;
	SClipResult eClipResult;
	if ( ( eClipResult = m_pState->m_pClip->CheckRectangle( nX - pGlyph->nX, nY - pGlyph->nY, nX - pGlyph->nX + pGlyph->nWidth - 1, nY - pGlyph->nY + pGlyph->nHeight - 1 ) ) != clipAllOutside ) 
	{
		BOOL bNoClip = (eClipResult == clipAllInside);

		if ( bNoClip ) 
		{
			if ( pGlyph->bAA ) 
			{
				PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, m_pState->m_pFillPattern, NULL, m_pState->m_dFillAlpha, TRUE, FALSE );
				unsigned char *pData = pGlyph->pData;
				
				for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
				{
					PipeSetXY( &oPipe, nX - pGlyph->nX, nY1 );
					for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; ++nXX, ++nX1 ) 
					{
						int nAlpha = *pData++;
						if ( nAlpha != 0 ) 
						{
							oPipe.dShape = (double)(nAlpha / 255.0);
							PipeRun( &oPipe );
							UpdateModX( nX1 );
							UpdateModY( nY1 );
						} 
						else 
						{
							PipeIncreaseX( &oPipe );
						}
					}
				}
			} 
			else 
			{
				PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, m_pState->m_pFillPattern, NULL, m_pState->m_dFillAlpha, FALSE, FALSE );
				unsigned char *pData = pGlyph->pData;

				for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
				{
					PipeSetXY( &oPipe, nX - pGlyph->nX, nY1); 
					for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; nXX += 8 ) 
					{
						int nAlpha0 = *pData++;
						for ( int nXX1 = 0; nXX1 < 8 && nXX + nXX1 < pGlyph->nWidth; ++nXX1, ++nX1 ) 
						{
							if ( nAlpha0 & 0x80 ) 
							{
								PipeRun( &oPipe );
								UpdateModX( nX1 );
								UpdateModY( nY1 );
							} 
							else 
							{
								PipeIncreaseX( &oPipe );
							}
							nAlpha0 <<= 1;
						}
					}
				}
			}
		} 
		else 
		{
			if ( pGlyph->bAA ) 
			{
				PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, m_pState->m_pFillPattern, NULL, m_pState->m_dFillAlpha, TRUE, FALSE );
				unsigned char *pData = pGlyph->pData;

				for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
				{
					PipeSetXY( &oPipe, nX - pGlyph->nX, nY1 );
					for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; ++nXX, ++nX1 ) 
					{
						if ( m_pState->m_pClip->IsInsideClip( nX1, nY1 ) ) 
						{
							int nAlpha = *pData++;
							if ( nAlpha != 0 ) 
							{
								oPipe.dShape = (double)(nAlpha / 255.0);
								PipeRun( &oPipe );
								UpdateModX( nX1 );
								UpdateModY( nY1 );
							} 
							else 
							{
								PipeIncreaseX( &oPipe );
							}
						} 
						else 
						{
							PipeIncreaseX( &oPipe );
							++pData;
						}
					}
				}
			} 
			else 
			{
				PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, m_pState->m_pFillPattern, NULL, m_pState->m_dFillAlpha, FALSE, FALSE );
				unsigned char *pData = pGlyph->pData;

				for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
				{
					PipeSetXY( &oPipe, nX - pGlyph->nX, nY1 );
					for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; nXX += 8 ) 
					{
						int nAlpha0 = *pData++;
						for ( int nXX1 = 0; nXX1 < 8 && nXX + nXX1 < pGlyph->nWidth; ++nXX1, ++nX1 ) 
						{
							if ( m_pState->m_pClip->IsInsideClip( nX1, nY1 ) ) 
							{
								if ( nAlpha0 & 0x80 ) 
								{
									PipeRun( &oPipe );
									UpdateModX( nX1 );
									UpdateModY( nY1 );
								} 
								else 
								{
									PipeIncreaseX( &oPipe );
								}
							} 
							else 
							{
								PipeIncreaseX( &oPipe );
							}
							nAlpha0 <<= 1;
						}
					}
				}
			}
		}
	}
	m_pOpClipRes = eClipResult;

	return SNoError;
}

int SImage::FillImageMask(SImageMaskSource pSrc, void *pSrcData, int nWidth, int nHeight, double *pMatrix, BOOL bGlyphMode) 
{
	// Проверяем матрицу на невырожденность
	if ( fabs( pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2] ) < 0.000001 ) 
	{
		return SErrorSingularMatrix;
	}

	// Вычисялем растяжение, сдвиг, поворот и перенос
	BOOL bRotate = fabs( pMatrix[1] ) > fabs( pMatrix[0] );
	double dScaleX, dScaleY, dShearX, dShearY;
	if ( bRotate ) 
	{
		dScaleX = -pMatrix[1];
		dScaleY =  pMatrix[2] - ( pMatrix[0] * pMatrix[3] ) / pMatrix[1];
		dShearX = -pMatrix[3] / dScaleY;
		dShearY = -pMatrix[0] / pMatrix[1];
	} 
	else 
	{
		dScaleX = pMatrix[0];
		dScaleY = pMatrix[3] - ( pMatrix[1] * pMatrix[2] ) / pMatrix[0];
		dShearX = pMatrix[2] / dScaleY;
		dShearY = pMatrix[1] / pMatrix[0];
	}

	int nTransX, nTransY, nTransX2, nTransY2;
	if ( bGlyphMode ) 
	{
		if ( dScaleX >= 0 ) 
		{
			nTransX  = round( pMatrix[4] );
			nTransX2 = round( pMatrix[4] + dScaleX ) - 1;
		} 
		else 
		{
			nTransX  = round( pMatrix[4] ) - 1;
			nTransX2 = round( pMatrix[4] + dScaleX );
		}
	} 
	else 
	{
		if ( dScaleX >= 0 ) 
		{
			nTransX  = (int)floor( pMatrix[4] - 0.01 );
			nTransX2 = (int)floor( pMatrix[4] + dScaleX + 0.01 );
		} 
		else 
		{
			nTransX  = (int)floor( pMatrix[4] + 0.01 );
			nTransX2 = (int)floor( pMatrix[4] + dScaleX - 0.01 );
		}
	}
	int nScaledWidth = abs( nTransX2 - nTransX ) + 1;
	if ( bGlyphMode ) 
	{
		if ( dScaleY >= 0 ) 
		{
			nTransY  = round( pMatrix[5] );
			nTransY2 = round( pMatrix[5] + dScaleY ) - 1;
		} 
		else 
		{
			nTransY  = round( pMatrix[5] ) - 1;
			nTransY2 = round( pMatrix[5] + dScaleY );
		}
	} 
	else 
	{
		if ( dScaleY >= 0 ) 
		{
			nTransY  = (int)floor( pMatrix[5] - 0.01 );
			nTransY2 = (int)floor( pMatrix[5] + dScaleY + 0.01 );
		} 
		else 
		{
			nTransY  = (int)floor( pMatrix[5] + 0.01 );
			nTransY2 = (int)floor( pMatrix[5] + dScaleY - 0.01 );
		}
	}
	int nScaledHeight = abs( nTransY2 - nTransY ) + 1;
	int nSignX = ( dScaleX < 0 ) ? -1 : 1;
	int nSignY = ( dScaleY < 0 ) ? -1 : 1;
	double dShearY1 = (double)nSignX * dShearY;

	// Clipping
	int nTLx1 = 0; // Top-Left-x
	int nTLy1 = 0; // Top-Left-y
	int nTRx1 = nSignX * ( nScaledWidth - 1 ); // Top-Right-x
	int nTRy1 = (int)( dShearY * nTRx1 );      // Top-Right-y
	int nBLx1 = round( dShearX * nSignY * ( nScaledHeight - 1 ));          // Bottom-Left-x
	int nBLy1 = nSignY * ( nScaledHeight - 1 ) + (int)( dShearY * nBLx1 ); // Bottom-Left-y
	int nBRx1 = nSignX * ( nScaledWidth - 1 ) + round( dShearX * nSignY * ( nScaledHeight - 1 )); // Bottom-Right-x
	int nBRy1 = nSignY * ( nScaledHeight - 1 ) + (int)( dShearY * nBRx1 );                        // Bottom-Right-y

	int nTLx, nTLy, nTRx, nTRy, nBLx, nBLy, nBRx, nBRy;
	if ( bRotate ) 
	{
		nTLx = nTransX + nTLy1; nTLy = nTransY - nTLx1;
		nTRx = nTransX + nTRy1; nTRy = nTransY - nTRx1;
		nBLx = nTransX + nBLy1; nBLy = nTransY - nBLx1;
		nBRx = nTransX + nBRy1; nBRy = nTransY - nBRx1;
	} 
	else 
	{
		nTLx = nTransX + nTLx1; nTLy = nTransY + nTLy1;
		nTRx = nTransX + nTRx1; nTRy = nTransY + nTRy1;
		nBLx = nTransX + nBLx1; nBLy = nTransY + nBLy1;
		nBRx = nTransX + nBRx1; nBRy = nTransY + nBRy1;
	}
	int nMinX = (nTLx < nTRx) ? (nTLx < nBLx) ? (nTLx < nBRx) ? nTLx : nBRx : (nBLx < nBRx) ? nBLx : nBRx : (nTRx < nBLx) ? (nTRx < nBRx) ? nTRx : nBRx : (nBLx < nBRx) ? nBLx : nBRx;
	int nMaxX = (nTLx > nTRx) ? (nTLx > nBLx) ? (nTLx > nBRx) ? nTLx : nBRx : (nBLx > nBRx) ? nBLx : nBRx : (nTRx > nBLx) ? (nTRx > nBRx) ? nTRx : nBRx : (nBLx > nBRx) ? nBLx : nBRx;
	int nMinY = (nTLy < nTRy) ? (nTLy < nBLy) ? (nTLy < nBRy) ? nTLy : nBRy : (nBLy < nBRy) ? nBLy : nBRy : (nTRy < nBLy) ? (nTRy < nBRy) ? nTRy : nBRy : (nBLy < nBRy) ? nBLy : nBRy;
	int nMaxY = (nTLy > nTRy) ? (nTLy > nBLy) ? (nTLy > nBRy) ? nTLy : nBRy : (nBLy > nBRy) ? nBLy : nBRy : (nTRy > nBLy) ? (nTRy > nBRy) ? nTRy : nBRy : (nBLy > nBRy) ? nBLy : nBRy;
	SClipResult eClipResult = m_pState->m_pClip->CheckRectangle( nMinX, nMinY, nMaxX, nMaxY);
	m_pOpClipRes = eClipResult;

	// Вычисляем параметры Брезенхема для растяжения по X и Y
	int nYp = nHeight / nScaledHeight;
	int nYq = nHeight % nScaledHeight;
	int nXp = nWidth / nScaledWidth;
	int nXq = nWidth % nScaledWidth;

	// Выделяем память
	SColorPointer pPixels = (SColorPointer)MemUtilsMalloc( ( nYp + 1 ) * nWidth );
	if ( !pPixels )
		return SErrorMemory;

	// Начинаем Pipeline для пикселей
	SPipe oPipe;
	PipeInit( &oPipe, 0, 0, m_pState->m_pFillPattern, NULL, m_pState->m_dFillAlpha, TRUE, FALSE );
	if ( m_bVectorAA ) 
	{
		DrawAAPixelInit();
	}

	int nYt = 0;
	int nLastStepY = 1;

	for ( int nY = 0; nY < nScaledHeight; ++nY ) 
	{
		int nStepY = nYp;
		nYt += nYq;
		if ( nYt >= nScaledHeight ) 
		{
			nYt -= nScaledHeight;
			++nStepY;
		}

		// Считываем линию(линии) из pSrc
		int nLen = ( nYp > 0 ) ? nStepY : nLastStepY;
		if ( nLen > 0 ) 
		{
			SColorPointer pCur = pPixels;
			for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
			{
				(*pSrc)( pSrcData, pCur );
				pCur += nWidth;
			}
		}
		nLastStepY = nStepY;

		int nKoef = round( dShearX * nSignY * nY );

		// Проверяем Clipping 
		SClipResult eClipResult2;
		if ( eClipResult != clipAllInside && !bRotate && (int)( dShearY * nKoef ) == (int)( dShearY * ( nSignX * ( nScaledWidth - 1 ) + nKoef ) ) ) 
		{
			int nSpanMinX, nSpanMaxX;
			if ( nSignX > 0 ) 
			{
				nSpanMinX = nTransX + nKoef;
				nSpanMaxX = nSpanMinX + ( nScaledWidth - 1 );
			} 
			else 
			{
				nSpanMaxX = nTransX + nKoef;
				nSpanMinX = nSpanMaxX - ( nScaledWidth - 1 );
			}
			int nSpanY = nTransY + nSignY * nY + (int)( dShearY * nKoef );
			eClipResult2 = m_pState->m_pClip->CheckSpan( nSpanMinX, nSpanMaxX, nSpanY );
			if ( eClipResult2 == clipAllOutside ) 
			{
				continue;
			}
		} 
		else 
		{
			eClipResult2 = eClipResult;
		}

		int nXt = 0;
		int nSrcX = 0;

		int nX1 = nKoef;

		double dY1 = (double)nSignY * nY + dShearY * nX1;

		// Если dShearY1 отрицательно, тогда (int)y1 изменится сразу после первого пикселя, этого нужно избежать
		if ( dShearY1 < 0 ) 
		{
			dY1 += 0.999;
		}

		nLen = ( nStepY > 0 ) ? nStepY : 1;

		for ( int nX = 0; nX < nScaledWidth; ++nX ) 
		{
			int nStepX = nXp;
			nXt += nXq;
			if ( nXt >= nScaledWidth ) 
			{
				nXt -= nScaledWidth;
				++nStepX;
			}

			// Поворот
			int nX2, nY2;
			if ( bRotate ) 
			{
				nX2 = (int)dY1;
				nY2 = -nX1;
			} 
			else 
			{
				nX2 = nX1;
				nY2 = (int)dY1;
			}

			// Вычислим значение альфа для точки (x,y) после операций растяжения
			int nM = ( nStepX > 0 ) ? nStepX : 1;
			SColorPointer pCur = pPixels + nSrcX;
			int nPixAcc = 0;
			for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
			{
				for ( int nJ = 0; nJ < nM; ++nJ ) 
				{
					nPixAcc += *pCur++;
				}
				pCur += nWidth - nM;
			}

			if ( nPixAcc != 0 ) 
			{
				oPipe.dShape = ( nPixAcc == nLen * nM ) ? (double)1 : (double)nPixAcc / (double)( nLen * nM );
				if ( m_bVectorAA && eClipResult2 != clipAllInside ) 
				{
					DrawAAPixel( &oPipe, nTransX + nX2, nTransY + nY2 );
				} 
				else 
				{
					DrawPixel( &oPipe, nTransX + nX2, nTransY + nY2, eClipResult2 == clipAllInside );
				}
			}

			nSrcX += nStepX;

			// сдвиг по x
			nX1 += nSignX;

			// сдвиг по y
			dY1 += dShearY1;
		}
	}

	// Освобождаем память
	MemUtilsFree( pPixels );

	return SNoError;
}

int SImage::DrawImage(SImageSource pSrc, void *pSrcData, SColorMode eColorMode, BOOL bAlpha, int nWidth, int nHeight, double *pMatrix) 
{
	// Проверяем корректность 
	BOOL bValidMode = FALSE;
	int  nComponentsCount = 0;
	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
	case colorModeMono8:
		bValidMode = ( eColorMode == colorModeMono8 );
		nComponentsCount = 1;
		break;
	case colorModeRGB8:
		bValidMode = ( eColorMode == colorModeRGB8 );
		nComponentsCount = 3;
		break;
	case colorModeBGR8:
		bValidMode = ( eColorMode == colorModeBGR8 );
		nComponentsCount = 3;
	break;
	}
	if ( !bValidMode ) 
	{
		return SErrorColorModeMismatch;
	}

	// Проверяем матрицу на невырожденность
	if ( fabs( pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2] ) < 0.000001 ) 
	{
		return SErrorSingularMatrix;
	}

	// Вычисялем растяжение, сдвиг, поворот и перенос
	BOOL bRotate = fabs( pMatrix[1] ) > fabs( pMatrix[0] );
	double dScaleX, dScaleY, dShearX, dShearY;
	if ( bRotate ) 
	{
		dScaleX = -pMatrix[1];
		dScaleY =  pMatrix[2] - ( pMatrix[0] * pMatrix[3] ) / pMatrix[1];
		dShearX = -pMatrix[3] / dScaleY;
		dShearY = -pMatrix[0] / pMatrix[1];
	} 
	else 
	{
		dScaleX = pMatrix[0];
		dScaleY = pMatrix[3] - ( pMatrix[1] * pMatrix[2] ) / pMatrix[0];
		dShearX = pMatrix[2] / dScaleY;
		dShearY = pMatrix[1] / pMatrix[0];
	}

	int nTransX, nTransY, nTransX2, nTransY2; 
	if ( dScaleX >= 0 ) 
	{
		nTransX  = (int)floor( pMatrix[4] - 0.01 );
		nTransX2 = (int)floor( pMatrix[4] + dScaleX + 0.01 );
	} 
	else 
	{
		nTransX  = (int)floor( pMatrix[4] + 0.01 );
		nTransX2 = (int)floor( pMatrix[4] + dScaleX - 0.01 );
	}
	int nScaledWidth = abs( nTransX2 - nTransX ) + 1;
	if ( dScaleY >= 0 ) 
	{
		nTransY  = (int)floor( pMatrix[5] - 0.01 );
		nTransY2 = (int)floor( pMatrix[5] + dScaleY + 0.01 );
	} 
	else 
	{
		nTransY  = (int)floor( pMatrix[5] + 0.01 );
		nTransY2 = (int)floor( pMatrix[5] + dScaleY - 0.01 );
	}
	int nScaledHeight = abs( nTransY2 - nTransY ) + 1;
	int nSignX = ( dScaleX < 0 ) ? -1 : 1;
	int nSignY = ( dScaleY < 0 ) ? -1 : 1;
	
	double dShearY1 = (double)nSignX * dShearY;

	// Clipping
	int nTLx1 = 0; // Top-Left-x
	int nTLy1 = 0; // Top-Left-y
	int nTRx1 = nSignX * ( nScaledWidth - 1 ); // Top-Right-x
	int nTRy1 = (int)( dShearY * nTRx1 );      // Top-Right-y
	int nBLx1 = round( dShearX * nSignY * ( nScaledHeight - 1 ) );         // Bottom-Left-x
	int nBLy1 = nSignY * ( nScaledHeight - 1 ) + (int)( dShearY * nBLx1 ); // Bottom-Left-y
	int nBRx1 = nSignX * ( nScaledWidth - 1 ) +  round( dShearX * nSignY * ( nScaledHeight - 1 ) ); // Bottom-Right-x
	int nBRy1 = nSignY * ( nScaledHeight - 1 ) + (int)( dShearY * nBRx1 );                          // Bottom-Right-y

	int nTLx, nTLy, nTRx, nTRy, nBLx, nBLy, nBRx, nBRy;
	if ( bRotate ) 
	{
		nTLx = nTransX + nTLy1; nTLy = nTransY - nTLx1;
		nTRx = nTransX + nTRy1; nTRy = nTransY - nTRx1;
		nBLx = nTransX + nBLy1; nBLy = nTransY - nBLx1;
		nBRx = nTransX + nBRy1; nBRy = nTransY - nBRx1;
	} 
	else 
	{
		nTLx = nTransX + nTLx1; nTLy = nTransY + nTLy1;
		nTRx = nTransX + nTRx1; nTRy = nTransY + nTRy1;
		nBLx = nTransX + nBLx1; nBLy = nTransY + nBLy1;
		nBRx = nTransX + nBRx1; nBRy = nTransY + nBRy1;
	}
	int nMinX = (nTLx < nTRx) ? (nTLx < nBLx) ? (nTLx < nBRx) ? nTLx : nBRx : (nBLx < nBRx) ? nBLx : nBRx : (nTRx < nBLx) ? (nTRx < nBRx) ? nTRx : nBRx : (nBLx < nBRx) ? nBLx : nBRx;
	int nMaxX = (nTLx > nTRx) ? (nTLx > nBLx) ? (nTLx > nBRx) ? nTLx : nBRx : (nBLx > nBRx) ? nBLx : nBRx : (nTRx > nBLx) ? (nTRx > nBRx) ? nTRx : nBRx : (nBLx > nBRx) ? nBLx : nBRx;
	int nMinY = (nTLy < nTRy) ? (nTLy < nBLy) ? (nTLy < nBRy) ? nTLy : nBRy : (nBLy < nBRy) ? nBLy : nBRy : (nTRy < nBLy) ? (nTRy < nBRy) ? nTRy : nBRy : (nBLy < nBRy) ? nBLy : nBRy;
	int nMaxY = (nTLy > nTRy) ? (nTLy > nBLy) ? (nTLy > nBRy) ? nTLy : nBRy : (nBLy > nBRy) ? nBLy : nBRy : (nTRy > nBLy) ? (nTRy > nBRy) ? nTRy : nBRy : (nBLy > nBRy) ? nBLy : nBRy;
	SClipResult eClipResult = m_pState->m_pClip->CheckRectangle( nMinX, nMinY, nMaxX, nMaxY );
	m_pOpClipRes = eClipResult;
	if ( eClipResult == clipAllOutside ) 
	{
		return SNoError;
	}

	// Вычисляем параметры Брезенхема для растяжения по X и Y
	int nYp = nHeight / nScaledHeight;
	int nYq = nHeight % nScaledHeight;
	int nXp = nWidth / nScaledWidth;
	int nXq = nWidth % nScaledWidth;

	// Выделяем память
	SColorPointer pPixels = (SColorPointer)MemUtilsMalloc( ( nYp + 1 ) * nWidth * nComponentsCount );
	if ( !pPixels )
		return SErrorMemory;

	unsigned char *pAlphaBuffer;
	if ( bAlpha ) 
	{
		pAlphaBuffer = (unsigned char *)MemUtilsMalloc( ( nYp + 1 ) * nWidth );
		if ( !pAlphaBuffer )
		{
			MemUtilsFree( pPixels );
			return SErrorMemory;
		}
	} 
	else 
	{
		pAlphaBuffer = NULL;
	}

	int nPixAcc0 = 0, nPixAcc1 = 0, nPixAcc2 = 0;

	// Начинаем Pipeline для пикселей
	SPipe oPipe;

	SColor pPixelColor;
	PipeInit( &oPipe, 0, 0, NULL, pPixelColor, m_pState->m_dFillAlpha, bAlpha || ( m_bVectorAA && eClipResult != clipAllInside ), FALSE );
	if ( m_bVectorAA ) 
	{
		DrawAAPixelInit();
	}

	SColorPointer pCur;
	unsigned char *pCurAlpha;
	double dPixMul, dAlphaMul, dAlpha;
	int nX1, nX2, nY2;
	double dY1;
	int nLen, nM;

	if ( bAlpha ) 
	{
		int nYt = 0;
		int nLastStepY = 1;

		for ( int nY = 0; nY < nScaledHeight; ++nY ) 
		{
			int nStepY = nYp;
			nYt += nYq;
			if ( nYt >= nScaledHeight ) 
			{
				nYt -= nScaledHeight;
				++nStepY;
			}

			// Считываем одну линию из pSrcData
			nLen = ( nYp > 0 ) ? nStepY : nLastStepY;
			if ( nLen > 0 ) 
			{
				pCur = pPixels;
				pCurAlpha = pAlphaBuffer;
				for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
				{
					(*pSrc)( pSrcData, pCur, pCurAlpha );
					pCur += nWidth * nComponentsCount;
					pCurAlpha += nWidth;
				}
			}
			nLastStepY = nStepY;

			int nKoef = round( dShearX * nSignY * nY );

			// Проверяем Clipping
			SClipResult eClipResult2;
			if ( eClipResult != clipAllInside && !bRotate && (int)( dShearY * nKoef ) == (int)( dShearY * ( nSignX * ( nScaledWidth - 1 ) + nKoef ) ) ) 
			{
				int nSpanMinX, nSpanMaxX;
				if ( nSignX > 0 ) 
				{
					nSpanMinX = nTransX + nKoef;
					nSpanMaxX = nSpanMinX + ( nScaledWidth - 1 );
				} 
				else 
				{
					nSpanMaxX = nTransX + nKoef;
					nSpanMinX = nSpanMaxX - ( nScaledWidth - 1 );
				}
				int nSpanY = nTransY + nSignY * nY + (int)( dShearY * nKoef );
				eClipResult2 = m_pState->m_pClip->CheckSpan( nSpanMinX, nSpanMaxX, nSpanY );
				if ( eClipResult2 == clipAllOutside ) 
				{
					continue;
				}
			} 
			else 
			{
				eClipResult2 = eClipResult;
			}

			int nXt = 0;
			int nSrcX = 0;

			// Сдвиг по x
			nX1 = nKoef;

			// Сдвиг по y
			dY1 = (double)nSignY * nY + dShearY * nX1;

			// Если dShearY1 отрицательно, тогда (int)y1 изменится сразу после первого пикселя, этого нужно избежать
			if ( dShearY1 < 0 ) 
			{
				dY1 += 0.999;
			}

			nLen = ( nStepY > 0 ) ? nStepY : 1;

			switch ( eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				for ( int nX = 0; nX < nScaledWidth; ++nX ) 
				{
					int nStepX = nXp;
					nXt += nXq;
					
					if ( nXt >= nScaledWidth ) 
					{
						nXt -= nScaledWidth;
						++nStepX;
					}
					// Поворот
					if ( bRotate ) 
					{
						nX2 = (int)dY1;
						nY2 = -nX1;
					}
					else 
					{
						nX2 = nX1;
						nY2 = (int)dY1;
					}
					
					nM = ( nStepX > 0 ) ? nStepX : 1;
					int nAlphaAcc = 0;
					pCur = pPixels + nSrcX;
					pCurAlpha = pAlphaBuffer + nSrcX;
					nPixAcc0 = 0;
					for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
					{
						for ( int nJ = 0; nJ < nM; ++nJ ) 
						{
							nPixAcc0 += *pCur++;
							nAlphaAcc += *pCurAlpha++;
						}
						pCur += nWidth - nM;
						pCurAlpha += nWidth - nM;
					}
					dPixMul = (double)1 / (double)(nLen * nM);
					dAlphaMul = dPixMul * (1.0 / 255.0);
					dAlpha = (double)nAlphaAcc * dAlphaMul;

					if ( dAlpha > 0 ) 
					{
						pPixelColor[0] = (int)((double)nPixAcc0 * dPixMul);

						oPipe.dShape = dAlpha;
						if ( m_bVectorAA && eClipResult != clipAllInside ) 
						{
							DrawAAPixel( &oPipe, nTransX + nX2, nTransY + nY2 );
						} 
						else 
						{
							DrawPixel( &oPipe, nTransX + nX2, nTransY + nY2, eClipResult2 == clipAllInside );
						}
					}

					nSrcX += nStepX;

					// Сдвиг по x
					nX1 += nSignX;

					// Сдвиг по y
					dY1 += dShearY1;
				}
				break;

			case colorModeRGB8:
			case colorModeBGR8:
				for ( int nX = 0; nX < nScaledWidth; ++nX ) 
				{
					int nStepX = nXp;
					nXt += nXq;
					if ( nXt >= nScaledWidth ) 
					{
						nXt -= nScaledWidth;
						++nStepX;
					}
					// Поворот
					if ( bRotate ) 
					{
						nX2 = (int)dY1;
						nY2 = -nX1;
					} 
					else 
					{
						nX2 = nX1;
						nY2 = (int)dY1;
					}

					nM = ( nStepX > 0 ) ? nStepX : 1;
					int nAlphaAcc = 0;
					pCur = pPixels + nSrcX * 3;
					pCurAlpha = pAlphaBuffer + nSrcX;
					nPixAcc0 = nPixAcc1 = nPixAcc2 = 0;
					
					for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
					{
						for ( int nJ = 0; nJ < nM; ++nJ ) 
						{
							nPixAcc0 += *pCur++;
							nPixAcc1 += *pCur++;
							nPixAcc2 += *pCur++;
							nAlphaAcc += *pCurAlpha++;
						}
						pCur += 3 * (nWidth - nM);
						pCurAlpha += nWidth - nM;
					}
					dPixMul = (double)1 / (double)(nLen * nM);
					dAlphaMul = dPixMul * (1.0 / 255.0);
					dAlpha = (double)nAlphaAcc * dAlphaMul;

					if ( dAlpha > 0 ) 
					{
						pPixelColor[0] = (int)((double)nPixAcc0 * dPixMul);
						pPixelColor[1] = (int)((double)nPixAcc1 * dPixMul);
						pPixelColor[2] = (int)((double)nPixAcc2 * dPixMul);

						oPipe.dShape = dAlpha;
						if ( m_bVectorAA && eClipResult != clipAllInside ) 
						{
							DrawAAPixel( &oPipe, nTransX + nX2, nTransY + nY2 );
						} 
						else 
						{
							DrawPixel( &oPipe, nTransX + nX2, nTransY + nY2, eClipResult2 == clipAllInside );
						}
					}

					nSrcX += nStepX;

					// Сдвиг по X
					nX1 += nSignX;

					// Сдвиг по Y
					dY1 += dShearY1;
				}
				break;
			}
		}
	} 
	else 
	{
		int nYt = 0;
		int nLastStepY = 1;

		for ( int nY = 0; nY < nScaledHeight; ++nY ) 
		{
			int nStepY = nYp;
			nYt += nYq;
			if ( nYt >= nScaledHeight ) 
			{
				nYt -= nScaledHeight;
				++nStepY;
			}

			// Считываем одну линию из pSrcData
			nLen = ( nYp > 0 ) ? nStepY : nLastStepY;
			if ( nLen > 0 ) 
			{
				pCur = pPixels;
				for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
				{
					(*pSrc)( pSrcData, pCur, NULL );
					pCur += nWidth * nComponentsCount;
				}
			}
			nLastStepY = nStepY;

			int nKoef = round( dShearX * nSignY * nY );

			// Проверяем Clipping
			SClipResult eClipResult2;
			if ( eClipResult != clipAllInside && !bRotate && (int)( dShearY * nKoef ) == (int)( dShearY * ( nSignX * ( nScaledWidth - 1 ) + nKoef ) ) ) 
			{
				int nSpanMinX, nSpanMaxX;
				if ( nSignX > 0 ) 
				{
					nSpanMinX = nTransX + nKoef;
					nSpanMaxX = nSpanMinX + ( nScaledWidth - 1 );
				} 
				else 
				{
					nSpanMaxX = nTransX + nKoef;
					nSpanMinX = nSpanMaxX - ( nScaledWidth - 1 );
				}
				int nSpanY = nTransY + nSignY * nY + (int)( dShearY * nKoef );
				eClipResult2 = m_pState->m_pClip->CheckSpan( nSpanMinX, nSpanMaxX, nSpanY );
				if ( eClipResult2 == clipAllOutside ) 
				{
					continue;
				}
			} 
			else 
			{
				eClipResult2 = eClipResult;
			}

			int nXt = 0;
			int nSrcX = 0;

			// Сдвиг по x
			nX1 = nKoef;

			// Сдвиг по y 
			dY1 = (double)nSignY * nY + dShearY * nX1;

			// Если dShearY1 отрицательно, тогда (int)y1 изменится сразу после первого пикселя, этого нужно избежать
			if ( dShearY1 < 0 ) 
			{
				dY1 += 0.999;
			}

			nLen = ( nStepY > 0 ) ? nStepY : 1;

			switch ( eColorMode ) 
			{
			case colorModeMono1:
			case colorModeMono8:
				for ( int nX = 0; nX < nScaledWidth; ++nX ) 
				{
					int nStepX = nXp;
					nXt += nXq;
					if ( nXt >= nScaledWidth ) 
					{
						nXt -= nScaledWidth;
						++nStepX;
					}
					// Поворот
					if ( bRotate ) 
					{
						nX2 = (int)dY1;
						nY2 = -nX1;
					} 
					else 
					{
						nX2 = nX1;
						nY2 = (int)dY1;
					}

					nM = ( nStepX > 0 ) ? nStepX : 1;
					pCur = pPixels + nSrcX;
					nPixAcc0 = 0;
					
					for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
					{
						for ( int nJ = 0; nJ < nM; ++nJ ) 
						{
							nPixAcc0 += *pCur++;
						}
						pCur += nWidth - nM;
					}
					dPixMul = (double)1 / (double)(nLen * nM);

					pPixelColor[0] = (int)((double)nPixAcc0 * dPixMul);

					if ( m_bVectorAA && eClipResult != clipAllInside ) 
					{
						oPipe.dShape = (double)1;
						DrawAAPixel( &oPipe, nTransX + nX2, nTransY + nY2 );
					} 
					else 
					{
						DrawPixel( &oPipe, nTransX + nX2, nTransY + nY2, eClipResult2 == clipAllInside );
					}
					nSrcX += nStepX;

					// Сдвиг по x
					nX1 += nSignX;

					// Сдвиг по y
					dY1 += dShearY1;
				}
				break;
			case colorModeRGB8:
			case colorModeBGR8:
				for ( int nX = 0; nX < nScaledWidth; ++nX ) 
				{
					int nStepX = nXp;
					nXt += nXq;
					if ( nXt >= nScaledWidth ) 
					{
						nXt -= nScaledWidth;
						++nStepX;
					}

					// Поворот
					if ( bRotate ) 
					{
						nX2 = (int)dY1;
						nY2 = -nX1;
					} 
					else 
					{
						nX2 = nX1;
						nY2 = (int)dY1;
					}

					nM = ( nStepX > 0 ) ? nStepX : 1;
					pCur = pPixels + nSrcX * 3;
					nPixAcc0 = nPixAcc1 = nPixAcc2 = 0;
					for ( int nIndex = 0; nIndex < nLen; ++nIndex ) 
					{
						for ( int nJ = 0; nJ < nM; ++nJ ) 
						{
							nPixAcc0 += *pCur++;
							nPixAcc1 += *pCur++;
							nPixAcc2 += *pCur++;
						}
						pCur += 3 * (nWidth - nM);
					}
					dPixMul = (double)1 / (double)(nLen * nM);

					pPixelColor[0] = (int)((double)nPixAcc0 * dPixMul);
					pPixelColor[1] = (int)((double)nPixAcc1 * dPixMul);
					pPixelColor[2] = (int)((double)nPixAcc2 * dPixMul);

					if ( m_bVectorAA && eClipResult != clipAllInside ) 
					{
						oPipe.dShape = (double)1;
						DrawAAPixel( &oPipe, nTransX + nX2, nTransY + nY2 );
					} 
					else 
					{
						DrawPixel( &oPipe, nTransX + nX2, nTransY + nY2, eClipResult2 == clipAllInside );
					}
					nSrcX += nStepX;

					// Сдвиг по x
					nX1 += nSignX;

					// Сдвиг по y
					dY1 += dShearY1;
				}
				break;
			}
		}
	}

	MemUtilsFree( pPixels );
	MemUtilsFree( pAlphaBuffer );

	return SNoError;
}

int SImage::Composite(SBitmap *pSrc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nWidth, int nHeight, BOOL bNoClip, BOOL bNonIsolated) 
{
	if ( pSrc->m_eMode != m_pBitmap->m_eMode )
	{
		return SErrorColorModeMismatch;
	}

	SPipe oPipe;
	SColor pPixelColor;
	if ( pSrc->m_pAlpha ) 
	{
		PipeInit( &oPipe, nDestX, nDestY, NULL, pPixelColor, m_pState->m_dFillAlpha, TRUE, bNonIsolated );
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			PipeSetXY( &oPipe, nDestX, nDestY + nY );
			unsigned char *pAlpha = pSrc->GetAlpha() + ( nSrcY + nY ) * pSrc->GetWidth() + nSrcX;
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				pSrc->GetPixel( nSrcX + nX, nSrcY + nY, pPixelColor );
				unsigned char unAlpha = *pAlpha++;
				if ( bNoClip || m_pState->m_pClip->IsInsideClip( nDestX + nX, nDestY + nY ) ) 
				{
					// Используем dShape всместо альфы(вообще говоря, это неправильно, но работает)
					oPipe.dShape = (double)(unAlpha / 255.0);
					PipeRun( &oPipe );
					UpdateModX( nDestX + nX );
					UpdateModY( nDestY + nY );
				} 
				else 
				{
					PipeIncreaseX( &oPipe );
				}
			}
		}
	} 
	else 
	{
		PipeInit( &oPipe, nDestX, nDestY, NULL, pPixelColor, m_pState->m_dFillAlpha, FALSE, bNonIsolated );
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			PipeSetXY( &oPipe, nDestX, nDestY + nY );
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				pSrc->GetPixel( nSrcX + nX, nSrcY + nY, pPixelColor );
				if ( bNoClip || m_pState->m_pClip->IsInsideClip( nDestX + nX, nDestY + nY ) ) 
				{
					PipeRun( &oPipe );
					UpdateModX( nDestX + nX );
					UpdateModY( nDestY + nY );
				} 
				else 
				{
					PipeIncreaseX( &oPipe );
				}
			}
		}
	}

	return SNoError;
}

void SImage::CompositeBackground(SColorPointer pColor) 
{
	SColorPointer pCurPixel;
	unsigned char *pCurAlpha;
	unsigned char unAlpha, unAlpha1, unColor0, unColor1, unColor2;

	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
		unColor0 = pColor[0];
		for ( int nY = 0; nY < m_pBitmap->m_nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride];
			pCurAlpha = &m_pBitmap->m_pAlpha[nY * m_pBitmap->m_nWidth];
			int nMask = 0x80;
			for ( int nX = 0; nX < m_pBitmap->m_nWidth; ++nX ) 
			{
				unAlpha = *pCurAlpha++;
				unAlpha1 = 255 - unAlpha;
				unsigned char unChar = (*pCurPixel & nMask) ? 0xff : 0x00;
				unChar = Div255( unAlpha1 * unColor0 + unAlpha * unChar );
				if ( unChar & 0x80 ) 
				{
					*pCurPixel |= nMask;
				} 
				else 
				{
					*pCurPixel &= ~nMask;
				}
				if ( !( nMask >>= 1 ) ) 
				{
					nMask = 0x80;
					++pCurPixel;
				}
			}
		}
		break;
	case colorModeMono8:
		unColor0 = pColor[0];
		for ( int nY = 0; nY < m_pBitmap->m_nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride];
			pCurAlpha = &m_pBitmap->m_pAlpha[nY * m_pBitmap->m_nWidth];
			for ( int nX = 0; nX < m_pBitmap->m_nWidth; ++nX ) 
			{
				unAlpha = *pCurAlpha++;
				unAlpha1 = 255 - unAlpha;
				pCurPixel[0] = Div255( unAlpha1 * unColor0 + unAlpha * pCurPixel[0] );
				++pCurPixel;
			}
		}
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		unColor0 = pColor[0];
		unColor1 = pColor[1];
		unColor2 = pColor[2];
		for ( int nY = 0; nY < m_pBitmap->m_nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[nY * m_pBitmap->m_nStride];
			pCurAlpha = &m_pBitmap->m_pAlpha[nY * m_pBitmap->m_nWidth];
			for ( int nX = 0; nX < m_pBitmap->m_nWidth; ++nX ) 
			{
				unAlpha = *pCurAlpha++;
				unAlpha1 = 255 - unAlpha;
				pCurPixel[0] = Div255( unAlpha1 * unColor0 + unAlpha * pCurPixel[0] );
				pCurPixel[1] = Div255( unAlpha1 * unColor1 + unAlpha * pCurPixel[1] );
				pCurPixel[2] = Div255( unAlpha1 * unColor2 + unAlpha * pCurPixel[2] );
				pCurPixel += 3;
			}
		}
		break;
	}
	memset( m_pBitmap->m_pAlpha, 255, m_pBitmap->m_nWidth * m_pBitmap->m_nHeight );
}

int SImage::BlitTransparent(SBitmap *pSrc, int nSrcX, int nSrcY, int nDestX, int nDestY, int nWidth, int nHeight) 
{
	if ( pSrc->m_eMode != m_pBitmap->m_eMode ) 
	{
		return SErrorColorModeMismatch;
	}

	SColor pPixelColor;
	SColorPointer pCurPixel;
	switch ( m_pBitmap->m_eMode ) 
	{
	case colorModeMono1:
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[(nDestY + nY) * m_pBitmap->m_nStride + (nDestX >> 3)];
			int nMask = 0x80 >> (nDestX & 7);
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				pSrc->GetPixel( nSrcX + nX, nSrcY + nY, pPixelColor );
				if ( pPixelColor[0] ) 
				{
					*pCurPixel |= nMask;
				} 
				else 
				{
					*pCurPixel &= ~nMask;
				}
				if ( !(nMask >>= 1) ) 
				{
					nMask = 0x80;
					++pCurPixel;
				}
			}
		}
		break;
	case colorModeMono8:
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[(nDestY + nY) * m_pBitmap->m_nStride + nDestX];
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				pSrc->GetPixel( nSrcX + nX, nSrcY + nY, pPixelColor );
				*pCurPixel++ = pPixelColor[0];
			}
		}
		break;
	case colorModeRGB8:
	case colorModeBGR8:
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			pCurPixel = &m_pBitmap->m_pData[(nDestY + nY) * m_pBitmap->m_nStride + 3 * nDestX];
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				pSrc->GetPixel( nSrcX + nX, nSrcY + nY, pPixelColor );
				*pCurPixel++ = pPixelColor[0];
				*pCurPixel++ = pPixelColor[1];
				*pCurPixel++ = pPixelColor[2];
			}
		}
		break;
	}

	if ( m_pBitmap->m_pAlpha ) 
	{
		for ( int nY = 0; nY < nHeight; ++nY ) 
		{
			unsigned char *pCurAlpha = &m_pBitmap->m_pAlpha[(nDestY + nY) * m_pBitmap->m_nWidth + nDestX];
			for ( int nX = 0; nX < nWidth; ++nX ) 
			{
				*pCurAlpha++ = 0x00;
			}
		}
	}

	return SNoError;
}

SPath *SImage::MakeStrokePath(SPath *pPath, BOOL bFlatten) 
{
	SPath *pPathIn, *pPathOut;
	if ( bFlatten ) 
	{
		pPathIn = FlattenPath( pPath, m_pState->m_arrdMatrix, m_pState->m_dFlatness );
		if ( m_pState->m_nLineDashCount > 0 ) 
		{
			pPathOut = MakeDashedPath( pPathIn );
			delete pPathIn;
			pPathIn = pPathOut;
		}
	} 
	else 
	{
		pPathIn = pPath;
	}

	int nSubpathStart = 0;
	BOOL bClosed = FALSE;
	int nLeft0 = 0, nLeft1 = 0, nRight0 = 0, nRight1 = 0, nJoin0 = 0, nJoin1 = 0, nLeft2, nRight2, nJoin2;
	int nLeftFirst = 0, nRightFirst = 0, nFirstPoint = 0;

	pPathOut = new SPath();
	double dWidth = m_pState->m_dLineWidth;

	for ( int nIndex = 0; nIndex < pPathIn->m_nPointsCount - 1; ++nIndex ) 
	{
		if ( pPathIn->m_pFlags[nIndex] & SPathLast ) 
		{
			continue;
		}

		BOOL bFirst;
		if ( ( bFirst = pPathIn->m_pFlags[nIndex] & SPathFirst ) ) 
		{
			nSubpathStart = nIndex;
			bClosed = pPathIn->m_pFlags[nIndex] & SPathClosed;
		}
		BOOL bLast = pPathIn->m_pFlags[nIndex + 1] & SPathLast;

		// Вычислим диагональ сегмента ( nIndex, nIndex + 1 )
		double dDist = distance( pPathIn->m_pPoints[nIndex].dX, pPathIn->m_pPoints[nIndex].dY, pPathIn->m_pPoints[nIndex + 1].dX, pPathIn->m_pPoints[nIndex + 1].dY );
		double dDx, dDy;
		if ( dDist == 0 ) 
		{
			dDx = 0;
			dDy = 1;
		} 
		else 
		{
			dDist = (double)1 / dDist;
			dDx = dDist * ( pPathIn->m_pPoints[nIndex + 1].dX - pPathIn->m_pPoints[nIndex].dX );
			dDy = dDist * ( pPathIn->m_pPoints[nIndex + 1].dY - pPathIn->m_pPoints[nIndex].dY );
		}
		double dWidthDx = (double)0.5 * dWidth * dDx;
		double dWidthDy = (double)0.5 * dWidth * dDy;

		// Вычислим диагональ сегмента ( nIndex + 1, nNext )
		int nNext = bLast ? nSubpathStart + 1 : nIndex + 2;
		dDist = distance( pPathIn->m_pPoints[nIndex + 1].dX, pPathIn->m_pPoints[nIndex + 1].dY, pPathIn->m_pPoints[nNext].dX, pPathIn->m_pPoints[nNext].dY );
		double dNextDx, dNextDy;
		if ( dDist == 0 ) 
		{
			dNextDx = 0;
			dNextDy = 1;
		} 
		else 
		{
			dDist = (double)1 / dDist;
			dNextDx = dDist * ( pPathIn->m_pPoints[nNext].dX - pPathIn->m_pPoints[nIndex + 1].dX );
			dNextDy = dDist * ( pPathIn->m_pPoints[nNext].dY - pPathIn->m_pPoints[nIndex + 1].dY );
		}
		double dWidthNextDx = (double)0.5 * dWidth * dNextDx;
		double dWidthNextDy = (double)0.5 * dWidth * dNextDy;

		// Рисуем начало линии
		pPathOut->MoveTo( pPathIn->m_pPoints[nIndex].dX - dWidthDy, pPathIn->m_pPoints[nIndex].dY + dWidthDx );
		if ( nIndex == nSubpathStart ) 
		{
			nFirstPoint = pPathOut->m_nPointsCount - 1;
		}
		if ( bFirst && !bClosed ) 
		{
			switch ( m_pState->m_nLineCap ) 
			{ 
			case LineCapButt:
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx );
				break;
			case LineCapRound:
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex].dX - dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dY + dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dX - dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dX - dWidthDx, pPathIn->m_pPoints[nIndex].dY - dWidthDy );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex].dX - dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dX + dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dY - dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dX + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx );
				break;
			case LineCapProjecting:
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX - dWidthDx - dWidthDy, pPathIn->m_pPoints[nIndex].dY + dWidthDx - dWidthDy );
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX - dWidthDx + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx - dWidthDy );
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX + dWidthDy,            pPathIn->m_pPoints[nIndex].dY - dWidthDx            );
				break;
			}
		} 
		else 
		{
			pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx );
		}

		// Рисуем левую стороную для прямоугольника сегмента
		nLeft2 = pPathOut->m_nPointsCount - 1;
		pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx );

		// Рисуем окончание линии
		if ( bLast && !bClosed ) 
		{
			switch (m_pState->m_nLineCap) 
			{
			case LineCapButt:
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
				break;
			case LineCapRound:
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
				break;
			case LineCapProjecting:
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy + dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx + dWidthDy ); 
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy + dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx + dWidthDy );
				pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy,            pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx            );
				break;
			}
		} 
		else 
		{
			pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
		}

		// Рисуем правую стороную для прямоугольника сегмента
		nRight2 = pPathOut->m_nPointsCount - 1;
		pPathOut->Close();

		// Рисуем соединение
		nJoin2 = pPathOut->m_nPointsCount;
		if ( !bLast || bClosed ) 
		{
			double dCrossprod =   dDx * dNextDy - dDy * dNextDx;
			double dDotprod   = -(dDx * dNextDx + dDy * dNextDy);
			double dMiter, dMit;
			if ( dDotprod > 0.99999 ) 
			{
				// Для избежания делений на ноль
				dMiter = ( m_pState->m_dMiterLimit + 1 ) * ( m_pState->m_dMiterLimit + 1 );
				dMit = 0;
			} 
			else 
			{
				dMiter = (double)2 / ((double)1 - dDotprod);
				if ( dMiter < 1 ) 
				{
					dMiter = 1;
				}
				dMit = sqrt( dMiter - 1 );
			}

			// Скругленное соединение
			if ( m_pState->m_nLineJoin == LineJoinRound ) 
			{
				pPathOut->MoveTo ( pPathIn->m_pPoints[nIndex + 1].dX + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY +  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX +  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX,                        pPathIn->m_pPoints[nIndex + 1].dY + (double)0.5 * dWidth );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX -  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX - (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY +  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX - (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY                        );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX - (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY -  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX -  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY - (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX,                        pPathIn->m_pPoints[nIndex + 1].dY - (double)0.5 * dWidth );
				pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX +  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY - (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY -  c_dKappa_2 * dWidth, pPathIn->m_pPoints[nIndex + 1].dX + (double)0.5 * dWidth, pPathIn->m_pPoints[nIndex + 1].dY                        );
			} 
			else 
			{
				pPathOut->MoveTo( pPathIn->m_pPoints[nIndex + 1].dX, pPathIn->m_pPoints[nIndex + 1].dY );

				if ( dCrossprod < 0 ) // Угол < 180
				{
					pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthNextDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthNextDx );

					if ( m_pState->m_nLineJoin == LineJoinMiter && sqrt( dMiter ) <= m_pState->m_dMiterLimit ) // тип соединения = LineJoinMiter и dMiter меньше dMiterLimit
					{
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy + dWidthDx * dMit, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx + dWidthDy * dMit );
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy,                   pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx                   );
					} 
					else // либо тип соединения = LineJoinBevel, либо dMiter больше dMiterLimit
					{
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
					}
				} 
				else // Угол >= 180
				{
					pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx );

					if ( m_pState->m_nLineJoin == LineJoinMiter && sqrt( dMiter ) <= m_pState->m_dMiterLimit ) // тип соединения = LineJoinMiter и dMiter меньше dMiterLimit
					{
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy + dWidthDx * dMit, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx + dWidthDy * dMit );
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthNextDy,               pPathIn->m_pPoints[nIndex + 1].dY - dWidthNextDx               );
					} 
					else // либо тип соединения = LineJoinBevel, либо dMiter больше dMiterLimit
					{
						pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthNextDy, pPathIn->m_pPoints[nIndex + 1].dY - dWidthNextDx );
					}
				}
			}
			pPathOut->Close();
		}

		if ( m_pState->m_bStrokeAdjust ) 
		{
			if ( nIndex >= nSubpathStart + 1 ) 
			{
				if ( nIndex >= nSubpathStart + 2 ) 
				{
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nLeft0 + 1, nRight0 );
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nJoin0, nLeft2 );
				} 
				else 
				{
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nFirstPoint, nLeft2 );
				}
				pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nRight2 + 1, nRight2 + 1 );
			}
			nLeft0  = nLeft1;
			nLeft1  = nLeft2;
			nRight0 = nRight1;
			nRight1 = nRight2;
			nJoin0  = nJoin1;
			nJoin1  = nJoin2;
			if ( nIndex == nSubpathStart )
			{
				nLeftFirst  = nLeft2;
				nRightFirst = nRight2;
			}
			if ( bLast ) 
			{
				if ( nIndex >= nSubpathStart + 2 ) 
				{
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nLeft0 + 1, nRight0 );
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nJoin0, pPathOut->m_nPointsCount - 1 );
				} 
				else 
				{
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nFirstPoint, pPathOut->m_nPointsCount - 1 );
				}
				if ( bClosed ) 
				{
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nFirstPoint, nLeftFirst );
					pPathOut->AddStrokeAdjustHint( nLeft1, nRight1, nRightFirst + 1, nRightFirst + 1 );
					pPathOut->AddStrokeAdjustHint( nLeftFirst, nRightFirst, nLeft1 + 1, nRight1 );
					pPathOut->AddStrokeAdjustHint( nLeftFirst, nRightFirst, nJoin1, pPathOut->m_nPointsCount - 1 );
				}
			}
		}
	}

	if ( pPathIn != pPath ) 
	{
		delete pPathIn;
	}

	return pPathOut;
}
