#pragma once

#include "..\stdafx.h"
#include "..\VectorGraphics\STypes.h"
#include "..\VectorGraphics\MemoryUtils.h"

#include "..\Interfaces\IASCPattern.h"
#include "..\..\..\Common\ASCUtils.h"

class CPixels
{
public:
	BYTE*		m_pPixels;
	SColorMode	m_eMode;
	LONG		m_lStride;
	LONG		m_lPitch;

	LONG		m_lLeft;
	LONG		m_lTop;
	LONG		m_lWidth;
	LONG		m_lHeight;

	BYTE*		m_pAlpha;   	// Указатель на начало данных альфа канала (не зависит от flip)
								// используется только для colorspaces, не содержащих альфу
	LONG		m_lPitchAlpha;

private:
	BOOL m_bIsDestroy;

public:
	CPixels()
	{
		m_bIsDestroy = FALSE;

		m_pPixels = NULL;
		m_pAlpha = NULL;
	}

	CPixels(BYTE* pPixels, LONG nWidth, LONG nHeight, LONG nRowPad = 0, SColorMode eMode = colorModeBGRA8, BOOL bAlpha = FALSE, LONG lPitch = 0, BOOL bTopDown = TRUE)
	{
		//Release();
		Create(pPixels, nWidth, nHeight, nRowPad, eMode, bAlpha, lPitch, bTopDown);
	}

	~CPixels()
	{
		Release();
	}

	void Release()
	{
		if (m_bIsDestroy)
		{
			RELEASEARRAYOBJECTS(m_pPixels);
		}
		RELEASEARRAYOBJECTS(m_pAlpha);
	}
	void Create(BYTE* pPixels, LONG nWidth = 0, LONG nHeight = 0, LONG nRowPad = 0, SColorMode eMode = colorModeBGRA8, BOOL bAlpha = FALSE, LONG lPitch = 0, BOOL bTopDown = TRUE, BOOL bIsPixOffset = FALSE)
	{
		m_lWidth  = nWidth;
		m_lHeight = nHeight;
		m_eMode   = eMode;

		m_lPitchAlpha = lPitch;

		switch ( m_eMode ) 
		{
		case colorModeMono1:
			m_lStride	= ( m_lWidth + 7 ) >> 3;
			m_lPitch	= ( m_lPitchAlpha + 7) >> 3;
			break;
		case colorModeMono8:
			m_lStride	= m_lWidth;
			m_lPitch	= m_lPitchAlpha;
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			m_lStride	= m_lWidth * 3;
			m_lPitch	= m_lPitchAlpha * 3;
			break;
		case colorModeBGRA8:
		case colorModeRGBA8:
			m_lStride	= m_lWidth * 4;
			m_lPitch	= m_lPitchAlpha * 4;
			bAlpha = FALSE;
			break;
		}

		m_lStride += nRowPad - 1;
		m_lStride -= m_lStride % nRowPad;

		if (NULL == pPixels)
		{
			m_pPixels = (BYTE*)MemUtilsMalloc( m_lStride * m_lHeight );
			m_bIsDestroy = TRUE;
		}
		else
		{
			m_pPixels = pPixels;
			m_bIsDestroy = FALSE;
		}

		if ( !bTopDown ) 
		{
			if (!bIsPixOffset)
			{
				m_pPixels += ( m_lHeight - 1 ) * m_lStride;
			}
			m_lStride = -m_lStride;
			m_lPitch  = -m_lPitch;
		}

		if ( bAlpha ) 
		{
			m_pAlpha = (BYTE*)MemUtilsMalloc( m_lWidth * m_lHeight );
		} 
		else 
		{
			m_pAlpha = NULL;
		}
	}

public:

