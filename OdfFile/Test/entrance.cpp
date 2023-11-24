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

#include "entrance.h"

#include "common.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include "Writer/Converter/Converter.h"
#include "Writer/Converter/PptxConverter.h"
#include "Writer/Format/office_elements.h"

boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> ODP2OOX_AnimationEntranceEnvironment::sConverionContext;
boost::shared_ptr<cpdoccore::odf_reader::odf_document> ODP2OOX_AnimationEntranceEnvironment::sInputOdf;

ODP2OOX_AnimationEntranceEnvironment::ODP2OOX_AnimationEntranceEnvironment()
	: ODP2OOX_AnimationEnvironment(
		L"ExampleFiles" FILE_SEPARATOR_STR "entrance.odp",
		&sInputOdf,
		&sConverionContext)
{

}

const cpdoccore::oox::pptx_animation_context& ODP2OOX_AnimationEntranceEnvironment::GetAnimationContext()
{
	return sConverionContext->get_slide_context().get_animation_context();
}

void ODP2OOX_EntranceAnimationTest::SetUp()
{
	mAnimationContext = &ODP2OOX_AnimationEntranceEnvironment::GetAnimationContext();
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& ODP2OOX_EntranceAnimationTest::GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimParArray[0];
}

const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr& ODP2OOX_EntranceAnimationTest::GetMainSequence()
{
	if(mAnimationContext->get_root_par_animation()->AnimSeq.size())
		return mAnimationContext->get_root_par_animation()->AnimSeq[0];
	return nullptr;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array& ODP2OOX_EntranceAnimationTest::GetMainSequenceArray()
{
	return mAnimationContext->get_root_par_animation()->AnimSeq[0]->AnimParArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_EntranceAnimationTest::GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimationActionArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr ODP2OOX_EntranceAnimationTest::GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	using namespace cpdoccore::oox;
	cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr inner = par;

	while (inner->AnimParArray.size())
	{
		inner = GetInnerPar(inner);
	}

	return inner;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_EntranceAnimationTest::GetAnimationActionsByIndex(size_t index)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();
	const pptx_animation_context::Impl::_animation_element_array& actions = GetInnermostPar(main_seq[index])->AnimationActionArray;

	return actions;
}

TEST_F(ODP2OOX_EntranceAnimationTest, timing_root_par)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_ptr& root = mAnimationContext->get_root_par_animation();

	const std::wstring nodeTypeExp = L"tmRoot";

	EXPECT_EQ(root->NodeType.value(), nodeTypeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, main_sequence)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_seq_animation_ptr& main_seq = GetMainSequence();

	const std::wstring nodeTypeExp = L"mainSeq";

	EXPECT_EQ(main_seq->PresentationNodeType.value(), nodeTypeExp);
	EXPECT_FALSE(main_seq->Duration.has_value());
}

TEST_F(ODP2OOX_EntranceAnimationTest, main_sequence_par_animations)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& par_animations = mAnimationContext->get_root_par_animation()->AnimSeq[0]->AnimParArray;

	const std::wstring delayExp = L"indefinite";

	for (size_t i = 0; i < par_animations.size(); i++)
	{
		EXPECT_EQ(par_animations[i]->Delay.value(), delayExp);
	}
}

TEST_F(ODP2OOX_EntranceAnimationTest, main_sequence_inner_par_animation)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();

	std::wstring delayExp = L"0";

	for (size_t i = 0; i < main_seq.size(); i++)
	{
		const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnerPar(main_seq[i]);

		EXPECT_EQ(par->Delay.value(), delayExp);
	}
}

