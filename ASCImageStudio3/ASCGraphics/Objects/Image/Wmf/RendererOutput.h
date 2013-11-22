#ifndef _RENDERER_OUTPUT_H
#define _RENDERER_OUTPUT_H

#include "..\..\..\Interfaces\ASCRenderer.h"
#include "..\..\Structures.h"
#include "WmfUtils.h"
#include "WmfCharsets.h"

//------ Для дебага ----------------------------------------------------
//#define NO_PIE       // отключаем команду Draw_Pie
//#define NO_ARC       // отключаем команду Draw_Arc
//#define NO_ELLIPSE   // отключаем команду Draw_Ellipse
//#define NO_LINE      // отключаем команду Draw_Line
//#define NO_POLYGON   // отключаем команду Draw_Polygon
//#define NO_POLYGONS  // отключаем команду Draw_Polypolygon
//#define NO_POLYLINE  // отключаем команду Draw_Polyline
//#define NO_CLIP      // отключаем комануд Region_Clip
//#define NO_RECT      // отключаем команду Draw_Rectangle
//#define NO_TEXT      // отключаем комунду Draw_Text
//#define DRAW_BOUNDS  // рисуем границы
//----------------------------------------------------------------------

#define RGBA(r, g, b) ((DWORD)( ( (BYTE)(r) )| ( ( (BYTE)(g) ) << 8 ) | ( ( (BYTE)(b) ) << 16 ) | ( (BYTE)(0) << 24 ) ) )

class CRendererOutput : public CWmfOutputDevice
{

public:

	CRendererOutput(CWmfFile *pFile, IASCRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		m_pWmfFile = pFile;

		//m_oPen.SetSize( 0.1 );
		m_oPen.Size = 0.1;

		m_dX = 0;
		m_dY = 0;

		m_dX1 = dX;
		m_dY1 = dY;
		m_pRenderer = NULL;

		if ( !pRenderer )
			return;

		m_pRenderer = pRenderer;

		m_oShadow.Visible = FALSE;
		m_pRenderer->put_ShadowVisible(m_oShadow.Visible);
		/*BSTR bsShadow = m_oShadow.ToXmlString().AllocSysString();
		m_pRenderer->SetShadow( bsShadow );
		::SysFreeString( bsShadow );*/
	}
	~CRendererOutput()
	{
	}


	virtual bool IsSupportPolypolygon() { return true; }
	void SetDpi(double dDpiX, double dDpiY)
	{
		m_dDpiX = dDpiX;
		m_dDpiY = dDpiY;
	}

	void SetWmfRect(TWmfRectF oRect)
	{
		m_oWmfRect = oRect;
	}
	void SetScales(double dScaleX, double dScaleY)
	{
		m_dScaleX = dScaleX;
		m_dScaleY = dScaleY;
	}

	//----------------------------------------------------------------------------------
	void Begin()
	{
	}	
	void End()
	{
		DrawBounds( m_pWmfFile->m_pPlayerData->pDC );
	}

