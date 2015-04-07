#ifndef __COMMANDS_UTILS_HDR__
#define __COMMANDS_UTILS_HDR__

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif


#include "../../Common/ImageTransformsCoreBGRA.h"
using namespace ImageStudio;
using namespace ImageStudio::Transforms;
using namespace ImageStudio::Transforms::Core;
using namespace ImageStudio::Transforms::Core::BGRA;

//---------------------------- common ---------------------------------//
inline int BGRA_Rotate_(BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, BYTE* pResultBGRA, int nResultWidth, int nResultHeight, double dResultCenterX, double dResultCenterY, double dAngleInDegrees, double dSourceScaleX, double dSourceScaleY, BOOL bInterpolate )
{
	if (!pSourceBGRA || !pResultBGRA || nSourceWidth <= 0 || nSourceHeight <= 0 || nResultWidth <= 0 || nResultHeight <= 0)
		return -1;

	if( abs( dSourceScaleX ) < 0.0001 || abs( dSourceScaleY ) < 0.0001 )
		return 0;

	// расчитываем рабочую область на result
	dAngleInDegrees *= -IPP_PI180;
	double dSin = sin( dAngleInDegrees );
	double dCos = cos( dAngleInDegrees );

	double dSourceCenterX = (nSourceWidth - 1)  / 2.0;
	double dSourceCenterY = (nSourceHeight - 1) / 2.0;

	int left   = 0;
	int top    = 0;
	int right  = 0;
	int bottom = 0;

	if( TRUE )
	{
		double dLeft   = dResultCenterX;
		double dRight  = dResultCenterX;
		double dTop    = dResultCenterY;
		double dBottom = dResultCenterY;

		double dPX[4] = {0, nSourceWidth - 1, nSourceWidth - 1, 0};
		double dPY[4] = {0, 0, nSourceHeight - 1, nSourceHeight - 1};

		for( int i = 0; i < 4; i++ )
		{
			double dX = dPX[i] - dSourceCenterX;
			double dY = dPY[i] - dSourceCenterY;

			dPX[i] = (dY * dSin + dX * dCos) * dSourceScaleX + dResultCenterX;
			dPY[i] = (dY * dCos - dX * dSin) * dSourceScaleY + dResultCenterY;

			if( dPX[i] < dLeft )   dLeft   = dPX[i];
			else
				if( dPX[i] > dRight )  dRight  = dPX[i];

			if( dPY[i] < dTop )    dTop    = dPY[i];
			else
				if( dPY[i] > dBottom ) dBottom = dPY[i];
		}

		left   = int(dLeft)   - 1;
		right  = int(dRight)  + 1;
		top    = int(dTop)    - 1;
		bottom = int(dBottom) + 1;

		if( left   >= nResultWidth )  return 0;
		if( right  <  0 )             return 0;
		if( top    >= nResultHeight ) return 0;
		if( bottom <  0 )             return 0;

		if( left   <  0 )             left   = 0;
		if( right  >= nResultWidth )  right  = nResultWidth - 1;
		if( top    <  0 )             top    = 0;
		if( bottom >= nResultHeight ) bottom = nResultHeight - 1;
	}

	double dScaleFactorX = 1 / dSourceScaleX;
	double dScaleFactorY = 1 / dSourceScaleY;

	BgraInterpolationFunc pInterFunc = bInterpolate ? BGRA_Billinear : BGRA_Nearest;

	// пробегаем по всем пикселям result-картинки
	BYTE* pResult = pResultBGRA + (top * nResultWidth + left) * 4;
	int   nStride = (nResultWidth - (right - left + 1)) * 4;

	dResultCenterY = top  - dResultCenterY;
	dResultCenterX = left - dResultCenterX;

	top  = bottom - top  + 1;
	left = right  - left + 1;

	double dNewY = dResultCenterY;
	for( bottom = top; bottom > 0; dNewY += 1, --bottom, pResult += nStride )
	{
		double dNewX = dResultCenterX;
		for( right = left; right > 0; dNewX += 1, --right, pResult += 4 )
		{
			double dSourceX = (dNewX * dCos - dNewY * dSin) * dScaleFactorX + dSourceCenterX;
			double dSourceY = (dNewX * dSin + dNewY * dCos) * dScaleFactorY + dSourceCenterY;

			//BYTE pPixelColor[4];
			//*((DWORD*)pPixelColor) = *((DWORD*)pResult);

			pInterFunc( pResult, pSourceBGRA, nSourceWidth, nSourceHeight, nSourceWidth * 4, dSourceX, dSourceY );

			//if( pInterFunc( pPixelColor, pSourceBGRA, nSourceWidth, nSourceHeight, nSourceWidth * 4, dSourceX, dSourceY ) )
			//{
			//	// alpha-blend двух пикселов - исходного и результирующего
			//	int nAlpha = pPixelColor[3];
			//	pResult[0] = AlphaComp( pResult[0], pPixelColor[0], nAlpha );
			//	pResult[1] = AlphaComp( pResult[1], pPixelColor[1], nAlpha );
			//	pResult[2] = AlphaComp( pResult[2], pPixelColor[2], nAlpha );
			//	// -- альфу - делаем как в исходной картинке
			//	pResult[3] = nAlpha;

			//	/*int alpha = AlphaComp( 0, pPixelColor[3], 255 );
			//	pResult[0] = AlphaComp( pResult[0], pPixelColor[0], alpha );
			//	pResult[1] = AlphaComp( pResult[1], pPixelColor[1], alpha );
			//	pResult[2] = AlphaComp( pResult[2], pPixelColor[2], alpha );
			//	pResult[3] = AlphaComp( pResult[3], 255, alpha );*/
			//}
		}
	}

	return 0;
}
//---------------------------------------------------------------------//


