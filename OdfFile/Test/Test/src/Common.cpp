///*
// * (c) Copyright Ascensio System SIA 2010-2023
// *
// * This program is a free software product. You can redistribute it and/or
// * modify it under the terms of the GNU Affero General Public License (AGPL)
// * version 3 as published by the Free Software Foundation. In accordance with
// * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
// * that Ascensio System SIA expressly excludes the warranty of non-infringement
// * of any third-party rights.
// *
// * This program is distributed WITHOUT ANY WARRANTY; without even the implied
// * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
// * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
// *
// * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
// * street, Riga, Latvia, EU, LV-1050.
// *
// * The  interactive user interfaces in modified source and object code versions
// * of the Program must display Appropriate Legal Notices, as required under
// * Section 5 of the GNU AGPL version 3.
// *
// * Pursuant to Section 7(b) of the License you must retain the original Product
// * logo when distributing the program. Pursuant to Section 7(e) we decline to
// * grant you any rights under trademark law for use of our trademarks.
// *
// * All the Product's GUI elements, including illustrations and icon sets, as
// * well as technical writing content are licensed under the terms of the
// * Creative Commons Attribution-ShareAlike 4.0 International. See the License
// * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
// *
// */
//
#pragma once

#include "Common.h"

#include <stdio.h>
#include <tchar.h>

#include "..\..\..\..\Common\OfficeFileFormatChecker.h"
#include "..\..\..\..\OfficeUtils\src\OfficeUtils.h"
#include "..\..\..\..\DesktopEditor\common\Directory.h"
#include "..\..\..\..\OdfFile\Reader\Converter\ConvertOO2OOX.h"
#include "..\..\..\..\OdfFile\Common\logging.h"
#include "..\..\..\..\OOXML\SystemUtility\SystemUtility.h"


#if defined(_WIN64)
	#ifdef _DEBUG
		#pragma comment(lib, "../../../Common/3dParty/icu/win_64/build/icuuc.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/UnicodeConverter.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/CryptoPPLib.lib")
		#pragma comment(lib, "../../../build/lib/win_64/DEBUG/HtmlFile2.lib")
	#else
		#pragma comment(lib, "../../../Common/3dParty/icu/win_64/build/icuuc.lib")
		#pragma comment(lib, "../../../build/lib/win_64/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_64/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_64/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_64/UnicodeConverter.lib")
		#pragma comment(lib, "../../../build/lib/win_64/CryptoPPLib.lib")
	#endif // #if defined(_DEBUG)
#elif defined (_WIN32)
	#ifdef _DEBUG
		#pragma comment(lib, "../../../Common/3dParty/icu/win_64/build/icuuc.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
		#pragma comment(lib, "../../../build/lib/win_32/DEBUG/CryptoPPLib.lib")
	#else
		#pragma comment(lib, "../../../Common/3dParty/icu/win_64/build/icuuc.lib")
		#pragma comment(lib, "../../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../../build/lib/win_32/kernel_network.lib")
		#pragma comment(lib, "../../../build/lib/win_32/UnicodeConverter.lib")
		#pragma comment(lib, "../../../build/lib/win_32/CryptoPPLib.lib")
	#endif // #if defined(_DEBUG)
#endif

boost::optional<std::wstring> get_conversion_destination_path(const std::wstring& srcFileName)
{
	COfficeFileFormatChecker fileChecker(srcFileName);

	switch (fileChecker.nFileType)
	{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT:		return srcFileName + L"-tmp.docx";

		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT:	return srcFileName + L"-tmp.xlsx";

		case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT:	return srcFileName + L"-tmp.pptx";

		default:
			return boost::none;
	}
}

boost::optional<std::wstring> convert_odf_to_ooxml(std::wstring srcFileName)
{
	int nResult = 0;

	boost::optional<std::wstring> dstPath = get_conversion_destination_path(srcFileName);
	if (!dstPath)
		return boost::none;

	COfficeFileFormatChecker fileChecker(srcFileName);
	COfficeUtils oCOfficeUtils(NULL);

	std::wstring outputDir = NSDirectory::GetFolderPath(*dstPath);
	std::wstring dstTempPath = NSDirectory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring srcTempPath;
	std::wstring srcTempPath2 = NSDirectory::CreateDirectoryWithUniqueName(outputDir);

	if (fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP)
	{
		srcTempPath = NSDirectory::CreateDirectoryWithUniqueName(outputDir);

		if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
			return boost::none;
	}
	else // flat
	{
		srcTempPath = srcFileName;
	}
	_CP_LOG << L"[info] " << srcFileName << std::endl;

	nResult = ConvertODF2OOXml(srcTempPath, dstTempPath, L"C:\\Windows\\Fonts", srcTempPath2, L"");

	if (srcTempPath != srcFileName)
		NSDirectory::DeleteDirectory(srcTempPath);
	NSDirectory::DeleteDirectory(srcTempPath2);

	return nResult == 0 ? boost::optional<std::wstring>(dstTempPath) : boost::none;
}

ODT2DOCX_ConversionEnvironment::ODT2DOCX_ConversionEnvironment(const std::wstring& filename)
	: mFilename(filename)
{ }

OOX::CDocx* ODT2DOCX_ConversionEnvironment::GetDocument()
{
	return mDocx;
}

void ODT2DOCX_ConversionEnvironment::SetUp()
{
	boost::optional<std::wstring> convertedPath = convert_odf_to_ooxml(mFilename);

	if (convertedPath)
	{
		const OOX::CPath ooxPath(*convertedPath);
		mDocx = new OOX::CDocx(ooxPath);
	}
}

void ODT2DOCX_ConversionEnvironment::TearDown()
{
	NSDirectory::DeleteDirectory(mDocx->m_sDocumentPath);

	delete mDocx;
}
