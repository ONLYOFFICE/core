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

#include "../../graphics/GraphicsRenderer.h"
#include "../../raster/BgraFrame.h"

#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

#include "Common/MetaFileRenderer.h"

namespace MetaFile
{
	IMetaFile* Create(NSFonts::IApplicationFonts *pAppFonts)
	{
		return new CMetaFile(pAppFonts);
	}

	CMetaFile::CMetaFile(NSFonts::IApplicationFonts *pAppFonts) : MetaFile::IMetaFile(pAppFonts)
	{
		m_pAppFonts = (CApplicationFonts*)pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем

		if (pAppFonts)
		{
			m_pFontManager = (CFontManager*)pAppFonts->GenerateFontManager();
			
			CFontsCache* pMeasurerCache = new CFontsCache();
			pMeasurerCache->SetStreams(pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}
		
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
	NSFonts::IFontManager* CMetaFile::get_FontManager()
	{
		return m_pFontManager;
	}
	bool CMetaFile::LoadFromFile(const wchar_t *wsFilePath)
	{
		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем 
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------
		RELEASEINTERFACE(m_pFontManager);

		if (m_pAppFonts)
		{
			m_pFontManager = (CFontManager*)m_pAppFonts->GenerateFontManager();
			CFontsCache* pMeasurerCache = new CFontsCache();
			pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
			m_pFontManager->SetOwnerCache(pMeasurerCache);
		}

		m_oWmfFile.SetFontManager(m_pFontManager);
		m_oEmfFile.SetFontManager(m_pFontManager);
		m_oSvmFile.SetFontManager(m_pFontManager);
        m_oSvgFile.SetFontManager(m_pFontManager);

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
        // Это не svm
        if (m_oSvgFile.OpenFromFile(wsFilePath) == true)
        {
            m_lType = c_lMetaSvg;
            return true;
        }

		return false;
    }
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
        else if (c_lMetaSvg == m_lType)
        {
            m_oSvgFile.Draw(pRenderer, dX, dY, dWidth, dHeight);
        }
		pRenderer->EndCommand(c_nImageType);
		return true;
    }
	void CMetaFile::Close()
	{
		m_oWmfFile.Close();
		m_oEmfFile.Close();
		m_oSvmFile.Close();
        m_oSvgFile.Close();

		m_lType  = 0;
    }
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

            if (*pdW > 10000 || *pdH > 10000)
            {
                *pdW /= 10;
                *pdH /= 10;
            }
        }
        else if (c_lMetaSvg == m_lType)
        {
            *pdX = 0;
            *pdY = 0;
            *pdW = m_oSvgFile.get_Width();
            *pdH = m_oSvgFile.get_Height();
        }
        else
        {
            *pdX = 0;
            *pdY = 0;
            *pdW = 0;
            *pdH = 0;
        }
        if (*pdW < 0) *pdW = -*pdW;
        if (*pdH < 0) *pdH = -*pdH;
    };
    void CMetaFile::ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight)
	{
		CFontManager *pFontManager = (CFontManager*)m_pAppFonts->GenerateFontManager();
		CFontsCache* pFontCache = new CFontsCache();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		CGraphicsRenderer oRenderer;	
		oRenderer.SetFontManager(pFontManager);

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

        _UINT32 alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым 
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
		{
            ((_UINT32*)pBgraData)[i] = alfa;
		}
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
