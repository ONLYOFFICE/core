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
#include "OnlineOfficeBinToPdf.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/StringExt.h"

#include "../DesktopEditor/graphics/MetafileToRenderer.h"
#include "../DesktopEditor/raster/BgraFrame.h"

namespace NSOnlineOfficeBinToPdf
{
    class CMetafileToRenderterPDF : public IMetafileToRenderter
    {
    public:
        std::wstring wsHtmlPlace;

    public:
        CMetafileToRenderterPDF(IRenderer* pRenderer) : IMetafileToRenderter(pRenderer)
        {
            wsHtmlPlace = L"";
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

                    bool bIsOnlyOfficeHatch = false;
                    if (nFind > 0 && (std::wstring::npos != wsTempString.find(L"onlyoffice_hatch")))
                        bIsOnlyOfficeHatch = true;

                    wsTempString = wsTempString.substr(nFind + 1);

                    std::wstring wsBase64TempFile = ((CPdfRenderer*)m_pRenderer)->GetTempFile();
                    std::string sBase64MultyByte(wsTempString.begin(), wsTempString.end());

                    int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
                    BYTE* pImageBuffer = new BYTE[nBufferLen + 64];

                    if (NSBase64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(), pImageBuffer, &nBufferLen))
                    {
                        if (!bIsOnlyOfficeHatch)
                        {
                            NSFile::CFileBinary oFile;
                            if (oFile.CreateFileW(wsBase64TempFile))
                            {
                                oFile.WriteFile(pImageBuffer, nBufferLen);
                                oFile.CloseFile();
                                wsTempString = wsBase64TempFile;
                            }
                        }
                        else
                        {
                            int nSize = (int)sqrt(nBufferLen >> 2);
                            CBgraFrame oFrame;
                            oFrame.put_Data(pImageBuffer);
                            oFrame.put_Width(nSize);
                            oFrame.put_Height(nSize);
                            oFrame.put_Stride(4 * nSize);
                            oFrame.SaveFile(wsBase64TempFile, 4); // PNG
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
                        std::wstring wsThemesPlace = ((CPdfRenderer*)m_pRenderer)->GetThemesPlace();
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

        virtual void EnableBrushRect(bool bValue)
        {
            ((CPdfRenderer*)m_pRenderer)->EnableBrushRect(bValue ? 1 : 0);
        }
        virtual void SetLinearGradiant(const double& x0, const double& y0, const double& x1, const double& y1)
        {
            ((CPdfRenderer*)m_pRenderer)->SetLinearGradient(x0, y0, x1, y1);
        }

        virtual void SetRadialGradiant(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
        {
            ((CPdfRenderer*)m_pRenderer)->SetRadialGradient(dX0, dY0, dR0, dX1, dY1, dR1);
        }
    };

    static bool ConvertBufferToPdf(CPdfRenderer* pPdf, BYTE* pBuffer, LONG lBufferLen, const std::wstring& wsHtmlPlace, const bool& bIsUsePicker = false)
    {
        CMetafileToRenderterPDF oCorrector(pPdf);
        oCorrector.wsHtmlPlace = wsHtmlPlace;
        if (bIsUsePicker)
            oCorrector.InitPicker(pPdf->GetApplicationFonts());
        NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(pBuffer, lBufferLen, &oCorrector);

        return true;
    }
    bool ConvertBinToPdf(CPdfRenderer* pPdf, const std::wstring& wsSrcFile, const std::wstring& wsDstFile, bool bBinary, const bool& bIsUsePicker)
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(wsSrcFile))
			return false;

		DWORD dwFileSize = oFile.GetFileSize();
		BYTE* pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			return false;
		}

		DWORD dwReaded;
		oFile.ReadFile(pFileContent, dwFileSize, dwReaded);
		oFile.CloseFile();

		std::wstring wsHtmlPlace = NSDirectory::GetFolderPath(wsSrcFile);
		if (bBinary)
		{
            ConvertBufferToPdf(pPdf, pFileContent, dwFileSize, wsHtmlPlace, bIsUsePicker);
		}
		else
		{
			int   nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
			BYTE* pBuffer    = new BYTE[nBufferLen];
			if (!pBuffer)
			{
				RELEASEARRAYOBJECTS(pFileContent);
				return false;
			}

			if (NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen))
			{
                ConvertBufferToPdf(pPdf, pBuffer, nBufferLen, wsHtmlPlace, bIsUsePicker);
			}
			else
			{
				RELEASEARRAYOBJECTS(pBuffer);
				RELEASEARRAYOBJECTS(pFileContent);
				return false;
			}

			RELEASEARRAYOBJECTS(pBuffer);
		}
		RELEASEARRAYOBJECTS(pFileContent);

		pPdf->SaveToFile(wsDstFile);

		return true;
	}
}
