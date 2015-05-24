#include "MetaFile.h"

#include "../../graphics/GraphicsRenderer.h"
#include "../../raster/BgraFrame.h"

#include "Common/MetaFileRenderer.h"

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
		m_oSvmFile.SetFontManager(m_pFontManager);

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
		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем 
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------
		RELEASEINTERFACE(m_pFontManager);
		m_pFontManager = m_pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
		pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
		m_pFontManager->SetOwnerCache(pMeasurerCache);
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
		m_oSvmFile.SetFontManager(m_pFontManager);
		//------------------------------------------------------


		// Сначала пытаемся открыть файл как Wmf
		if (m_oWmfFile.OpenFromFile(wsFilePath) == true)
		{
			m_oWmfFile.Scan();

			if (!m_oWmfFile.CheckError())
			{
				m_lType = c_lMetaWmf;
				return true;
			}
			m_oWmfFile.Close();
		}
		// Это не Wmf
		if (m_oEmfFile.OpenFromFile(wsFilePath) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}

			m_oEmfFile.Close();
		}
		// Это не Emf
		if (m_oSvmFile.OpenFromFile(wsFilePath) == true)
		{
			m_oSvmFile.Scan();

			if (!m_oSvmFile.CheckError())
			{
				m_lType = c_lMetaSvm;
				return true;
			}

			m_oSvmFile.Close();
		}

		return false;
	};
	bool CMetaFile::DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		if (NULL == pRenderer)
			return false;

		pRenderer->BeginCommand(c_nImageType);

		if (c_lMetaWmf == m_lType)
		{
			CMetaFileRenderer oWmfOut(&m_oWmfFile, pRenderer, dX, dY, dWidth, dHeight);
			m_oWmfFile.SetOutputDevice((IOutputDevice*)&oWmfOut);
			m_oWmfFile.PlayMetaFile();
		}
		else if (c_lMetaEmf == m_lType)
		{
			CMetaFileRenderer oEmfOut(&m_oEmfFile, pRenderer, dX, dY, dWidth, dHeight);
			m_oEmfFile.SetOutputDevice((IOutputDevice*)&oEmfOut);
			m_oEmfFile.PlayMetaFile();
		}
		else if (c_lMetaSvm == m_lType)
		{
			CMetaFileRenderer oSvmOut(&m_oSvmFile, pRenderer, dX, dY, dWidth, dHeight);
			m_oSvmFile.SetOutputDevice((IOutputDevice*)&oSvmOut);
			m_oSvmFile.PlayMetaFile();
		}
		pRenderer->EndCommand(c_nImageType);
		return true;
	};
	void CMetaFile::Close()
	{
		m_oWmfFile.Close();
		m_oEmfFile.Close();
		m_oSvmFile.Close();

		m_lType  = 0;
	};
	int  CMetaFile::GetType()
	{
		return m_lType;
	}
	void CMetaFile::GetBounds(double* pdX, double* pdY, double* pdW, double* pdH)
	{
		if (c_lMetaWmf == m_lType)
		{
            const TRectD& oRect = m_oWmfFile.GetBounds();
			*pdX = oRect.dLeft;
			*pdY = oRect.dTop;
			*pdW = oRect.dRight - oRect.dLeft;
			*pdH = oRect.dBottom - oRect.dTop;
		}
		else if (c_lMetaEmf == m_lType)
		{
			TEmfRectL* pRect = m_oEmfFile.GetBounds();
			*pdX = pRect->lLeft;
			*pdY = pRect->lTop;
			*pdW = pRect->lRight - pRect->lLeft;
			*pdH = pRect->lBottom - pRect->lTop;
		}
		else if (c_lMetaSvm == m_lType)
		{
			TRect* pRect = m_oSvmFile.GetBounds();
			*pdX = pRect->nLeft;
			*pdY = pRect->nTop;
			*pdW = pRect->nRight - pRect->nLeft;
			*pdH = pRect->nBottom - pRect->nTop;
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

		CGraphicsRenderer oRenderer;		
		oRenderer.SetFontManager(pFontManager);

		if (-1 == nHeight)
		{
			double dX, dY, dW, dH;
			GetBounds(&dX, &dY, &dW, &dH);

			if (nWidth  < 0) nWidth = dW;
			nHeight = (int)((double)nWidth * dH / dW);
		}

		//double dDpiX, dDpiY;
		//oRenderer.get_DpiX(&dDpiX);
		//oRenderer.get_DpiX(&dDpiY);

		double dWidth  = nWidth ;//* 72 / 25.4 / dDpiX;
		double dHeight = nHeight ;//* 72 / 25.4 / dDpiY;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
		if (!pBgraData)
			return;

		memset(pBgraData, 0xff, nWidth * nHeight * 4);
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		oRenderer.CreateFromBgraFrame(&oFrame);
		oRenderer.SetSwapRGB(false);
		oRenderer.put_Width(dWidth);
		oRenderer.put_Height(dHeight);

		DrawOnRenderer(&oRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(wsOutFilePath, unFileType);
		RELEASEINTERFACE(pFontManager);
	}
}
