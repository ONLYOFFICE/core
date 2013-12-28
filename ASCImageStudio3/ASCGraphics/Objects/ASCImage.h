// CASCBrush.h : Declaration of the CAVSPen

#pragma once
#include "..\stdafx.h"
#include "..\resource.h"       // main symbols

#include "Image\Wmf\WmfFile.h"
#include "Image\Wmf\RendererOutput.h"

#include "..\Interfaces\IASCImage.h"
#include "..\Interfaces\ASCRenderer.h"

#include "GdiplusEx.h"
#include "MediaFormatDefine.h"

#include "Image\EmfFile.h"
#include "Image\SVG\SVGTransformer.h"

const long c_lImageTypeUnknown  = 0x0000;
const long c_lImageTypeMetafile = 0x1000;
const long c_lImageTypeBitmap   = 0x2000;

const long c_lMetaWmf = 0x01;
const long c_lMetaEmf = 0x02;
const long c_lMetaSVG = 0x04;

namespace NSGdiMeta
{
	static BOOL ByteArrayToMediaData(BYTE* pArray, int nWidth, int nHeight, MediaCore::IAVSUncompressedVideoFrame** pInterface, BOOL bFlipVertical = TRUE)
	{
		if (!pInterface || nWidth < 1 || nHeight < 1)
			return FALSE;

		*pInterface = NULL;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
		if (NULL == pMediaData)
			return FALSE;

		if (bFlipVertical)
			pMediaData->put_ColorSpace(CSP_BGRA | CSP_VFLIP);
		else
			pMediaData->put_ColorSpace(CSP_BGRA);

		// specify settings
		pMediaData->put_Width(nWidth);
		pMediaData->put_Height(nHeight);
		pMediaData->put_AspectRatioX(nWidth);
		pMediaData->put_AspectRatioY(nHeight);
		pMediaData->put_Interlaced(VARIANT_FALSE);
		pMediaData->put_Stride(0, 4*nWidth);
		pMediaData->AllocateBuffer(4*nWidth*nHeight);
		
		// allocate necesasry buffer
		BYTE* pBufferPtr = 0;
		long nCreatedBufferSize = 0;
		pMediaData->get_Buffer(&pBufferPtr);
		pMediaData->get_BufferSize(&nCreatedBufferSize);
		pMediaData->put_Plane(0, pBufferPtr);

		// check for valid created buffer
		if (!pBufferPtr || nCreatedBufferSize != 4*nWidth*nHeight)
		{
			RELEASEINTERFACE(pMediaData);
			return FALSE;
		}
		
		// copy safearray's data to the buffer
		if (pArray != NULL)
			memcpy(pBufferPtr, pArray, nCreatedBufferSize);

		// save interface
		*pInterface = pMediaData;
		return TRUE;
	}
		
	static BOOL GdiPlusBitmapToMediaData(Gdiplus::Bitmap* pBitmap, MediaCore::IAVSUncompressedVideoFrame** pInterface, BOOL bFlipVertical = TRUE)
	{
		if (!pBitmap)
			return FALSE;

		int nWidth = pBitmap->GetWidth();
		int nHeight = pBitmap->GetHeight();

		double dHorDpi = (double)pBitmap->GetHorizontalResolution();
		double dVerDpi = (double)pBitmap->GetVerticalResolution();

		int nWidthDest	= (int)(96.0 * nWidth / dHorDpi);
		int nHeightDest = (int)(96.0 * nHeight / dVerDpi);

		if (nWidth == nWidthDest && nHeight == nHeightDest)
		{
			Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
			Gdiplus::BitmapData oBitmapData;

			if (pBitmap->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData) != Ok)
				return FALSE;

			BOOL bSuccess = ByteArrayToMediaData((BYTE*)oBitmapData.Scan0, nWidth, nHeight, pInterface, bFlipVertical);
			
			pBitmap->UnlockBits(&oBitmapData);

			return bSuccess;
		}

		BYTE* pData = new BYTE[4 * nWidthDest * nHeightDest];
		Gdiplus::Bitmap oBitmap(nWidthDest, nHeightDest, 4 * nWidthDest, PixelFormat32bppARGB, pData);

		Graphics* pGraphics = Graphics::FromImage(&oBitmap);

		pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
		pGraphics->DrawImage(pBitmap, 0, 0, nWidthDest, nHeightDest);

		RELEASEOBJECT(pGraphics);

		BOOL bSuccess = ByteArrayToMediaData(pData, nWidthDest, nHeightDest, pInterface, bFlipVertical);
		RELEASEARRAYOBJECTS(pData);

		return bSuccess;
	}
}

