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

#include "interactions.h"

#include "Writer/Format/anim_elements.h"

boost::shared_ptr<Oox2Odf::Converter> OOX2ODP_InteractionAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_InteractionAnimationEnvironment::mContext;

OOX2ODP_InteractionAnimationEnvironment::OOX2ODP_InteractionAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"interaction.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_InteractionAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_InteractionAnimationTest::SetUp()
{
	mContext = OOX2ODP_InteractionAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	EXPECT_NE(root, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);

	const int durExp = -1;

	EXPECT_EQ(root->attlist_.smil_dur_.value().get_value(), durExp);
}

TEST_F(OOX2ODP_InteractionAnimationTest, timing_root_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::parse(L"timing-root");

	EXPECT_EQ(root->attlist_.presentation_node_type_.value().get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());

	EXPECT_NE(interactive, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::parse(L"interactive-sequence");

	EXPECT_NE(interactive, nullptr);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_restart)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const std::wstring restartExp = L"";

	EXPECT_NE(interactive->attlist_.smil_restart_.value(), restartExp);
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const smil_fill fillExp = smil_fill::_remove;

	EXPECT_NE(interactive->attlist_.smil_fill_.value().get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_InteractionAnimationTest, interactive_seq_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* root = GetTimingRoot();
	ASSERT_NE(root, nullptr);
	const anim_seq* interactive = dynamic_cast<anim_seq*>(root->anim_seq_array_[0].get());
	ASSERT_NE(interactive, nullptr);

	const std::wstring restartExp = L"";

	EXPECT_NE(interactive->attlist_.smil_begin_.value(), restartExp);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<cpdoccore::odf_reader::odf_document>		ODP2OOX_AnimationPlayAudioEnvironment::sInputOdf;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	ODP2OOX_AnimationPlayAudioEnvironment::sConverionContext;

ODP2OOX_AnimationPlayAudioEnvironment::ODP2OOX_AnimationPlayAudioEnvironment()
	: ODP2OOX_AnimationEnvironment(L"ExampleFiles" FILE_SEPARATOR_STR "playAudio.odp",
		&sInputOdf,
		&sConverionContext)
{

}

cpdoccore::oox::pptx_conversion_context& ODP2OOX_AnimationPlayAudioEnvironment::GetConversionContext()
{
	return *sConverionContext;
}

void ODP2OOX_AnimationPlayAudioTest::SetUp()
{
	mConversionContext = &ODP2OOX_AnimationPlayAudioEnvironment::GetConversionContext();
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, mediaitems_size)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto items = mConversionContext->get_mediaitems()->items();
	const size_t itemsSizeExp = 1;

	EXPECT_EQ(items.size(), itemsSizeExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, rels_size)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto rels_ = mConversionContext->current_slide().Rels().relationships();
	const size_t relsSizeExp = 2;

	EXPECT_EQ(rels_.size(), relsSizeExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_slide_rel_type)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto slideRel = mConversionContext->current_slide().Rels().relationships()[0];
	const std::wstring typeExp = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout";

	EXPECT_EQ(slideRel.type(), typeExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_slide_rel_target)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto slideRel = mConversionContext->current_slide().Rels().relationships()[0];
	const std::wstring targetExp = L"../slideLayouts/slideLayout1.xml";

	EXPECT_EQ(slideRel.target(), targetExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_slide_rel_target_mode)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto slideRel = mConversionContext->current_slide().Rels().relationships()[0];
	const std::wstring targetModeExp = L"";

	EXPECT_EQ(slideRel.target_mode(), targetModeExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_notes_rel_type)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto notesRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring typeExp = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide";

	EXPECT_EQ(notesRel.type(), typeExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_notes_rel_target)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto notesRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetExp = L"../notesSlides/notesSlide1.xml";

	EXPECT_EQ(notesRel.target(), targetExp);
}

TEST_F(ODP2OOX_AnimationPlayAudioTest, audio_notes_rel_target_mode)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto notesRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetModeExp = L"";

	EXPECT_EQ(notesRel.target_mode(), targetModeExp);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<cpdoccore::odf_reader::odf_document>		ODP2OOX_AnimationOpenDocumentEnvironment::sInputOdf;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	ODP2OOX_AnimationOpenDocumentEnvironment::sConverionContext;

ODP2OOX_AnimationOpenDocumentEnvironment::ODP2OOX_AnimationOpenDocumentEnvironment()
	: ODP2OOX_AnimationEnvironment(L"ExampleFiles" FILE_SEPARATOR_STR "openDocument.odp",
		&sInputOdf,
		&sConverionContext)
{
}

cpdoccore::oox::pptx_conversion_context& ODP2OOX_AnimationOpenDocumentEnvironment::GetConversionContext()
{
	return *sConverionContext;
}

void ODP2OOX_AnimationOpenDocumentTest::SetUp()
{
	mConversionContext = &ODP2OOX_AnimationOpenDocumentEnvironment::GetConversionContext();
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, mediaitems_size)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto items = mConversionContext->get_mediaitems()->items();
	const size_t itemsSizeExp = 1;

	EXPECT_EQ(items.size(), itemsSizeExp);
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, rels_size)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto rels_ = mConversionContext->current_slide().Rels().relationships();
	const size_t relsSizeExp = 3;

	EXPECT_EQ(rels_.size(), 3);
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, document_rel_id)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto docRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring idExp = L"hId1";

	EXPECT_EQ(docRel.id(), idExp);
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, document_rel_type)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto docRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring typeExp = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";

	EXPECT_EQ(docRel.type(), typeExp);
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, document_rel_target)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto docRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetExp = L"motion.odp";

	EXPECT_EQ(docRel.target(), targetExp);
}

