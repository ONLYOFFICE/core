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

#include "common.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "Reader/Converter/pptx_package.h"

#include "Writer/Converter/Converter.h"
#include "Writer/Converter/PptxConverter.h"
#include "Writer/Format/office_elements.h"

boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument(const std::wstring& from, const std::wstring& temp, const std::wstring& tempUnpackedOdf)
{
	COfficeUtils oCOfficeUtils(NULL);
	if (S_OK == oCOfficeUtils.ExtractToDirectory(from, tempUnpackedOdf, NULL, 0))
		return boost::make_shared<cpdoccore::odf_reader::odf_document>(tempUnpackedOdf, temp, L"");
	else
		return nullptr;
}

boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf)
{
	cpdoccore::oox::package::pptx_document outputPptx;
	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> conversionContext = boost::make_shared<cpdoccore::oox::pptx_conversion_context>(inputOdf.get());

	conversionContext->set_output_document(&outputPptx);
	conversionContext->set_font_directory(L"");

	inputOdf->pptx_convert(*conversionContext);

	return conversionContext;
}

ODP2OOX_AnimationEnvironment::ODP2OOX_AnimationEnvironment(const std::wstring& exampleFilename, boost::shared_ptr<cpdoccore::odf_reader::odf_document>* doc, boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>* context)
	: testing::Environment()
{
	sExampleFilename = NSFile::GetFileName(exampleFilename);
	mDocument = doc;
	mContext = context;
}

ODP2OOX_AnimationEnvironment::~ODP2OOX_AnimationEnvironment()
{
}

void ODP2OOX_AnimationEnvironment::SetUp()
{
	std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
	std::wstring sExampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");

	sFrom = sExampleFilesDirectory + FILE_SEPARATOR_STR + sExampleFilename;
	sTemp = sFrom + _T(".tmp");
	sTempUnpackedOdf = sTemp + CH_DIR("odf_unpacked");

	NSDirectory::CreateDirectory(sTemp);
	NSDirectory::CreateDirectory(sTempUnpackedOdf);

	*mDocument = ReadOdfDocument(sFrom, sTemp, sTempUnpackedOdf);
	*mContext = Convert(*mDocument);
}

void ODP2OOX_AnimationEnvironment::TearDown()
{
	NSDirectory::DeleteDirectory(sTemp);
}

//////////////////////////////////////////////////////////////////////////

OOX2ODP_AnimationEnvironment::OOX2ODP_AnimationEnvironment(const std::wstring& exampleFilename, boost::shared_ptr<Oox2Odf::Converter>* converter, cpdoccore::odf_writer::odp_conversion_context** context)
{
	mExampleFilename = exampleFilename;
	mConverter = converter;
	mContext = context;
}

void OOX2ODP_AnimationEnvironment::SetUp()
{
	std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
	std::wstring exampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");

	mFrom = exampleFilesDirectory + FILE_SEPARATOR_STR + mExampleFilename;
	mTemp = mFrom + _T(".tmp");
	mTempUnpackedOox = mTemp + CH_DIR("pptx_unpacked");

	NSDirectory::CreateDirectory(mTemp);
	NSDirectory::CreateDirectory(mTempUnpackedOox);

	COfficeUtils oCOfficeUtils(NULL);
	if (S_OK == oCOfficeUtils.ExtractToDirectory(mFrom, mTempUnpackedOox, NULL, 0))
	{
		*mConverter = boost::make_shared<Oox2Odf::Converter>(mTempUnpackedOox, _T("presentation"), L"", false, mTemp);

		try
		{
			(*mConverter)->convert();
			*mContext = dynamic_cast<cpdoccore::odf_writer::odp_conversion_context*>((*mConverter)->get_ooxConverter()->odf_context());
		}
		catch (...)
		{
			_CP_LOG << L"[ error ]: Failed to setup OOX2ODP_EntranceAnimationTestEnvironment";
		}
	}
}

void OOX2ODP_AnimationEnvironment::TearDown()
{
	NSDirectory::DeleteDirectory(mTemp);
}

const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetTimingRoot()
{
	using namespace cpdoccore::odf_writer;
	draw_page* page = dynamic_cast<draw_page*>(mContext->current_slide().page_elm_.get());
	if (!page)
		return nullptr;

	anim_par* timing_root = dynamic_cast<anim_par*>(page->animation_.get());
	if (!timing_root)
		return nullptr;

	return timing_root;

}

const cpdoccore::odf_writer::anim_seq* OOX2ODP_AnimationTest::GetMainSequence()
{
	using namespace cpdoccore::odf_writer;
	const anim_par* timing_root = GetTimingRoot();
	if (!timing_root)
		return nullptr;

	if (timing_root->anim_seq_array_.size() < 1)
		return nullptr;

	const anim_seq* main_sequence = dynamic_cast<anim_seq*>(timing_root->anim_seq_array_.at(0).get());
	if (!main_sequence)
		return nullptr;

	return main_sequence;
}

const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetMainSequenceParByIndex(size_t index)
{
	using namespace cpdoccore::odf_writer;
	const anim_seq* main_sequence = GetMainSequence();

	if (index >= main_sequence->anim_par_array_.size())
		return nullptr;

	const anim_par* par = dynamic_cast<anim_par*>(main_sequence->anim_par_array_[index].get());
	if (!par)
		return nullptr;

	return par;
}

const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetInnerPar(const cpdoccore::odf_writer::anim_par* par)
{
	using namespace cpdoccore::odf_writer;

	if (!par)
		return nullptr;

	const anim_par* inner_par = dynamic_cast<anim_par*>(par->anim_par_.get());

	if (!inner_par)
		return nullptr;

	return inner_par;
}

const cpdoccore::odf_writer::anim_par* OOX2ODP_AnimationTest::GetInnermostPar(const cpdoccore::odf_writer::anim_par* par)
{
	using namespace cpdoccore::odf_writer;

	if (!par)
		return nullptr;

	const anim_par* innermost = GetInnerPar(par);

	while (innermost->anim_par_)
	{
		innermost = dynamic_cast<anim_par*>(innermost->anim_par_.get());
		if (!innermost)
			return nullptr;
	}

	return innermost;
}

const cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_AnimationTest::GetContext() const
{
	return mContext;
}
