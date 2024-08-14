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
#include "common.h"
//
//#include "../../OfficeUtils/src/OfficeUtils.h"
//#include "Reader/Converter/pptx_package.h"
//
//#include "Writer/Converter/Converter.h"
//#include "Writer/Converter/PptxConverter.h"
//#include "Writer/Format/office_elements.h"
//
//boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument(const std::wstring& from, const std::wstring& temp, const std::wstring& tempUnpackedOdf)
//{
//	COfficeUtils oCOfficeUtils(NULL);
//	if (S_OK == oCOfficeUtils.ExtractToDirectory(from, tempUnpackedOdf, NULL, 0))
//		return boost::make_shared<cpdoccore::odf_reader::odf_document>(tempUnpackedOdf, temp, L"");
//	else
//		return nullptr;
//}
//
//boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf)
//{
//	cpdoccore::oox::package::pptx_document outputPptx;
//	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> conversionContext = boost::make_shared<cpdoccore::oox::pptx_conversion_context>(inputOdf.get());
//
//	conversionContext->set_output_document(&outputPptx);
//	conversionContext->set_font_directory(L"");
//
//	inputOdf->pptx_convert(*conversionContext);
//
//	return conversionContext;
//}
//
//ODP2OOX_AnimationEnvironment::ODP2OOX_AnimationEnvironment(const std::wstring& exampleFilename, boost::shared_ptr<cpdoccore::odf_reader::odf_document>* doc, boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>* context)
//	: testing::Environment()
//{
//	sExampleFilename = NSFile::GetFileName(exampleFilename);
//	mDocument = doc;
//	mContext = context;
//}
//
//ODP2OOX_AnimationEnvironment::~ODP2OOX_AnimationEnvironment()
//{
//}
//
//void ODP2OOX_AnimationEnvironment::SetUp()
//{
//	std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
//	std::wstring sExampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");
//
//	sFrom = sExampleFilesDirectory + FILE_SEPARATOR_STR + sExampleFilename;
//	sTemp = sFrom + _T(".tmp");
//	sTempUnpackedOdf = sTemp + CH_DIR("odf_unpacked");
//
//	NSDirectory::CreateDirectory(sTemp);
//	NSDirectory::CreateDirectory(sTempUnpackedOdf);
//
//	*mDocument = ReadOdfDocument(sFrom, sTemp, sTempUnpackedOdf);
//	*mContext = Convert(*mDocument);
//}
//
//void ODP2OOX_AnimationEnvironment::TearDown()
//{
//	NSDirectory::DeleteDirectory(sTemp);
//}
//
////////////////////////////////////////////////////////////////////////////
//
//OOX2ODP_AnimationEnvironment::OOX2ODP_AnimationEnvironment(const std::wstring& exampleFilename, boost::shared_ptr<Oox2Odf::Converter>* converter, cpdoccore::odf_writer::odp_conversion_context** context)
//{
//	mExampleFilename = exampleFilename;
//	mConverter = converter;
//	mContext = context;
//}
//
//void OOX2ODP_AnimationEnvironment::SetUp()
//{
//	std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
//	std::wstring exampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");
//
//	mFrom = exampleFilesDirectory + FILE_SEPARATOR_STR + mExampleFilename;
//	mTemp = mFrom + _T(".tmp");
//	mTempUnpackedOox = mTemp + CH_DIR("pptx_unpacked");
//
//	NSDirectory::CreateDirectory(mTemp);
//	NSDirectory::CreateDirectory(mTempUnpackedOox);
//
//	COfficeUtils oCOfficeUtils(NULL);
//	if (S_OK == oCOfficeUtils.ExtractToDirectory(mFrom, mTempUnpackedOox, NULL, 0))
//	{
//		*mConverter = boost::make_shared<Oox2Odf::Converter>(mTempUnpackedOox, _T("presentation"), L"", false, mTemp);
//
//		try
//		{
//			(*mConverter)->convert();
//			*mContext = dynamic_cast<cpdoccore::odf_writer::odp_conversion_context*>((*mConverter)->get_ooxConverter()->odf_context());
//		}
//		catch (...)
//		{
//			_CP_LOG << L"[ error ]: Failed to setup OOX2ODP_EntranceAnimationTestEnvironment";
//		}
//	}
//}
//
//void OOX2ODP_AnimationEnvironment::TearDown()
//{
//	NSDirectory::DeleteDirectory(mTemp);
//}
//
//const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetTimingRoot()
//{
//	using namespace cpdoccore::odf_writer;
//	draw_page* page = dynamic_cast<draw_page*>(mContext->current_slide().page_elm_.get());
//	if (!page)
//		return nullptr;
//
//	anim_par* timing_root = dynamic_cast<anim_par*>(page->animation_.get());
//	if (!timing_root)
//		return nullptr;
//
//	return timing_root;
//
//}
//
//const cpdoccore::odf_writer::anim_seq* OOX2ODP_AnimationTest::GetMainSequence()
//{
//	using namespace cpdoccore::odf_writer;
//	const anim_par* timing_root = GetTimingRoot();
//	if (!timing_root)
//		return nullptr;
//
//	if (timing_root->anim_seq_array_.size() < 1)
//		return nullptr;
//
//	const anim_seq* main_sequence = dynamic_cast<anim_seq*>(timing_root->anim_seq_array_.at(0).get());
//	if (!main_sequence)
//		return nullptr;
//
//	return main_sequence;
//}
//
//const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetMainSequenceParByIndex(size_t index)
//{
//	using namespace cpdoccore::odf_writer;
//	const anim_seq* main_sequence = GetMainSequence();
//
//	if (index >= main_sequence->anim_par_array_.size())
//		return nullptr;
//
//	const anim_par* par = dynamic_cast<anim_par*>(main_sequence->anim_par_array_[index].get());
//	if (!par)
//		return nullptr;
//
//	return par;
//}
//
//const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetInnerPar(const cpdoccore::odf_writer::anim_par* par)
//{
//	using namespace cpdoccore::odf_writer;
//
//	if (!par)
//		return nullptr;
//
//	if (par->anim_par_.size() == 0)
//		return nullptr;
//
//	const anim_par* inner_par = dynamic_cast<anim_par*>(par->anim_par_[0].get());
//
//	if (!inner_par)
//		return nullptr;
//
//	return inner_par;
//}
//
//const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetInnermostPar(const cpdoccore::odf_writer::anim_par* par)
//{
//	using namespace cpdoccore::odf_writer;
//
//	if (!par)
//		return nullptr;
//
//	const anim_par* innermost = GetInnerPar(par);
//
//	while (innermost->anim_par_.size())
//	{
//		innermost = dynamic_cast<anim_par*>(innermost->anim_par_[0].get());
//		if (!innermost)
//			return nullptr;
//	}
//
//	return innermost;
//}
//
//const cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_AnimationTest::GetContext() const
//{
//	return mContext;
//}

