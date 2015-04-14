#include "CASCImage.h"

#include "../ASCImageStudio3/ASCGraphics/Objects/Image/Wmf/WmfFile.h"
#include "../ASCImageStudio3/ASCGraphics/Objects/Image/Wmf/RendererOutput.h"
#include "../DesktopEditor/graphics/Image.h"

#if defined(_WIN32) || defined (_WIN64)
#include "../Common/GdiPlusEx.h"
#import "../Redist/ASCMediaCore3.dll"			named_guids raw_interfaces_only rename_namespace("MediaCore"), exclude("tagRECT")
#include "../Common/MediaFormatDefine.h"
namespace NSGdiMeta
{
	static BOOL ByteArrayToMediaData(BYTE* pArray, int nWidth, int nHeight, Aggplus::CImage** ppImage, BOOL bFlipVertical = TRUE)
	{
		if (!ppImage || nWidth < 1 || nHeight < 1)
			return FALSE;

		*ppImage = new Aggplus::CImage;
		Aggplus::CImage* pImage = *ppImage;

		// specify settings
		long nBufferSize = 4 * nWidth * nHeight;
		long nStride;
		if(TRUE == bFlipVertical)
			nStride = -4 * nWidth;
		else
			nStride = 4 * nWidth;
		BYTE* pBuffer = new BYTE[nBufferSize];
		memcpy(pBuffer, pArray, nBufferSize);
		pImage->Create(pBuffer, nWidth, nHeight, nStride);

		return TRUE;
	}
		
	static BOOL GdiPlusBitmapToMediaData(Gdiplus::Bitmap* pBitmap, Aggplus::CImage** ppImage, BOOL bFlipVertical = TRUE)
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

			if (pBitmap->LockBits(&oRect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData) != Gdiplus::Ok)
				return FALSE;

			BOOL bSuccess = ByteArrayToMediaData((BYTE*)oBitmapData.Scan0, nWidth, nHeight, ppImage, bFlipVertical);
			
			pBitmap->UnlockBits(&oBitmapData);

			return bSuccess;
		}

		BYTE* pData = new BYTE[4 * nWidthDest * nHeightDest];
		Gdiplus::Bitmap oBitmap(nWidthDest, nHeightDest, 4 * nWidthDest, PixelFormat32bppARGB, pData);

		Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromImage(&oBitmap);

		pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
		pGraphics->DrawImage(pBitmap, 0, 0, nWidthDest, nHeightDest);

		RELEASEOBJECT(pGraphics);

		BOOL bSuccess = ByteArrayToMediaData(pData, nWidthDest, nHeightDest, ppImage, bFlipVertical);
		RELEASEARRAYOBJECTS(pData);

		return bSuccess;
	}
}
#else
#include <unistd.h>
#endif

