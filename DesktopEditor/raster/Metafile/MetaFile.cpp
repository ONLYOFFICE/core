#include "MetaFile.h"

#include "../../graphics/GraphicsRenderer.h"
#include "../../raster/BgraFrame.h"

#include "Emf/RendererOutput.h"
#ifndef NEW_WMF
#include "Wmf/RendererOutput.h"
#endif

namespace MetaFile
{
	CMetaFile::CMetaFile(CApplicationFonts *pAppFonts)
	{
		m_pAppFonts = pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем
		m_pFontManager = pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
		pMeasurerCache->SetStreams(pAppFonts->GetStreams());
		m_pFontManager->SetOwnerCache(pMeasurerCache);
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);

		m_lType  = 0;
	}
	CMetaFile::~CMetaFile()
	{
		Close();
		RELEASEINTERFACE(m_pFontManager);
	}
	CFontManager* CMetaFile::get_FontManager()
	{
		return m_pFontManager;
	}
	bool CMetaFile::LoadFromFile(const wchar_t *wsFilePath)
	{
		// Сначала пытаемся открыть файл как Wmf
		m_oWmfFile.OpenFromFile(wsFilePath);

#ifdef NEW_WMF
		m_oWmfFile.Scan();
#else
		m_oWmfFile.Scan(&m_oWmfRect);
#endif

		if (!m_oWmfFile.CheckError())
		{
			m_lType = c_lMetaWmf;
			return true;
		}

		// Это не Wmf, попробуем открыть его как Emf
		m_oWmfFile.Close();

		m_oEmfFile.OpenFromFile(wsFilePath);
		m_oEmfFile.Scan();

		if (!m_oEmfFile.CheckError())
		{
			m_lType = c_lMetaEmf;
			return true;
		}

		// Это не Emf
		m_oEmfFile.Close();

		return false;
	};
	bool CMetaFile::DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		if (NULL == pRenderer)
			return false;

		pRenderer->BeginCommand(c_nImageType);

		if (c_lMetaWmf == m_lType)
		{
#ifdef NEW_WMF
#else
			double dRendererDpix, dRendererDpiY;
			pRenderer->get_DpiX(&dRendererDpix);
			pRenderer->get_DpiY(&dRendererDpiY);

			CRendererOutput oWmfOut(&m_oWmfFile, pRenderer, dX, dY, dWidth, dHeight);

			double fSrcWidth, fSrcHeight;

			float fW, fH;
			m_oWmfFile.GetSize(&fW, &fH);
			m_oWmfFile.GetDisplaySize(&fSrcWidth, &fSrcHeight, dRendererDpix, dRendererDpiY);

			//m_oWmfFile.GetDisplaySize( &fSrcWidth, &fSrcHeight, 25.4, 25.4 );
			TWmfRectF oRectB = m_oWmfFile.GetBounds();

			//double dW = m_oRect.oBR.fX - m_oRect.oTL.fX;
			//double dH = m_oRect.oBR.fY - m_oRect.oTL.fY;
			double dW = oRectB.oBR.fX - oRectB.oTL.fX;
			double dH = oRectB.oBR.fY - oRectB.oTL.fY;

			double dScaleX = dWidth / dW;//fSrcWidth;
			double dScaleY = dHeight / dH;//fSrcHeight;
			//double dScaleX = dWidth  / fSrcWidth;
			//double dScaleY = dHeight / fSrcHeight;

			double dSrcDpiX, dSrcDpiY;
			m_oWmfFile.GetDpi(&dSrcDpiX, &dSrcDpiY);

			double dDpiKoefX = dRendererDpix / dSrcDpiX;
			double dDpiKoefY = dRendererDpiY / dSrcDpiY;

			double dDpi = dSrcDpiY * fSrcHeight / fH;
			oWmfOut.SetDpi(dRendererDpix, dDpi);
			oWmfOut.SetWmfRect(oRectB);
			oWmfOut.SetScales(dScaleX, dScaleY);

			m_oWmfFile.SetOutputDevice(&oWmfOut);

			TWmfRectF oRect;
			m_oWmfFile.Play(&oRect);
#endif
		}
		else if (c_lMetaEmf == m_lType)
		{
			CEmfRendererOutput oEmfOut(&m_oEmfFile, pRenderer, dX, dY, dWidth, dHeight);
			m_oEmfFile.SetOutputDevice(&oEmfOut);
			m_oEmfFile.PlayMetaFile();
		}

		pRenderer->EndCommand(c_nImageType);
		return true;
	};
	void CMetaFile::Close()
	{
		m_oWmfFile.Close();
		//m_oEmfFile.Close();

		m_lType  = 0;
	};
	int CMetaFile::GetType()
	{
		return m_lType;
	}
	void CMetaFile::GetBounds(double* pdX, double* pdY, double* pdW, double* pdH)
	{
		if (c_lMetaWmf == m_lType)
		{
			*pdX = m_oWmfRect.oTL.fX;
			*pdY = m_oWmfRect.oTL.fY;
			*pdW = m_oWmfRect.oBR.fX - m_oWmfRect.oTL.fX;
			*pdH = m_oWmfRect.oBR.fY - m_oWmfRect.oTL.fY;
		}
		else if (c_lMetaEmf == m_lType)
		{
			TEmfRectL* pRect = m_oEmfFile.GetBounds();
			*pdX = pRect->lLeft;
			*pdY = pRect->lTop;
			*pdW = pRect->lRight - pRect->lLeft;
			*pdH = pRect->lBottom - pRect->lTop;
		}
		else
		{
			*pdX = 0;
			*pdY = 0;
			*pdW = 0;
			*pdH = 0;
		}
	};
	void CMetaFile::ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight)
	{
		CFontManager *pFontManager = m_pAppFonts->GenerateFontManager();
		CFontsCache* pFontCache = new CFontsCache();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);
		CImageFilesCache oCache;

		CGraphicsRenderer oRenderer;
		oRenderer.SetFontManager(pFontManager);
		oRenderer.SetImageCache(&oCache);

		if (-1 == nHeight)
		{
			double dX, dY, dW, dH;
			GetBounds(&dX, &dY, &dW, &dH);
			nHeight = (int)((double)nWidth * dH / dW);
		}

		//double dDpiX, dDpiY;
		//oRenderer.get_DpiX(&dDpiX);
		//oRenderer.get_DpiX(&dDpiY);

		double dWidth  = nWidth ;//* 72 / 25.4 / dDpiX;
		double dHeight = nHeight ;//* 72 / 25.4 / dDpiY;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		oRenderer.CreateFromBgraFrame(&oFrame);
		oRenderer.put_Width(dWidth);
		oRenderer.put_Height(dHeight);

		DrawOnRenderer(&oRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(wsOutFilePath, unFileType);
		RELEASEINTERFACE(pFontManager);
	}
}