	void Flood_Interior(TWmfFlood *pFlood)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k = 10;
	}
	void Flood_Exterior(TWmfFlood *pFlood)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k = 10;
	}

	void Draw_Pixel      (TWmfDrawPixel *pDrawPixel)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k =10;
	}

	void Draw_Pie        (TWmfDrawArc *pArc)
	{
#ifdef NO_PIE
		return;
#endif
		long lDrawPathType = -1;
		
		if ( SetBrush( pArc->pDC ) )
			lDrawPathType = c_nWindingFillMode;

		if ( SetPen( pArc->pDC ) )
		{
			if ( -1 == lDrawPathType )
				lDrawPathType = c_nStroke;
			else
				lDrawPathType |= c_nStroke;
		}

		if ( -1 == lDrawPathType )
			return;

		double dX = pArc->oTL.fX;
		double dY = pArc->oTL.fY;

		double dW = pArc->oBR.fX - pArc->oTL.fX;
		double dH = pArc->oBR.fY - pArc->oTL.fY;


		double dStartAngle;
		if ( 0 == pArc->oStart.fX )
		{
			if ( pArc->oStart.fY >= 0 )
				dStartAngle = 90;
			else
				dStartAngle = -90;
		}
		else
		{
			dStartAngle = tan( pArc->oStart.fY / pArc->oStart.fX ) * 180 / M_PI;
		}

		double dEndAngle;
		if ( 0 == pArc->oEnd.fX )
		{
			if ( pArc->oEnd.fY >= 0 )
				dEndAngle = 90;
			else
				dEndAngle = -90;
		}
		else
		{
			dEndAngle = tan( pArc->oEnd.fY / pArc->oEnd.fX ) * 180 / M_PI;
		}


		Trans( pArc->pDC, &dX, &dY );
		double dX1 = pArc->oBR.fX;
		double dY1 = pArc->oBR.fY;
		Trans( pArc->pDC, &dX1, &dY1 );
		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		double dXX = pArc->oStart.fX + ( pArc->oTL.fX + pArc->oBR.fX ) / 2;
		double dYY = pArc->oStart.fY + ( pArc->oTL.fY + pArc->oBR.fY ) / 2;
		Trans( pArc->pDC, &dXX, &dYY );
		m_pRenderer->PathCommandMoveTo( (float)dXX, (float)dYY );
		m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), (float)dStartAngle, (float)(dEndAngle - dStartAngle) );

		dX = ( pArc->oTL.fX + pArc->oBR.fX ) / 2;
		dY = ( pArc->oTL.fY + pArc->oBR.fY ) / 2;
		Trans( pArc->pDC, &dX, &dY );
		m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );
		m_pRenderer->PathCommandClose();
		m_pRenderer->DrawPath( lDrawPathType );

		m_pRenderer->EndCommand( c_nPathType );	
	}
	void Draw_Chord      (TWmfDrawArc *pArc)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k =10;
	}
	void Draw_Arc        (TWmfDrawArc *pArc)
	{
#ifdef NO_ARC
		return;
#endif

		if ( !SetPen( pArc->pDC ) )
			return;

		double dX = pArc->oTL.fX;
		double dY = pArc->oTL.fY;

		double dW = pArc->oBR.fX - pArc->oTL.fX;
		double dH = pArc->oBR.fY - pArc->oTL.fY;


		double dStartAngle;
		if ( 0 == pArc->oStart.fX )
		{
			if ( pArc->oStart.fY >= 0 )
				dStartAngle = 90;
			else
				dStartAngle = -90;
		}
		else
		{
			dStartAngle = tan( pArc->oStart.fY / pArc->oStart.fX ) * 180 / M_PI;
		}

		double dEndAngle;
		if ( 0 == pArc->oEnd.fX )
		{
			if ( pArc->oEnd.fY >= 0 )
				dEndAngle = 90;
			else
				dEndAngle = -90;
		}
		else
		{
			dEndAngle = tan( pArc->oEnd.fY / pArc->oEnd.fX ) * 180 / M_PI;
		}


		Trans( pArc->pDC, &dX, &dY );
		double dX1 = pArc->oBR.fX;
		double dY1 = pArc->oBR.fY;
		Trans( pArc->pDC, &dX1, &dY1 );
		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		double dXX = pArc->oStart.fX + ( pArc->oTL.fX + pArc->oBR.fX ) / 2;
		double dYY = pArc->oStart.fY + ( pArc->oTL.fY + pArc->oBR.fY ) / 2;
		Trans( pArc->pDC, &dXX, &dYY );
		m_pRenderer->PathCommandMoveTo( (float)dXX, (float)dYY );

		m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), (float)dStartAngle, (float)(dEndAngle - dStartAngle) );
		m_pRenderer->DrawPath( c_nStroke );

		m_pRenderer->EndCommand( c_nPathType );	
	}

	void Draw_Ellipse    (TWmfDrawArc *pArc)
	{
#ifdef NO_ELLIPSE
		return;
#endif
		long lDrawPathType = -1;
		
		if ( SetBrush( pArc->pDC ) )
			lDrawPathType = c_nWindingFillMode;

		if ( SetPen( pArc->pDC ) )
		{
			if ( -1 == lDrawPathType )
				lDrawPathType = c_nStroke;
			else
				lDrawPathType |= c_nStroke;
		}

		if ( -1 == lDrawPathType )
			return;

		double dX = pArc->oTL.fX;
		double dY = pArc->oTL.fY;

		double dX1 = pArc->oBR.fX;
		double dY1 = pArc->oBR.fY;

		Trans( pArc->pDC, &dX, &dY );
		Trans( pArc->pDC, &dX1, &dY1 );
		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();
		m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), 0.f, 360.f );
		m_pRenderer->DrawPath( lDrawPathType );

		m_pRenderer->EndCommand( c_nPathType );
	}
	void Draw_Line       (TWmfDrawLine *pLine)
	{
#ifdef NO_LINE
		return;
#endif
		if ( !SetPen( pLine->pDC ) )
			return;

		double dX, dY;
		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		dX = pLine->oFrom.fX;
		dY = pLine->oFrom.fY;
		Trans( pLine->pDC, &dX, &dY );
		m_pRenderer->PathCommandMoveTo( (float)dX, (float)dY );

		dX = pLine->oTo.fX;
		dY = pLine->oTo.fY;
		Trans( pLine->pDC, &dX, &dY );
		m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );
		m_pRenderer->DrawPath( c_nStroke );
		m_pRenderer->EndCommand( c_nPathType );
	}
	void Poly_Line       (TWmfPolyLine *pPolyLine)
	{
#ifdef NO_POLYLINE
		return;
#endif
		TWmfDC *pDC = pPolyLine->pDC;

		if ( !SetPen( pDC ) )
			return;

		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		for ( unsigned short ushIndex = 0; ushIndex < pPolyLine->ushCount; ushIndex++ )
		{
			double dX = (double)pPolyLine->pPoints[ushIndex].fX;
			double dY = (double)pPolyLine->pPoints[ushIndex].fY;

			Trans( pDC, &dX, &dY );
			if ( 0 == ushIndex )
				m_pRenderer->PathCommandMoveTo( (float)dX, (float)dY );
			else
				m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );
		}

		m_pRenderer->DrawPath( c_nStroke );
		
		m_pRenderer->EndCommand( c_nPathType );
	}
	void Draw_Polypolygon(TWmfPolyPoly *pPolyPoly)
	{
#ifdef NO_POLYGONS
		return;
#endif
		TWmfDC *pDC = pPolyPoly->pDC;

		long lDrawPathType = -1;
		
		if ( SetBrush( pDC ) )
		{
			if ( WINDING == pDC->ushPolyFillMode )
				lDrawPathType = c_nWindingFillMode;
			else if ( ALTERNATE == pDC->ushPolyFillMode )
				lDrawPathType = c_nEvenOddFillMode;
		}

		if ( SetPen( pDC ) )
		{
			if ( -1 == lDrawPathType )
				lDrawPathType = c_nStroke;
			else
				lDrawPathType |= c_nStroke;
		}

		if ( -1 == lDrawPathType )
			return;

		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		for ( unsigned short ushPolyIndex = 0; ushPolyIndex < pPolyPoly->ushPolyCount; ushPolyIndex++ )
		{
			unsigned short ushCount = pPolyPoly->pCount[ushPolyIndex];
			TWmfCoordF    *pPoints  = pPolyPoly->ppPoints[ushPolyIndex];
			for ( unsigned short ushIndex = 0; ushIndex < ushCount; ushIndex++ )
			{
				double fX = pPoints[ushIndex].fX;
				double fY = pPoints[ushIndex].fY;

				Trans( pDC, (double *)&fX, (double *)&fY );
				if ( 0 == ushIndex )
				{
					m_pRenderer->PathCommandMoveTo( (float)fX, (float)fY );
				}
				else
				{
					m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );
				}
			}
			m_pRenderer->PathCommandClose();
		}

		m_pRenderer->DrawPath( lDrawPathType );

		m_pRenderer->EndCommand( c_nPathType );
	}
	void Draw_Polygon    (TWmfPolyLine *pPolyLine)
	{
#ifdef NO_POLYGON
		return;
#endif
		TWmfDC *pDC = pPolyLine->pDC;

		long lDrawPathType = -1;
		
		if ( SetBrush( pDC ) )
		{
			if ( WINDING == pDC->ushPolyFillMode )
				lDrawPathType = c_nWindingFillMode;
			else if ( ALTERNATE == pDC->ushPolyFillMode )
				lDrawPathType = c_nEvenOddFillMode;
		}

		if ( SetPen( pDC ) )
		{
			if ( -1 == lDrawPathType )
				lDrawPathType = c_nStroke;
			else
				lDrawPathType |= c_nStroke;
		}

		if ( -1 == lDrawPathType )
			return;

		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		for ( unsigned short ushIndex = 0; ushIndex < pPolyLine->ushCount; ushIndex++ )
		{
			double fX = pPolyLine->pPoints[ushIndex].fX;
			double fY = pPolyLine->pPoints[ushIndex].fY;

			Trans( pDC, (double *)&fX, (double *)&fY );
			if ( 0 == ushIndex )
			{
				m_pRenderer->PathCommandMoveTo( (float)fX, (float)fY );
			}
			else
			{
				m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );
			}
		}

		m_pRenderer->PathCommandClose();
		m_pRenderer->DrawPath( lDrawPathType );

		m_pRenderer->EndCommand( c_nPathType );
	}
	void Draw_Rectangle  (TWmfDrawRectangle *pRect)
	{
#ifdef NO_RECT
		return;
#endif
		long lDrawPathType = -1;
		
		if ( SetBrush( pRect->pDC ) )
			lDrawPathType = c_nWindingFillMode;

		if ( SetPen( pRect->pDC ) )
		{
			if ( -1 == lDrawPathType )
				lDrawPathType = c_nStroke;
			else
				lDrawPathType |= c_nStroke;
		}

		if ( -1 == lDrawPathType )
			return;

		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();


		if ( 0 == pRect->fWidth && 0 == pRect->fHeight ) // Простой прямоугольник
		{
			double dX = pRect->oTL.fX;
			double dY = pRect->oTL.fY;

			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandMoveTo( (float)dX, (float)dY );

			dX = pRect->oTL.fX;
			dY = pRect->oBR.fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->oBR.fX;
			dY = pRect->oBR.fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->oBR.fX;
			dY = pRect->oTL.fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			m_pRenderer->PathCommandClose();
		}
		else // прямоугольник с закругленными углами
		{
			double dA = pRect->fWidth / 2;
			double dB = pRect->fHeight / 2;

			double dX = pRect->oTL.fX;
			double dY = pRect->oTL.fY + dB;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandMoveTo( (float)dX, (float)dY );

			dX = pRect->oTL.fX;
			dY = pRect->oTL.fY;

			double dX1 = dX + pRect->fWidth;
			double dY1 = dY + pRect->fHeight;

			Trans( pRect->pDC, &dX, &dY );
			Trans( pRect->pDC, &dX1, &dY1 );

			m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), 180.f, 90.f );

			dX = pRect->oBR.fX - dA;
			dY = pRect->oTL.fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->oBR.fX - pRect->fWidth;
			dY = pRect->oTL.fY;
			dX1 = dX + pRect->fWidth;
			dY1 = dY + pRect->fHeight;
			Trans( pRect->pDC, &dX, &dY );
			Trans( pRect->pDC, &dX1, &dY1 );
			m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), 270.f, 90.f );

			dX = pRect->oBR.fX;
			dY = pRect->oBR.fY - dB;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->oBR.fX - pRect->fWidth;
			dY = pRect->oBR.fY - pRect->fHeight;
			dX1 = pRect->oBR.fX;
			dY1 = pRect->oBR.fY;
			Trans( pRect->pDC, &dX, &dY );
			Trans( pRect->pDC, &dX1, &dY1 );
			m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), 0.f, 90.f );

			dX = pRect->oTL.fX + dA;
			dY = pRect->oBR.fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->oTL.fX;
			dY = pRect->oBR.fY - pRect->fHeight;
			dX1 = dX + pRect->fWidth;
			dY1 = dY + pRect->fHeight;
			Trans( pRect->pDC, &dX, &dY );
			Trans( pRect->pDC, &dX1, &dY1 );
			m_pRenderer->PathCommandArcTo( (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY), 90.f, 90.f );

			m_pRenderer->PathCommandClose();
		}
		m_pRenderer->DrawPath( lDrawPathType );

		m_pRenderer->EndCommand( c_nPathType );
	}

	void Rop_Draw(TWmfROPDraw *pROP)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k =10;
	}
	void Bmp_Draw(TWmfBMPDraw *pDrawBitmap)
	{
		TWmfDC *pDC = pDrawBitmap->pDC;

		// create result interface
		IUnknown *pInterface = NULL;
		HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
		if ( hRes != S_OK || NULL == pInterface )
			return;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

		int nWidth  = pDrawBitmap->oBitmap.ushWidth;
		int nHeight = pDrawBitmap->oBitmap.ushHeight;
		int nBufferSize = 4 * nWidth * nHeight;
		if ( nBufferSize < 1 )
			return;

		pMediaData->put_ColorSpace( ( 1 << 6) /*| ( 1 << 31)*/ ); // CPS_BGRA | CPS_FLIP
		pMediaData->put_Width( nWidth );
		pMediaData->put_Height( nHeight );
		pMediaData->put_AspectRatioX( nWidth );
		pMediaData->put_AspectRatioY( nHeight );
		pMediaData->put_Interlaced( VARIANT_FALSE );
		pMediaData->put_Stride( 0, 4 * nWidth );
		pMediaData->AllocateBuffer( nBufferSize );

		unsigned char *pBufferPtr = NULL;
		long nCreatedBufferSize = 0;

		pMediaData->get_Buffer( &pBufferPtr );
		pMediaData->get_BufferSize( &nCreatedBufferSize );
		pMediaData->put_Plane( 0, pBufferPtr );

		if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
			return;

		pInterface = pMediaData;

		// Пишем данные в pBufferPtr

		for ( int nIndex = 0; nIndex < 4 * nWidth * nHeight; nIndex += 4 ) 
		{
			pBufferPtr[0] = (unsigned char)pDrawBitmap->oBitmap.pData[nIndex + 0];
			pBufferPtr[1] = (unsigned char)pDrawBitmap->oBitmap.pData[nIndex + 1];
			pBufferPtr[2] = (unsigned char)pDrawBitmap->oBitmap.pData[nIndex + 2];
			pBufferPtr[3] = (unsigned char)pDrawBitmap->oBitmap.pData[nIndex + 3];
			pBufferPtr += 4;
		}

		double dX = pDrawBitmap->oPoint.fX;
		double dY = pDrawBitmap->oPoint.fY;

		double dX1 = dX + pDrawBitmap->oBitmap.ushWidth  * pDrawBitmap->dPixelWidth;
		double dY1 = dY + pDrawBitmap->oBitmap.ushHeight * pDrawBitmap->dPixelHeight;

		Trans( pDC, &dX,  &dY );
		Trans( pDC, &dX1, &dY1 );
		m_pRenderer->DrawImage( pInterface, (float)dX, (float)dY, (float)(dX1 - dX), (float)(dY1 - dY));

	}
	void Bmp_Read(TWmfBMPRead *pReadBitmap)
	{
		if ( pReadBitmap->lLength <= 0 || NULL == pReadBitmap->pBuffer )
			return;
		

		BYTE *pBuffer = pReadBitmap->pBuffer;
		long lBufLen  = pReadBitmap->lLength;

		// Считываем заголовок
		unsigned int   unHeaderSize = ReadUInt  ( pBuffer ); pBuffer += 4; lBufLen -=4;

		if ( unHeaderSize < 0x0000000C )
			return;
		else if ( 0x0000000C == unHeaderSize ) // BitmapCoreHeader
			ReadImage_CoreHeader( pReadBitmap, pBuffer, lBufLen );
		else // BitmapInfoHeader
			ReadImage_InfoHeader( pReadBitmap, pBuffer, lBufLen );

	}
	void Bmp_Free(TWmfBMP *pBitmap)
	{
		if ( pBitmap->pData )
			free( pBitmap->pData );
	}

	void Draw_Text(TWmfDrawText *pText)
	{
#ifdef NO_TEXT
		return;
#endif

		double dSize = pText->dFontHeight * m_dScaleY * 72 / 25.4;
		SetFont( pText->pDC, dSize );

		USES_CONVERSION;

		double dTheta = - ( ( ( (double)pText->pDC->pFont->shEscapement ) / 10 ) * M_PI / 180 );

		float fCosTheta = (float)cos( dTheta );
		float fSinTheta = (float)sin( dTheta );

		double dX = 0, dY = 0;
		float fL = 0, fT = 0, fW = 0, fH = 0;
		float fUndX1 = 0, fUndY1 = 0, fUndX2 = 0, fUndY2 = 0, fUndSize = 1;
		if ( m_pWmfFile->m_pFontManager )
		{
			TWmfFont *pFont = pText->pDC->pFont;
			long lStyle = ( pFont->ushWeight > 550 ? 1 : 0 ) + ( pFont->unItalic ? 2 : 0 );
			m_pWmfFile->m_pFontManager->LoadFontByName( A2W(pText->pDC->pFont->sFaceName), dSize, lStyle, 72, 72 );
			m_pWmfFile->m_pFontManager->LoadStringW( A2W(pText->sText), 0, 0 );
			m_pWmfFile->m_pFontManager->MeasureString2( &fL, &fT, &fW, &fH );
			m_pWmfFile->m_pFontManager->GetUnderline( &fUndX1, &fUndY1, &fUndX2, &fUndY2, &fUndSize );

			double fKoef = 25.4 / 72;

			fL *= fKoef;
			fT *= fKoef;

			fW *= fKoef;
			fH *= fKoef;

			fUndX1 *= fKoef; fUndY1 *= fKoef;
			fUndX2 *= fKoef; fUndY2 *= fKoef;
			fUndSize *= fKoef / 2;
		}

		if ( !pText->bUseDx )
		{
			dX = pText->oOrigin.fX;
			dY = pText->oOrigin.fY;
			Trans( pText->pDC, &dX, &dY );

			// Найдем начальную точку текста

			if ( pText->pDC->ushTextAlign & TA_BASELINE )
			{	
				// Ничего не делаем
			}
			else if ( pText->pDC->ushTextAlign & TA_BOTTOM )
			{	
				float fTemp = -(-fT + fH);

				dX += -fTemp * fSinTheta;
				dY +=  fTemp * fCosTheta;
			}
			else // if ( pPlayer->pDC->ushTextAlign & TA_TOP )
			{	
				float fTemp = -fT;

				dX += -fTemp * fSinTheta;
				dY +=  fTemp * fCosTheta;
			}

			if ( pText->pDC->ushTextAlign & TA_CENTER )
			{
				dX += -fW / 2 * fCosTheta;
				dY += -fW / 2 * fSinTheta;
			}
			else if ( pText->pDC->ushTextAlign & TA_RIGHT )
			{
				dX += -fW * fCosTheta;
				dY += -fW * fSinTheta;
			}
			else //if ( pText->pDC->ushTextAlign & TA_LEFT )
			{
				// Ничего не делаем
			}
		}
		else
		{
			dX = pText->oPoint.fX;
			dY = pText->oPoint.fY;
			Trans( pText->pDC, &dX, &dY );
		}

		if ( pText->pDC->pFont->unUnderline )
		{
			fUndX1 += dX;
			fUndX2 += dX;

			fUndY1 += dY;
			fUndY2 += dY;
		}

		CStringW wsText = TextToUnicode( pText->pDC, pText->sText );
		BSTR bsText = wsText.AllocSysString();

		bool bChangeCTM = false;

		if ( 0 != pText->pDC->pFont->shEscapement )
		{
			// TO DO: тут реализован только параметр shEscapement, еще нужно реализовать параметр shOrientation
			m_pRenderer->SetTransform( fCosTheta, fSinTheta, -fSinTheta, fCosTheta, dX - dX * fCosTheta + dY * fSinTheta, dY - dX * fSinTheta - dY * fCosTheta );
			bChangeCTM = true;
		}

		// Для начала нарисуем фон текста

		if ( OPAQUE == pText->pDC->ushBGMode )
		{
			long lOldColor = m_oBrush.Color1;
			long lOldAlpha = m_oBrush.Alpha1;
			long lOldType  = m_oBrush.Type;

			m_oBrush.Color1 = ( RGB( pText->pDC->oBGColor.r, pText->pDC->oBGColor.g, pText->pDC->oBGColor.b ) );
			m_oBrush.Alpha1 = 255;
			m_oBrush.Type   = c_BrushTypeSolid; // Solid

			m_pRenderer->put_BrushType(m_oBrush.Type);
			m_pRenderer->put_BrushColor1(m_oBrush.Color1);
			m_pRenderer->put_BrushAlpha1(m_oBrush.Alpha1);

			float fX = (float)dX;
			float fY = (float)dY;

			m_pRenderer->BeginCommand( c_nPathType );
			m_pRenderer->PathCommandEnd();

			m_pRenderer->PathCommandMoveTo( fX + fL, fY + fT );
			m_pRenderer->PathCommandLineTo( fX + fL + fW, fY + fT );
			m_pRenderer->PathCommandLineTo( fX + fL + fW, fY + fT + fH );
			m_pRenderer->PathCommandLineTo( fX + fL, fY + fT + fH );

			m_pRenderer->PathCommandClose();
			m_pRenderer->DrawPath( c_nWindingFillMode );

			m_pRenderer->EndCommand( c_nPathType );

			m_oBrush.Color1 = lOldColor;
			m_oBrush.Alpha1 = lOldAlpha;
			m_oBrush.Type   = lOldType;

			m_pRenderer->put_BrushType(m_oBrush.Type);
			m_pRenderer->put_BrushColor1(m_oBrush.Color1);
			m_pRenderer->put_BrushAlpha1(m_oBrush.Alpha1);
		}

		// Нарисуем подчеркивание 
		if ( pText->pDC->pFont->unUnderline )
		{
			double        dOldSize      = m_oPen.Size;
			unsigned char unOldEndCap   = m_oPen.LineEndCap;
			unsigned char unOldStartCap = m_oPen.LineStartCap;
			m_pRenderer->put_PenSize( (double)fUndSize );
			m_pRenderer->put_PenLineEndCap( 0 );
			m_pRenderer->put_PenLineStartCap( 0 );

			m_pRenderer->BeginCommand( c_nPathType );
			m_pRenderer->PathCommandEnd();

			m_pRenderer->PathCommandMoveTo( fUndX1, fUndY1 );
			m_pRenderer->PathCommandLineTo( fUndX2, fUndY2 );

			m_pRenderer->DrawPath( c_nStroke );

			m_pRenderer->EndCommand( c_nPathType );

			m_pRenderer->put_PenSize( dOldSize );
			m_pRenderer->put_PenLineEndCap( unOldEndCap );
			m_pRenderer->put_PenLineStartCap( unOldStartCap );
		}

		// Рисуем сам текст


		m_pRenderer->CommandDrawText( bsText, (float)dX, (float)(dY ), 0, 0, 0 );
		::SysFreeString( bsText );

		if ( bChangeCTM )
			m_pRenderer->ResetTransform( );

	}

	void Region_Frame(TWmfPolyRectangle *pRect)
	{
		// TO DO: Сделать, как будут тестовые файлы
		int k =10;
	}
	void Region_Paint(TWmfPolyRectangle *pRect)
	{
		// TO DO: Сделать, как будут тестовые файлы		
		int k =10;
	}
	void Region_Clip (TWmfPolyRectangle *pRect)
	{
#ifdef NO_CLIP
		return;
#endif
	
		// TO DO: Сделать, как будут тестовые файлы
		m_pRenderer->BeginCommand( c_nResetClipType );
		m_pRenderer->EndCommand( c_nResetClipType );

		if ( pRect->unCount	<= 0 )
			return;
		
		m_pRenderer->BeginCommand( c_nClipType );
		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandStart();
		for ( unsigned int unIndex = 0; unIndex < pRect->unCount; unIndex++ )
		{
			double dX = pRect->pTL[unIndex].fX;
			double dY = pRect->pTL[unIndex].fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandMoveTo( (float)dX, (float)dY );

			dX = pRect->pTL[unIndex].fX;
			dY = pRect->pBR[unIndex].fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->pBR[unIndex].fX;
			dY = pRect->pBR[unIndex].fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );

			dX = pRect->pBR[unIndex].fX;
			dY = pRect->pTL[unIndex].fY;
			Trans( pRect->pDC, &dX, &dY );
			m_pRenderer->PathCommandLineTo( (float)dX, (float)dY );
		}
		m_pRenderer->PathCommandEnd();
		m_pRenderer->EndCommand( c_nPathType );
		m_pRenderer->EndCommand( c_nClipType );
	}