TEST_F(ODP2OOX_EntranceAnimationTest, main_sequence_innermost_par_animation)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();

	std::wstring delayExp = L"0";
	std::wstring fillExp = L"hold";
	std::wstring nodeTypeExp = L"clickEffect";
	std::wstring presetClassExp = L"entr";

	for (size_t i = 0; i < main_seq.size(); i++)
	{
		const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(main_seq[i]);

		EXPECT_EQ(par->Delay.value(), delayExp);
		EXPECT_EQ(par->Fill.value(), fillExp);
		EXPECT_EQ(par->NodeType.value(), nodeTypeExp);
		EXPECT_EQ(par->PresetClass.value(), presetClassExp);
	}
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_appear_acitons_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 1;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_actions_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_action_set_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const int durationExp = 1;

	EXPECT_EQ(set->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_action_set_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_action_set_delay)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_action_set_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring attributeNameExp = L"style.visibility";

	EXPECT_EQ(set->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, enerance_appear_action_set_to_value)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring toValueExp = L"visible";

	EXPECT_EQ(set->ToValue.value(), toValueExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_actions_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 3;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());

	EXPECT_TRUE(animate1 != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());

	EXPECT_TRUE(animate2 != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const int durationExp = 1;

	EXPECT_EQ(set->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set_delay)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring delayExp = L"0";

	EXPECT_EQ(set->Delay.value(), delayExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring attributeNameExp = L"style.visibility";

	EXPECT_EQ(set->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_set_to_value)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring toValueExp = L"visible";

	EXPECT_EQ(set->ToValue.value(), toValueExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_calc_mode)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring calcModeExp = L"lin";

	EXPECT_EQ(animate1->CalcMode.value(), calcModeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_value_type)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring valueTypeExp = L"num";

	EXPECT_EQ(animate1->ValueType.value(), valueTypeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const int durationExp = 500;

	EXPECT_EQ(animate1->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring attributeNameExp = L"ppt_x";

	EXPECT_EQ(animate1->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_additive)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring additiveExp = L"repl";

	EXPECT_EQ(animate1->Additive.value(), additiveExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_1_key_points)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	std::vector<pptx_animation_context::Impl::_anim::_keypoint> keypointsExp;
	keypointsExp.push_back(pptx_animation_context::Impl::_anim::_keypoint(0, L"#ppt_x", boost::none));
	keypointsExp.push_back(pptx_animation_context::Impl::_anim::_keypoint(100000, L"#ppt_x", boost::none));

	EXPECT_EQ(animate1->KeypointArray->size(), keypointsExp.size());
	for (size_t i = 0; i < animate1->KeypointArray->size(); i++)
	{
		EXPECT_EQ(animate1->KeypointArray->at(i).Time, keypointsExp[i].Time);
		EXPECT_EQ(animate1->KeypointArray->at(i).Value, keypointsExp[i].Value);
		EXPECT_EQ(animate1->KeypointArray->at(i).Fmla.has_value(), keypointsExp[i].Fmla.has_value());
		EXPECT_EQ(animate1->KeypointArray->at(i).Fmla.value_or(L""), keypointsExp[i].Fmla.value_or(L""));
	}
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_calc_mode)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring calcModeExp = L"lin";

	EXPECT_EQ(animate2->CalcMode.value(), calcModeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_value_type)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring valueTypeExp = L"num";

	EXPECT_EQ(animate2->ValueType.value(), valueTypeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const int durationExp = 500;

	EXPECT_EQ(animate2->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring attributeNameExp = L"ppt_y";

	EXPECT_EQ(animate2->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_additive)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring additiveExp = L"repl";

	EXPECT_EQ(animate2->Additive.value(), additiveExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_fly_in_action_animate_2_key_points)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	std::vector<pptx_animation_context::Impl::_anim::_keypoint> keypointsExp;
	keypointsExp.push_back(pptx_animation_context::Impl::_anim::_keypoint(0, L"1+#ppt_h/2", boost::none));
	keypointsExp.push_back(pptx_animation_context::Impl::_anim::_keypoint(100000, L"#ppt_y", boost::none));

	EXPECT_EQ(animate2->KeypointArray->size(), keypointsExp.size());
	for (size_t i = 0; i < animate2->KeypointArray->size(); i++)
	{
		EXPECT_EQ(animate2->KeypointArray->at(i).Time, keypointsExp[i].Time);
		EXPECT_EQ(animate2->KeypointArray->at(i).Value, keypointsExp[i].Value);
		EXPECT_EQ(animate2->KeypointArray->at(i).Fmla.has_value(), keypointsExp[i].Fmla.has_value());
		EXPECT_EQ(animate2->KeypointArray->at(i).Fmla.value_or(L""), keypointsExp[i].Fmla.value_or(L""));
	}
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_actions_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 2;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_anim_effect)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	EXPECT_TRUE(animEffect != nullptr);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_anim_effect_filter)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"blinds(horizontal)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_anim_effect_transition)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring transitionExp = L"in";

	EXPECT_EQ(animEffect->Transition.value(), transitionExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_venetian_blinds_anim_effect_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const int durationExp = 500;

	EXPECT_EQ(animEffect->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_transition_filter_blinds_wipe_horizontal)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"blinds(horizontal)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_transition_filter_iris_wipe_rectangle)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 3;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"box(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_transition_filter_checker_board_wipe_across)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 4;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"checkerboard(across)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EntranceAnimationTest, entrance_transition_filter_checker_iris_wipe_diamond)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 8;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"diamond(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

//////////////////////////////////////////////////////////////////////////
// OOX2ODP

boost::shared_ptr<Oox2Odf::Converter> OOX2ODP_EntranceAnimationEnvironment::mConverter;
cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_EntranceAnimationEnvironment::mContext;

OOX2ODP_EntranceAnimationEnvironment::OOX2ODP_EntranceAnimationEnvironment()
	: OOX2ODP_AnimationEnvironment(L"entrance.pptx", &mConverter, &mContext)
{
}

cpdoccore::odf_writer::odp_conversion_context* OOX2ODP_EntranceAnimationEnvironment::GetContext()
{
	return mContext;
}

void OOX2ODP_EntranceAnimationTest::SetUp()
{
	mContext = OOX2ODP_EntranceAnimationEnvironment::GetContext();
}

TEST_F(OOX2ODP_EntranceAnimationTest, timing_root_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* timing_root = GetTimingRoot();
	ASSERT_NE(timing_root, nullptr);
	
	const presentation_node_type NodeTypeExp = presentation_node_type::timing_root;

	EXPECT_EQ(timing_root->attlist_.presentation_node_type_->get_type(), NodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, main_sequence_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_seq* main_sequence = GetMainSequence();
	ASSERT_NE(main_sequence, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::main_sequence;

	EXPECT_EQ(main_sequence->attlist_.presentation_node_type_->get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, main_sequence_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_seq* main_sequence = GetMainSequence();
	ASSERT_NE(main_sequence, nullptr);

	const odf_types::clockvalue durationExp(-1);

	EXPECT_EQ(main_sequence->attlist_.smil_dur_->get_value(), durationExp.get_value());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_outer_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(0);
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"next";

	EXPECT_EQ(par->attlist_.smil_begin_.get_value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_outer_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(0);
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_inner_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_inner_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_innermost_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_innermost_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_innermost_par_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::on_click;

	EXPECT_EQ(par->attlist_.presentation_node_type_.value_or(presentation_node_type::none).get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_innermost_par_preset_class)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const preset_class presetClassExp = preset_class::entrance;

	EXPECT_EQ(par->par_attlist_.presentation_preset_class_.value_or(preset_class::custom).get_type(), presetClassExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_innermost_par_preset_id)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(0));
	ASSERT_NE(par, nullptr);

	const preset_id presetIdExp = preset_id::ooo_entrance_appear;

	EXPECT_EQ(par->par_attlist_.presentation_preset_id_.value_or(preset_id::none).get_type(), presetIdExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_set_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_set* set = GetAnimationBehaviourByIndex<anim_set>(GetInnermostPar(GetMainSequenceParByIndex(0)), 0);
	ASSERT_NE(set, nullptr);

	const clockvalue durationExp = clockvalue(1);

	EXPECT_EQ(set->common_attlist_.smil_dur_->get_value(), durationExp.get_value());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_set_attribute_name)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_set* set = GetAnimationBehaviourByIndex<anim_set>(GetInnermostPar(GetMainSequenceParByIndex(0)), 0);
	ASSERT_NE(set, nullptr);

	const smil_attribute_name attributeNameExp = smil_attribute_name::visibility;

	EXPECT_EQ(set->common_attlist_.smil_attribute_name_->get_type(), attributeNameExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_set_to)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_set* set = GetAnimationBehaviourByIndex<anim_set>(GetInnermostPar(GetMainSequenceParByIndex(0)), 0);
	ASSERT_NE(set, nullptr);

	const std::wstring toExp = L"visible";

	EXPECT_EQ(set->set_attlist_.smil_to_.value_or(L"Empty"), toExp);
}

////////////////////////////////////////////////////////////

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_outer_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(1);
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"next";

	EXPECT_EQ(par->attlist_.smil_begin_.get_value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_outer_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(1);
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_inner_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_inner_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_innermost_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_innermost_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_innermost_par_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::on_click;

	EXPECT_EQ(par->attlist_.presentation_node_type_.value_or(presentation_node_type::none).get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_innermost_par_preset_class)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const preset_class presetClassExp = preset_class::entrance;

	EXPECT_EQ(par->par_attlist_.presentation_preset_class_.value_or(preset_class::custom).get_type(), presetClassExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fade_in_innermost_par_preset_id)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(1));
	ASSERT_NE(par, nullptr);

	const preset_id presetIdExp = preset_id::ooo_entrance_fade_in;

	EXPECT_EQ(par->par_attlist_.presentation_preset_id_.value_or(preset_id::none).get_type(), presetIdExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_transition_filter_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_transitionFilter* transitionFilter = GetAnimationBehaviourByIndex<anim_transitionFilter>(GetInnermostPar(GetMainSequenceParByIndex(1)), 1);
	ASSERT_NE(transitionFilter, nullptr);

	const clockvalue durationExp = clockvalue(500);

	EXPECT_EQ(transitionFilter->common_attlist_.smil_dur_.value_or(clockvalue(-1)).get_value(), durationExp.get_value());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_transition_filter_subtype)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_transitionFilter* transitionFilter = GetAnimationBehaviourByIndex<anim_transitionFilter>(GetInnermostPar(GetMainSequenceParByIndex(1)), 1);
	ASSERT_NE(transitionFilter, nullptr);

	const std::wstring subtypeExp = L"crossfade";

	EXPECT_EQ(transitionFilter->filter_attlist_.smil_subtype_.value_or(L"Empty"), subtypeExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_transition_filter_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_transitionFilter* transitionFilter = GetAnimationBehaviourByIndex<anim_transitionFilter>(GetInnermostPar(GetMainSequenceParByIndex(1)), 1);
	ASSERT_NE(transitionFilter, nullptr);

	const smil_transition_type typeExp = smil_transition_type::fade;

	EXPECT_EQ(transitionFilter->filter_attlist_.smil_type_.value().get_type(), typeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, appear_transition_filter_mode)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_transitionFilter* transitionFilter = GetAnimationBehaviourByIndex<anim_transitionFilter>(GetInnermostPar(GetMainSequenceParByIndex(1)), 1);
	ASSERT_NE(transitionFilter, nullptr);

	const std::wstring modeExp = L"in";

	EXPECT_EQ(transitionFilter->filter_attlist_.smil_mode_.value_or(L"Empty"), modeExp);
}

////////////////////////////////////////////////////////////

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_outer_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(2);
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"next";

	EXPECT_EQ(par->attlist_.smil_begin_.get_value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_outer_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(2);
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_inner_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_inner_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_innermost_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_innermost_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_innermost_par_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::on_click;

	EXPECT_EQ(par->attlist_.presentation_node_type_.value_or(presentation_node_type::none).get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_innermost_par_preset_class)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const preset_class presetClassExp = preset_class::entrance;

	EXPECT_EQ(par->par_attlist_.presentation_preset_class_.value_or(preset_class::custom).get_type(), presetClassExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_innermost_par_preset_id)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(2));
	ASSERT_NE(par, nullptr);

	const preset_id presetIdExp = preset_id::ooo_entrance_fly_in;

	EXPECT_EQ(par->par_attlist_.presentation_preset_id_.value_or(preset_id::none).get_type(), presetIdExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_animate_duration)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_animate* animate = GetAnimationBehaviourByIndex<anim_animate>(GetInnermostPar(GetMainSequenceParByIndex(2)), 1);
	ASSERT_NE(animate, nullptr);

	const clockvalue durationExp = clockvalue(500);

	EXPECT_EQ(animate->common_attlist_.smil_dur_.value_or(clockvalue(-1)).get_value(), durationExp.get_value());
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_animate_values)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_animate* animate = GetAnimationBehaviourByIndex<anim_animate>(GetInnermostPar(GetMainSequenceParByIndex(2)), 1);
	ASSERT_NE(animate, nullptr);

	const smil_values valuesExp = smil_values::parse(L"x;x");

	ASSERT_EQ(animate->animate_attlist_.smil_values_->get_values().size(), valuesExp.get_values().size());

	for (size_t i = 0; i < valuesExp.get_values().size(); i++) 
	{
		EXPECT_EQ(
			animate->animate_attlist_.smil_values_->get_values()[i],
			valuesExp.get_values()[i]
		);
	}
}

TEST_F(OOX2ODP_EntranceAnimationTest, fly_in_animate_key_times)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_animate* animate = GetAnimationBehaviourByIndex<anim_animate>(GetInnermostPar(GetMainSequenceParByIndex(2)), 1);
	ASSERT_NE(animate, nullptr);

	const smil_key_times keyTimesExp = smil_key_times::parse(L"0;1");

	ASSERT_EQ(animate->animate_attlist_.smil_key_times_->get_values().size(), keyTimesExp.get_values().size());

	for (size_t i = 0; i < keyTimesExp.get_values().size(); i++)
	{
		EXPECT_EQ(
			animate->animate_attlist_.smil_key_times_->get_values()[i],
			keyTimesExp.get_values()[i]
		);
	}
}

////////////////////////////////////////////////////////////

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_outer_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(3);
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"next";

	EXPECT_EQ(par->attlist_.smil_begin_.get_value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_outer_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetMainSequenceParByIndex(3);
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_inner_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_inner_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnerPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_innermost_par_begin)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const std::wstring beginExp = L"0s";

	EXPECT_EQ(par->attlist_.smil_begin_.value_or(L"Empty"), beginExp);
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_innermost_par_fill)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const smil_fill fillExp = smil_fill::_hold;

	EXPECT_EQ(par->attlist_.smil_fill_.value_or(smil_fill::none).get_type(), fillExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_innermost_par_node_type)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const presentation_node_type nodeTypeExp = presentation_node_type::on_click;

	EXPECT_EQ(par->attlist_.presentation_node_type_.value_or(presentation_node_type::none).get_type(), nodeTypeExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_innermost_par_preset_class)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const preset_class presetClassExp = preset_class::entrance;

	EXPECT_EQ(par->par_attlist_.presentation_preset_class_.value_or(preset_class::custom).get_type(), presetClassExp.get_type());
}

TEST_F(OOX2ODP_EntranceAnimationTest, ascend_innermost_par_preset_id)
{
	using namespace cpdoccore::odf_writer;
	using namespace cpdoccore::odf_types;

	const anim_par* par = GetInnermostPar(GetMainSequenceParByIndex(3));
	ASSERT_NE(par, nullptr);

	const preset_id presetIdExp = preset_id::ooo_entrance_ascend;

	EXPECT_EQ(par->par_attlist_.presentation_preset_id_.value_or(preset_id::none).get_type(), presetIdExp.get_type());
}