int BGRA_TransformResize_( const BYTE* pSrcPixels, int nWidth, int nHeight, BYTE* pDstPixels, long nNewWidth, long nNewHeight, long Color, long Type, double dAspect, double dCropZoom )
{
	if(TRUE)
	{
		if (nWidth == nNewWidth && nHeight == nNewHeight)
		{
			memcpy(pDstPixels, pSrcPixels, 4 * nNewWidth * nNewHeight);
		}
		else
		{
			// variables
			IppiRect SrcRect = {0, 0, nWidth, nHeight};
			IppiSize SrcSize = {nWidth, nHeight};
			IppiSize DstSize = {nNewWidth, nNewHeight};

			double dScaleX = nNewWidth  /(double)nWidth;
			double dScaleY = nNewHeight /(double)nHeight;

			if (Type == c_nResizeStretchNearest || Type == c_nResizeStretchBillinear || Type == c_nResizeStretchBicubic)
			{
				int inter = IPPI_INTER_NN;

				switch( Type )
				{
				case c_nResizeStretchBillinear: inter = IPPI_INTER_LINEAR; break;
				case c_nResizeStretchBicubic:   inter = IPPI_INTER_CUBIC; break;
				}

				ippiResize_8u_C4R( pSrcPixels, SrcSize, nWidth*4, SrcRect, pDstPixels, nNewWidth*4, DstSize, dScaleX, dScaleY, inter );
			}
			else if (Type == c_nResizeShrinkNearest || Type == c_nResizeShrinkBillinear || Type == c_nResizeShrinkBicubic)
			{
				// clear output image first
				ClearImage( pDstPixels, nNewWidth, nNewHeight, nNewWidth*4, Color, 255 );

				double dZoom = min(1.0, min(dScaleX, dScaleY));

				dScaleX = dZoom;
				dScaleY = dZoom;

				if (dAspect != 1)
				{
					double dW = dAspect * nHeight;
					dScaleX *= (dW / nWidth);

					int nNewWidthAsp = int(dScaleX * nWidth);
					if (nNewWidthAsp > nNewWidth)
					{
						double dCorrect = (double)nNewWidth / nNewWidthAsp;
						dScaleX *= dCorrect;
						dScaleY *= dCorrect;
					}
				}

				if( nNewHeight > dScaleY * nHeight )
				{
					pDstPixels += 4 * nNewWidth * (int)((nNewHeight - dScaleY * nHeight) / 2.0);
				}
				if( nNewWidth > dScaleX * nWidth )
				{
					pDstPixels += 4 * (int)((nNewWidth - dScaleX * nWidth) / 2.0);
				}

				int inter = IPPI_INTER_NN;

				switch( Type )
				{
				case c_nResizeShrinkBillinear: inter = IPPI_INTER_LINEAR; break;
				case c_nResizeShrinkBicubic:   inter = IPPI_INTER_CUBIC; break;
				}

				ippiResize_8u_C4R( pSrcPixels, SrcSize, nWidth*4, SrcRect, pDstPixels, nNewWidth*4, DstSize, dScaleX, dScaleY, inter );
			}
			else if (Type == c_nResizeCropNearest || Type == c_nResizeCropBillinear || Type == c_nResizeCropBicubic )
			{
				// variables
				double dZoom1 = min(dScaleX, dScaleY);
				double dZoom2 = max(dScaleX, dScaleY);
				double dFactor = 1.0 / (dZoom1 + (dZoom2 - dZoom1) * dCropZoom );

				double dOffsetX = (nWidth  - nNewWidth  * dFactor) / 2.0;
				double dOffsetY = (nHeight - nNewHeight * dFactor) / 2.0;

				// compose background color
				BYTE colors[4] = {
					GetBValue(Color),
					GetGValue(Color),
					GetRValue(Color),
					255
				};

				BgraInterpolationFunc pInterFunc = (Type != c_nResizeCropNearest ? BGRA_Billinear : BGRA_Nearest);

				BYTE* pResized = pDstPixels;

				for (int y = 0; y < nNewHeight; ++y)
				{
					for (int x = 0; x < nNewWidth; ++x, pResized += 4)
					{
						// compute old pixel angle and radius
						double dSourceX = dOffsetX + x * dFactor;
						double dSourceY = dOffsetY + y * dFactor;

						*(PDWORD(pResized)) = *(PDWORD(colors));
						pInterFunc( pResized, pSrcPixels, nWidth, nHeight, nWidth*4, dSourceX, dSourceY );
					}
				}
			}
		}
	}

	return 0;
}


