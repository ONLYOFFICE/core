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

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Base64.h"
#include "../../DesktopEditor/graphics/MetafileToRenderer.h"

namespace NSOnlineOfficeBinToPdf
{
    class CMetafileToRenderterPDF : public IMetafileToRenderter
    {
    public:
        CMetafileToRenderterPDF(IRenderer* pRenderer) : IMetafileToRenderter(pRenderer)
        {
        }

	public:
        virtual void SetLinearGradiant(const double& x0, const double& y0, const double& x1, const double& y1)
        {
            ((CPdfFile*)m_pRenderer)->SetLinearGradient(x0, y0, x1, y1);
        }

        virtual void SetRadialGradiant(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
        {
            ((CPdfFile*)m_pRenderer)->SetRadialGradient(dX0, dY0, dR0, dX1, dY1, dR1);
        }
    };

    static bool ConvertBufferToPdf(CPdfFile* pPdf, BYTE* pBuffer, LONG lBufferLen, CConvertFromBinParams* pParams)
    {
        CMetafileToRenderterPDF oCorrector(pPdf);
        oCorrector.SetTempDirectory(pPdf->GetTempDirectory());
        if (pParams)
        {
            oCorrector.SetMediaDirectory(pParams->m_sMediaDirectory);
            oCorrector.SetInternalMediaDirectory(pParams->m_sInternalMediaDirectory);
            oCorrector.SetThemesDirectory(pParams->m_sThemesDirectory);

            if (pParams->m_bIsUsePicker)
                oCorrector.InitPicker(pPdf->GetFonts());
        }
        NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(pBuffer, lBufferLen, &oCorrector);

        return true;
    }
    bool ConvertBinToPdf(CPdfFile* pPdf, const std::wstring& wsSrcFile, const std::wstring& wsDstFile, bool bBinary, CConvertFromBinParams* pParams)
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

		bool bIsNeedDestroy = (NULL == pParams) ? true : false;
		if (bIsNeedDestroy)
			pParams = new CConvertFromBinParams();

		if (pParams->m_sMediaDirectory.empty())
			pParams->m_sMediaDirectory = NSFile::GetDirectoryName(wsSrcFile);

		if (bBinary)
		{
			ConvertBufferToPdf(pPdf, pFileContent, dwFileSize, pParams);
		}
		else
		{
			int   nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
			BYTE* pBuffer    = new BYTE[nBufferLen];
			if (!pBuffer)
			{
				RELEASEARRAYOBJECTS(pFileContent);

				if (bIsNeedDestroy)
					RELEASEOBJECT(pParams);
				return false;
			}

			if (NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen))
			{
				ConvertBufferToPdf(pPdf, pBuffer, nBufferLen, pParams);
			}
			else
			{
				RELEASEARRAYOBJECTS(pBuffer);
				RELEASEARRAYOBJECTS(pFileContent);

				if (bIsNeedDestroy)
					RELEASEOBJECT(pParams);
				return false;
			}

			RELEASEARRAYOBJECTS(pBuffer);
		}
		RELEASEARRAYOBJECTS(pFileContent);

		if (bIsNeedDestroy)
			RELEASEOBJECT(pParams);

		if (!wsDstFile.empty())
		{
			if (0 != pPdf->SaveToFile(wsDstFile))
				return false;
		}

		return true;
	}
}
