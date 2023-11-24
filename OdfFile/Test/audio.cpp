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

#include "audio.h"


#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "Writer/Converter/Converter.h"
#include "Writer/Converter/PptxConverter.h"
#include "Writer/Format/office_elements.h"

boost::shared_ptr<cpdoccore::odf_reader::odf_document>		ODP2OOX_AnimationAudioEnvironment::sInputOdf;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	ODP2OOX_AnimationAudioEnvironment::sConverionContext;

ODP2OOX_AnimationAudioEnvironment::ODP2OOX_AnimationAudioEnvironment()
	: ODP2OOX_AnimationEnvironment(
		L"ExampleFiles" FILE_SEPARATOR_STR "audio.odp",
		&sInputOdf,
		&sConverionContext)
{
}

const cpdoccore::oox::pptx_animation_context& ODP2OOX_AnimationAudioEnvironment::GetAnimationContext()
{
	return sConverionContext->get_slide_context().get_animation_context();
}

void ODP2OOX_AnimationAudioTest::SetUp()
{
	mAnimationContext = &ODP2OOX_AnimationAudioEnvironment::GetAnimationContext();
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& ODP2OOX_AnimationAudioTest::GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimParArray[0];
}

const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr& ODP2OOX_AnimationAudioTest::GetMainSequence()
{
	if(mAnimationContext->get_root_par_animation()->AnimSeq.size())
		return mAnimationContext->get_root_par_animation()->AnimSeq[0];
	return nullptr;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array& ODP2OOX_AnimationAudioTest::GetMainSequenceArray()
{
	return GetMainSequence()->AnimParArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_AnimationAudioTest::GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimationActionArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr ODP2OOX_AnimationAudioTest::GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	using namespace cpdoccore::oox;
	cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr inner = par;

	while (inner->AnimParArray.size())
		inner = GetInnerPar(inner);

	return inner;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_AnimationAudioTest::GetAnimationActionsByIndex(size_t index)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();
	const pptx_animation_context::Impl::_animation_element_array& actions = GetInnermostPar(main_seq[index])->AnimationActionArray;

	return actions;
}

TEST_F(ODP2OOX_AnimationAudioTest, r_embed_id)
{
	using namespace cpdoccore::oox;

	const auto& mainArray = GetMainSequenceArray();
	ASSERT_GE(mainArray.size(), 1);
	const auto& actions = GetInnermostPar(mainArray[0])->AnimationActionArray;
	ASSERT_EQ(actions.size(), 4);
	const auto audio = dynamic_cast<pptx_animation_context::Impl::_audio*>(actions[3].get()); ;
	ASSERT_NE(audio, nullptr);
	
	const std::wstring rIdExp = L"aId1";

	EXPECT_EQ(audio->RId.value(), rIdExp);
}

TEST_F(ODP2OOX_AnimationAudioTest, name)
{
	using namespace cpdoccore::oox;

	const auto& mainArray = GetMainSequenceArray();
	ASSERT_GE(mainArray.size(), 1);
	const auto& actions = GetInnermostPar(mainArray[0])->AnimationActionArray;
	ASSERT_EQ(actions.size(), 4);
	const auto audio = dynamic_cast<pptx_animation_context::Impl::_audio*>(actions[3].get()); ;
	ASSERT_NE(audio, nullptr);

	const std::wstring nameExp = L"apert.wav";

	EXPECT_EQ(audio->Name.value(), nameExp);
}

//////////////////////////////////////////////////////////////////////////

boost::shared_ptr<Oox2Odf::Converter> OOX2ODP_AudioAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_AudioAnimationEnvironment::mContext;

OOX2ODP_AudioAnimationEnvironment::OOX2ODP_AudioAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"audio.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_AudioAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_AudioAnimationTest::SetUp()
{
	mContext = OOX2ODP_AudioAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_AudioAnimationTest, a_test)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_audio* audio = GetAnimationBehaviourByIndex<anim_audio>(GetInnermostPar(GetMainSequenceParByIndex(0)), 1);
	ASSERT_NE(audio, nullptr);

	const std::wstring xpathExp = L"Media/media1.wav";

	EXPECT_EQ(audio->audio_attlist_.xlink_href_.value(), xpathExp);
}