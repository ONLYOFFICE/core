#include "CASCImage.h"

#include "../DesktopEditor/raster/Metafile/MetaFile.h"
#include "../DesktopEditor/graphics/Image.h"

#if defined(_WIN32) || defined (_WIN64)
	
#else
	#include <unistd.h>
#endif

namespace NSHtmlRenderer
{
    CASCImage::CASCImage(CApplicationFonts *pAppFonts)
	{
        m_pMetafile		= new MetaFile::CMetaFile(pAppFonts);
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
		RELEASEOBJECT(m_pMetafile);
	}
	void CASCImage::Open(const std::wstring& bsFilePath)
	{
		// Закроем раннее открытый файл (если он был открыт)
		Close();

		if (m_pMetafile == NULL) return;

		// Сначала попытаемя открыть файл как WMF/EMF

		if ( m_pMetafile->LoadFromFile( bsFilePath.c_str() ) == true )
		{
			// Файл открылся нормально
			long MetaType = m_pMetafile->GetType();
			
			m_lImageType =  c_lImageTypeMetafile | MetaType;
			if (MetaType == c_lMetaWmf) return;
 
//#if !defined(_WIN32) && !defined (_WIN64)			// emf всеже под win лучше читать через gdi+
			if (MetaType == c_lMetaEmf) return;
//#endif
		}
		// Это не Wmf & Emf
		m_pMetafile->Close();

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
			m_pMetafile->Close();
			
			if ( m_lImageType & c_lMetaEmf )
			{
				// Удаляем временный файл
				//todo может проверять не пустой ли m_wsTempFilePath, а не m_lImageType & c_lMetaEmf
#if defined(_WIN32) || defined (_WIN64)
                ::_wunlink( m_wsTempFilePath.GetBuffer() );
#else
                std::string sTempFilePath = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(m_wsTempFilePath.c_str(), m_wsTempFilePath.length());
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
				double x=0, y=0, w=0, h=0;
				m_pMetafile->GetBounds(&x,&y,&w,&h);
				*lWidth = (LONG)w;
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
				double x=0, y=0, w=0, h=0;
				m_pMetafile->GetBounds(&x,&y,&w,&h);
				*lHeight = (LONG)h;
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
		else if ( (c_lImageTypeMetafile | c_lMetaWmf) == m_lImageType || 
					(c_lImageTypeMetafile | c_lMetaEmf) == m_lImageType )
		{
			m_pMetafile->DrawOnRenderer(pRenderer, dX, dY, dWidth, dHeight);
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
		return m_pMetafile->get_FontManager();
	}

	Aggplus::CImage* CASCImage::get_BitmapImage()
	{
		return m_pMediaData;
	}
}
