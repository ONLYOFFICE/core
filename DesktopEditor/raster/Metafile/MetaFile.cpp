/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "MetaFile.h"
#include "Common/MetaFileTypes.h"
#include "Common/MetaFileRenderer.h"
#include "../../raster/BgraFrame.h"
#include "../../graphics/pro/Graphics.h"

#ifdef METAFILE_SUPPORT_WMF_EMF
#include "Wmf/WmfInterpretator/CWmfInterpretatorSvg.h"
#include "Emf/EmfInterpretator/CEmfInterpretatorSvg.h"
#endif

namespace MetaFile
{
	IMetaFile* Create(NSFonts::IApplicationFonts *pAppFonts)
	{
		return new CMetaFile(pAppFonts);
	}

	CMetaFile::CMetaFile(NSFonts::IApplicationFonts *pAppFonts) : MetaFile::IMetaFile(pAppFonts)
	{
		m_pFontManager = NULL;
		m_pAppFonts = pAppFonts;
		// Создаем менеджер шрифтов с собственным кэшем
		if (pAppFonts)
		{
			m_pFontManager = pAppFonts->GenerateFontManager();
			NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
			pMeasurerCache->SetStreams(pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif
		m_lType  = 0;
	}

	NSFonts::IFontManager* CMetaFile::get_FontManager()
	{
		return m_pFontManager;
	}

	CMetaFile::~CMetaFile()
	{
		Close();
		RELEASEINTERFACE(m_pFontManager);
	}

	std::wstring CMetaFile::ConvertToSvg(unsigned int unWidth, unsigned int unHeight)
	{

	#ifdef METAFILE_SUPPORT_WMF_EMF
		if (c_lMetaWmf == m_lType)
		{
			m_oWmfFile.SetOutputDevice(InterpretatorType::Svg, unWidth, unHeight);
			m_oWmfFile.PlayMetaFile();
			return ((CWmfInterpretatorSvg*)m_oWmfFile.GetWmfParser()->GetInterpretator())->GetFile();
		}
		else if (c_lMetaEmf == m_lType)
		{
			m_oEmfFile.SetOutputDevice(InterpretatorType::Svg, unWidth, unHeight);
			m_oEmfFile.PlayMetaFile();
			return ((CEmfInterpretatorSvg*)m_oEmfFile.GetEmfParser()->GetInterpretator())->GetFile();
		}
	#endif
        return L"";
	}

#ifdef METAFILE_SUPPORT_WMF_EMF
	void CMetaFile::ConvertToXml(const wchar_t *wsFilePath)
	{
		if (NULL == wsFilePath)
			return;

		m_oEmfFile.SetOutputDevice(wsFilePath, InterpretatorType::XML);
		m_oEmfFile.PlayMetaFile();
	}

	void CMetaFile::ConvertToXmlAndRaster(const wchar_t *wsXmlFilePath, const wchar_t *wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight)
	{
		if (NULL == wsXmlFilePath || NULL == wsOutFilePath)
			return;

		m_oEmfFile.SetOutputDevice(NULL, wsXmlFilePath);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		NSFonts::IFontManager* pFontManager = m_pAppFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		pGrRenderer->SetFontManager(pFontManager);

		if (-1 == nHeight)
		{
			double dX, dY, dW, dH;
			GetBounds(&dX, &dY, &dW, &dH);

			if (dW < 0)
				dW = -dW;
			if (dH < 0)
				dH = -dH;

			if (nWidth < 0) nWidth = (int)(dW * 96 / 25.4);
			nHeight = (int)((double)nWidth * dH / dW);
		}

		double dWidth  = 25.4 * nWidth / 96;
		double dHeight = 25.4 * nHeight / 96;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			((unsigned int*)pBgraData)[i] = alfa;
		}
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->put_Width(dWidth);
		pGrRenderer->put_Height(dHeight);

		DrawOnRenderer(wsXmlFilePath, pGrRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(wsOutFilePath, unFileType);

		RELEASEINTERFACE(pFontManager);
		RELEASEINTERFACE(pGrRenderer);
	}

	bool CMetaFile::DrawOnRenderer(const wchar_t *wsXmlFilePath, IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		if (NULL == wsXmlFilePath || NULL == pRenderer)
			return false;

		pRenderer->BeginCommand(c_nImageType);

		switch (m_lType)
		{
		#ifdef METAFILE_SUPPORT_WMF_EMF
			case c_lMetaWmf:
			{
				CMetaFileRenderer oWmfOut(m_oWmfFile.GetWmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				m_oWmfFile.SetOutputDevice((IOutputDevice*)&oWmfOut);
				m_oWmfFile.PlayMetaFile();
				break;
			}
			case c_lMetaEmf:
			{
				CMetaFileRenderer oEmfOut(m_oEmfFile.GetEmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				m_oEmfFile.SetOutputDevice((IOutputDevice*)&oEmfOut, wsXmlFilePath);
				m_oEmfFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVM
			case c_lMetaSvm:
			{
				CMetaFileRenderer oSvmOut(&m_oSvmFile, pRenderer, dX, dY, dWidth, dHeight);
				m_oSvmFile.SetOutputDevice((IOutputDevice*)&oSvmOut);
				m_oSvmFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVG
			case c_lMetaSvg:
			{
				m_oSvgFile.Draw(pRenderer, dX, dY, dWidth, dHeight);
				break;
			}
		#endif
			default:
				break;
		}

		pRenderer->EndCommand(c_nImageType);
		return true;
	}

	bool CMetaFile::LoadFromXmlFile(const wchar_t *wsFilePath)
	{
		RELEASEINTERFACE(m_pFontManager);

		if (m_pAppFonts)
		{
			m_pFontManager = m_pAppFonts->GenerateFontManager();
			NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
			pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_WMF_EMF_XML
		if (m_oEmfFile.OpenFromXmlFile(wsFilePath) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif

		return false;
	}

	void CMetaFile::ConvertToEmf(const wchar_t *wsFilePath)
	{
		if (m_lType != c_lMetaEmf || m_oEmfFile.GetEmfParser()->GetType() != EmfParserType::EmfxParser)
			return;

		m_oEmfFile.SetOutputDevice(wsFilePath, InterpretatorType::Emf);
		m_oEmfFile.PlayMetaFile();

		//TODO:: сохранение в *.emf файл
	}
#endif

	bool CMetaFile::LoadFromFile(const wchar_t *wsFilePath)
	{
		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------

		RELEASEINTERFACE(m_pFontManager);

		if (m_pAppFonts)
		{
			m_pFontManager = m_pAppFonts->GenerateFontManager();
			NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
			pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

		//------------------------------------------------------

	#ifdef METAFILE_SUPPORT_WMF_EMF
		// Сначала пытаемся открыть файл как Wmf
		if (m_oWmfFile.OpenFromWmfFile(wsFilePath) == true)
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
		if (m_oEmfFile.OpenFromEmfFile(wsFilePath) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif
		// Это не Emf
	#ifdef METAFILE_SUPPORT_SVM
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
	#endif
		// Это не svm
	#ifdef METAFILE_SUPPORT_SVG
		if (m_oSvgFile.OpenFromFile(wsFilePath) == true)
		{
			m_lType = c_lMetaSvg;
			return true;
		}
	#endif

		return false;
	}

	bool CMetaFile::LoadFromBuffer(BYTE *pBuffer, unsigned int unSize)
	{
		if (NULL == pBuffer || 0 == unSize)
			return false;

		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------

		RELEASEINTERFACE(m_pFontManager);

		if (m_pAppFonts)
		{
			m_pFontManager = m_pAppFonts->GenerateFontManager();
			NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
			pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.SetFontManager(m_pFontManager);
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.SetFontManager(m_pFontManager);
	#endif

		//------------------------------------------------------

	#ifdef METAFILE_SUPPORT_WMF_EMF
		// Сначала пытаемся открыть файл как Wmf
		if (m_oWmfFile.ReadFromBuffer(pBuffer, unSize) == true)
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
		if (m_oEmfFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_oEmfFile.Scan();

			if (!m_oEmfFile.CheckError())
			{
				m_lType = c_lMetaEmf;
				return true;
			}
			m_oEmfFile.Close();
		}
	#endif
		// Это не Emf
	#ifdef METAFILE_SUPPORT_SVM
		if (m_oSvmFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_oSvmFile.Scan();

			if (!m_oSvmFile.CheckError())
			{
				m_lType = c_lMetaSvm;
				return true;
			}

			m_oSvmFile.Close();
		}
	#endif
		// Это не svm
	#ifdef METAFILE_SUPPORT_SVG
		if (m_oSvgFile.ReadFromBuffer(pBuffer, unSize) == true)
		{
			m_lType = c_lMetaSvg;
			return true;
		}
	#endif

		return false;
	}

	bool CMetaFile::DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
	{
		if (NULL == pRenderer)
			return false;

		pRenderer->BeginCommand(c_nImageType);

		switch (m_lType)
		{
		#ifdef METAFILE_SUPPORT_WMF_EMF
			case c_lMetaWmf:
			{
				CMetaFileRenderer oWmfOut(m_oWmfFile.GetWmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				m_oWmfFile.SetOutputDevice((IOutputDevice*)&oWmfOut);
				m_oWmfFile.PlayMetaFile();
				break;
			}
			case c_lMetaEmf:
			{
				CMetaFileRenderer oEmfOut(m_oEmfFile.GetEmfParser(), pRenderer, dX, dY, dWidth, dHeight);
				m_oEmfFile.SetOutputDevice((IOutputDevice*)&oEmfOut);
				m_oEmfFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVM
			case c_lMetaSvm:
			{
				CMetaFileRenderer oSvmOut(&m_oSvmFile, pRenderer, dX, dY, dWidth, dHeight);
				m_oSvmFile.SetOutputDevice((IOutputDevice*)&oSvmOut);
				m_oSvmFile.PlayMetaFile();
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVG
			case c_lMetaSvg:
			{
				m_oSvgFile.Draw(pRenderer, dX, dY, dWidth, dHeight);
				break;
			}
		#endif
			default:
				break;
		}

		pRenderer->EndCommand(c_nImageType);
		return true;
	}

	void CMetaFile::Close()
	{
	#ifdef METAFILE_SUPPORT_WMF_EMF
		m_oWmfFile.Close();
		m_oEmfFile.Close();
	#endif

	#ifdef METAFILE_SUPPORT_SVM
		m_oSvmFile.Close();
	#endif

	#ifdef METAFILE_SUPPORT_SVG
		m_oSvgFile.Close();
	#endif

		m_lType  = 0;
	}

	int  CMetaFile::GetType()
	{
		return m_lType;
	}

	void CMetaFile::GetBounds(double* pdX, double* pdY, double* pdW, double* pdH)
	{
		switch (m_lType)
		{
		#ifdef METAFILE_SUPPORT_WMF_EMF
			case c_lMetaWmf:
			{
				const TRectD& oRect = m_oWmfFile.GetBounds();
				*pdX = oRect.dLeft;
				*pdY = oRect.dTop;
				*pdW = oRect.dRight - oRect.dLeft;
				*pdH = oRect.dBottom - oRect.dTop;
				break;
			}
			case c_lMetaEmf:
			{
				TEmfRectL* pRect = m_oEmfFile.GetBounds();
				*pdX = pRect->lLeft;
				*pdY = pRect->lTop;
				*pdW = pRect->lRight - pRect->lLeft;
				*pdH = pRect->lBottom - pRect->lTop;
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVM
			case c_lMetaSvm:
			{
				TRect* pRect = m_oSvmFile.GetBounds();
				*pdX = pRect->nLeft;
				*pdY = pRect->nTop;
				*pdW = pRect->nRight - pRect->nLeft;
				*pdH = pRect->nBottom - pRect->nTop;

				if (*pdW > 10000 || *pdH > 10000)
				{
					*pdW /= 10;
					*pdH /= 10;
				}
				break;
			}
		#endif
		#ifdef METAFILE_SUPPORT_SVG
			case c_lMetaSvg:
			{
				*pdX = 0;
				*pdY = 0;
				*pdW = m_oSvgFile.get_Width();
				*pdH = m_oSvgFile.get_Height();
				break;
			}
		#endif
			default:
			{
				*pdX = 0;
				*pdY = 0;
				*pdW = 0;
				*pdH = 0;
				break;
			}
		}

		if (*pdW < 0) *pdW = -*pdW;
		if (*pdH < 0) *pdH = -*pdH;
	}

	void CMetaFile::ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight)
	{
		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		NSFonts::IFontManager* pFontManager = m_pAppFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		pGrRenderer->SetFontManager(pFontManager);

		if (-1 == nHeight)
		{
			double dX, dY, dW, dH;
			GetBounds(&dX, &dY, &dW, &dH);

			if (dW < 0)
				dW = -dW;
			if (dH < 0)
				dH = -dH;

			if (nWidth < 0) nWidth = (int)(dW * 96 / 25.4);
			nHeight = (int)((double)nWidth * dH / dW);
		}

		double dWidth  = 25.4 * nWidth / 96;
		double dHeight = 25.4 * nHeight / 96;

		BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		if (!pBgraData)
		{
			double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

			nWidth = (int)(dKoef * nWidth);
			nHeight = (int)(dKoef * nHeight);

			dWidth  = 25.4 * nWidth / 96;
			dHeight = 25.4 * nHeight / 96;

			pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
		}

		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			((unsigned int*)pBgraData)[i] = alfa;
		}
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->put_Width(dWidth);
		pGrRenderer->put_Height(dHeight);

		DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

		oFrame.SaveFile(wsOutFilePath, unFileType);
		oFrame.put_Data(NULL);
		RELEASEINTERFACE(pFontManager);
		RELEASEINTERFACE(pGrRenderer);

		if (pBgraData)
			free(pBgraData);
	}
}
