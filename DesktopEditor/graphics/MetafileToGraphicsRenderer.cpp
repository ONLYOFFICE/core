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
#include "./MetafileToGraphicsRenderer.h"
#include "./MetafileToRendererCheck.h"
#include "GraphicsRenderer.h"
#include "../common/StringExt.h"

namespace NSOnlineOfficeBinToPdf
{
    class CMetafileToRenderterRaster_private
    {
    public:
		NSFonts::IApplicationFonts* appFonts;

        int m_nRasterFormat;
        int m_nSaveType; // 0 = stretch, 1 = aspect (width == maxsize)
        bool m_bIsOnlyFirst;
        std::wstring m_sFileName;
        int m_nRasterW;
        int m_nRasterH;

        double m_dDpiX;
        double m_dDpiY;

    public:
        CMetafileToRenderterRaster_private()
        {
            appFonts = NULL;

            m_nRasterFormat = 4;
            m_nSaveType = 2;
            m_bIsOnlyFirst = true;
            m_sFileName = L"";

            m_nRasterW = 100;
            m_nRasterH = 100;

            m_dDpiX = 96.0;
            m_dDpiY = 96.0;
        }

		~CMetafileToRenderterRaster_private()
        {
        }
    };

    CMetafileToRenderterRaster::CMetafileToRenderterRaster(IRenderer* pRenderer) : IMetafileToRenderter(pRenderer)
    {
        m_internal = new CMetafileToRenderterRaster_private();
    }
    CMetafileToRenderterRaster::~CMetafileToRenderterRaster()
    {
        RELEASEOBJECT(m_internal);
    }

    bool CMetafileToRenderterRaster::ConvertBuffer(BYTE* pBuffer, LONG lBufferLen)
    {
        std::wstring sMain = m_internal->m_sFileName;
        std::wstring sExt = L"";
        if (!m_internal->m_bIsOnlyFirst)
        {
            std::wstring::size_type pos = m_internal->m_sFileName.find_last_of('.');
            if (std::wstring::npos == pos)
                return false;

            sMain = m_internal->m_sFileName.substr(0, pos);
            sExt = m_internal->m_sFileName.substr(pos);
        }

        CMetafilePagesInfo oInfo;
        oInfo.CheckBuffer(pBuffer, lBufferLen);

        int nPagesCount = oInfo.PagesCount;
        if (0 == nPagesCount)
            return false;

        if (m_internal->m_bIsOnlyFirst)
            nPagesCount = 1;

		NSFonts::IFontManager* pFontManager = m_internal->appFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontsCache = NSFonts::NSFontCache::Create();
        pFontsCache->SetStreams(m_internal->appFonts->GetStreams());
        pFontManager->SetOwnerCache(pFontsCache);

        CImageFilesCache* pImagesCache = new CImageFilesCache(m_internal->appFonts);

        for (int nPageIndex = 0; nPageIndex < nPagesCount; ++nPageIndex)
        {
            CBgraFrame oFrame;
            int nRasterW = m_internal->m_nRasterW;
            int nRasterH = m_internal->m_nRasterH;

            int nOffsetX = 0;
            int nResultW = nRasterW;
            int nResultH = nRasterH;

            if (1 == m_internal->m_nSaveType)
            {
                double w = oInfo.arSizes[nPageIndex].width;
                double h = oInfo.arSizes[nPageIndex].height;

                double dKoef1 = nRasterW / w;
                double dKoef2 = nRasterH / h;
                if (dKoef1 > dKoef2)
                    dKoef1 = dKoef2;

                nRasterW = (int)(w * dKoef1 + 0.5);
                nRasterH = (int)(h * dKoef1 + 0.5);
            }
            else if (2 == m_internal->m_nSaveType)
            {
                double w = oInfo.arSizes[nPageIndex].width;
                double h = oInfo.arSizes[nPageIndex].height;

                nRasterW = (int)((w * m_internal->m_dDpiX / 25.4) + 0.5);
                nRasterH = (int)((h * m_internal->m_dDpiY / 25.4) + 0.5);
            }
            else if (3 == m_internal->m_nSaveType)
            {
                double w = oInfo.arSizes[nPageIndex].width;
                double h = oInfo.arSizes[nPageIndex].height;

                double dKoef1 = nRasterW / w;
                double dKoef2 = nRasterH / h;

                if (dKoef1 > dKoef2)
                {
                    nRasterH = (int)(dKoef1 * h) + 1;
                }
                else
                {
                    nRasterW = (int)(dKoef2 * w) + 1;
                    nOffsetX = (nRasterW - nResultW) >> 1;
                }
            }

            oFrame.put_Width(nRasterW);
            oFrame.put_Height(nRasterH);
            oFrame.put_Stride(4 * nRasterW);

            BYTE* pDataRaster = new BYTE[4 * nRasterW * nRasterH];
            memset(pDataRaster, 0xFF, 4 * nRasterW * nRasterH);
            oFrame.put_Data(pDataRaster);

            CGraphicsRenderer oRenderer;
            oRenderer.SetFontManager(pFontManager);
            oRenderer.SetImageCache(pImagesCache);

            oRenderer.CreateFromBgraFrame(&oFrame);
            oRenderer.SetTileImageDpi(96.0);
            oRenderer.SetSwapRGB(false);

            this->m_pRenderer = &oRenderer;
            BYTE* pBufferPage = oInfo.arSizes[nPageIndex].data;
            LONG nLen = lBufferLen - ((LONG)(pBufferPage - pBuffer));
            NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(pBufferPage, nLen, this);

            if (3 == m_internal->m_nSaveType)
            {
                int nStride = 4 * nResultW;
                BYTE* pDataResult = new BYTE[nStride * nResultH];

                if (0 == nOffsetX)
                {
                    memcpy(pDataResult, pDataRaster, nStride * nResultH);
                }
                else
                {
                    BYTE* pDataCopy = pDataRaster + 4 * nOffsetX;
                    BYTE* pDataDst = pDataResult;
                    int nStrideSrc = 4 * nRasterW;
                    for (int i = 0; i < nResultH; ++i)
                    {
                        memcpy(pDataDst, pDataCopy, nStride);
                        pDataCopy += nStrideSrc;
                        pDataDst += nStride;
                    }
                }

                oFrame.put_Data(pDataResult);
                oFrame.put_Width(nResultW);
                oFrame.put_Height(nResultH);
                oFrame.put_Stride(nStride);

                RELEASEARRAYOBJECTS(pDataRaster);
            }

            if (m_internal->m_bIsOnlyFirst)
                oFrame.SaveFile(sMain + sExt, m_internal->m_nRasterFormat);
            else
                oFrame.SaveFile(sMain + std::to_wstring(nPageIndex + 1) + sExt, m_internal->m_nRasterFormat);
        }

        RELEASEINTERFACE(pFontManager);
        RELEASEINTERFACE(pImagesCache);

        return true;
    }