private:

	void DrawBounds(TWmfDC *pDC)
	{
#ifdef DRAW_BOUNDS

		m_pRenderer->put_PenAlpha( 255 );
		m_pRenderer->put_PenColor( 0 );
		m_pRenderer->put_PenSize( 1 );
		m_pRenderer->put_PenDashStyle( 0 );

		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		double dW_4 = (m_oWmfRect.oBR.fX - m_oWmfRect.oTL.fX) / 4;
		double dH_4 = (m_oWmfRect.oBR.fY - m_oWmfRect.oTL.fY) / 4;
		double fX = m_oWmfRect.oTL.fX;
		double fY = m_oWmfRect.oTL.fY;

		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandMoveTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oBR.fX;
		fY = m_oWmfRect.oTL.fY;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oBR.fX;
		fY = m_oWmfRect.oBR.fY;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oTL.fX;
		fY = m_oWmfRect.oBR.fY;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		m_pRenderer->PathCommandClose();
		m_pRenderer->DrawPath( c_nStroke );

		m_pRenderer->EndCommand( c_nPathType );


		m_pRenderer->BeginCommand( c_nPathType );
		m_pRenderer->PathCommandEnd();

		fX = m_oWmfRect.oTL.fX + dW_4;
		fY = m_oWmfRect.oTL.fY + dH_4;

		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandMoveTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oBR.fX - dW_4;
		fY = m_oWmfRect.oTL.fY + dH_4;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oBR.fX - dW_4;
		fY = m_oWmfRect.oBR.fY - dH_4;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		fX = m_oWmfRect.oTL.fX + dW_4;
		fY = m_oWmfRect.oBR.fY - dH_4;
		Trans( pDC, (double *)&fX, (double *)&fY );
		m_pRenderer->PathCommandLineTo( (float)fX, (float)fY );

		m_pRenderer->PathCommandClose();
		m_pRenderer->DrawPath( c_nStroke );

		m_pRenderer->EndCommand( c_nPathType );

#endif
	}
	void Trans(TWmfDC *pDC, double *pdX, double *pdY)
	{
		double dX = *pdX;
		double dY = *pdY;

		if ( pDC->oWindow.nHeight < 0 )
		{
			*pdY = m_dScaleY * ( (m_oWmfRect.oBR.fY - m_oWmfRect.oTL.fY) - (dY - m_oWmfRect.oTL.fY) ) + m_dY1;
		}
		else
			*pdY = m_dScaleY * ( dY - m_oWmfRect.oTL.fY ) + m_dY1;

		if ( pDC->oWindow.nWidth < 0 )
		{
			*pdX = m_dScaleX * ( (m_oWmfRect.oBR.fX - m_oWmfRect.oTL.fX) - (dX - m_oWmfRect.oTL.fX) ) + m_dX1;
		}
		else
			*pdX = m_dScaleX * (dX - m_oWmfRect.oTL.fX) + m_dX1;
	}

	CStringW TextToUnicode(TWmfDC *pDC, char *sText)
	{
		// Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
		// http://msdn.microsoft.com/en-us/library/cc194829.aspx

		//  Charset Name       Charset Value(hex)  Codepage number
		//  ------------------------------------------------------
		//
		//  DEFAULT_CHARSET           1 (x01)
		//  SYMBOL_CHARSET            2 (x02)
		//  OEM_CHARSET             255 (xFF)
		//  ANSI_CHARSET              0 (x00)            1252
		//  RUSSIAN_CHARSET         204 (xCC)            1251
		//  EASTEUROPE_CHARSET      238 (xEE)            1250
		//  GREEK_CHARSET           161 (xA1)            1253
		//  TURKISH_CHARSET         162 (xA2)            1254
		//  BALTIC_CHARSET          186 (xBA)            1257
		//  HEBREW_CHARSET          177 (xB1)            1255
		//  ARABIC _CHARSET         178 (xB2)            1256
		//  SHIFTJIS_CHARSET        128 (x80)             932
		//  HANGEUL_CHARSET         129 (x81)             949
		//  GB2313_CHARSET          134 (x86)             936
		//  CHINESEBIG5_CHARSET     136 (x88)             950
		//  THAI_CHARSET            222 (xDE)             874	
		//  JOHAB_CHARSET	        130 (x82)            1361
		//  VIETNAMESE_CHARSET      163 (xA3)            1258

		if ( ANSI_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1252[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;

			//USES_CONVERSION;
			//wchar_t *wsText = A2W( sText );
			//CStringW wsResult( wsText ); 
			//return wsResult;
		}
		else if ( DEFAULT_CHARSET == pDC->pFont->unCharSet || UNKNOWN_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				wsText[nIndex] = (unsigned char)(sText[nIndex]);
			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( SYMBOL_CHARSET == pDC->pFont->unCharSet )
		{
			// Ко всем символам, кроме пробела добавляем 0xF000 (Вот и вся кодировка :))

			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				wsText[nIndex] = (unsigned char)(sText[nIndex]);
				//if ( wsText[nIndex] != 0x0020 )
					wsText[nIndex] += 0xF000;
			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( MAC_CHARSET == pDC->pFont->unCharSet )
		{
			// Не используется

			return _T("");
		}
		else if ( SHIFTJIS_CHARSET == pDC->pFont->unCharSet )
		{
			return CP932_CharToUnicode( sText );
		}
		else if ( HANGUL_CHARSET == pDC->pFont->unCharSet )
		{
			return CP949_CharToUnicode( sText );
		}
		else if ( JOHAB_CHARSET == pDC->pFont->unCharSet )
		{
			return CP1361_CharToUnicode( sText );
		}
		else if ( GB2312_CHARSET == pDC->pFont->unCharSet )
		{
			return CP936_CharToUnicode( sText );
		}
		else if ( CHINESEBIG5_CHARSET == pDC->pFont->unCharSet )
		{
			return CP950_CharToUnicode( sText );
		}
		else if ( GREEK_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1253[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( TURKISH_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1254[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( VIETNAMESE_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1258[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( HEBREW_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1255[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( ARABIC_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1256[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( BALTIC_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1257[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( RUSSIAN_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1251[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( THAI_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP874[unChar - MSCP_FIRST_CHAR] );

			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( EASTEUROPE_CHARSET == pDC->pFont->unCharSet )
		{
			wchar_t *wsText = new wchar_t[strlen(sText) + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[strlen(sText)] = '\0';
			int nLen = (int)strlen(sText);
			for ( int nIndex = 0; nIndex < nLen; nIndex++ )
			{
				unsigned char unChar = (unsigned char)(sText[nIndex]);
				if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
					wsText[nIndex] = (unsigned char)(sText[nIndex]);
				else
					wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP1250[unChar - MSCP_FIRST_CHAR] );
			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else if ( OEM_CHARSET == pDC->pFont->unCharSet )
		{
			int nLen = (int)strlen(sText);
			wchar_t *wsText = new wchar_t[nLen + 1];
			if ( NULL == wsText )
				return _T("");

			wsText[nLen] = '\0';

			// Пытаемся воспользоваться системной функцией
			if ( !OemToCharBuffW( sText, wsText, nLen ) )
			{
				// Системная функция почему-то не сработала. Конвертируем самостоятельно.

				// Запрашиваем кодировку
				UINT unCP = GetOEMCP();

				// Предполагаем, что кодировка CP866 (на самом деле, надо исходить из unCP)
				for ( int nIndex = 0; nIndex < nLen; nIndex++ )
				{
					unsigned char unChar = (unsigned char)(sText[nIndex]);
					if ( unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR )
						wsText[nIndex] = (unsigned char)(sText[nIndex]);
					else
						wsText[nIndex] = (wchar_t)( c_anUnicodeMapCP866[unChar - MSCP_FIRST_CHAR] );
				}
			}

			CStringW wsResult( wsText ); 
			delete []wsText;

			return wsResult;
		}
		else
		{
			// 
		}

		return _T("");
	}
	bool SetPen  (TWmfDC *pDC)
	{
		TWmfPen *pPen = pDC->pPen;

		long lColor = RGBA( pPen->oColor.r, pPen->oColor.g, pPen->oColor.b );
		double dWidth = pPen->dWidth * m_dScaleX ;

		if ( dWidth <= 0.01 )
			dWidth = 0;

		m_oPen.Size  = dWidth;
		m_oPen.Color = lColor;

		unsigned short ushPenType   = pPen->ushStyle & PS_TYPE_MASK;
		unsigned short ushPenEndCap = pPen->ushStyle & PS_ENDCAP_MASK;
		unsigned short ushPenJoin   = pPen->ushStyle & PS_JOIN_MASK;
		unsigned short ushPenStyle  = pPen->ushStyle & PS_STYLE_MASK;

		byte nCapStyle = 0;
		if ( 0 == ushPenEndCap )
			nCapStyle = 2;
		else if ( 1 == ushPenEndCap )
			nCapStyle = 1;
		else if ( 2 == ushPenEndCap )
			nCapStyle = 0;

		m_oPen.LineStartCap = nCapStyle;
		m_oPen.LineEndCap	= nCapStyle;

		byte nJoinStyle = 0;
		if ( 0 == ushPenJoin )
			nJoinStyle = 2;
		else if ( 1 == ushPenJoin )
			nJoinStyle = 1;
		else if ( 2 == ushPenJoin )
			nJoinStyle = 2;

		m_oPen.LineJoin = nJoinStyle;

		if ( PS_NULL != ushPenStyle )
			m_oPen.DashStyle = (byte)ushPenStyle;

		m_pRenderer->put_PenDashStyle( m_oPen.DashStyle );
		m_pRenderer->put_PenLineJoin( m_oPen.LineJoin );
		m_pRenderer->put_PenLineStartCap( m_oPen.LineStartCap );
		m_pRenderer->put_PenLineEndCap( m_oPen.LineEndCap );
		m_pRenderer->put_PenColor( m_oPen.Color );
		m_pRenderer->put_PenSize( m_oPen.Size );
		m_pRenderer->put_PenAlpha( 255 );

		// TO DO: С текущим интерфейсом AVSRenderer, остальные случаи ushROPMode
		//        реализовать невозможно. Потому что данный параметр нужно протаскивать
		//        как параметр Pen'a, и тот кто рисует сам должен разруливать все случаи.

		switch (pDC->ushROPMode)
		{
		case R2_BLACK:   m_pRenderer->put_PenColor( 0 ); break;
		case R2_NOP:     m_pRenderer->put_PenAlpha( 0 ); break;
		case R2_COPYPEN: break;
		case R2_WHITE:   m_pRenderer->put_PenColor( RGBA( 255, 255, 255 ) ); break;
		}

		if ( PS_NULL == ushPenStyle )
			return false;

		return true;
	}

	bool SetBrush(TWmfDC *pDC)
	{
		TWmfBrush *pBrush = pDC->pBrush;
		long lColor = RGBA( pBrush->oColor.r, pBrush->oColor.g, pBrush->oColor.b );
		
		if ( BS_NULL == pBrush->ushStyle )
			return false;
		else if ( BS_SOLID == pBrush->ushStyle )
		{
			m_oBrush.Color1 = lColor;
			m_oBrush.Alpha1 = 255;
			m_oBrush.Type	= c_BrushTypeSolid;

			m_pRenderer->put_BrushColor1( m_oBrush.Color1 );
			m_pRenderer->put_BrushAlpha1( m_oBrush.Alpha1 );
			m_pRenderer->put_BrushType( m_oBrush.Type );
		}
		else if ( BS_PATTERN == pBrush->ushStyle )
		{
			// TO DO: сделать этот вариант
		}
		else if ( BS_DIBPATTERN == pBrush->ushStyle )
		{
			if ( NULL == pBrush->oBitmap.pData || 0 == pBrush->oBitmap.ushHeight || 0 == pBrush->oBitmap.ushWidth )
				return false;

			// create result interface
			IUnknown *pInterface = NULL;
			HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
			if ( hRes != S_OK || NULL == pInterface )
				return false;

			MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

			int nWidth  = pBrush->oBitmap.ushWidth;
			int nHeight = pBrush->oBitmap.ushHeight;
			int nBufferSize = 4 * nWidth * nHeight;
			if ( nBufferSize < 1 )
				return false;

			pMediaData->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
			pMediaData->put_Width( nWidth );
			pMediaData->put_Height( nHeight );
			pMediaData->put_AspectRatioX( nWidth );
			pMediaData->put_AspectRatioY( nHeight );
			pMediaData->put_Interlaced( VARIANT_FALSE );
			pMediaData->put_Stride( 0, 4 * nWidth );
			pMediaData->AllocateBuffer( nBufferSize );

			unsigned char *pBufferPtr = NULL;
			long nCreatedBufferSize = 0;

			pMediaData->get_Buffer( &pBufferPtr );
			pMediaData->get_BufferSize( &nCreatedBufferSize );
			pMediaData->put_Plane( 0, pBufferPtr );

			if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
				return false;

			pInterface = pMediaData;

			// Пишем данные в pBufferPtr

			for ( int nIndex = 0; nIndex < 4 * nWidth * nHeight; nIndex += 4 ) 
			{
				pBufferPtr[0] = (unsigned char)pBrush->oBitmap.pData[nIndex + 0];
				pBufferPtr[1] = (unsigned char)pBrush->oBitmap.pData[nIndex + 1];
				pBufferPtr[2] = (unsigned char)pBrush->oBitmap.pData[nIndex + 2];
				pBufferPtr[3] = (unsigned char)pBrush->oBitmap.pData[nIndex + 3];
				pBufferPtr += 4;
			}

			// Пишем во временный файл
			ImageStudio::IImageTransforms *pTransform = NULL;
			CoCreateInstance( __uuidof( ImageStudio::ImageTransforms ), NULL, CLSCTX_INPROC_SERVER, __uuidof(ImageStudio::IImageTransforms), (void **)(&pTransform) );
			if ( !pTransform )
			{
				RELEASEINTERFACE( pInterface );
				return false;
			}

			FILE *pTempFile = NULL;
			CString wsTempFileName;
			if ( !WmfOpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), _T(".wmf0"), NULL ) ) 
			{
				RELEASEINTERFACE( pInterface );
				return false;
			}
			::fclose( pTempFile );

			VARIANT vSource;
			vSource.vt = VT_UNKNOWN;
			vSource.punkVal = (IUnknown *)(pInterface);
			pTransform->SetSource( 0, vSource );

			CString sXml("<transforms><ImageFile-SaveAsPng destinationpath=\"");
			sXml += wsTempFileName.GetBuffer();
			sXml += "\"></ImageFile-SaveAsPng></transforms>";

			BSTR bsXML = sXml.AllocSysString();

			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			pTransform->SetXml( bsXML, &vbSuccess );
			if ( VARIANT_TRUE != vbSuccess )
			{
				::SysFreeString( bsXML );
				RELEASEINTERFACE( pInterface );
				RELEASEINTERFACE( pTransform );
				_wunlink( wsTempFileName.GetBuffer() );
				return false;
			}

			pTransform->Transform( &vbSuccess );
			if ( VARIANT_TRUE != vbSuccess )
			{
				::SysFreeString( bsXML );
				RELEASEINTERFACE( pInterface );
				RELEASEINTERFACE( pTransform );
				_wunlink( wsTempFileName.GetBuffer() );
				return false;
			}

			::SysFreeString( bsXML );
			RELEASEINTERFACE( pInterface );
			RELEASEINTERFACE( pTransform );


			m_oBrush.TexturePath = wsTempFileName;
			m_oBrush.TextureMode = c_BrushTextureModeTile;
			m_oBrush.Type = c_BrushTypeTexture;

			m_pRenderer->put_BrushTextureMode( m_oBrush.Type );
			m_pRenderer->put_BrushType( m_oBrush.Type );
			BSTR bsTexture = m_oBrush.TexturePath.AllocSysString();
			m_pRenderer->put_BrushTexturePath(bsTexture);
			::SysFreeString(bsTexture);

			//m_oBrush.SetTexturePath( wsTempFileName );
			//m_oBrush.SetTextureMode( 1 );
			//m_oBrush.SetType( 8 ); // Текстурный

			//BSTR bsBrush = m_oBrush.ToXmlString().AllocSysString();
			//m_pRenderer->SetBrush( bsBrush );
			//::SysFreeString( bsBrush );

			// TO DO: Либо переделать этот вариант, либо разобраться что делать с темповым файлом в конце

			return true;
		}
		else if ( BS_HATCHED == pBrush->ushStyle )
		{
			/*m_oBrush.SetType( 9 + pBrush->ushHatch );

			m_oBrush.SetColor1( RGB( pBrush->oColor.r, pBrush->oColor.g, pBrush->oColor.b ) );
			m_oBrush.SetColor2( RGB( pDC->oBGColor.r,  pDC->oBGColor.g,  pDC->oBGColor.b  ) );
			m_oBrush.SetAlpha1( 255 );
			m_oBrush.SetAlpha2( 255 );

			if ( TRANSPARENT == pDC->ushBGMode )
				m_oBrush.SetAlpha2( 0 );
			else // OPAQUE
				m_oBrush.SetAlpha2( 255 );*/

			m_oBrush.Type = ( 9 + pBrush->ushHatch );

			m_oBrush.Color1 = ( RGB( pBrush->oColor.r, pBrush->oColor.g, pBrush->oColor.b ) );
			m_oBrush.Color2 = ( RGB( pDC->oBGColor.r,  pDC->oBGColor.g,  pDC->oBGColor.b  ) );
			m_oBrush.Alpha1 = ( 255 );
			m_oBrush.Alpha2 = ( 255 );

			if ( TRANSPARENT == pDC->ushBGMode )
				m_oBrush.Alpha2 = ( 0 );
			else // OPAQUE
				m_oBrush.Alpha2 = ( 255 );

			m_pRenderer->put_BrushColor1( m_oBrush.Color1 );
			m_pRenderer->put_BrushAlpha1( m_oBrush.Alpha1 );
			m_pRenderer->put_BrushColor2( m_oBrush.Color2 );
			m_pRenderer->put_BrushAlpha2( m_oBrush.Alpha2 );
			m_pRenderer->put_BrushType( m_oBrush.Type );
		}

		return true;
	}

	bool SetFont (TWmfDC *pDC, double dFontHeight)
	{
		TWmfFont *pFont = pDC->pFont;

		CString strName(pFont->sFaceName);
		BSTR bsName = strName.AllocSysString();
		m_pRenderer->put_FontName(bsName);
		SysFreeString(bsName);

		m_pRenderer->put_FontSize(dFontHeight);

		LONG lStyle = 0;
		if (pFont->ushWeight > 550)
			lStyle |= 0x01;
		if (pFont->unItalic)
			lStyle |= 0x02;
		if (pFont->unUnderline)
			lStyle |= (1 << 2);
		if (pFont->unStrikeOut)
			lStyle |= (1 << 7);
		

		m_pRenderer->put_FontStyle(lStyle);

		// Установим цвет текста и фона

		m_oBrush.Color1 = ( RGB( pDC->oTextColor.r, pDC->oTextColor.g, pDC->oTextColor.b ) );
		m_oBrush.Alpha1 = ( 255 );
		m_oBrush.Color2 = ( RGB( pDC->oBGColor.r, pDC->oBGColor.g, pDC->oBGColor.b ) );
		m_oBrush.Alpha2 = ( 255 );
		m_oBrush.Type = ( c_BrushTypeSolid );

		m_pRenderer->put_BrushType(m_oBrush.Type);
		m_pRenderer->put_BrushColor1(m_oBrush.Color1);
		m_pRenderer->put_BrushAlpha1(m_oBrush.Alpha1);


		return true;
	}
	unsigned int   ReadUInt  (BYTE *pBuffer)
	{
		return (unsigned int)( (pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | pBuffer[0] );
	}
	unsigned short ReadUShort(BYTE *pBuffer)
	{
		return (unsigned short)( (pBuffer[1] << 8) | pBuffer[0] );
	}
	int            ReadInt   (BYTE *pBuffer)
	{
		return (int)( (pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | pBuffer[0] );
	}
	BYTE           ReadByte  (BYTE *pBuffer)
	{
		return (BYTE)(pBuffer[0]);
	}

	bool           ReadImage_CoreHeader(TWmfBMPRead *pReadBitmap, BYTE *pBuffer, long lBufLen)
	{
		unsigned short ushWidth    = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;
		unsigned short ushHeight   = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;
		unsigned short ushPlanes   = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;
		unsigned short ushBitCount = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;

		if ( 0x0001 != ushPlanes )
			return false;


		return false;
	}
	bool           ReadImage_InfoHeader(TWmfBMPRead *pReadBitmap, BYTE *pBuffer, long lBufLen)
	{
		int nWidth  = ReadInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		int nHeight = ReadInt( pBuffer ); pBuffer += 4; lBufLen -= 4;

		unsigned short ushPlanes   = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;
		unsigned short ushBitCount = ReadUShort( pBuffer ); pBuffer += 2; lBufLen -= 2;

		unsigned int unCompression    = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		unsigned int unImageSize      = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		unsigned int unXPelsPerMeter  = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		unsigned int unYPelsPerMeter  = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		unsigned int unColorUsed      = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;
		unsigned int unColorImportant = ReadUInt( pBuffer ); pBuffer += 4; lBufLen -= 4;

		if ( 0x0001 != ushPlanes )
			return false;

		if ( nHeight < 0x00000000 && ( BI_RGB != unCompression ) )
			return false;

		if ( nWidth < 0 )
			return false;


		if ( BI_BITCOUNT_0 == ushBitCount ) // Значит компрессия либо PNG, либо JPEG
		{
			if ( BI_JPEG != unCompression || BI_PNG != unCompression )
				return false;

			// Читаем через ImageStudio

			USES_CONVERSION;
			CStringW wsTempFileName = _T("");
			FILE *pTempFile = NULL;
			if ( !WmfOpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), _T(".wmf0"), NULL ) ) 
				return false;
			::fwrite( pBuffer, 1, unImageSize, pTempFile );
			::fclose( pTempFile );

			ImageStudio::IImageTransforms *pTransform = NULL;
			CoCreateInstance( __uuidof( ImageStudio::ImageTransforms ), NULL, CLSCTX_INPROC_SERVER, __uuidof(ImageStudio::IImageTransforms), (void **)(&pTransform) );
			if ( !pTransform )
				return false;

			CString sXml("<transforms><ImageFile-LoadImage sourcepath=\"");
			sXml += wsTempFileName.GetBuffer();
			sXml += "\"></ImageFile-LoadImage>";

			BSTR bsXML = sXml.AllocSysString();

			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			pTransform->SetXml( bsXML, &vbSuccess );
			if ( VARIANT_TRUE != vbSuccess )
			{
				::SysFreeString( bsXML );
				RELEASEINTERFACE( pTransform );
				_wunlink( wsTempFileName.GetBuffer() );
				return false;
			}

			pTransform->Transform( &vbSuccess );
			if ( VARIANT_TRUE != vbSuccess )
			{
				::SysFreeString( bsXML );
				RELEASEINTERFACE( pTransform );
				_wunlink( wsTempFileName.GetBuffer() );
				return false;
			}

			// TO DO: Сделать чтение

			//VARIANT oVar;
			//pTransform->GetResult( 0, &oVar );

			::SysFreeString( bsXML );
			RELEASEINTERFACE( pTransform );
			::_wunlink( wsTempFileName.GetBuffer() );


			return false;
		}
		else if ( BI_BITCOUNT_1 == ushBitCount )
		{
			// Двуцветная картинка, значит палитра состоит из 2-х цветов
			TWmfRGB oColor1, oColor2;

			oColor1.r = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			oColor1.g = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			oColor1.b = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			pBuffer++; lBufLen--;

			oColor2.r = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			oColor2.g = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			oColor2.b = ReadByte( pBuffer ); pBuffer++; lBufLen--;
			pBuffer++; lBufLen--;

			// Считываем саму картинку
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if ( lCalcLen != lBufLen )
				return false;

			pReadBitmap->oBitmap.pData = (BYTE *)malloc( nWidth * nHeight * 4 * sizeof( BYTE ) );
			if ( NULL == pReadBitmap->oBitmap.pData )
				return false;

			pReadBitmap->oBitmap.ushHeight = (unsigned short)abs(nHeight);
			pReadBitmap->oBitmap.ushWidth  = (unsigned short)nWidth;

			int nWidthBytes = ( nWidth + 7 ) / 8;
			int nAdditBytes = 4 - div_t(div( ( ( nWidth + 7 ) / 8 ), 4 )).rem;
			if ( 4 == nAdditBytes )
				nAdditBytes = 0;

			int nLastBitCount = div_t(div( nWidth, 8 )).rem - 1;
			if ( -1 == nLastBitCount )
				nLastBitCount = 7;

			nLastBitCount = (int)pow( (double)2, (double)nLastBitCount );

			for ( int nY = 0, nIndex = 0; nY < abs(nHeight); nY++ )
			{
				for ( int nX = 0; nX < nWidthBytes  ; nX++ )
				{
					int nByte = ReadByte( pBuffer ); pBuffer++; lBufLen--;
					int nBitCount = 128;
					if ( nX == nWidthBytes - 1 )
						nBitCount = nLastBitCount;

					for ( int nBitIndex = nBitCount; nBitIndex > 0; nBitIndex /= 2 )
					{
						int nBit = (nByte & nBitIndex);
						TWmfRGB oColor = ( nBit ? oColor2 : oColor1 );
						pReadBitmap->oBitmap.pData[nIndex * 4 + 0] = oColor.b;
						pReadBitmap->oBitmap.pData[nIndex * 4 + 1] = oColor.g;
						pReadBitmap->oBitmap.pData[nIndex * 4 + 2] = oColor.r;
						pReadBitmap->oBitmap.pData[nIndex * 4 + 3] = 255;
						nIndex++;
					}
				}
				for ( int nAddIndex = 0; nAddIndex < nAdditBytes; nAddIndex++ )
				{
					int nByte = ReadByte( pBuffer ); pBuffer++; lBufLen--;
				}
			}

			return true;
		}
		else if ( BI_BITCOUNT_2 == ushBitCount )
		{
			// TO DO: Сделать данный вариант, как только будет файлы с данным типом

			int nTODO = 100;

			return false;
		}
		else if ( BI_BITCOUNT_3 == ushBitCount )
		{
			unsigned short ushColorTableLen = 256;
			
			if ( 0 != unColorUsed  )
				ushColorTableLen = min( 256, unColorUsed );

			TWmfRGB oColorTable[256];

			if ( lBufLen < ushColorTableLen * 4 ) 
				return false;

			// Считываем палитру
			for ( unsigned short ushIndex = 0; ushIndex < ushColorTableLen; ushIndex++ )
			{
				oColorTable[ushIndex].r = ReadByte( pBuffer ); pBuffer++; lBufLen--;
				oColorTable[ushIndex].g = ReadByte( pBuffer ); pBuffer++; lBufLen--;
				oColorTable[ushIndex].b = ReadByte( pBuffer ); pBuffer++; lBufLen--;
				pBuffer++; lBufLen--;
			}

			// 1 байт - 1 пиксел

			// Ширина должна быть кратна 4.
			int nAdd = 0;
			while ( 0 != div_t(div( nWidth + nAdd, 4 )).rem )
			{
				nAdd++;
			}

			if  ( lBufLen < nWidth * nHeight )
				return false;

			pReadBitmap->oBitmap.pData = (BYTE *)malloc( nWidth * nHeight * 4 * sizeof( BYTE ) );
			if ( NULL == pReadBitmap->oBitmap.pData )
				return false;

			pReadBitmap->oBitmap.ushHeight = (unsigned short)abs(nHeight);
			pReadBitmap->oBitmap.ushWidth  = (unsigned short)nWidth;


			if ( nHeight < 0 )
			{
				for ( int nY = 0; nY < abs(nHeight); nY++ )
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						int nIndex = 4 * (nWidth * nY + nX);
						
						BYTE nByte = ReadByte( pBuffer ); pBuffer++; lBufLen--;

						pReadBitmap->oBitmap.pData[nIndex + 0] = oColorTable[nByte].b;
						pReadBitmap->oBitmap.pData[nIndex + 1] = oColorTable[nByte].g;
						pReadBitmap->oBitmap.pData[nIndex + 2] = oColorTable[nByte].r;
						pReadBitmap->oBitmap.pData[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for ( int nY = abs(nHeight) - 1; nY >= 0; nY-- )
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						int nIndex = 4 * (nWidth * nY + nX);

						BYTE nByte = ReadByte( pBuffer ); pBuffer++; lBufLen--;

						pReadBitmap->oBitmap.pData[nIndex + 0] = oColorTable[nByte].b;
						pReadBitmap->oBitmap.pData[nIndex + 1] = oColorTable[nByte].g;
						pReadBitmap->oBitmap.pData[nIndex + 2] = oColorTable[nByte].r;
						pReadBitmap->oBitmap.pData[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}

			return true;
		}
		else if ( BI_BITCOUNT_4 == ushBitCount )
		{
			// TO DO: Сделать данный вариант, как только будет файлы с данным типом

			int nTODO = 100;

			return false;
		}
		else if ( BI_BITCOUNT_5 == ushBitCount )
		{
			// Пропускаем таблицу цветов (она не нужна)
			pBuffer += unColorUsed * 4; lBufLen -= unColorUsed * 4;

			if ( BI_RGB != unCompression )
				return false; // TO DO: Сделать данный вариант, как только будет файлы с данным типом

			// Считываем саму картинку
			long lCalcLen = (((nWidth * ushPlanes * ushBitCount + 31) & ~31) / 8) * abs(nHeight);
			if ( lCalcLen != lBufLen )
				return false;

			// 1 байт на каждый канал
			// (Ширина * 3) должна быть кратна 4.
			int nAdd = 0;
			while ( 0 != div_t(div( 3 * nWidth + nAdd, 4 )).rem )
			{
				nAdd++;
			}

			int nSize = nWidth * nHeight * 4;

			pReadBitmap->oBitmap.pData = (BYTE *)malloc( nWidth * nHeight * 4 * sizeof( BYTE ) );
			if ( NULL == pReadBitmap->oBitmap.pData )
				return false;

			pReadBitmap->oBitmap.ushHeight = (unsigned short)abs(nHeight);
			pReadBitmap->oBitmap.ushWidth  = (unsigned short)nWidth;

			if ( nHeight < 0 )
			{
				for ( int nY = 0; nY < abs(nHeight); nY++ )
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						int nIndex = 4 * (nWidth * nY + nX);

						pReadBitmap->oBitmap.pData[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			else
			{
				for ( int nY = abs(nHeight) - 1; nY >= 0; nY-- )
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						int nIndex = 4 * (nWidth * nY + nX);
						pReadBitmap->oBitmap.pData[nIndex + 0] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 1] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 2] = pBuffer[0]; pBuffer++; lBufLen--;
						pReadBitmap->oBitmap.pData[nIndex + 3] = 255;
					}
					pBuffer += nAdd; lBufLen -= nAdd;
				}
			}
			return true;
		}
		else if ( BI_BITCOUNT_6 == ushBitCount )
		{
			// TO DO: Сделать данный вариант, как только будет файлы с данным типом

			int nTODO = 100;

			return false;
		}


		return false;
	}
	
private:

	IASCRenderer *m_pRenderer;

	NSStructures::CPen      m_oPen;
	NSStructures::CBrush    m_oBrush;
	NSStructures::CShadow   m_oShadow;
	NSStructures::CEdgeText m_oEdgeText;
	NSStructures::CFont     m_oFont;


	double m_dDpiX;
	double m_dDpiY;

	TWmfRectF m_oWmfRect; // Общий рект, в котором рисуется вся картинка
	double m_dX;        // Координаты левого верхнего угла
	double m_dY;        //
	double m_dScaleX;   // Коэффициенты сжатия/растяжения, чтобы 
	double m_dScaleY;   // результирующая картинка была нужных размеров.
	double m_dX1;
	double m_dY1;

	CWmfFile *m_pWmfFile;
};

#endif /* _RENDERER_OUTPUT_H */