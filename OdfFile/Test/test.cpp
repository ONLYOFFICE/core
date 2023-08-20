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

#include "gtest/gtest.h"

#include <boost/shared_ptr.hpp>

#include "Reader/Converter/ConvertOO2OOX.h"
#include "Reader/Converter/pptx_conversion_context.h"
#include "Reader/Converter/pptx_package.h"
#include "Reader/Format/odf_document.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#define CH_DIR(x) FILE_SEPARATOR_STR + _T(x)

class TestEnvironment : public ::testing::Environment {
public:

	~TestEnvironment() override {}

	void SetUp() override
	{
		Init();
		mInputOdf = ReadOdfDocument();
		mConverionContext = Convert(mInputOdf);
	}

	void TearDown() override
	{
		NSDirectory::DeleteDirectory(sTemp);
	}

	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> GetConversionContext()
	{
		return mConverionContext;
	}

	static const cpdoccore::oox::pptx_animation_context& GetAnimationContext()
	{
		return mConverionContext->get_slide_context().get_animation_context();
	}

private:

	void Init()
	{
		std::wstring exampleFilename = L"enterence";

		std::wstring rootDir = NSFile::GetProcessDirectory() + CH_DIR("..");
		std::wstring sExampleFilesDirectory = rootDir + CH_DIR("ExampleFiles");

		sFrom = sExampleFilesDirectory + FILE_SEPARATOR_STR + exampleFilename + L".odp";
		sTo = sExampleFilesDirectory + FILE_SEPARATOR_STR + exampleFilename + L".pptx";
		sTemp = rootDir + CH_DIR("tmp");
		sTempUnpackedOdf = sTemp + CH_DIR("odf_unpacked");
		sTempUnpackedOox = sTemp + CH_DIR("oox_unpacked");

		NSDirectory::CreateDirectory(sTemp);
		NSDirectory::CreateDirectory(sTempUnpackedOdf);
		NSDirectory::CreateDirectory(sTempUnpackedOox);

		mOutputPptx = boost::make_shared<cpdoccore::oox::package::pptx_document>();
	}

	boost::shared_ptr<cpdoccore::odf_reader::odf_document> ReadOdfDocument()
	{
		COfficeUtils oCOfficeUtils(NULL);
		if (S_OK == oCOfficeUtils.ExtractToDirectory(sFrom, sTempUnpackedOdf, NULL, 0))
			return boost::make_shared<cpdoccore::odf_reader::odf_document>(sTempUnpackedOdf, sTemp, L"");
		else
			return nullptr;
	}

	boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> Convert(boost::shared_ptr<cpdoccore::odf_reader::odf_document> inputOdf)
	{
		boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> conversionContext = boost::make_shared<cpdoccore::oox::pptx_conversion_context>(mInputOdf.get());

		conversionContext->set_output_document(mOutputPptx.get());
		conversionContext->set_font_directory(L"");

		mInputOdf->pptx_convert(*conversionContext);

		return conversionContext;
	}

private:
	std::wstring sFrom;
	std::wstring sTo;
	std::wstring sTemp;
	std::wstring sTempUnpackedOdf;
	std::wstring sTempUnpackedOox;

	static boost::shared_ptr<cpdoccore::oox::package::pptx_document> mOutputPptx;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> mConverionContext;
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document> mInputOdf;
};

boost::shared_ptr<cpdoccore::oox::package::pptx_document> TestEnvironment::mOutputPptx;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> TestEnvironment::mConverionContext;
boost::shared_ptr<cpdoccore::odf_reader::odf_document> TestEnvironment::mInputOdf;

class ODP2OOX_EnteanceAnimationTest : public testing::Test
{
public:
	void SetUp() override
	{
		mAnimationContext = &TestEnvironment::GetAnimationContext();
	}

	void TearDown() override
	{

	}

	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
	{
		return par->AnimParArray[0];
	}

	const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr& GetMainSequence()
	{
		return mAnimationContext->get_root_par_animation()->AnimSeq;
	}

	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array& GetMainSequenceArray()
	{
		return mAnimationContext->get_root_par_animation()->AnimSeq->AnimParArray;
	}

	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
	{
		return par->AnimationActionArray;
	}

	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
	{
		using namespace cpdoccore::oox;
		cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr inner = par;

		while (inner->AnimParArray.size())
		{
			inner = GetInnerPar(inner);
		}

		return inner;
	}

	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& GetAnimationActionsByIndex(size_t index)
	{
		using namespace cpdoccore::oox;
		const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();
		const pptx_animation_context::Impl::_animation_element_array& actions = GetInnermostPar(main_seq[index])->AnimationActionArray;

		return actions;
	}

public:
	const cpdoccore::oox::pptx_animation_context* mAnimationContext;

};

TEST_F(ODP2OOX_EnteanceAnimationTest, timing_root_par)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_ptr& root = mAnimationContext->get_root_par_animation();

	const std::wstring nodeTypeExp = L"tmRoot";

	EXPECT_EQ(root->NodeType.value(), nodeTypeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, main_sequence)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_seq_animation_ptr& main_seq = GetMainSequence();

	const std::wstring nodeTypeExp = L"mainSeq";

	EXPECT_EQ(main_seq->PresentationNodeType.value(), nodeTypeExp);
	EXPECT_FALSE(main_seq->Duration.has_value());
}

