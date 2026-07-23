/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		oBufferParams.m_sMediaDirectory = sMediaDirectory;
		oBufferParams.m_sInternalMediaDirectory = sMediaDirectory;
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
				pBuffer = new BYTE[lBufferLen];
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
