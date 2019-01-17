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

namespace NSOnlineOfficeBinToPdf
{
    class CMetafileToRenderterRaster_private
    {
    public:
        std::wstring wsHtmlPlace;
        std::wstring wsThemesPlace;
        std::wstring wsTempDir;

        CApplicationFonts* appFonts;

        int m_nRasterFormat;
        int m_nSaveType; // 0 = stretch, 1 = aspect (width == maxsize)
        bool m_bIsOnlyFirst;
        std::wstring m_sFileName;
        int m_nRasterW;
        int m_nRasterH;

        double m_dDpiX;
        double m_dDpiY;

        std::vector<std::wstring> m_arTempFiles;

    public:
        CMetafileToRenderterRaster_private()
        {
            wsHtmlPlace = L"";
            wsThemesPlace = L"";

            wsTempDir = L"";

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
            for (std::vector<std::wstring>::iterator i = m_arTempFiles.begin(); i != m_arTempFiles.end(); i++)
            {
                std::wstring sPath = *i;
                if (NSFile::CFileBinary::Exists(sPath))
                    NSFile::CFileBinary::Remove(sPath);
            }
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

    std::wstring CMetafileToRenderterRaster::GetImagePath(const std::wstring& sImagePath)
    {
        std::wstring wsTempString = sImagePath;
        if (0 == wsTempString.find(L"data:"))
        {
            try
            {
                int nFind = wsTempString.find(L",");
                wsTempString = wsTempString.substr(nFind + 1);

                std::wstring sTempDirectory = m_internal->wsTempDir;
                if (sTempDirectory.empty())
                    sTempDirectory = NSFile::CFileBinary::GetTempPath();

                std::wstring sTempFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTempDirectory, L"IMG");
                m_internal->m_arTempFiles.push_back(sTempFilePath);

                std::wstring wsBase64TempFile = sTempFilePath;
                std::string sBase64MultyByte(wsTempString.begin(), wsTempString.end());

                int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
                BYTE* pImageBuffer = new BYTE[nBufferLen + 64];

                if (NSBase64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(), pImageBuffer, &nBufferLen))
                {
                    NSFile::CFileBinary oFile;
                    if (oFile.CreateFileW(wsBase64TempFile))
                    {
                        oFile.WriteFile(pImageBuffer, nBufferLen);
                        oFile.CloseFile();
                        wsTempString = wsBase64TempFile;
                    }
                }
                else throw;
            }
            catch (...)
            {
            }
        }
        else
        {
            if (0 != wsTempString.find(L"http:")
                && 0 != wsTempString.find(L"https:")
                && 0 != wsTempString.find(L"ftp:")
                && 0 != wsTempString.find(L"file:"))
            {
                if (0 == wsTempString.find(L"theme"))
                {
                    if (L"" != m_internal->wsThemesPlace)
                        wsTempString = m_internal->wsThemesPlace + L"/" + wsTempString;
                }
                else
                {
                    if (m_internal->wsHtmlPlace.length() > 0)
                    {
                        if (0 == wsTempString.find(L"media") || NSFile::CFileBinary::Exists(m_internal->wsHtmlPlace + L"/" + wsTempString))
                            wsTempString = m_internal->wsHtmlPlace + L"/" + wsTempString;
                        else
                            wsTempString = m_internal->wsHtmlPlace + L"/media/" + wsTempString;
                    }

                    std::wstring wsSvgExt(L".svg");
                    if (0 == wsTempString.compare(wsTempString.length() - wsSvgExt.length(), std::wstring::npos, wsSvgExt))
                    {
                        std::wstring wsTestPath = wsTempString.substr(0, wsTempString.length() - wsSvgExt.length());
                        if (NSFile::CFileBinary::Exists(wsTestPath + L".emf"))
                            wsTempString = wsTestPath + L".emf";
                        else if (NSFile::CFileBinary::Exists(wsTestPath + L".wmf"))
                            wsTempString = wsTestPath + L".wmf";
                    }
                }

                NSStringExt::Replace(wsTempString, L"\\", L"/");
            }

            if (0 == wsTempString.find(L"file:///"))
            {
                // TODO: под linux код неправильный
                NSStringExt::Replace(wsTempString, L"file:///", L"");
                NSStringExt::Replace(wsTempString, L"\\", L"/");
            }
        }

        return wsTempString;
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

        CFontManager* pFontManager = (CFontManager*)m_internal->appFonts->GenerateFontManager();
        CFontsCache* pFontsCache = new CFontsCache();
        pFontsCache->SetStreams(m_internal->appFonts->GetStreams());
        pFontManager->SetOwnerCache(pFontsCache);

        CImageFilesCache* pImagesCache = new CImageFilesCache(m_internal->appFonts);

        for (int nPageIndex = 0; nPageIndex < nPagesCount; ++nPageIndex)
        {
            CBgraFrame oFrame;
            int nRasterW = m_internal->m_nRasterW;
            int nRasterH = m_internal->m_nRasterH;

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

            if (m_internal->m_bIsOnlyFirst)
                oFrame.SaveFile(sMain + sExt, m_internal->m_nRasterFormat);
            else
                oFrame.SaveFile(sMain + std::to_wstring(nPageIndex + 1) + sExt, m_internal->m_nRasterFormat);
        }

        RELEASEINTERFACE(pFontManager);
        RELEASEINTERFACE(pImagesCache);

        return true;
    }

    std::wstring CMetafileToRenderterRaster::GetHtmlPlace()
    {
        return m_internal->wsHtmlPlace;
    }
    void CMetafileToRenderterRaster::SetHtmlPlace(const std::wstring& value)
    {
        m_internal->wsHtmlPlace = value;
    }

    std::wstring CMetafileToRenderterRaster::GetThemesPlace()
    {
        return m_internal->wsThemesPlace;
    }
    void CMetafileToRenderterRaster::SetThemesPlace(const std::wstring& value)
    {
        m_internal->wsThemesPlace = value;
    }

    std::wstring CMetafileToRenderterRaster::GetTempDir()
    {
        return m_internal->wsTempDir;
    }
    void CMetafileToRenderterRaster::SetTempDir(const std::wstring& value)
    {
        m_internal->wsTempDir = value;
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
        m_internal->appFonts = (CApplicationFonts*)pFonts;
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