TEST_F(ODP2OOX_EnteanceAnimationTest, main_sequence_par_animations)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& par_animations = mAnimationContext->get_root_par_animation()->AnimSeq->AnimParArray;

	const std::wstring delayExp = L"indefinite";

	for (size_t i = 0; i < par_animations.size(); i++)
	{
		EXPECT_EQ(par_animations[i]->Delay.value(), delayExp);
	}
}

TEST_F(ODP2OOX_EnteanceAnimationTest, main_sequence_inner_par_animation)
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

TEST_F(ODP2OOX_EnteanceAnimationTest, main_sequence_innermost_par_animation)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();

	std::wstring delayExp	= L"0";
	std::wstring fillExp	= L"hold";
	std::wstring nodeTypeExp = L"clickEffect";
	std::wstring presetClassExp = L"entr";

	for (size_t i = 0; i < main_seq.size(); i++)
	{
		const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(main_seq[i]);

		EXPECT_EQ(par->Delay.value()		, delayExp);
		EXPECT_EQ(par->Fill.value()			, fillExp);
		EXPECT_EQ(par->NodeType.value()		, nodeTypeExp);
		EXPECT_EQ(par->PresetClass.value()	, presetClassExp);
	}
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_appear_acitons_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 1;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_actions_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_action_set_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const int durationExp = 1;

	EXPECT_EQ(set->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_action_set_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_action_set_delay)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_action_set_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring attributeNameExp = L"style.visibility";

	EXPECT_EQ(set->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, enerance_appear_action_set_to_value)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	const std::wstring toValueExp = L"visible";

	EXPECT_EQ(set->ToValue.value(), toValueExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_actions_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 3;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());

	EXPECT_TRUE(animate1 != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());

	EXPECT_TRUE(animate2 != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const int durationExp = 1;

	EXPECT_EQ(set->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set_delay)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring delayExp = L"0";

	EXPECT_EQ(set->Delay.value(), delayExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring fillExp = L"hold";

	EXPECT_EQ(set->Fill.value(), fillExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring attributeNameExp = L"style.visibility";

	EXPECT_EQ(set->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_set_to_value)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const std::wstring toValueExp = L"visible";

	EXPECT_EQ(set->ToValue.value(), toValueExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_calc_mode)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring calcModeExp = L"lin";

	EXPECT_EQ(animate1->CalcMode.value(), calcModeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_value_type)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring valueTypeExp = L"num";

	EXPECT_EQ(animate1->ValueType.value(), valueTypeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const int durationExp = 500;

	EXPECT_EQ(animate1->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring attributeNameExp = L"ppt_x";

	EXPECT_EQ(animate1->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_additive)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const std::wstring additiveExp = L"repl";

	EXPECT_EQ(animate1->Additive.value(), additiveExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_1_key_points)
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
		EXPECT_EQ(animate1->KeypointArray->at(i).Time					, keypointsExp[i].Time);
		EXPECT_EQ(animate1->KeypointArray->at(i).Value					, keypointsExp[i].Value);
		EXPECT_EQ(animate1->KeypointArray->at(i).Fmla.has_value()		, keypointsExp[i].Fmla.has_value());
		EXPECT_EQ(animate1->KeypointArray->at(i).Fmla.value_or(L"")		, keypointsExp[i].Fmla.value_or(L""));
	}
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_calc_mode)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring calcModeExp = L"lin";

	EXPECT_EQ(animate2->CalcMode.value(), calcModeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_value_type)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring valueTypeExp = L"num";

	EXPECT_EQ(animate2->ValueType.value(), valueTypeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const int durationExp = 500;

	EXPECT_EQ(animate2->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_attribute_name)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring attributeNameExp = L"ppt_y";

	EXPECT_EQ(animate2->AttributeName.value(), attributeNameExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_additive)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());
	const std::wstring additiveExp = L"repl";

	EXPECT_EQ(animate2->Additive.value(), additiveExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_fly_in_action_animate_2_key_points)
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
		EXPECT_EQ(animate2->KeypointArray->at(i).Time				, keypointsExp[i].Time);
		EXPECT_EQ(animate2->KeypointArray->at(i).Value				, keypointsExp[i].Value);
		EXPECT_EQ(animate2->KeypointArray->at(i).Fmla.has_value()	, keypointsExp[i].Fmla.has_value());
		EXPECT_EQ(animate2->KeypointArray->at(i).Fmla.value_or(L"")	, keypointsExp[i].Fmla.value_or(L""));
	}
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_actions_size)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const size_t actionsSizeExp = 2;

	EXPECT_EQ(actions.size(), actionsSizeExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_set)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());

	EXPECT_TRUE(set != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_anim_effect)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	EXPECT_TRUE(animEffect != nullptr);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_anim_effect_filter)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"blinds(horizontal)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_anim_effect_transition)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring transitionExp = L"in";

	EXPECT_EQ(animEffect->Transition.value(), transitionExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_venetian_blinds_anim_effect_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const int durationExp = 500;

	EXPECT_EQ(animEffect->Duration.value(), durationExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_transition_filter_blinds_wipe_horizontal)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"box(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_transition_filter_iris_wipe_rectangle)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 3;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"circle(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_transition_filter_checker_board_wipe_across)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 4;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"circle(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

TEST_F(ODP2OOX_EnteanceAnimationTest, entrance_transition_filter_checker_iris_wipe_diamond)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 8;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);
	const pptx_animation_context::Impl::_anim_effect* animEffect = dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const std::wstring filterExp = L"diamond(in)";

	EXPECT_EQ(animEffect->Filter.value(), filterExp);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new TestEnvironment);
	return RUN_ALL_TESTS();
}