Gdiplus::Bitmap* BitmapFromResource(HINSTANCE hInstance, LPCTSTR szResName, LPCTSTR szResType)
{
	HRSRC hrsrc = FindResource(hInstance, szResName, szResType);
	if(!hrsrc) return 0;
	// "ненастоящий" HGLOBAL - см. описание LoadResource
	HGLOBAL hgTemp = LoadResource(hInstance, hrsrc);
	DWORD sz = SizeofResource(hInstance, hrsrc);
	void* ptrRes = LockResource(hgTemp);
	HGLOBAL hgRes = GlobalAlloc(GMEM_MOVEABLE, sz);
	if(!hgRes) return 0;
	void* ptrMem = GlobalLock(hgRes);
	// Копируем растровые данные
	CopyMemory(ptrMem, ptrRes, sz);
	GlobalUnlock(hgRes);
	IStream *pStream;
	// TRUE означает освободить память при последнем Release
	HRESULT hr = CreateStreamOnHGlobal(hgRes, TRUE, &pStream);
	if(FAILED(hr))
	{
		GlobalFree(hgRes);
		return 0;
	}
	// Используем загрузку из IStream
	Gdiplus::Bitmap *image = Gdiplus::Bitmap::FromStream(pStream);
	pStream->Release();
	return image;
}

/*
// int BGRA_Rotate_(BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, 
//				 BYTE* pResultBGRA, int nResultWidth, int nResultHeight, 
//				 double dResultCenterX, double dResultCenterY, double dAngleInDegrees, double dSourceScaleX, 
//				 double dSourceScaleY, BOOL bInterpolate );

//---------------------------------------------------------------------//

//
// int BGRA_TransformResize_(BYTE* pSrcPixels, int nWidth, int nHeight, const BYTE* pDstPixels, long nNewWidth, long nNewHeight, long Color, long Type, double dAspect, double dCropZoom );
//
*/

class ICommandButtonEvent
{
public:
	virtual void SendEvent(LONG lButtonID, LONG lType) = 0;
};

class ICommandTrackEvent
{
public:
	virtual void TrackEvent(LONG lType, double dParam1, double dParam2) = 0;
};

class CCommandButton
{
public:
	enum ButtonState
	{
		bsNone				= 0,
		bsCover				= 1,
		bsSelect			= 2
	};
	enum ButtonType
	{
		btMouseMove			= 0,
		btMouseUp			= 1,
		btMouseDown			= 2,
		btMouseButton       = 3
	};
private:
	Rect m_oBounds;
	
