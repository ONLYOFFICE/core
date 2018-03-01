/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _BUILD_METAFILE_TO_GRRENDERER_H_
#define _BUILD_METAFILE_TO_GRRENDERER_H_

#pragma once

#include "./MetafileToRenderer.h"
#include "GraphicsRenderer.h"

namespace NSOnlineOfficeBinToPdf
{
    class CMetafileToRenderterRaster : public IMetafileToRenderter
    {
    public:
        std::wstring wsHtmlPlace;
        std::wstring wsThemesPlace;

        std::wstring wsTempDir;

        CApplicationFonts* appFonts;

    public:
        int m_nRasterFormat;
        int m_nSaveType; // 0 = stretch, 1 = aspect (width == maxsize)
        bool m_bIsOnlyFirst;
        std::wstring m_sFileName;
        int m_nRasterW;
        int m_nRasterH;

    private:
        std::vector<std::wstring> m_arTempFiles;

    public:
        CMetafileToRenderterRaster(IRenderer* pRenderer) : IMetafileToRenderter(pRenderer)
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
        }
        virtual ~CMetafileToRenderterRaster()
        {
            for (std::vector<std::wstring>::iterator i = m_arTempFiles.begin(); i != m_arTempFiles.end(); i++)
            {
                std::wstring sPath = *i;
                if (NSFile::CFileBinary::Exists(sPath))
                    NSFile::CFileBinary::Remove(sPath);
            }
        }

    public:
        virtual std::wstring GetImagePath(const std::wstring& sImagePath)
        {
            std::wstring wsTempString = sImagePath;
            if (0 == wsTempString.find(L"data:"))
            {
                try
                {
                    int nFind = wsTempString.find(L",");
                    wsTempString = wsTempString.substr(nFind + 1);

                    std::wstring sTempDirectory = wsTempDir;
                    if (sTempDirectory.empty())
                        sTempDirectory = NSFile::CFileBinary::GetTempPath();

                    std::wstring sTempFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTempDirectory, L"IMG");
                    m_arTempFiles.push_back(sTempFilePath);

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
                        if (L"" != wsThemesPlace)
                            wsTempString = wsThemesPlace + L"/" + wsTempString;
                    }
                    else
                    {
                        if (wsHtmlPlace.length() > 0)
                        {
                            if (0 == wsTempString.find(L"media") || NSFile::CFileBinary::Exists(wsHtmlPlace + L"/" + wsTempString))
                                wsTempString = wsHtmlPlace + L"/" + wsTempString;
                            else
                                wsTempString = wsHtmlPlace + L"/media/" + wsTempString;
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

        bool ConvertBuffer(BYTE* pBuffer, LONG lBufferLen)
        {
            std::wstring sMain = m_sFileName;
            std::wstring sExt = L"";
            if (!m_bIsOnlyFirst)
            {
                std::wstring::size_type pos = m_sFileName.find_last_of('.');
                if (std::wstring::npos == pos)
                    return false;

                sMain = m_sFileName.substr(0, pos);
                sExt = m_sFileName.substr(pos);
            }

            CMetafilePagesInfo oInfo;
            oInfo.CheckBuffer(pBuffer, lBufferLen);

            int nPagesCount = oInfo.PagesCount;
            if (0 == nPagesCount)
                return false;

            if (m_bIsOnlyFirst)
                nPagesCount = 1;

            CFontManager* pFontManager = appFonts->GenerateFontManager();
            CFontsCache* pFontsCache = new CFontsCache();
            pFontsCache->SetStreams(appFonts->GetStreams());
            pFontManager->SetOwnerCache(pFontsCache);

            CImageFilesCache* pImagesCache = new CImageFilesCache(appFonts);

            for (int nPageIndex = 0; nPageIndex < nPagesCount; ++nPageIndex)
            {
                CBgraFrame oFrame;
                int nRasterW = m_nRasterW;
                int nRasterH = m_nRasterH;

                if (1 == m_nSaveType)
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

                if (m_bIsOnlyFirst)
                    oFrame.SaveFile(sMain + sExt, m_nRasterFormat);
                else
                    oFrame.SaveFile(sMain + std::to_wstring(nPageIndex + 1) + sExt, m_nRasterFormat);
            }

            RELEASEINTERFACE(pFontManager);
            RELEASEINTERFACE(pImagesCache);

            return true;
        }
    };
}

#endif // _BUILD_METAFILE_TO_GRRENDERER_H_