//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <tchar.h>

#include "D:\ONLYOFFICE\core\Common\OfficeFileFormatChecker.h"
#include "D:\ONLYOFFICE\core\OfficeUtils\src\OfficeUtils.h"
#include "D:\ONLYOFFICE\core\DesktopEditor\common\Directory.h"
#include "D:\ONLYOFFICE\core\OdfFile\Reader\Converter\ConvertOO2OOX.h"
#include "D:\ONLYOFFICE\core\OdfFile\Common\logging.h"

#if defined(_WIN64)
#pragma comment(lib, "D:/ONLYOFFICE/core/Common/3dParty/icu/win_64/build/icuuc.lib")
#pragma comment(lib, "D:/ONLYOFFICE/core/build/lib/win_64/DEBUG/graphics.lib")
#pragma comment(lib, "D:/ONLYOFFICE/core/build/lib/win_64/DEBUG/kernel.lib")
#pragma comment(lib, "D:/ONLYOFFICE/core/build/lib/win_64/DEBUG/UnicodeConverter.lib")
#pragma comment(lib, "D:/ONLYOFFICE/core/build/lib/win_64/DEBUG/CryptoPPLib.lib")

#elif defined (_WIN32)

#if defined(DEBUG)
#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/graphics.lib")
#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/kernel.lib")
#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/UnicodeConverter.dll")
#else
#pragma comment(lib, "../../../../build/lib/win_32/graphics.lib")
#pragma comment(lib, "../../../../build/lib/win_32/kernel.lib")
#pragma comment(lib, "../../../../build/lib/win_32/UnicodeConverter.dll")
#endif
#pragma comment(lib, "../../../../build/bin/icu/win_32/icuuc.lib")
#endif

HRESULT convert_single(std::wstring srcFileName)
{
	int nResult = 0;

	COfficeFileFormatChecker fileChecker(srcFileName);

	std::wstring dstPath = srcFileName;// + ; //xlsx pptx docx
	switch (fileChecker.nFileType)
	{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT:		dstPath += L"-tmp.docx"; break;

		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT:	dstPath += L"-tmp.xlsx"; break;

		case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT:	dstPath += L"-tmp.pptx"; break;

		default:
			return -1;
	}
	//---------------------------------------------------------------------------------------------------
	COfficeUtils oCOfficeUtils(NULL);

	std::wstring outputDir = NSDirectory::GetFolderPath(dstPath);
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
			return -2;
	}
	else // flat
	{
		srcTempPath = srcFileName;
	}
	_CP_LOG << L"[info] " << srcFileName << std::endl;

	nResult = ConvertODF2OOXml(srcTempPath, dstTempPath, L"C:\\Windows\\Fonts", srcTempPath2, L"");

	if (srcTempPath != srcFileName)
	{
		NSDirectory::DeleteDirectory(srcTempPath);
	}
	NSDirectory::DeleteDirectory(srcTempPath2);

	if (nResult == 0)
	{
		if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath, dstPath, true))
			nResult = -2;
	}

	NSDirectory::DeleteDirectory(dstTempPath);

	return nResult;
}

HRESULT convert_directory(std::wstring pathName)
{
	HRESULT hr = S_OK;

	std::vector<std::wstring> arFiles = NSDirectory::GetFiles(pathName, false);

	for (size_t i = 0; i < arFiles.size(); i++)
	{
		convert_single(arFiles[i]);
	}
	return S_OK;
}

ODT2DOCX_ConversionEnvironment::ODT2DOCX_ConversionEnvironment(const std::wstring& filename)
	: mFilename(filename)
{
}

void ODT2DOCX_ConversionEnvironment::SetUp()
{
	convert_single(mFilename);
}

void ODT2DOCX_ConversionEnvironment::TearDown()
{

}