	void GetPixel(int nX, int nY, SColorPointer pPixel) 
	{
		SColorPointer pColor = NULL;

		if ( nY < 0 || nY >= m_lHeight || nX < 0 || nX >= m_lWidth ) 
		{
			return;
		}

		LONG lStride = m_lStride + m_lPitch;

		switch (m_eMode)
		{
		case colorModeMono1:
			lStride += (m_lPitch + 7) >> 3;
			
			pColor = &m_pPixels[nY * lStride + (nX >> 3)];
			pPixel[0] = (pColor[0] & (0x80 >> (nX & 7))) ? 0xff : 0x00;
			break;
		case colorModeMono8:
			pColor = &m_pPixels[nY * lStride + nX];
			pPixel[0] = pColor[0];
			break;
		case colorModeRGB8:
			pColor = &m_pPixels[nY * lStride + 3 * nX];
			pPixel[0] = pColor[0];
			pPixel[1] = pColor[1];
			pPixel[2] = pColor[2];
			break;
		case colorModeBGR8:
			pColor = &m_pPixels[nY * lStride + 3 * nX];
			pPixel[0] = pColor[2];
			pPixel[1] = pColor[1];
			pPixel[2] = pColor[0];
			break;
		case colorModeRGBA8:
			pColor = &m_pPixels[nY * lStride + 4 * nX];
			pPixel[0] = pColor[2];
			pPixel[1] = pColor[1];
			pPixel[2] = pColor[0];
			break;
		case colorModeBGRA8:
			pColor = &m_pPixels[nY * lStride + 4 * nX];
			pPixel[0] = pColor[2];
			pPixel[1] = pColor[1];
			pPixel[2] = pColor[0];
			pPixel[3] = pColor[3];
			break;
		}
	}
};

struct SPipe 
{
	int				nX;               // Координаты
	int				nY;               //
	IAVSPattern*	pPattern;         // Source Pattern
	double			dAlphaInput;      //
	BOOL			bUsesShape;       //
	unsigned char	unAlphaSrc;       // Source Alpha
	SColorPointer	pColorSrc;        // Source Color
	SColor			arrColorSrcVal;   //
	unsigned char*	pAlpha0;          // Альфа-канал для Non-isolated групп
	SColorPointer	pSMask;           // Soft Mask 
 
  	SColorPointer	pColorDst;        // Destination Color
	int				nColorMaskDst;    // Destination Color Mask
	unsigned char*	pAlphaDst;        // Destination Alpha 

	double			dShape;           // Shape

	BOOL			bNoTransparency;  //
	int				nNonIsolatedGroup;// Non-isolated group correction

	SPipeResultColorCtrl eResultColorCtrl;

};

class CGlyph
{
public:
	int            nX;        // Сдвиг по X начальной точки для рисования символа
	int            nY;        // Сдвиг по Y начальной точки для рисования символа
	int            nWidth;    // Ширина символа
	int            nHeight;   // Высота символа
	BOOL           bAA;       // Anti-aliased: True означает, что Bitmap 8-битный(т.е. с альфой); False - Bitmap 1-битный
	unsigned char *pData;     // Bitmap data(картинка с символом)
	BOOL           bFreeData; // True, если память в pData нужно освободить
};

class CState
{
public:
	
	double      m_arrdMatrix[6];
	
	double      m_dStrokeAlpha;
	
	double      m_dLineWidth;
	int         m_nLineCap;
	int         m_nLineJoin;
	double      m_dMiterLimit;
	
	double      m_dFlatness;
	double     *m_pdLineDash;
	int         m_nLineDashCount;
	double      m_dLineDashPhase;
	BOOL        m_bStrokeAdjust;
};

//-------------------------------------------------------------------------------------------------------------------------------
// Line cap
//-------------------------------------------------------------------------------------------------------------------------------

//#define LineCapButt       0
//#define LineCapRound      1
//#define LineCapProjecting 2
//
////-------------------------------------------------------------------------------------------------------------------------------
//// Line join
////-------------------------------------------------------------------------------------------------------------------------------
//
//#define LineJoinMiter     0
//#define LineJoinRound     1
//#define LineJoinBevel     2

