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

#include "../../../../Common/OfficeFileFormatChecker.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../OdfFile/Reader/Converter/ConvertOO2OOX.h"
#include "../../../../OdfFile/Common/logging.h"
#include "../../../../OOXML/SystemUtility/SystemUtility.h"

#include "../../../../OdfFile/Writer/Converter/Oox2OdfConverter.h"

#if defined(_WIN32) || defined (_WIN64)
#include "windows.h"
#include "windef.h"
#include <shlobj.h>
#include <Rpc.h>
#endif


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

std::wstring get_oox_to_odf_conversion_type(const std::wstring& filename)
{
	COfficeFileFormatChecker fileChecker(filename);

	if (fileChecker.nFileType & AVS_OFFICESTUDIO_FILE_DOCUMENT)
		return L"text";
	else if (fileChecker.nFileType & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
		return L"spreadsheet";
	else if (fileChecker.nFileType & AVS_OFFICESTUDIO_FILE_PRESENTATION)
		return L"presentation";

	return L"";
}

std::wstring create_unique_name_with_prefix(const std::wstring& strFolderPathRoot, const std::wstring& prefix)
{
#if defined(_WIN32) || defined (_WIN64)
	UUID uuid;
	RPC_WSTR str_uuid;
	UuidCreate(&uuid);
	UuidToStringW(&uuid, &str_uuid);
	std::wstring pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR + prefix + L"_";
	pcTemplate += (wchar_t*)str_uuid;
	RpcStringFreeW(&str_uuid);

	int attemps = 10;
	while (!CreateDirectory(pcTemplate.c_str(), nullptr))
	{
		UuidCreate(&uuid);
		UuidToStringW(&uuid, &str_uuid);
		pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
		pcTemplate += (wchar_t*)str_uuid;
		RpcStringFreeW(&str_uuid);
		attemps--;

		if (0 == attemps)
		{
			pcTemplate = L"";
			break;
		}
	}
	return pcTemplate;
#else
	std::string pcTemplate = U_TO_UTF8(strFolderPathRoot) + "/ascXXXXXX";
	char* pcRes = mkdtemp(const_cast <char*> (pcTemplate.c_str()));
	if (NULL == pcRes)
		return L"";

	std::string sRes = pcRes;
	return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sRes.c_str(), (LONG)sRes.length());
#endif
}

boost::optional<std::wstring> convert_odf_to_ooxml(const std::wstring& srcFileName)
{
	int nResult = 0;

	boost::optional<std::wstring> dstPath = get_conversion_destination_path(srcFileName);
	if (!dstPath)
		return boost::none;

	COfficeFileFormatChecker fileChecker(srcFileName);
	COfficeUtils oCOfficeUtils(NULL);

	std::wstring outputDir = NSDirectory::GetFolderPath(*dstPath);
	std::wstring dstTempPath = create_unique_name_with_prefix(outputDir, NSFile::GetFileName(srcFileName));
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

boost::shared_ptr<cpdoccore::odf_reader::odf_document> convert_ooxml_to_odf(const std::wstring& srcFileName)
{
	boost::shared_ptr<cpdoccore::odf_reader::odf_document> odf = nullptr;

	const std::wstring ooxUnpackedPath = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetFolderPath(srcFileName));

	COfficeUtils oCOfficeUtils(nullptr);
	if (S_OK == oCOfficeUtils.ExtractToDirectory(srcFileName, ooxUnpackedPath, NULL, 0))
	{
		const std::wstring dstPath = create_unique_name_with_prefix(NSDirectory::GetFolderPath(srcFileName), NSFile::GetFileName(srcFileName));
		std::wstring tempPath = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetFolderPath(srcFileName));

		Oox2Odf::Converter converter(ooxUnpackedPath, get_oox_to_odf_conversion_type(srcFileName), L"", false, tempPath);

		converter.convert();
		converter.write(dstPath, tempPath, L"", L"");

		NSDirectory::DeleteDirectory(tempPath);

		tempPath = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetFolderPath(srcFileName));
		odf.reset(new cpdoccore::odf_reader::odf_document(dstPath, tempPath, L""));

		NSDirectory::DeleteDirectory(tempPath);
	}

	NSDirectory::DeleteDirectory(ooxUnpackedPath);

	return odf;
}

ODT2DOCX_ConversionEnvironment::ODT2DOCX_ConversionEnvironment(const std::wstring& filename)
	: mFilename(filename),
	mDocx(nullptr)
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

DOCX2ODT_ConvertsionEnvironment::DOCX2ODT_ConvertsionEnvironment(const std::wstring& filename)
	: mFilename(filename),
	mOdf(nullptr)
{ }

cpdoccore::odf_reader::odf_document* DOCX2ODT_ConvertsionEnvironment::GetDocument()
{
	return mOdf.get();
}
cpdoccore::odf_reader::office_document_content* DOCX2ODT_ConvertsionEnvironment::GetContent()
{
	return dynamic_cast<cpdoccore::odf_reader::office_document_content*>(mOdf->get_impl()->get_content());
}

cpdoccore::odf_reader::office_body* DOCX2ODT_ConvertsionEnvironment::GetBody()
{
	cpdoccore::odf_reader::office_document_content* content = GetContent();
	if (!content)
		return nullptr;

	return dynamic_cast<cpdoccore::odf_reader::office_body*>(content->office_body_.get());
}

void DOCX2ODT_ConvertsionEnvironment::SetUp()
{
	mOdf = convert_ooxml_to_odf(mFilename);
}

void DOCX2ODT_ConvertsionEnvironment::TearDown()
{
	if (mOdf)
		NSDirectory::DeleteDirectory(mOdf->get_folder());
}

PPTX2ODP_ConversionEnvironment::PPTX2ODP_ConversionEnvironment(const std::wstring& filename)
	: mFilename(filename),
	mOdf(nullptr)
{}

cpdoccore::odf_reader::odf_document* PPTX2ODP_ConversionEnvironment::GetDocument()
{
	return mOdf.get();
}

cpdoccore::odf_reader::office_document_content* PPTX2ODP_ConversionEnvironment::GetContent()
{
	return dynamic_cast<cpdoccore::odf_reader::office_document_content*>(mOdf->get_impl()->get_content());
}

cpdoccore::odf_reader::office_body* PPTX2ODP_ConversionEnvironment::GetBody()
{
	using namespace cpdoccore::odf_reader;

	office_document_content* content = GetContent();
	if (content)
	{
		office_body* body = dynamic_cast<office_body*>(content->office_body_.get());
		return body;
	}

	return nullptr;
}

cpdoccore::odf_reader::draw_page* PPTX2ODP_ConversionEnvironment::GetPage(size_t page_index)
{
	using namespace cpdoccore::odf_reader;

	cpdoccore::odf_reader::office_body* body = GetBody();

	if (body)
	{
		office_presentation* presentation = dynamic_cast<office_presentation*>(body->content_.get());

		if (presentation)
		{
			if (page_index >= presentation->pages_.size())
				return nullptr;

			draw_page* page = dynamic_cast<draw_page*>(presentation->pages_[page_index].get());
			return page;
		}
	}

	return nullptr;
}

void PPTX2ODP_ConversionEnvironment::SetUp()
{
	mOdf = convert_ooxml_to_odf(mFilename);
}

void PPTX2ODP_ConversionEnvironment::TearDown()
{
	if (mOdf)
		NSDirectory::DeleteDirectory(mOdf->get_folder());
}