// CAVSImage
[coclass, uuid("196FFB80-B601-453d-B757-00EFDC60676C"), threading(apartment), vi_progid("AVSGraphics.Pen"), progid("AVSGraphics.Pen"), version(1.0), support_error_info(IAVSImage), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSImage
	:	public IAVSImage
{
protected:

private:

	CWmfFile			m_oWmfFile;
	CEMFFile			m_oEmfFile;
	ISVGTransformer*	m_pSVGFile;

	TWmfRectF m_oRect;

	CStringW  m_wsTempFilePath;

	long      m_lImageType; // 0 - BMP, 1 - WMF, 2 - EFM

	double   m_dDpiX;
	double   m_dDpiY;

	IUnknown* m_pMediaData;

	BOOL      m_bLoadOnlyMeta; // Загружаем только метафайл

	CGdiPlusInit			m_oGdiPlusInit;	

public:

	CAVSImage()
	{
		m_pMediaData	= NULL;
		m_pSVGFile		= NULL;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_bLoadOnlyMeta = FALSE;

		m_lImageType = c_lImageTypeUnknown;

		m_dDpiX = 72;
		m_dDpiY = 72;

		m_wsTempFilePath.Empty();

		m_pMediaData	= NULL;
		m_pSVGFile		= NULL;

		return S_OK;
	}

	void FinalRelease()
	{
		Close();
	}


private:

	void Open(BSTR bsFilePath)
	{
		if (!m_oGdiPlusInit.IsValid())
		{
			m_oGdiPlusInit.Init();				
		}

		// Закроем раннее открытый файл (если он был открыт)
		Close();

		// Сначала попытаемя открыть файл как WMF
		m_oWmfFile.OpenFromFile( bsFilePath );
		m_oWmfFile.Scan( &m_oRect );

		// Файл открылся нормально
		if ( !m_oWmfFile.CheckError() )
		{
			m_lImageType = c_lImageTypeMetafile | c_lMetaWmf;
			return;
		}

		// Это не Wmf, попробуем открыть его как Emf
		m_oWmfFile.Close();

		Gdiplus::Metafile *pMetaFile = new Gdiplus::Metafile( bsFilePath );
		if ( NULL != pMetaFile )
		{
			// Похоже это метафайл, конвертируем его в Wmf с помощью Gdi
			HENHMETAFILE hEmf = pMetaFile->GetHENHMETAFILE();
			if ( NULL != hEmf )
			{
				UINT unSize = Gdiplus::Metafile::EmfToWmfBits( hEmf, 0, NULL, MM_ANISOTROPIC, Gdiplus::EmfToWmfBitsFlagsIncludePlaceable);

				if (0 != unSize)
				{
					BYTE *pBuffer = new BYTE[unSize];
					INT nConvertedSize = Gdiplus::Metafile::EmfToWmfBits( hEmf, unSize, pBuffer, MM_ANISOTROPIC, Gdiplus::EmfToWmfBitsFlagsEmbedEmf );

					HMETAFILE hWmf = SetMetaFileBitsEx( unSize, pBuffer );

					FILE *pFile = NULL;
					WmfOpenTempFile( &m_wsTempFilePath, &pFile, _T("wb+"), _T(".wmf"), NULL );
					if ( !pFile )
					{
						DeleteMetaFile( hWmf );
						DeleteEnhMetaFile( hEmf );
						delete[] pBuffer;
						delete pMetaFile;

						m_lImageType = c_lImageTypeUnknown;
						return;
					}

					::fclose( pFile );

					// Сохраняем Wmf
					HMETAFILE hTempWmf = CopyMetaFile( hWmf, m_wsTempFilePath.GetBuffer() );
					DeleteMetaFile( hTempWmf );

					// Открываем Wmf
					m_oWmfFile.OpenFromFile( m_wsTempFilePath.GetBuffer() );
					m_oWmfFile.Scan( &m_oRect );

					if ( !m_oWmfFile.CheckError() )
					{
						// Wmf нормально открылся
						m_lImageType = c_lImageTypeMetafile | c_lMetaEmf;

						DeleteMetaFile( hWmf );
						DeleteEnhMetaFile( hEmf );
						delete[] pBuffer;
						delete pMetaFile;

						return;			
					}
					else if ( m_oWmfFile.UnSupportedWmf() )
					{
						// Исходный файл Emf, но после конвертации в Wmf он не открылся
						m_lImageType = c_lImageTypeMetafile | c_lMetaEmf;
					}
					else
					{
						// Сконвертированный файл не прочитался
						m_oWmfFile.Close();
						m_lImageType = c_lImageTypeUnknown;
					}

					DeleteMetaFile( hWmf );
					DeleteEnhMetaFile( hEmf );
					delete[] pBuffer;
					delete pMetaFile;
				}
				else
				{
					DeleteEnhMetaFile( hEmf );
					delete pMetaFile;

					Gdiplus::Bitmap* pBitmap = new Gdiplus::Bitmap(bsFilePath);

					MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
					BOOL bIsSuccess = NSGdiMeta::GdiPlusBitmapToMediaData(pBitmap, &pFrame, FALSE);

					RELEASEOBJECT(pBitmap);

					if (bIsSuccess)
					{
						m_lImageType = c_lImageTypeMetafile | c_lMetaWmf;
						pFrame->QueryInterface(IID_IUnknown, (void**)&m_pMediaData);

						RELEASEINTERFACE(pFrame);
						return;
					}
					RELEASEINTERFACE(pFrame);
				}
			}
		}


		if ( (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
		{
			m_oEmfFile.Open(bsFilePath); 
		}
		else // if ( c_lImageTypeUnknown == m_lImageType )
		{
			// попробуем зачитать SVG
			CoCreateInstance( __uuidof(SVGTransformer), NULL, CLSCTX_ALL, __uuidof(ISVGTransformer), (void**)(&m_pSVGFile) );
			if (NULL != m_pSVGFile)
			{
				if ( AVS_ERROR_FILEFORMAT == m_pSVGFile->LoadFile( bsFilePath ) )
				{
					RELEASEINTERFACE( m_pSVGFile );
				}
				else
				{
					m_lImageType = c_lImageTypeMetafile | c_lMetaSVG;
					return;
				}
			}

			if ( m_bLoadOnlyMeta )
				return;
			
			// Исходный файл не явлется метафайлом (Wmf/Emf)
			// TODO: Сделать чтение Bitmap
			m_pMediaData = ImageStudio::ISLoadImage(bsFilePath);
			m_lImageType = c_lImageTypeBitmap;
		}

		return;
	}


	void Close()
	{
		if ( m_lImageType & c_lImageTypeMetafile )
		{
			m_oWmfFile.Close();

			if ( m_lImageType & c_lMetaEmf )
			{
				// Удаляем временный файл
				::_wunlink( m_wsTempFilePath.GetBuffer() );
			}
		}

		RELEASEINTERFACE(m_pSVGFile);

		m_lImageType = c_lImageTypeUnknown;
		m_wsTempFilePath = _T("");

		RELEASEINTERFACE(m_pMediaData);
	}
public:

//-------------------------------------------------------------------------------------------
//   IAVSImage
//-------------------------------------------------------------------------------------------
	STDMETHOD(get_Type)(LONG* lType)
	{
		if (NULL != lType)
			*lType = m_lImageType;

		return S_OK;
	}
	STDMETHOD(get_Width)(LONG* lWidth)
	{
		if ( NULL != lWidth )
		{
			if (NULL != m_pMediaData)
			{
				*lWidth = 0;
				MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
				m_pMediaData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
				if (NULL != pFrame)
					pFrame->get_Width(lWidth);

				RELEASEINTERFACE(pFrame);
			}
			else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
			{
				float fWidth, fHeight;
				m_oWmfFile.GetSize( &fWidth, &fHeight );
				*lWidth = (LONG)fWidth;
			}
			else if (((c_lImageTypeMetafile | c_lMetaSVG) == m_lImageType) && (NULL != m_pSVGFile))
			{
				m_pSVGFile->get_Width(lWidth);
			}
			else
			{
				*lWidth = 0;
			}
		}

		return S_OK;
	}
	STDMETHOD(put_Width)(LONG lWidth)
	{
		return S_OK;
	}
	STDMETHOD(get_Height)(LONG* lHeight)
	{
		if ( NULL != lHeight )
		{
			if (NULL != m_pMediaData)
			{
				*lHeight = 0;
				MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
				m_pMediaData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
				if (NULL != pFrame)
					pFrame->get_Height(lHeight);

				RELEASEINTERFACE(pFrame);
			}
			else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
			{
				float fWidth, fHeight;
				m_oWmfFile.GetSize( &fWidth, &fHeight );
				*lHeight = (LONG)fHeight;
			}
			else if (((c_lImageTypeMetafile | c_lMetaSVG) == m_lImageType) && (NULL != m_pSVGFile))
			{
				m_pSVGFile->get_Height(lHeight);
			}
			else
			{
				*lHeight = 0;
			}
		}

		return S_OK;
	}
	STDMETHOD(put_Height)(LONG lHeight)
	{
		return S_OK;
	}
	STDMETHOD(get_DpiX)(double* dDpiX)
	{
		*dDpiX = m_dDpiX;
		return S_OK;
	}
	STDMETHOD(put_DpiX)(double dDpiX)
	{
		m_dDpiX = dDpiX;
		return S_OK;
	}
	STDMETHOD(get_DpiY)(double* dDpiY)
	{
		*dDpiY = m_dDpiY;
		return S_OK;
	}
	STDMETHOD(put_DpiY)(double dDpiY)
	{
		m_dDpiY = dDpiY;
		return S_OK;
	}
	STDMETHOD(Draw)(IASCGraphicsBase* pGraphics)
	{
		return S_OK;
	}
	STDMETHOD(LoadFromFile)(BSTR bsFilePath)
	{
		// Внутри комманды Open выполняется команда Close
		Open( bsFilePath );

		return S_OK;
	}

	STDMETHOD(DrawOnRenderer)(IASCRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		if (NULL == pRenderer)
			return S_FALSE;

		pRenderer->BeginCommand(c_nImageType);

		if ( NULL != m_pMediaData )
		{
			pRenderer->DrawImage(m_pMediaData, dX, dY, dWidth, dHeight);
		}
		else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
		{
			pRenderer->get_DpiX(&m_dDpiX);
			pRenderer->get_DpiY(&m_dDpiY);

			CRendererOutput oWmfOut( &m_oWmfFile, pRenderer, dX, dY, dWidth, dHeight );

			double fSrcWidth, fSrcHeight;

			float fW, fH;
			m_oWmfFile.GetSize( &fW, &fH );
			m_oWmfFile.GetDisplaySize( &fSrcWidth, &fSrcHeight,m_dDpiX, m_dDpiY );

			//m_oWmfFile.GetDisplaySize( &fSrcWidth, &fSrcHeight, 25.4, 25.4 );
			TWmfRectF oRectB = m_oWmfFile.GetBounds(  );

			//double dW = m_oRect.oBR.fX - m_oRect.oTL.fX;
			//double dH = m_oRect.oBR.fY - m_oRect.oTL.fY;
			double dW = oRectB.oBR.fX - oRectB.oTL.fX;
			double dH = oRectB.oBR.fY - oRectB.oTL.fY;

			double dScaleX = dWidth  / dW;//fSrcWidth;
			double dScaleY = dHeight / dH;//fSrcHeight;
			//double dScaleX = dWidth  / fSrcWidth;
			//double dScaleY = dHeight / fSrcHeight;

			double dSrcDpiX, dSrcDpiY;
			m_oWmfFile.GetDpi( &dSrcDpiX, &dSrcDpiY );

			double dDpiKoefX = m_dDpiX / dSrcDpiX;
			double dDpiKoefY = m_dDpiY / dSrcDpiY;

			double dDpi = dSrcDpiY * fSrcHeight / fH;
			oWmfOut.SetDpi( m_dDpiX, dDpi );
			oWmfOut.SetWmfRect( oRectB );
			oWmfOut.SetScales( dScaleX, dScaleY );

			m_oWmfFile.SetOutputDevice( &oWmfOut );

			TWmfRectF oRect;
			m_oWmfFile.Play( &oRect );

		}
		else if ( (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
		{
			m_oEmfFile.DrawOnRenderer(pRenderer, dX, dY, dWidth, dHeight);
		}
		else if ( (c_lImageTypeMetafile | c_lMetaSVG) == m_lImageType )
		{
			if (NULL != m_pSVGFile)
			{
				m_pSVGFile->Draw(pRenderer, dX, dY, dWidth, dHeight);
			}
		}

		pRenderer->EndCommand(c_nImageType);
		return S_OK;
	}

//-------------------------------------------------------------------------------------------
//   IASCGraphicsBase
//-------------------------------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName = ParamName;
		if ( _T("LoadOnlyMeta") == sParamName && VT_BOOL == ParamValue.vt )
		{
			m_bLoadOnlyMeta = ( ParamValue.boolVal == -1 ? TRUE : FALSE );
		}
		if ( _T("LoadSVG") == sParamName && VT_BSTR == ParamValue.vt )
		{
			Close();
			CoCreateInstance( __uuidof(SVGTransformer), NULL, CLSCTX_ALL, __uuidof(ISVGTransformer), (void**)(&m_pSVGFile) );
			if (NULL != m_pSVGFile)
			{
				if ( AVS_ERROR_FILEFORMAT == m_pSVGFile->Load( ParamValue.bstrVal ) )
				{
					RELEASEINTERFACE( m_pSVGFile );
					return S_FALSE;
				}
				else
				{
					m_lImageType = c_lImageTypeMetafile | c_lMetaSVG;
				}
			}
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(CreateDublicate)(IASCGraphicsBase** ppGraphicsBase)
	{
		return S_OK;
	}

	STDMETHOD(get_FontManager)(IASCFontManager** ppManager)
	{
		if (NULL == ppManager)
			return S_FALSE;

		*ppManager = m_oWmfFile.GetFontManager();
		ADDREFINTERFACE((*ppManager));
		return S_OK;
	}
	STDMETHOD(put_FontManager)(IASCFontManager* pManager)
	{
		m_oWmfFile.SetFontManager(pManager);
		m_oEmfFile.SetFontManager(pManager);
		return S_OK;
	}
};