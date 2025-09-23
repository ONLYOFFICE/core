/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../../PdfFile/PdfFile.h"
#include "../../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"

std::wstring getExtentionByRasterFormat(int format)
{
	switch (format)
	{
	case 1:  return L".bmp";
	case 2:  return L".gif";
	case 3:  return L".jpg";
	default: return L".png";
	}
	return L"";
}

int main(int argc, char *argv[])
{
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/../../../../../server/FileConverter/bin";
	oWorker.m_bIsNeedThumbnails = false;
	oWorker.m_arAdditionalFolders.push_back(NSFile::GetProcessDirectory() + L"/../../../../../core-fonts");

	NSFonts::IApplicationFonts* pFonts = oWorker.Check();

	std::wstring sTempDir = NSFile::GetProcessDirectory() + L"/temp";

	if (!NSDirectory::Exists(sTempDir))
		NSDirectory::CreateDirectory(sTempDir);

	std::wstring sThemesDirectory = NSFile::GetProcessDirectory() + L"/../../../../../sdkjs/slide/themes";
	std::wstring sPassword = L"";

	// SET PARAMETERS:
	bool bIsPDF = true;
	bool bIsBase64 = true;
	std::wstring sMetaFile = L"";
	std::wstring sMediaDirectory = L"";

	bool bResult = false;

	if (bIsPDF)
	{
		CPdfFile oPdfWriter(pFonts);
		oPdfWriter.CreatePdf();
		oPdfWriter.SetTempDirectory(sTempDir);

		CConvertFromBinParams oBufferParams;
		oBufferParams.m_sThemesDirectory = sThemesDirectory;

		if (!sPassword.empty())
			oPdfWriter.SetPassword(sPassword);

		std::wstring sOutputPath = NSFile::GetProcessDirectory() + L"/out.pdf";

		if (!bIsBase64)
		{
			bResult = S_OK == oPdfWriter.OnlineWordToPdfFromBinary(sMetaFile, sOutputPath, &oBufferParams);
		}
		else
		{
			bResult = S_OK == oPdfWriter.OnlineWordToPdf(sMetaFile, sOutputPath, &oBufferParams);
		}
	}
	else
	{
		NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);

		imageWriter.SetMediaDirectory(sMediaDirectory);
		imageWriter.SetThemesDirectory(sThemesDirectory);
		imageWriter.SetInternalMediaDirectory(sMediaDirectory);
		imageWriter.SetTempDirectory(sTempDir);
		imageWriter.SetApplication(pFonts);

		imageWriter.SetRasterFormat(4);
		std::wstring sOutputPath = NSFile::GetProcessDirectory() + L"/out" + getExtentionByRasterFormat(imageWriter.GetRasterFormat());

		imageWriter.SetSaveType(0);
		imageWriter.SetIsOnlyFirst(false);
		imageWriter.SetRasterW(1000);
		imageWriter.SetRasterH(1000);

		std::wstring sThumbnailDir;
		if (imageWriter.GetIsOnlyFirst())
		{
			imageWriter.SetFileName(sOutputPath);
		}
		else
		{
			std::wstring sFileAddon = L"image" + getExtentionByRasterFormat(imageWriter.GetRasterFormat());
			if (!NSDirectory::Exists(sOutputPath))
				NSDirectory::CreateDirectory(sOutputPath);
			imageWriter.SetFileName(sOutputPath + FILE_SEPARATOR_STR + sFileAddon);
		}

		BYTE* pBuffer = NULL;
		int lBufferLen = 0;

		if (bIsBase64)
		{
			BYTE* pFileContent = NULL;
			DWORD dwFileSize = 0;
			if (NSFile::CFileBinary::ReadAllBytes(sMetaFile, &pFileContent, dwFileSize))
			{
				lBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
				BYTE* pBuffer = new BYTE[lBufferLen];
				NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &lBufferLen);
			}

			RELEASEARRAYOBJECTS(pFileContent);
		}
		else
		{
			DWORD dwFileSize = 0;
			NSFile::CFileBinary::ReadAllBytes(sMetaFile, &pBuffer, dwFileSize);
			lBufferLen = (DWORD)dwFileSize;
		}

		bResult = imageWriter.ConvertBuffer(pBuffer, (LONG)lBufferLen);

		RELEASEARRAYOBJECTS(pBuffer);
	}

	pFonts->Release();
	return bResult ? 0 : 1;
}