    std::wstring CMetafileToRenderterRaster::GetFileName()
    {
        return m_internal->m_sFileName;
    }
    void CMetafileToRenderterRaster::SetFileName(const std::wstring& value)
    {
        m_internal->m_sFileName = value;
    }

    void CMetafileToRenderterRaster::SetApplication(NSFonts::IApplicationFonts* pFonts)
    {
		m_internal->appFonts = pFonts;
    }

    int CMetafileToRenderterRaster::GetRasterFormat()
    {
        return m_internal->m_nRasterFormat;
    }
    void CMetafileToRenderterRaster::SetRasterFormat(const int& value)
    {
        m_internal->m_nRasterFormat = value;
    }

    int CMetafileToRenderterRaster::GetSaveType()
    {
        return m_internal->m_nSaveType;
    }
    void CMetafileToRenderterRaster::SetSaveType(const int& value)
    {
        m_internal->m_nSaveType = value;
    }

    int CMetafileToRenderterRaster::GetRasterW()
    {
        return m_internal->m_nRasterW;
    }
    void CMetafileToRenderterRaster::SetRasterW(const int& value)
    {
        m_internal->m_nRasterW = value;
    }

    int CMetafileToRenderterRaster::GetRasterH()
    {
        return m_internal->m_nRasterH;
    }
    void CMetafileToRenderterRaster::SetRasterH(const int& value)
    {
        m_internal->m_nRasterH = value;
    }

    bool CMetafileToRenderterRaster::GetIsOnlyFirst()
    {
        return m_internal->m_bIsOnlyFirst;
    }
    void CMetafileToRenderterRaster::SetIsOnlyFirst(const bool& value)
    {
        m_internal->m_bIsOnlyFirst = value;
    }
    void CMetafileToRenderterRaster::SetOutputDpi(const double& dDpiX, const double& dDpiY)
    {
        m_internal->m_dDpiX = dDpiX;
        m_internal->m_dDpiY = dDpiY;
    }
}