	Bitmap* m_pImageNone;
	Bitmap* m_pImageCover;
	Bitmap* m_pImageSelect;

	Bitmap* m_pImageSplit;
	LONG m_lSplitType;
	LONG m_lSplitWidth;

	ButtonState m_lState;

	LONG m_lID;
	ICommandButtonEvent* m_pEventReceiver;

public:
	CCommandButton(LONG lID, LONG lResNameNone, LONG lResNameCover, LONG lResNameSelect, LONG lResNameSplit, LONG lSplitType, ICommandButtonEvent* pReceiver)
	{
		m_oBounds.X = 20;
		m_oBounds.Y = 20;
		m_oBounds.Width = 50;
		m_oBounds.Height = 50;

		m_lState = bsNone;

		m_pImageNone = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(lResNameNone), _T("PNG"));
		m_pImageCover = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(lResNameCover), _T("PNG"));
		m_pImageSelect = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(lResNameSelect), _T("PNG"));

		m_lSplitWidth = -1;
		m_lSplitType = -1;
		m_pImageSplit = NULL;
		if (lSplitType >= 0)
		{
			m_pImageSplit= BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(lResNameSplit), _T("PNG"));
			m_lSplitType = lSplitType;
			m_lSplitWidth = 3;
		}

		m_pEventReceiver = pReceiver;

		m_lID = lID;
	}
	~CCommandButton()
	{
		RELEASEOBJECT(m_pImageNone);
		RELEASEOBJECT(m_pImageCover);
		RELEASEOBJECT(m_pImageSelect);
		RELEASEOBJECT(m_pImageSplit);
	}
	
	void SetBounds(LONG lX, LONG lY, LONG lWidth, LONG lHeight)
	{
		m_oBounds.X = lX;
		m_oBounds.Y = lY;
		m_oBounds.Width = lWidth;
		m_oBounds.Height = lHeight;
	}

	bool DoChange(LONG lFlag, LONG nX, LONG nY, LONG nButton)
	{
		// проверяем где мы находимся
		if ((m_oBounds.X < nX) && (nX < (m_oBounds.X + m_oBounds.Width)) &&
			(m_oBounds.Y < nY) && (nY < (m_oBounds.Y + m_oBounds.Height)))
		{
			switch (lFlag)
			{
			case btMouseMove:
				{
					if (bsNone == m_lState)
					{
						m_lState = bsCover;
					}
					break;
				}
			case btMouseUp:
				{
					if (bsSelect == m_lState)
					{
						m_lState = bsCover;
						if (NULL != m_pEventReceiver)
						{
							m_pEventReceiver->SendEvent(m_lID, 0);
						}
					}
					break;
				}
			case btMouseDown:
				{
					if (bsCover == m_lState)
					{
						m_lState = bsSelect;
					}
					break;
				}
			case btMouseButton:
				{
					break;
				}
			default:
				break;
			};
			return true;
		}
		else
		{
			m_lState = bsNone;
			return false;
		}
	}

	void DrawOnGraphics(Graphics* pGraphics)
	{
		if (NULL == pGraphics)
			return;

		switch (m_lState)
		{
		case bsNone:
			{
				if (NULL != m_pImageNone)
					pGraphics->DrawImage(m_pImageNone, m_oBounds);
				break;
			}
		case bsCover:
			{
				if (NULL != m_pImageCover)
					pGraphics->DrawImage(m_pImageCover, m_oBounds);
				break;
			}
		case bsSelect:
			{
				if (NULL != m_pImageSelect)
					pGraphics->DrawImage(m_pImageSelect, m_oBounds);
				break;
			}
		default:
			break;
		};

		// теперь есть ли сплит...
		if (0 < m_lSplitWidth)
		{
			switch (m_lSplitType)
			{
			case 0:
				{
					Rect oSplitRect(m_oBounds.X, m_oBounds.Y, m_lSplitWidth, m_oBounds.Height);
					pGraphics->DrawImage(m_pImageSplit, oSplitRect);
					break;
				}
			case 1:
				{
					Rect oSplitRect(m_oBounds.GetRight() - m_lSplitWidth, m_oBounds.Y, m_lSplitWidth, m_oBounds.Height);
					pGraphics->DrawImage(m_pImageSplit, oSplitRect);
					break;
				}
			};
		}
	}
};

#endif