namespace NSHtmlRenderer
{
	CASCImage::CASCImage()
	{
		m_pWmfFile = new CWmfFile();
		m_pRect = new TWmfRectF();
		m_pMediaData	= NULL;

        m_bLoadOnlyMeta = false;

		m_lImageType = c_lImageTypeUnknown;

		m_dDpiX = 72;
		m_dDpiY = 72;

		m_wsTempFilePath.Empty();
	}
	CASCImage::~CASCImage()
	{
		Close();
		RELEASEOBJECT(m_pWmfFile);
		RELEASEOBJECT(m_pRect);
	}
	void CASCImage::Open(const std::wstring& bsFilePath)
	{
		// Закроем раннее открытый файл (если он был открыт)
		Close();

		// Сначала попытаемя открыть файл как WMF
		m_pWmfFile->OpenFromFile( bsFilePath.c_str() );
		m_pWmfFile->Scan( m_pRect );

		// Файл открылся нормально
		if ( !m_pWmfFile->CheckError() )
		{
			m_lImageType = c_lImageTypeMetafile | c_lMetaWmf;
			return;
		}

		// Это не Wmf, попробуем открыть его как Emf
		m_pWmfFile->Close();
#if defined(_WIN32) || defined (_WIN64)
		CGdiPlusInit oCGdiPlusInit;
		oCGdiPlusInit.Init();
		Gdiplus::Metafile *pMetaFile = new Gdiplus::Metafile( bsFilePath.c_str() );
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
					}
					else
					{
						::fclose( pFile );

						// Сохраняем Wmf
						HMETAFILE hTempWmf = CopyMetaFile( hWmf, m_wsTempFilePath.GetBuffer() );
						DeleteMetaFile( hTempWmf );

						// Открываем Wmf
						m_pWmfFile->OpenFromFile( m_wsTempFilePath.GetBuffer() );
						m_pWmfFile->Scan( m_pRect );

						if ( !m_pWmfFile->CheckError() )
						{
							// Wmf нормально открылся
							m_lImageType = c_lImageTypeMetafile | c_lMetaWmf;

							DeleteMetaFile( hWmf );
							DeleteEnhMetaFile( hEmf );
							delete[] pBuffer;
							delete pMetaFile;

							return;			
						}
						else if ( m_pWmfFile->UnSupportedWmf() )
						{
							// Исходный файл Emf, но после конвертации в Wmf он не открылся
							m_lImageType = c_lImageTypeMetafile | c_lMetaEmf;
						}
						else
						{
							// Сконвертированный файл не прочитался
							m_pWmfFile->Close();
							m_lImageType = c_lImageTypeUnknown;
						}

						DeleteMetaFile( hWmf );
						DeleteEnhMetaFile( hEmf );
						delete[] pBuffer;
						delete pMetaFile;
					}
				}
				else
				{
					DeleteEnhMetaFile( hEmf );
					delete pMetaFile;
				}
				if (TRUE)
				{
					Gdiplus::Bitmap* pBitmap = new Gdiplus::Bitmap(bsFilePath.c_str());

					Aggplus::CImage* pImage = NULL;
					BOOL bIsSuccess = NSGdiMeta::GdiPlusBitmapToMediaData(pBitmap, &pImage, FALSE);
					RELEASEOBJECT(pBitmap);

					if (bIsSuccess)
					{
						m_lImageType = c_lImageTypeBitmap;
						m_pMediaData = pImage;
						return;
					}
					RELEASEOBJECT(pImage);
					return;
				}
			}
		}