TEST_F(ODP2OOX_AnimationOpenDocumentTest, document_rel_target_mode)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto docRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetModeExp = L"External";

	EXPECT_EQ(docRel.target_mode(), targetModeExp);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<cpdoccore::odf_reader::odf_document>		ODP2OOX_AnimationRunProgramEnvironment::sInputOdf;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	ODP2OOX_AnimationRunProgramEnvironment::sConverionContext;

ODP2OOX_AnimationRunProgramEnvironment::ODP2OOX_AnimationRunProgramEnvironment()
	: ODP2OOX_AnimationEnvironment(L"ExampleFiles" FILE_SEPARATOR_STR "runProgram.odp",
		&sInputOdf,
		&sConverionContext)
{
}

cpdoccore::oox::pptx_conversion_context& ODP2OOX_AnimationRunProgramEnvironment::GetConversionContext()
{
	return *sConverionContext;
}

void ODP2OOX_AnimationRunProgramTest::SetUp()
{
	mConversionContext = &ODP2OOX_AnimationRunProgramEnvironment::GetConversionContext();
}

TEST_F(ODP2OOX_AnimationRunProgramTest, rels_size)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto rels_ = mConversionContext->current_slide().Rels().relationships();
	const size_t relsSizeExp = 3;

	EXPECT_EQ(rels_.size(), 3);
}

TEST_F(ODP2OOX_AnimationRunProgramTest, program_rel_id)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto programRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring idExp = L"hId1";

	EXPECT_EQ(programRel.id(), idExp);
}

TEST_F(ODP2OOX_AnimationRunProgramTest, program_rel_type)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto programRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring typeExp = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";

	EXPECT_EQ(programRel.type(), typeExp);
}

TEST_F(ODP2OOX_AnimationRunProgramTest, program_rel_target)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto programRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetExp = L"../../../../build_tools/out/win_64/onlyoffice/core/x2t.exe";

	EXPECT_EQ(programRel.target(), targetExp);
}

TEST_F(ODP2OOX_AnimationRunProgramTest, program_rel_target_mode)
{
	ASSERT_NE(mConversionContext, nullptr);

	auto programRel = mConversionContext->current_slide().Rels().relationships()[1];
	const std::wstring targetModeExp = L"External";

	EXPECT_EQ(programRel.target_mode(), targetModeExp);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<Oox2Odf::Converter>			OOX2ODP_PlayAudioAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context*	OOX2ODP_PlayAudioAnimationEnvironment::mContext;

OOX2ODP_PlayAudioAnimationEnvironment::OOX2ODP_PlayAudioAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"playAudio.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_PlayAudioAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_PlayAudioAnimationTest::SetUp()
{
	mContext = OOX2ODP_PlayAudioAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_PlayAudioAnimationTest, mediaitems_size)
{
	ASSERT_NE(mContext, nullptr);

	auto items = mContext->get_deferred_hyperlinks();
	const size_t mediaitemsSizeExp = 1;

	EXPECT_EQ(items.size(), mediaitemsSizeExp);
}

TEST_F(OOX2ODP_PlayAudioAnimationTest, mediaitem_path)
{
	ASSERT_NE(mContext, nullptr);
	auto items = mContext->get_deferred_hyperlinks();
	ASSERT_GE(items.size(), 1);

	auto audioItem = items[0];
	const std::wstring audioItemPath = L"../../../X2tConverter/test/win32Test/Res/media_example";

	EXPECT_EQ(audioItem.second, audioItemPath);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<Oox2Odf::Converter>			OOX2ODP_OpenDocumentAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context*	OOX2ODP_OpenDocumentAnimationEnvironment::mContext;

OOX2ODP_OpenDocumentAnimationEnvironment::OOX2ODP_OpenDocumentAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"openDocument.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_OpenDocumentAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_OpenDocumentAnimationTest::SetUp()
{
	mContext = OOX2ODP_OpenDocumentAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_OpenDocumentAnimationTest, mediaitems_size)
{
	ASSERT_NE(mContext, nullptr);

	auto items = mContext->get_deferred_hyperlinks();
	const size_t mediaitemsSizeExp = 1;

	EXPECT_EQ(items.size(), mediaitemsSizeExp);
}

TEST_F(OOX2ODP_OpenDocumentAnimationTest, mediaitem_path)
{
	ASSERT_NE(mContext, nullptr);
	auto items = mContext->get_deferred_hyperlinks();
	ASSERT_GE(items.size(), 1);

	auto audioItem = items[0];
	const std::wstring audioItemPath = L"entrance";

	EXPECT_EQ(audioItem.second, audioItemPath);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<Oox2Odf::Converter>			OOX2ODP_RunProgramAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context*	OOX2ODP_RunProgramAnimationEnvironment::mContext;

OOX2ODP_RunProgramAnimationEnvironment::OOX2ODP_RunProgramAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"runProgram.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_RunProgramAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_RunProgramAnimationTest::SetUp()
{
	mContext = OOX2ODP_RunProgramAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_RunProgramAnimationTest, mediaitems_size)
{
	ASSERT_NE(mContext, nullptr);

	auto items = mContext->get_deferred_hyperlinks();
	const size_t mediaitemsSizeExp = 1;

	EXPECT_EQ(items.size(), mediaitemsSizeExp);
}

TEST_F(OOX2ODP_RunProgramAnimationTest, mediaitem_path)
{
	ASSERT_NE(mContext, nullptr);
	auto items = mContext->get_deferred_hyperlinks();
	ASSERT_GE(items.size(), 1);

	auto audioItem = items[0];
	const std::wstring audioItemPath = L"../../../../build_tools/out/win_64/onlyoffice/core/x2t";

	EXPECT_EQ(audioItem.second, audioItemPath);
}