// AVSGraphicsPath.h : Declaration of the AVSGraphicsPath

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "..\Interfaces\IAVSGraphics.h"
#include "AVSMatrix.h"
#include "AVSGraphicsPath.h"
#include "AVSSolidPattern.h"
#include "AVSGlyphImage.h"

#include "..\VectorGraphics\STypes.h"
#include "..\Common\Clip.h"
#include "..\Common\Pixels.h"
#include "..\VectorGraphics\SScreen.h"
#include "..\VectorGraphics\SErrorCodes.h"
#include "..\VectorGraphics\SXPath.h"
#include "..\VectorGraphics\SXPathScanner.h"
#include "..\VectorGraphics\SMathExt.h"
#include "AVSFontManager.h"

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



// CAVSGraphicsPath
[coclass, uuid("C79DCE54-5676-4d95-9C36-2A136C5D0592"), threading(apartment), vi_progid("AVSGraphics.Graphics"), progid("AVSGraphics.Graphics"), version(1.0), support_error_info(IAVSGraphics), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSGraphics
	:	public IAVSGraphics
{
protected:

	double m_dWidthPix;
	double m_dHeightPix;

	double m_dDpiX;
	double m_dDpiY;

	LONG m_lSmoothMode;
	LONG m_lAntialiseMode;

	LONG m_lPageUnits;

	CPixels m_oPixels;
	CPixels	m_oAABuffer;

	CPixels m_oSoftMask;

	int          m_nAABufferY;
	
	CPixels m_oAlpha0Bitmap;	// Для Non-isolated групп, битмап хранящий значения альфа-канала
	int          m_nAlpha0X;    //
	int          m_nAlpha0Y;    // Сдвиг в m_pAlpha0Bitmap

	BOOL        m_bDeleteSoftMask;
	BOOL        m_bInNonIsolatedGroup;

	Gdiplus::Matrix m_oCoordTransform;
	Gdiplus::Matrix m_oBaseTransform;
	Gdiplus::Matrix m_oTransform;	

	CClip m_oClip;
	double m_dClipFlatness;
	double m_dFlatness;

	SBlendFunc  m_pBlendFunction;
	SScreen     m_oScreen;

	double       m_arrdAAGamma[AntiAliasingSize * AntiAliasingSize + 1];

	int          m_nModRegMinX;  //
	int          m_nModRegMinY;  // Границы региона, в котором производились последние изменения
	int          m_nModRegMaxX;  // (т.е. изменения после вызова функции GetModificationRegion())
	int          m_nModRegMaxY;  //

	SClipResult  m_pOpClipRes;   // Результат последней операции рисования по отношению к Clipping

	BOOL         m_bVectorAA;

	static SPipeResultColorCtrl m_arrePipeResultColorNoAlphaBlend[];
	static SPipeResultColorCtrl m_arrePipeResultColorAlphaNoBlend[];
	static SPipeResultColorCtrl m_arrePipeResultColorAlphaBlend[];
	static int                  m_arrnPipeNonIsoGroupCorrection[];

public:

	CAVSGraphics() :
	  m_oClip(), m_oCoordTransform(), m_oBaseTransform(), m_oTransform()
	{
		m_dWidthPix			= 0;
		m_dHeightPix		= 0;

		m_dDpiX				= 72.0;
		m_dDpiY				= 72.0;

		m_lSmoothMode		= c_ag_SmoothingModeAntiAlias;
		m_lAntialiseMode	= c_ag_TextRenderingHintAntiAlias;

		m_lPageUnits		= c_ag_PageUnitPixel;

		m_oPixels.m_pPixels	= NULL;
		m_dClipFlatness		= 1.0;

		m_dFlatness			= 1.0;

		m_bVectorAA			= TRUE;
		m_pBlendFunction	= NULL;
	}

	~CAVSGraphics()
	{
	}

public:
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IAVSGraphicsBase** ppGraphicsBase)
	{
		if (NULL == ppGraphicsBase)
			return S_FALSE;

		RELEASEINTERFACE((*ppGraphicsBase));
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSGraphics), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(Create)(BYTE* pPixels, LONG lWidth, LONG lHeight, LONG lStride, LONG lPitch, LONG lColorSpace)
	{
		// тут надо написать класс CPixels
		m_oPixels.Create(pPixels, lWidth, lHeight, 1, colorModeRGBA8, TRUE, 0, TRUE); 
		
		/*m_oPixels.m_pPixels			= pPixels;
		m_oPixels.m_lLeft			= 0;
		m_oPixels.m_lTop			= 0;
		m_oPixels.m_lWidth			= lWidth;
		m_oPixels.m_lHeight			= lHeight;
		m_oPixels.m_lPitch			= lPitch;
		m_oPixels.m_eMode			= (SColorMode)lColorSpace;*/

		m_dWidthPix					= (double)lWidth;
		m_dHeightPix				= (double)lHeight;

		m_oClip.Reset(0, 0, m_dWidthPix, m_dHeightPix, TRUE);

		m_pBlendFunction = NULL;
		m_dFlatness      = 1;

		m_oSoftMask.Release();
		m_oAlpha0Bitmap.Release();

		m_bDeleteSoftMask		= FALSE;

		m_bInNonIsolatedGroup = FALSE;

		if ( m_bVectorAA ) 
		{
			m_oAABuffer.Create( NULL, AntiAliasingSize * m_oPixels.m_lWidth, AntiAliasingSize, 1, colorModeMono1, FALSE );

			for ( int nIndex = 0; nIndex <= AntiAliasingSize * AntiAliasingSize; ++nIndex ) 
			{
				m_arrdAAGamma[nIndex] = pow((double)nIndex / (double)(AntiAliasingSize * AntiAliasingSize), 1.5);
			}
		} 
		else 
		{
			m_oAABuffer.Release();
		}
		ClearModRegion();

		UpdateUnits();
		
		return S_OK;
	}

	STDMETHOD(CreatePart)(LONG lLeft, LONG lTop, LONG lWidth, LONG lHeight, IAVSGraphics** ppPart)
	{
		// здесь минимум кода. Просто сделать дубликат - и выставить ему правильные границы.
		// а потом уже и dpi и все настройки.
		return S_OK;
	}

	STDMETHOD(get_DpiX)(double* lDpiX)
	{
		if (NULL == lDpiX)
			return S_FALSE;
		
		*lDpiX = m_dDpiX;
		return S_OK;
	}
	STDMETHOD(put_DpiX)(double lDpiX)
	{
		m_dDpiX = lDpiX;
		
		UpdateUnits();
		return S_OK;
	}
	STDMETHOD(get_DpiY)(double* lDpiY)
	{
		if (NULL == lDpiY)
			return S_FALSE;
		
		*lDpiY = m_dDpiY;
		return S_OK;
	}
	STDMETHOD(put_DpiY)(double lDpiY)
	{
		m_dDpiY = lDpiY;
		
		UpdateUnits();
		return S_OK;
	}

	STDMETHOD(SetPageWidth)(double lWidth, LONG lUnit)
	{
		// вычисилить dpi и выставить его
		switch (lUnit)
		{
		case c_ag_PageUnitPoint:
			{
				lWidth /= c_ag_Inch_to_Point;
				put_DpiX(m_dWidthPix / lWidth);

				break;
			}
		case c_ag_PageUnitMillimeter:
			{
				lWidth /= c_ag_Inch_to_MM;
				put_DpiX(m_dWidthPix / lWidth);

				break;
			}
		case c_ag_PageUnitInch:
			{
				put_DpiX(m_dWidthPix / lWidth);
				break;
			}
		default:
			break;
		};

		return S_OK;
	}
	STDMETHOD(SetPageHeight)(double lHeight, LONG lUnit)
	{
		// вычисилить dpi и выставить его
		switch (lUnit)
		{
		case c_ag_PageUnitPoint:
			{
				lHeight /= c_ag_Inch_to_Point;
				put_DpiY(m_dHeightPix / lHeight);

				break;
			}
		case c_ag_PageUnitMillimeter:
			{
				lHeight /= c_ag_Inch_to_MM;
				put_DpiY(m_dHeightPix / lHeight);

				break;
			}
		case c_ag_PageUnitInch:
			{
				put_DpiY(m_dHeightPix / lHeight);
				break;
			}
		default:
			break;
		};

		return S_OK;
	}
	
	STDMETHOD(get_PageUnit)(LONG* lUnit)
	{
		if (NULL != lUnit)
			return S_FALSE;

		*lUnit = m_lPageUnits;
		return S_OK;
	}
	STDMETHOD(put_PageUnit)(LONG lUnit)
	{
		if (lUnit != m_lPageUnits)
		{
			m_lPageUnits = lUnit;
			UpdateUnits();
		}
		return S_OK;
	}

	STDMETHOD(get_Transform)(IAVSMatrix** ppTransform)
	{
		if (NULL == ppTransform)
			return S_OK;

		CoCreateInstance(__uuidof(CAVSMatrix), NULL, CLSCTX_INPROC, __uuidof(IAVSMatrix), (void**)ppTransform);
		if (NULL != *ppTransform)
		{
			float mass[6];
			m_oTransform.GetElements(mass);

			(*ppTransform)->SetElements((double)mass[0], (double)mass[1], (double)mass[2], (double)mass[3], (double)mass[4], (double)mass[5]);
		}
		
		return S_OK;
	}
	STDMETHOD(put_Transform)(IAVSMatrix* pTransform)
	{
		if (NULL == pTransform)
			return S_FALSE;

		NSStructures::SetMatrix(m_oTransform, ((CAVSMatrix*)pTransform)->m_oMatrix);		
		return S_OK;
	}

	STDMETHOD(get_BaseTransform)(IAVSMatrix** ppTransform)
	{
		if (NULL == ppTransform)
			return S_OK;

		CoCreateInstance(__uuidof(CAVSMatrix), NULL, CLSCTX_INPROC, __uuidof(IAVSMatrix), (void**)ppTransform);
		if (NULL != *ppTransform)
		{
			float mass[6];
			m_oBaseTransform.GetElements(mass);

			(*ppTransform)->SetElements((double)mass[0], (double)mass[1], (double)mass[2], (double)mass[3], (double)mass[4], (double)mass[5]);
		}
		
		return S_OK;
	}
	STDMETHOD(put_BaseTransform)(IAVSMatrix* pTransform)
	{
		if (NULL == pTransform)
			return S_FALSE;

		NSStructures::SetMatrix(m_oBaseTransform, ((CAVSMatrix*)pTransform)->m_oMatrix);		
		return S_OK;
	}
	
	STDMETHOD(TranslateTransform)(double dX, double dY, LONG lMatrixOrder)
	{
		m_oTransform.Translate((float)dX, (float)dY, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}
	STDMETHOD(RotateTransform)(double dAngle, LONG lMatrixOrder)
	{
		m_oTransform.Rotate((float)dAngle, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}
	STDMETHOD(ScaleTransform)(double dScaleX, double dScaleY, LONG lMatrixOrder)
	{
		m_oTransform.Scale((float)dScaleX, (float)dScaleY, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(MultiplyTransform)(IAVSMatrix* pTransform, LONG lMatrixOrder)
	{		
		if (NULL == pTransform)
			return S_FALSE;

		m_oTransform.Multiply(&((CAVSMatrix*)pTransform)->m_oMatrix, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(Save)(IAVSGraphicsState** pState)
	{
		return S_OK;
	}
	STDMETHOD(Restore)(IAVSGraphicsState* pState)
	{
		return S_OK;
	}

	// функции отсечения
	STDMETHOD(SetClip1)(double dLeft, double dTop, double dWidth, double dHeight, LONG lCombineMode)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(dLeft, dTop, dWidth, dHeight);

		double mass[6];
		SetUpMatrix(mass);

		switch (lCombineMode)
		{
		case c_ag_CombineModeReplace:
			{
				m_oClip.Reset();				
				m_oClip.Intersect(&oPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		case c_ag_CombineModeIntersect:
			{
				m_oClip.Intersect(&oPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		case c_ag_CombineModeUnion:
			{
				m_oClip.Exclude(&oPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		default:
			{
				m_oClip.Reset();
				m_oClip.Intersect(&oPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		};
		
		return S_OK;
	}
	STDMETHOD(SetClip2)(IAVSGraphicsPath* punkPath, LONG lCombineMode)
	{
		if (NULL == punkPath)
			return S_FALSE;
		
		double mass[6];
		SetUpMatrix(mass);

		CGraphicsPath* pPath = &(((CAVSGraphicsPath*)punkPath)->m_oPath);

		switch (lCombineMode)
		{
		case c_ag_CombineModeReplace:
			{
				m_oClip.Reset();				
				m_oClip.Intersect(pPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		case c_ag_CombineModeIntersect:
			{
				m_oClip.Intersect(pPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		case c_ag_CombineModeUnion:
			{
				m_oClip.Exclude(pPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		default:
			{
				m_oClip.Reset();
				m_oClip.Intersect(pPath, mass, m_dClipFlatness, FALSE);
				break;
			}
		};
		
		return S_OK;
	}
	STDMETHOD(SetClip3)(IUnknown* pRegion, LONG lCombineMode)
	{
		return S_OK;
	}
	STDMETHOD(ExludeClip1)(double dLeft, double dTop, double dWidth, double dHeight, LONG lCombineMode)
	{
		CGraphicsPath oPath;
		oPath.AddRectangle(dLeft, dTop, dWidth, dHeight);

		double mass[6];
		SetUpMatrix(mass);

		m_oClip.Reset();
		m_oClip.Intersect(&oPath, mass, m_dClipFlatness, FALSE);

		return S_OK;
	}
	STDMETHOD(ExludeClip2)(IAVSGraphicsPath* punkPath, LONG lCombineMode)
	{
		if (NULL == punkPath)
			return S_FALSE;
		
		double mass[6];
		SetUpMatrix(mass);

		CGraphicsPath* pPath = &((CAVSGraphicsPath*)punkPath)->m_oPath;
		m_oClip.Reset();				
		m_oClip.Intersect(pPath, mass, m_dClipFlatness, FALSE);

		return S_OK;
	}
	STDMETHOD(ExludeClip3)(IUnknown* pRegion, LONG lCombineMode)
	{
		return S_OK;
	}

	// измерение текста
	STDMETHOD(MeasureString)(BSTR bstrText, IAVSFontManager* pManager, double* lWidth, double* lHeight)
	{
		if (NULL == pManager || NULL == lWidth || NULL == lHeight)
			return S_FALSE;

		pManager->LoadString(bstrText, 0, 0);

		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;
		
		pManager->MeasureString(&x, &y, &w, &h);

		*lWidth		= (double)w;
		*lHeight	= (double)h;

		return S_OK;
	}

	// тут еще надо продумать зачеркивание, подчеркивание. реализовывать нужно, понятно, в методе DrawString


	// настройки отрисовщика
	STDMETHOD(get_SmoothingMode)(LONG* lMode)
	{
		if (NULL == lMode)
			return S_FALSE;

		*lMode = m_lSmoothMode;
		return S_OK;
	}
	STDMETHOD(put_SmoothingMode)(LONG lMode)
	{
		m_lSmoothMode = lMode;
		return S_OK;
	}

	STDMETHOD(get_TextRenderingHint)(LONG* lMode)
	{
		if (NULL != lMode)
			return S_FALSE;

		*lMode = m_lAntialiseMode;
		return S_OK;
	}
	STDMETHOD(put_TextRenderingHint)(LONG lMode)
	{
		m_lAntialiseMode = lMode;
		return S_OK;
	}

	STDMETHOD(get_CompositingMode)(LONG* lMode)
	{
		return S_OK;
	}
	STDMETHOD(put_CompositingMode)(LONG lMode)
	{
		return S_OK;
	}

	STDMETHOD(get_CompositingQuality)(LONG* lMode)
	{
		return S_OK;
	}
	STDMETHOD(put_CompositingQuality)(LONG lMode)
	{
		return S_OK;
	}

	STDMETHOD(get_InterpolationMode)(LONG* lMode)
	{
		return S_OK;
	}
	STDMETHOD(put_InterpolationMode)(LONG lMode)
	{
		return S_OK;
	}

	STDMETHOD(get_PixelOffsetMode)(LONG* lMode)
	{
		return S_OK;
	}
	STDMETHOD(put_PixelOffsetMode)(LONG lMode)
	{
		return S_OK;
	}

	// отрисовка примитивов (без заливки)

	STDMETHOD(Clear)(LONG lColor)
	{
		SColorPointer pColor = (SColorPointer)&lColor;
		BYTE unAlpha = (BYTE)(lColor);
		
		unsigned char unMono;

		switch ( m_oPixels.m_eMode ) 
		{
		case colorModeMono1:
			unMono = (pColor[0] & 0x80) ? 0xff : 0x00;
			if ( m_oPixels.m_lStride < 0 ) 
			{
				memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), unMono, -m_oPixels.m_lStride * m_oPixels.m_lHeight );
			} 
			else 
			{
				memset( m_oPixels.m_pPixels, unMono, m_oPixels.m_lStride * m_oPixels.m_lHeight); 
			}
			break;
		case colorModeMono8:
			if ( m_oPixels.m_lStride < 0 ) 
			{
				memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), pColor[0], -m_oPixels.m_lStride * m_oPixels.m_lHeight );
			} 
			else 
			{
				memset( m_oPixels.m_pPixels, pColor[0], m_oPixels.m_lStride * m_oPixels.m_lHeight );
			}
			break;
		case colorModeRGB8:
			if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
			{
				if ( m_oPixels.m_lStride < 0 ) 
				{
					memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), pColor[0], -m_oPixels.m_lStride * m_oPixels.m_lHeight );
				} 
				else 
				{
					memset( m_oPixels.m_pPixels, pColor[0], m_oPixels.m_lStride * m_oPixels.m_lHeight );
				}
			} 
			else 
			{
				SColorPointer pLine = m_oPixels.m_pPixels;
				for ( int nY = 0; nY < m_oPixels.m_lHeight; ++nY ) 
				{
					SColorPointer pCur = pLine;
					for ( int nX = 0; nX < m_oPixels.m_lWidth; ++nX ) 
					{
						*pCur++ = pColor[2];
						*pCur++ = pColor[1];
						*pCur++ = pColor[0];
					}
					pLine += m_oPixels.m_lStride;
				}
			}
			break;
		case colorModeBGR8:
			if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
			{
				if ( m_oPixels.m_lStride < 0 ) 
				{
					memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), pColor[0], -m_oPixels.m_lStride * m_oPixels.m_lHeight );
				} 
				else 
				{
					memset( m_oPixels.m_pPixels, pColor[0], m_oPixels.m_lStride * m_oPixels.m_lHeight );
				}
			} 
			else 
			{
				SColorPointer pLine = m_oPixels.m_pPixels;
				for ( int nY = 0; nY < m_oPixels.m_lHeight; ++nY ) 
				{
					SColorPointer pCur = pLine;
					for ( int nX = 0; nX < m_oPixels.m_lWidth; ++nX ) 
					{
						*pCur++ = pColor[0];
						*pCur++ = pColor[1];
						*pCur++ = pColor[2];
					}
					pLine += m_oPixels.m_lStride;
				}
			}
			break;

		case colorModeRGBA8:
			if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
			{
				if ( m_oPixels.m_lStride < 0 ) 
				{
					memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), pColor[0], -m_oPixels.m_lStride * m_oPixels.m_lHeight );
				} 
				else 
				{
					memset( m_oPixels.m_pPixels, pColor[0], m_oPixels.m_lStride * m_oPixels.m_lHeight );
				}
			} 
			else 
			{
				SColorPointer pLine = m_oPixels.m_pPixels;
				for ( int nY = 0; nY < m_oPixels.m_lHeight; ++nY ) 
				{
					SColorPointer pCur = pLine;
					for ( int nX = 0; nX < m_oPixels.m_lWidth; ++nX ) 
					{
						*pCur++ = pColor[2];
						*pCur++ = pColor[1];
						*pCur++ = pColor[0];
						*pCur++ = unAlpha;
					}
					pLine += m_oPixels.m_lStride;
				}
			}
			break;
		case colorModeBGRA8:
			if ( pColor[0] == pColor[1] && pColor[1] == pColor[2] ) 
			{
				if ( m_oPixels.m_lStride < 0 ) 
				{
					memset( m_oPixels.m_pPixels + m_oPixels.m_lStride * (m_oPixels.m_lHeight - 1), pColor[0], -m_oPixels.m_lStride * m_oPixels.m_lHeight );
				} 
				else 
				{
					memset( m_oPixels.m_pPixels, pColor[0], m_oPixels.m_lStride * m_oPixels.m_lHeight );
				}
			} 
			else 
			{
				SColorPointer pLine = m_oPixels.m_pPixels;
				for ( int nY = 0; nY < m_oPixels.m_lHeight; ++nY ) 
				{
					SColorPointer pCur = pLine;
					for ( int nX = 0; nX < m_oPixels.m_lWidth; ++nX ) 
					{
						*pCur++ = pColor[0];
						*pCur++ = pColor[1];
						*pCur++ = pColor[2];
						*pCur++ = unAlpha;
					}
					pLine += m_oPixels.m_lStride;
				}
			}
			break;
		}

		if ( m_oPixels.m_pAlpha ) 
		{
			memset( m_oPixels.m_pAlpha, unAlpha, m_oPixels.m_lWidth * m_oPixels.m_lHeight );
		}

		UpdateModX( 0 );
		UpdateModY( 0 );
		UpdateModX( m_oPixels.m_lWidth - 1 );
		UpdateModY( m_oPixels.m_lHeight - 1 );

		return S_OK;
	}

	STDMETHOD(DrawArc)(IAVSPen* pPen, double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		CGraphicsPath oPath;
		oPath.AddArc(x, y, width, height, startAngle, sweepAngle);
		
		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawBezier)(IAVSPen* pPen, double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		CGraphicsPath oPath;
		oPath.AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawBeziers)(IAVSPen* pPen, LPSAFEARRAY pPoints)
	{
		if (NULL == pPoints)
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;
		
		CGraphicsPath oPath;
		oPath.AddBeziers(pData, lCount);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawCurve)(IAVSPen* pPen, LPSAFEARRAY pPoints)
	{
		if (NULL == pPoints)
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;
		
		CGraphicsPath oPath;
		oPath.AddCurve(pData, lCount);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawLine)(IAVSPen* pPen, double x1, double y1, double x2, double y2)
	{
		CGraphicsPath oPath;
		oPath.AddLine(x1, y1, x2, y2);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
    STDMETHOD(DrawLines)(IAVSPen* pPen, LPSAFEARRAY pPoints)
	{
		if (NULL == pPoints)
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;
		
		CGraphicsPath oPath;
		oPath.AddLines(pData, lCount);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawEllipse)(IAVSPen* pPen, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		BOOL bRes = DrawPath(pPen, &oPath);

		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(DrawPath)(IAVSPen* pPen, IAVSGraphicsPath* pPath)
	{
		BOOL bRes = DrawPath(pPen, &((CAVSGraphicsPath*)pPath)->m_oPath);

		return bRes ? S_OK : S_FALSE;
	}

	// заливка
	STDMETHOD(FillEllipse)(IAVSBrush* pBrush, double x, double y, double width, double height)
	{
		CGraphicsPath oPath;
		oPath.AddEllipse(x, y, width, height);

		IAVSPattern* pPattern = NULL;
		oPath.Pattern(pBrush, &pPattern);

		BOOL bRes = FillPath(pPattern, &oPath);

		RELEASEINTERFACE(pPattern);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(FillPath)(IAVSBrush* pBrush, IAVSGraphicsPath* pPath)
	{
		IAVSPattern* pPattern = NULL;
		pPath->Pattern(pBrush, &pPattern);

		BOOL bRes = FillPath(pPattern, &((CAVSGraphicsPath*)pPath)->m_oPath);

		RELEASEINTERFACE(pPattern);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(FillPolygon)(IAVSBrush* pBrush, LPSAFEARRAY pPoints)
	{
		if (NULL == pPoints)
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		double* pData = (double*)pPoints->pvData;
		
		CGraphicsPath oPath;
		oPath.AddPolygon(pData, lCount);

		IAVSPattern* pPattern = NULL;
		oPath.Pattern(pBrush, &pPattern);

		BOOL bRes = FillPath(pPattern, &oPath);

		RELEASEINTERFACE(pPattern);
		return bRes ? S_OK : S_FALSE;
	}

	// отрисовка картинки
	STDMETHOD(DrawImage)(IAVSImage* pImage, double x, double y, double width, double height)
	{
		return S_OK;
	}
	STDMETHOD(DrawImage2)(IAVSImage* pImage, LPSAFEARRAY pPoints)
	{
		return S_OK;
	}
	STDMETHOD(DrawImageUnscaled)(IAVSImage* pImage, double x, double y)
	{
		return S_OK;
	}

	// отрисовка текста
	STDMETHOD(DrawString)(BSTR bstrText, IAVSFontManager* pFont, IAVSBrush* pBrush, double x, double y)
	{
		double mass[6];
		GetElements(mass);

		pFont->SetTextMatrix(mass[0], mass[1], mass[2], mass[3], mass[4], mass[5]);
		pFont->LoadString(bstrText, (float)x, (float)y);

		CString str(bstrText);
		int nCount = str.GetLength();

		float fX = 0;
		float fY = 0;
		VARIANT_BOOL bRes = VARIANT_FALSE;

		IAVSPattern* pPattern = NULL;

		CGraphicsPath oPath;
		oPath.Pattern(pBrush, &pPattern);

		while (TRUE)
		{
			IAVSGlyphImage* pGlyph = NULL;
			float fX = 0, fY = 0;

			pFont->GetNextChar2(&pGlyph, &fX, &fY, &bRes);

			if (VARIANT_FALSE == bRes || NULL == pGlyph)
				break;

			FillGlyph((int)fX, (int)fY, &((CAVSGlyphImage*)pGlyph)->m_oGlyph, pPattern, 1.0);

			RELEASEINTERFACE(pGlyph);
		}

		RELEASEINTERFACE(pPattern);

		return S_OK;
	}
	STDMETHOD(DrawString2)(BSTR bstrText, IAVSFontManager* pFont, IAVSBrush* pBrush, 
														IAVSStringFormat* pFormat, double x, double y, double width, double height)
	{
		return S_OK;
	}

	static HRESULT LoadImageFromFile(BSTR bstrPath, IAVSImage** ppImage)
	{
		return S_OK;
	}

protected:
	
	int FillPath(IAVSPattern* pPattern, CGraphicsPath* pPath, double dAlpha = 1.0)
	{
		double matrix[6];
		GetElements(matrix);
		
		int nX0 = 0, nX1 = 0;

		if ( pPath->m_nPointsCount == 0 ) 
		{
			return SErrorEmptyPath;
		}
		
		SXPath *pXPath = new SXPath( pPath, matrix, m_dFlatness, TRUE );
		if ( !pXPath )
			return SErrorMemory;
		if ( m_bVectorAA ) 
		{
			pXPath->AntiAliasingScale();
		}
		pXPath->Sort();

		SXPathScanner *pScanner = new SXPathScanner( pXPath, (1 == pPath->m_lMode) );
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
		if ( ( eClipResult = m_oClip.CheckRectangle( nMinX, nMinY, nMaxX, nMaxY) ) != clipAllOutside ) 
		{
			if ( nMinY < m_oClip.GetMinY() ) 
			{
				nMinY = m_oClip.GetMinY();
			}
			if ( nMaxY > m_oClip.GetMaxY() ) 
			{
				nMaxY = m_oClip.GetMaxY();
			}
			SPipe oPipe;
			PipeInit( &oPipe, 0, nMinY, pPattern, NULL, dAlpha, m_bVectorAA, FALSE );

			if ( m_bVectorAA ) 
			{
				for ( int nY = nMinY; nY <= nMaxY; ++nY ) 
				{
					pScanner->RenderAALine( &m_oAABuffer, &nX0, &nX1, nY );
					if ( eClipResult != clipAllInside ) 
					{
						m_oClip.ClipAALine( &m_oAABuffer, &nX0, &nX1, nY );
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
							if ( nX0 < m_oClip.GetMinX() ) 
							{
								nX0 = m_oClip.GetMinX();
							}
							if ( nX1 > m_oClip.GetMaxX() ) 
							{
								nX1 = m_oClip.GetMaxX();
							}
							SClipResult eTempClipRes = m_oClip.CheckSpan( nX0, nX1, nY );
							DrawSpan( &oPipe, nX0, nX1, nY, eTempClipRes == clipAllInside );
						}
					}
				}
			}
		}
		m_pOpClipRes = eClipResult;

		delete pScanner;
		delete pXPath;
		
		//return SNoError;
		return TRUE;
	}

	BOOL DrawPath(IAVSPen* pPen, CGraphicsPath* pPath)
	{
		CState oState;

		IAVSPattern* pPattern = NULL;
		pPath->Pattern(pPen, &pPattern);

		BOOL bRes = (SNoError == DrawPath(pPattern, pPath, &oState));

		RELEASEINTERFACE(pPattern);
		return bRes;
	}

	int DrawPath(IAVSPattern* pPattern, CGraphicsPath* pPath, CState* pState)
	{
		m_pOpClipRes = clipAllOutside;
		if ( pPath->m_nPointsCount == 0 ) 
		{
			return SErrorEmptyPath;
		}
		CGraphicsPath *pResultPath = FlattenPath( pPath, pState->m_arrdMatrix, m_dFlatness );
		if ( !pResultPath )
			return SErrorMemory;

		if ( pState->m_nLineDashCount > 0 ) 
		{
			CGraphicsPath *pDashPath = MakeDashedPath( pResultPath, pState );
			delete pResultPath;
			pResultPath = pDashPath;
		}

		if ( pState->m_dLineWidth == 0 ) 
		{
			StrokeNarrow( pResultPath, pState, pPattern );
		} 
		else 
		{
			StrokeWide( pResultPath, pState, pPattern );
		}
		delete pResultPath;
		return SNoError;
	}

	void StrokeNarrow(CGraphicsPath *pPath, CState* pState, IAVSPattern* pPattern) 
	{
		SClipResult eClipResult;
		int arrnClipRes[3];

		arrnClipRes[0] = arrnClipRes[1] = arrnClipRes[2] = 0;

		SXPath *pXPath = new SXPath( pPath, pState->m_arrdMatrix, m_dFlatness, FALSE );
		if ( !pXPath )
			return;

		SPipe oPipe;
		PipeInit( &oPipe, 0, 0, pPattern, NULL, pState->m_dStrokeAlpha, FALSE, FALSE );

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

				if ( ( eClipResult = m_oClip.CheckSpan( nX0, nX1, nY0 ) ) != clipAllOutside ) 
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
				
				if ( ( eClipResult = m_oClip.CheckRectangle( nX0 <= nX1 ? nX0 : nX1, nY0, nX0 <= nX1 ? nX1 : nX0, nY1) ) != clipAllOutside ) 
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

				if ( ( eClipResult = m_oClip.CheckRectangle( nX0 <= nX1 ? nX0 : nX1, nY0, nX0 <= nX1 ? nX1 : nX0, nY1) ) != clipAllOutside ) 
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

	void StrokeWide(CGraphicsPath *pPath, CState* pState, IAVSPattern* pPattern) 
	{
		CGraphicsPath *pStrokePath = MakeStrokePath( pPath, FALSE, pState );
		if ( !pStrokePath )
			return;
		FillPath( pPattern, pStrokePath, pState->m_dStrokeAlpha );
		delete pStrokePath;
	}

	CGraphicsPath* MakeDashedPath(SPath *pPath, CState* pState) 
	{
		double dLineDashTotal = 0;
		int nIndex = 0;
		for ( nIndex = 0; nIndex < pState->m_nLineDashCount; ++nIndex ) 
		{
			dLineDashTotal += pState->m_pdLineDash[nIndex];
		}

		double dLineDashStartPhase = pState->m_dLineDashPhase;
		nIndex = (int)floor( dLineDashStartPhase / dLineDashTotal );
		dLineDashStartPhase -= (double)nIndex * dLineDashTotal;

		BOOL bLineDashStartOn = TRUE;
		int  nLineDashStartIndex = 0;

		while ( dLineDashStartPhase >= pState->m_pdLineDash[nLineDashStartIndex] ) 
		{
			bLineDashStartOn = !bLineDashStartOn;
			dLineDashStartPhase -= pState->m_pdLineDash[nLineDashStartIndex];
			++nLineDashStartIndex;
		}

		CGraphicsPath *pDashPath = new CGraphicsPath();

		nIndex = 0;
		while ( nIndex < pPath->m_nPointsCount ) 
		{
			// Ищем окончание данного SupPath
			int nEndIndex;
			for ( nEndIndex = nIndex; nEndIndex < pPath->m_nPointsCount - 1 && !(pPath->m_pFlags[nEndIndex] & SPathLast); ++nEndIndex) ;

			// Обнуляем параметры
			BOOL   bLineDashOn    = bLineDashStartOn;
			int    nLineDashIndex = nLineDashStartIndex;
			double dLineDashDist  = pState->m_pdLineDash[nLineDashIndex] - dLineDashStartPhase;

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
						if ( ++nLineDashIndex == pState->m_nLineDashCount ) 
						{
							nLineDashIndex = 0;
						}
						dLineDashDist = pState->m_pdLineDash[nLineDashIndex];
						bNewPath = TRUE;
					}
				}
			}
			nIndex = nEndIndex + 1;
		}

		return pDashPath;
	}


	CGraphicsPath* MakeStrokePath(CGraphicsPath *pPath, BOOL bFlatten, CState* pState) 
	{
		CGraphicsPath *pPathIn, *pPathOut;
		if ( bFlatten ) 
		{
			pPathIn = FlattenPath( pPath, pState->m_arrdMatrix, m_dFlatness );
			if ( pState->m_nLineDashCount > 0 ) 
			{
				pPathOut = MakeDashedPath( pPathIn, pState );
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

		pPathOut = new CGraphicsPath();
		double dWidth = pState->m_dLineWidth;

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
				switch ( pState->m_nLineCap ) 
				{ 
				case /*LineCapButt*/0:
					pPathOut->LineTo( pPathIn->m_pPoints[nIndex].dX + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx );
					break;
				case /*LineCapRound*/1:
					pPathOut->CurveTo( pPathIn->m_pPoints[nIndex].dX - dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dY + dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dX - dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dX - dWidthDx, pPathIn->m_pPoints[nIndex].dY - dWidthDy );
					pPathOut->CurveTo( pPathIn->m_pPoints[nIndex].dX - dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dX + dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex].dY - dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex].dX + dWidthDy, pPathIn->m_pPoints[nIndex].dY - dWidthDx );
					break;
				case /*LineCapProjecting*/2:
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
				switch (pState->m_nLineCap) 
				{
				case /*LineCapButt*/0:
					pPathOut->LineTo( pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
					break;
				case /*LineCapRound*/1:
					pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY - dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy - c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy );
					pPathOut->CurveTo( pPathIn->m_pPoints[nIndex + 1].dX + dWidthDx - c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy + c_dKappa * dWidthDx, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx + c_dKappa * dWidthDy, pPathIn->m_pPoints[nIndex + 1].dX - dWidthDy, pPathIn->m_pPoints[nIndex + 1].dY + dWidthDx );
					break;
				case /*LineCapProjecting*/2:
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
					dMiter = ( pState->m_dMiterLimit + 1 ) * ( pState->m_dMiterLimit + 1 );
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
				if ( pState->m_nLineJoin == /*LineJoinRound*/1 ) 
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

						if ( pState->m_nLineJoin == /*LineJoinMiter*/0 && sqrt( dMiter ) <= pState->m_dMiterLimit ) // тип соединения = LineJoinMiter и dMiter меньше dMiterLimit
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

						if ( pState->m_nLineJoin == /*LineJoinMiter*/0 && sqrt( dMiter ) <= pState->m_dMiterLimit ) // тип соединения = LineJoinMiter и dMiter меньше dMiterLimit
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

			if ( pState->m_bStrokeAdjust ) 
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

	CGraphicsPath* FlattenPath(SPath *pPath, double *pMatrix, double dFlatness) 
	{
		CGraphicsPath *pFlatPath = new CGraphicsPath();
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

	void FlattenCurve(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, double *matrix, double dFlatness_2, SPath *fPath) 
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

	inline void Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY)
	{
		*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
		*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
	}

	inline void PipeInit(SPipe *pPipe, int nX, int nY, IAVSPattern* pPattern, SColorPointer pColorSrc, double dAlphaInput, BOOL bUsesShape, BOOL bNonIsolatedGroup) 
	{
		PipeSetXY( pPipe, nX, nY );
		pPipe->pPattern = NULL;

		// Source color
		if ( pPattern ) 
		{
			LONG lTypePattern = 0;
			pPattern->get_Type(&lTypePattern);
			if ( 1 == lTypePattern ) 
			{
				// нужно сделать константы
				pPattern->GetPixel( nX, nY, (LONG*)pPipe->arrColorSrcVal );
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
		if ( !m_oSoftMask.m_pPixels ) 
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
		if ( dAlphaInput == 1 && !m_oSoftMask.m_pPixels && !bUsesShape && !m_bInNonIsolatedGroup ) 
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
			pPipe->eResultColorCtrl = m_arrePipeResultColorNoAlphaBlend[m_oPixels.m_eMode];
		} 
		else if ( !m_pBlendFunction ) 
		{
			pPipe->eResultColorCtrl = m_arrePipeResultColorAlphaNoBlend[m_oPixels.m_eMode];
		} 
		else 
		{
			pPipe->eResultColorCtrl = m_arrePipeResultColorAlphaBlend[m_oPixels.m_eMode];
		}

		// Non-isolated group correction
		if ( bNonIsolatedGroup ) 
		{
			pPipe->nNonIsolatedGroup = SColorModeNComps[m_oPixels.m_eMode];
		} 
		else 
		{
			pPipe->nNonIsolatedGroup = 0;
		}
	}

	inline void PipeRun(SPipe *pPipe) 
	{
		unsigned char aSrc, aDest, alpha2, alpha0, aResult;
		SColor cDest, cBlend;
		unsigned char cResult0, cResult1, cResult2, cResult3;

		//----- source color

		// static pattern: handled in pipeInit

		// dynamic pattern
		if ( pPipe->pPattern ) 
		{
			pPipe->pPattern->GetPixel( pPipe->nX, pPipe->nY, (LONG*)pPipe->arrColorSrcVal );
		}

		if ( pPipe->bNoTransparency && !m_pBlendFunction ) 
		{

			//----- write destination pixel

			BOOL bAlphaSet = FALSE;

			switch ( m_oPixels.m_eMode ) 
			{
			case colorModeMono1:
				cResult0 = pPipe->pColorSrc[0];
				if ( m_oScreen.GetGrayPixel( pPipe->nX, pPipe->nY, cResult0 ) ) 
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
			case colorModeBGRA8:
				*pPipe->pColorDst++ = pPipe->pColorSrc[2];
				*pPipe->pColorDst++ = pPipe->pColorSrc[1];
				*pPipe->pColorDst++ = pPipe->pColorSrc[0];
				*pPipe->pColorDst++ = pPipe->pColorSrc[3];

				bAlphaSet = TRUE;

				pPipe->pAlphaDst += 4;
				break;
			case colorModeRGBA8:
				*pPipe->pColorDst++ = pPipe->pColorSrc[0];
				*pPipe->pColorDst++ = pPipe->pColorSrc[1];
				*pPipe->pColorDst++ = pPipe->pColorSrc[2];
				*pPipe->pColorDst++ = pPipe->pColorSrc[3];

				bAlphaSet = TRUE;

				pPipe->pAlphaDst += 4;
				break;
			}
			if ( pPipe->pAlphaDst && !bAlphaSet ) 
			{
				*pPipe->pAlphaDst++ = 255;
			}
		} 
		else 
		{

			//----- read destination pixel

			BOOL bAlphaSet = FALSE;

			switch ( m_oPixels.m_eMode ) 
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
			case colorModeRGBA8:
				cDest[0] = pPipe->pColorDst[0];
				cDest[1] = pPipe->pColorDst[1];
				cDest[2] = pPipe->pColorDst[2];
				cDest[3] = pPipe->pColorDst[3];

				aDest = *pPipe->pAlphaDst;

				bAlphaSet = TRUE;
				break;
			case colorModeBGRA8:
				cDest[0] = pPipe->pColorDst[2];
				cDest[1] = pPipe->pColorDst[1];
				cDest[2] = pPipe->pColorDst[0];
				cDest[3] = pPipe->pColorDst[3];

				aDest = *pPipe->pAlphaDst;

				bAlphaSet = TRUE;
				break;
			}
			if ( pPipe->pAlphaDst )
			{
				aDest = *pPipe->pAlphaDst;
			} 
			else if(!bAlphaSet)
			{
				aDest = 0xFF;
			}

			//----- blend function

			if ( m_pBlendFunction ) 
			{
				(*m_pBlendFunction)( pPipe->pColorSrc, cDest, cBlend, m_oPixels.m_eMode );
			}

			//----- source alpha

			if ( m_oSoftMask.m_pPixels ) 
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

			bAlphaSet = FALSE;

			switch ( m_oPixels.m_eMode ) 
			{
			case colorModeMono1:
				if ( m_oScreen.GetGrayPixel(pPipe->nX, pPipe->nY, cResult0) ) 
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
			case colorModeRGBA8:
				*pPipe->pColorDst++ = cResult0;
				*pPipe->pColorDst++ = cResult1;
				*pPipe->pColorDst++ = cResult2;
				*pPipe->pColorDst++ = aResult;

				bAlphaSet = TRUE;
				pPipe->pAlphaDst += 4;

				break;
			case colorModeBGRA8:
				*pPipe->pColorDst++ = cResult2;
				*pPipe->pColorDst++ = cResult1;
				*pPipe->pColorDst++ = cResult0;
				*pPipe->pColorDst++ = aResult;

				bAlphaSet = TRUE;
				pPipe->pAlphaDst += 4;

				break;
			}
			if ( pPipe->pAlphaDst && !bAlphaSet ) 
			{
				*pPipe->pAlphaDst++ = aResult;
			}
		}
		++pPipe->nX;
	}

	inline void PipeSetXY(SPipe *pPipe, int nX, int nY) 
	{
		pPipe->nX = nX;
		pPipe->nY = nY;

		if ( NULL != m_oSoftMask.m_pPixels ) 
		{
			pPipe->pSMask = &m_oSoftMask.m_pPixels[nY * m_oSoftMask.m_lStride + nX];
		}

		LONG lStride = m_oPixels.m_lStride;
		
		BOOL bIsUseAlpha = FALSE;
		
		switch ( m_oPixels.m_eMode ) 
		{
		case colorModeMono1:
			pPipe->pColorDst = &m_oPixels.m_pPixels[nY * lStride + (nX >> 3)];
			pPipe->nColorMaskDst = 0x80 >> (nX & 7);
			break;
		case colorModeMono8:
			pPipe->pColorDst = &m_oPixels.m_pPixels[nY * lStride + nX];
			break;
		case colorModeRGB8:
		case colorModeBGR8:
			pPipe->pColorDst = &m_oPixels.m_pPixels[nY * lStride + 3 * nX];
			break;
		case colorModeRGBA8:
		case colorModeBGRA8:
			pPipe->pColorDst = &m_oPixels.m_pPixels[nY * lStride + 4 * nX];
			pPipe->pAlphaDst = &m_oPixels.m_pPixels[nY * lStride + 4 * nX + 3];

			bIsUseAlpha = TRUE;
			break;
		}

		if ( m_oPixels.m_pAlpha ) 
		{
			pPipe->pAlphaDst = &m_oPixels.m_pPixels[nY * m_oPixels.m_lWidth + nX];
		} 
		else if (!bIsUseAlpha)
		{
			pPipe->pAlphaDst = NULL;
		}
		if ( m_bInNonIsolatedGroup && m_oAlpha0Bitmap.m_pAlpha ) 
		{
			pPipe->pAlpha0 = &m_oAlpha0Bitmap.m_pAlpha[( m_nAlpha0Y + nY ) * m_oAlpha0Bitmap.m_lWidth + ( m_nAlpha0X + nX )];
		} 
		else 
		{
			pPipe->pAlpha0 = NULL;
		}
	}

	inline void PipeIncreaseX(SPipe *pPipe) 
	{
		++pPipe->nX;
		if ( m_oSoftMask.m_pPixels ) 
		{
			++pPipe->pSMask;
		}

		BOOL bAlphaSet = FALSE;

		switch ( m_oPixels.m_eMode ) 
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
		case colorModeRGBA8:
		case colorModeBGRA8:
			pPipe->pColorDst += 4;
			pPipe->pAlphaDst += 4;
			bAlphaSet = TRUE;
			break;
		}

		if ( pPipe->pAlphaDst && !bAlphaSet ) 
		{
			++pPipe->pAlphaDst;
		}

		if ( pPipe->pAlpha0 ) 
		{
			++pPipe->pAlpha0;
		}
	}

	inline void DrawPixel(SPipe *pPipe, int nX, int nY, BOOL bNoClip) 
	{
		if ( bNoClip || m_oClip.IsInsideClip( nX, nY ) ) 
		{
			PipeSetXY( pPipe, nX, nY );
			PipeRun( pPipe );
			UpdateModX( nX );
			UpdateModY( nY );
		}
	}

	inline void UpdateModX(int nX) 
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

	inline void UpdateModY(int nY) 
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

	void ClearModRegion() 
	{
		m_nModRegMinX = m_oPixels.m_lWidth;
		m_nModRegMinY = m_oPixels.m_lHeight;
		m_nModRegMaxX = -1;
		m_nModRegMaxY = -1;
	}


	inline void DrawAAPixelInit() 
	{
		m_nAABufferY = -1;
	}

	inline void DrawAAPixel(SPipe* pPipe, int nX, int nY) 
	{
		static int arrnBitCount4[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

		if ( nX < 0 || nX >= m_oPixels.m_lWidth || nY < m_oClip.GetMinY() || nY > m_oClip.GetMaxY() ) 
		{
			return;
		}

		// Пересчитываем m_nAABufferY
		if ( nY != m_nAABufferY ) 
		{
			memset( m_oAABuffer.m_pPixels, 0xFF, m_oAABuffer.m_lStride * m_oAABuffer.m_lHeight );

			int nX0 = 0;
			int nX1 = m_oPixels.m_lWidth - 1;
			
			m_oClip.ClipAALine( &m_oAABuffer, &nX0, &nX1, nY );
			m_nAABufferY = nY;
		}

		SColorPointer pColor = m_oAABuffer.m_pPixels + (nX >> 1);
		int nStride = m_oAABuffer.m_lStride;

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

	inline void DrawSpan(SPipe *pPipe, int nX0, int nX1, int nY, BOOL bNoClip) 
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
				if ( m_oClip.IsInsideClip( nX, nY ) ) 
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

	inline void DrawAALine(SPipe *pPipe, int nX0, int nX1, int nY) 
	{
		static int arrnBitCount4[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

		SColorPointer pLine0 = m_oAABuffer.m_pPixels + (nX0 >> 1);

		LONG lStride = m_oAABuffer.m_lStride;
		SColorPointer pLine1 = pLine0 + lStride;
		SColorPointer pLine2 = pLine1 + lStride;
		SColorPointer pLine3 = pLine2 + lStride;

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

	int FillGlyph(int nX, int nY, TGlyphBitmap *pGlyph, IAVSPattern* pPattern, double dFillAlpha) 
	{
		SPipe oPipe;
		SClipResult eClipResult;
		//if ( ( eClipResult = m_oClip.CheckRectangle( nX - pGlyph->nX, nY - pGlyph->nY, nX - pGlyph->nX + pGlyph->nWidth - 1, nY - pGlyph->nY + pGlyph->nHeight - 1 ) ) != clipAllOutside ) 
		if ( ( eClipResult = m_oClip.CheckRectangle( nX, nY, nX + pGlyph->nWidth - 1, nY + pGlyph->nHeight - 1 ) ) != clipAllOutside )
		{
			BOOL bNoClip = (eClipResult == clipAllInside);

			if ( bNoClip ) 
			{
				if ( pGlyph->bAA ) 
				{
					/*PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, pPattern, NULL, dFillAlpha, TRUE, FALSE );
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
					}*/

					PipeInit( &oPipe, nX, nY, pPattern, NULL, dFillAlpha, TRUE, FALSE );
					unsigned char *pData = pGlyph->pData;
					
					for ( int nYY = 0, nY1 = nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
					{
						PipeSetXY( &oPipe, nX, nY1 );
						for ( int nXX = 0, nX1 = nX; nXX < pGlyph->nWidth; ++nXX, ++nX1 ) 
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
					PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, pPattern, NULL, dFillAlpha, FALSE, FALSE );
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
					PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, pPattern, NULL, dFillAlpha, TRUE, FALSE );
					unsigned char *pData = pGlyph->pData;

					for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
					{
						PipeSetXY( &oPipe, nX - pGlyph->nX, nY1 );
						for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; ++nXX, ++nX1 ) 
						{
							if ( m_oClip.IsInsideClip( nX1, nY1 ) ) 
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
					PipeInit( &oPipe, nX - pGlyph->nX, nY - pGlyph->nY, pPattern, NULL, dFillAlpha, FALSE, FALSE );
					unsigned char *pData = pGlyph->pData;

					for ( int nYY = 0, nY1 = nY - pGlyph->nY; nYY < pGlyph->nHeight; ++nYY, ++nY1 ) 
					{
						PipeSetXY( &oPipe, nX - pGlyph->nX, nY1 );
						for ( int nXX = 0, nX1 = nX - pGlyph->nX; nXX < pGlyph->nWidth; nXX += 8 ) 
						{
							int nAlpha0 = *pData++;
							for ( int nXX1 = 0; nXX1 < 8 && nXX + nXX1 < pGlyph->nWidth; ++nXX1, ++nX1 ) 
							{
								if ( m_oClip.IsInsideClip( nX1, nY1 ) ) 
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

	void SetUpMatrix(double* pElements)
	{
		Gdiplus::Matrix oMatrix;
		oMatrix.Multiply(&m_oCoordTransform, Gdiplus::MatrixOrderAppend);
		oMatrix.Multiply(&m_oBaseTransform, Gdiplus::MatrixOrderAppend);
		oMatrix.Multiply(&m_oTransform, Gdiplus::MatrixOrderAppend);

		float mass[6];
		oMatrix.GetElements(mass);

		pElements[0] = (double)mass[0];
		pElements[1] = (double)mass[1];
		pElements[2] = (double)mass[2];
		pElements[3] = (double)mass[3];
		pElements[4] = (double)mass[4];
		pElements[5] = (double)mass[5];
	}
	void GetElements(double* mass)
	{
		Gdiplus::Matrix oMatrix;
		oMatrix.Multiply(&m_oCoordTransform, Gdiplus::MatrixOrderAppend);
		oMatrix.Multiply(&m_oBaseTransform, Gdiplus::MatrixOrderAppend);
		oMatrix.Multiply(&m_oTransform, Gdiplus::MatrixOrderAppend);

		float massR[6];
		oMatrix.GetElements(massR);

		mass[0] = (double)massR[0];
		mass[1] = (double)massR[1];
		mass[2] = (double)massR[2];
		mass[3] = (double)massR[3];
		mass[4] = (double)massR[4];
		mass[5] = (double)massR[5];
	}
	
	void UpdateUnits()
	{
		// здесь - пересчет координат
		m_oCoordTransform.Reset();

		double dScaleX = 1.0;
		double dScaleY = 1.0;

		switch (m_lPageUnits)
		{
		case c_ag_PageUnitPoint:
			{
				dScaleX = m_dDpiX / c_ag_Inch_to_Point;
				dScaleY = m_dDpiY / c_ag_Inch_to_Point;
				break;
			}
		case c_ag_PageUnitMillimeter:
			{
				dScaleX = m_dDpiX / c_ag_Inch_to_MM;
				dScaleY = m_dDpiY / c_ag_Inch_to_MM;
				break;
			}
		case c_ag_PageUnitInch:
			{
				dScaleX = m_dDpiX;
				dScaleY = m_dDpiY;
				break;
			}
		default:
			break;
		};

		m_oCoordTransform.Scale((float)dScaleX, (float)dScaleY, Gdiplus::MatrixOrderAppend);
	}

	friend class CAVSGraphicsPath;
	friend class CGraphicsPath;
	friend class CAVSMatrix;
	friend class SXPath;
	friend class SPath;
	friend class CAVSGlyphImage;
};

SPipeResultColorCtrl CAVSGraphics::m_arrePipeResultColorNoAlphaBlend[] = 
{
	PipeResultColorNoAlphaBlendMono,  // 1 bit
	PipeResultColorNoAlphaBlendMono,  // 8 bit
	PipeResultColorNoAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorNoAlphaBlendRGB,    // 24 bit BGR
	PipeResultColorNoAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorNoAlphaBlendRGB    // 24 bit BGR
};

SPipeResultColorCtrl CAVSGraphics::m_arrePipeResultColorAlphaNoBlend[] = 
{
	PipeResultColorAlphaNoBlendMono,  // 1 bit
	PipeResultColorAlphaNoBlendMono,  // 8 bit
	PipeResultColorAlphaNoBlendRGB,   // 24 bit RGB
	PipeResultColorAlphaNoBlendRGB,
	PipeResultColorAlphaNoBlendRGB,   // 24 bit RGB
	PipeResultColorAlphaNoBlendRGB    // 24 bit BGR
};

SPipeResultColorCtrl CAVSGraphics::m_arrePipeResultColorAlphaBlend[] = 
{
	PipeResultColorAlphaBlendMono,  // 1 bit
	PipeResultColorAlphaBlendMono,  // 8 bit
	PipeResultColorAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorAlphaBlendRGB,    // 24 bit BGR
	PipeResultColorNoAlphaBlendRGB,   // 24 bit RGB
	PipeResultColorNoAlphaBlendRGB    // 24 bit BGR
};