#endif
		//todo
		{
			m_pMediaData = new Aggplus::CImage(bsFilePath);
            if(Aggplus::Ok == m_pMediaData->GetLastStatus())
				m_lImageType = c_lImageTypeBitmap;
			else
				m_pMediaData = NULL;
		}
		return;
	}


	void CASCImage::Close()
	{
		if ( m_lImageType & c_lImageTypeMetafile )
		{
			m_pWmfFile->Close();
			if ( m_lImageType & c_lMetaEmf )
			{
				// Удаляем временный файл
				//todo может проверять не пустой ли m_wsTempFilePath, а не m_lImageType & c_lMetaEmf
#if defined(_WIN32) || defined (_WIN64)
                ::_wunlink( m_wsTempFilePath.GetBuffer() );
#else
                std::string sTempFilePath = U_TO_UTF8(m_wsTempFilePath);
                ::unlink( sTempFilePath.c_str() );
#endif

			}
		}

		m_lImageType = c_lImageTypeUnknown;
		m_wsTempFilePath = _T("");

		RELEASEOBJECT(m_pMediaData);
	}
	HRESULT CASCImage::get_Type(LONG* lType)
	{
		if (NULL != lType)
			*lType = m_lImageType;

		return S_OK;
	}
	HRESULT CASCImage::get_Width(LONG* lWidth)
	{
		if ( NULL != lWidth )
		{
			if (NULL != m_pMediaData)
			{
				*lWidth = m_pMediaData->GetWidth();
			}
			else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
			{
				float fWidth, fHeight;
				m_pWmfFile->GetSize( &fWidth, &fHeight );
				*lWidth = (LONG)fWidth;
			}
			else
			{
				*lWidth = 0;
			}
		}

		return S_OK;
	}
	HRESULT CASCImage::put_Width(LONG lWidth)
	{
		return S_OK;
	}
	HRESULT CASCImage::get_Height(LONG* lHeight)
	{
		if ( NULL != lHeight )
		{
			if (NULL != m_pMediaData)
			{
				*lHeight = m_pMediaData->GetHeight();
			}
			else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || (c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
			{
				float fWidth, fHeight;
				m_pWmfFile->GetSize( &fWidth, &fHeight );
				*lHeight = (LONG)fHeight;
			}
			else
			{
				*lHeight = 0;
			}
		}

		return S_OK;
	}
	HRESULT CASCImage::put_Height(LONG lHeight)
	{
		return S_OK;
	}
	HRESULT CASCImage::get_DpiX(double* dDpiX)
	{
		*dDpiX = m_dDpiX;
		return S_OK;
	}
	HRESULT CASCImage::put_DpiX(double dDpiX)
	{
		m_dDpiX = dDpiX;
		return S_OK;
	}
	HRESULT CASCImage::get_DpiY(double* dDpiY)
	{
		*dDpiY = m_dDpiY;
		return S_OK;
	}
	HRESULT CASCImage::put_DpiY(double dDpiY)
	{
		m_dDpiY = dDpiY;
		return S_OK;
	}
	HRESULT CASCImage::LoadFromFile(const std::wstring& bsFilePath)
	{
		// Внутри комманды Open выполняется команда Close
		Open( bsFilePath );

		return S_OK;
	}
	HRESULT CASCImage::DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
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

			CRendererOutput oWmfOut( m_pWmfFile, pRenderer, dX, dY, dWidth, dHeight );

			double fSrcWidth, fSrcHeight;

			float fW, fH;
			m_pWmfFile->GetSize( &fW, &fH );
			m_pWmfFile->GetDisplaySize( &fSrcWidth, &fSrcHeight,m_dDpiX, m_dDpiY );

			//m_pWmfFile->GetDisplaySize( &fSrcWidth, &fSrcHeight, 25.4, 25.4 );
			TWmfRectF oRectB = m_pWmfFile->GetBounds(  );

			//double dW = m_oRect.oBR.fX - m_oRect.oTL.fX;
			//double dH = m_oRect.oBR.fY - m_oRect.oTL.fY;
			double dW = oRectB.oBR.fX - oRectB.oTL.fX;
			double dH = oRectB.oBR.fY - oRectB.oTL.fY;

			double dScaleX = dWidth  / dW;//fSrcWidth;
			double dScaleY = dHeight / dH;//fSrcHeight;
			//double dScaleX = dWidth  / fSrcWidth;
			//double dScaleY = dHeight / fSrcHeight;

			double dSrcDpiX, dSrcDpiY;
			m_pWmfFile->GetDpi( &dSrcDpiX, &dSrcDpiY );

			double dDpiKoefX = m_dDpiX / dSrcDpiX;
			double dDpiKoefY = m_dDpiY / dSrcDpiY;

			double dDpi = dSrcDpiY * fSrcHeight / fH;
			oWmfOut.SetDpi( m_dDpiX, dDpi );
			oWmfOut.SetWmfRect( oRectB );
			oWmfOut.SetScales( dScaleX, dScaleY );

			m_pWmfFile->SetOutputDevice( &oWmfOut );

			TWmfRectF oRect;
			m_pWmfFile->Play( &oRect );

		}

		pRenderer->EndCommand(c_nImageType);
		return S_OK;
	}

	//-------------------------------------------------------------------------------------------
	//   IASCGraphicsBase
	//-------------------------------------------------------------------------------------------
	HRESULT CASCImage::LoadOnlyMeta(bool bVal)
	{
		m_bLoadOnlyMeta = bVal;
		return S_OK;
	}
	HRESULT CASCImage::LoadSVG(const std::wstring& sVal)
	{
		//Close();
		//CoCreateInstance( __uuidof(SVGTransformer), NULL, CLSCTX_ALL, __uuidof(ISVGTransformer), (void**)(&m_pSVGFile) );
		//if (NULL != m_pSVGFile)
		//{
		//	if ( AVS_ERROR_FILEFORMAT == m_pSVGFile->Load( ParamValue.bstrVal ) )
		//	{
		//		RELEASEINTERFACE( m_pSVGFile );
		//		return S_FALSE;
		//	}
		//	else
		//	{
		//		m_lImageType = c_lImageTypeMetafile | c_lMetaSVG;
		//	}
		//}
		return S_OK;
	}
	CFontManager* CASCImage::get_FontManager()
	{
		return m_pWmfFile->GetFontManager();
	}
	void CASCImage::put_FontManager(CFontManager* pManager)
	{
		m_pWmfFile->SetFontManager(pManager);
	}

	Aggplus::CImage* CASCImage::get_BitmapImage()
	{
		return m_pMediaData;
	}
}
