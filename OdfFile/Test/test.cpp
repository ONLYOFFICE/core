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

class ODP2OOX_EnterExitAnimationTest : public testing::Test
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

void CompareAnimSet(const cpdoccore::oox::pptx_animation_context::Impl::_set& left, const cpdoccore::oox::pptx_animation_context::Impl::_set& right)
{
	EXPECT_EQ(left.Direction	.has_value()	, right.Direction		.has_value());
	EXPECT_EQ(left.Restart		.has_value()	, right.Restart			.has_value());
	EXPECT_EQ(left.Duration		.has_value()	, right.Duration		.has_value());
	EXPECT_EQ(left.Delay		.has_value()	, right.Delay			.has_value());
	EXPECT_EQ(left.End			.has_value()	, right.End				.has_value());
	EXPECT_EQ(left.Fill			.has_value()	, right.Fill			.has_value());
	EXPECT_EQ(left.ShapeID		.has_value()	, right.ShapeID			.has_value());
	EXPECT_EQ(left.AttributeName.has_value()	, right.AttributeName	.has_value());
	EXPECT_EQ(left.ToValue		.has_value()	, right.ToValue			.has_value());

	const std::wstring emptyStr = std::wstring();
	const int emptyInt			= 0;

	EXPECT_EQ(left.Direction	.value_or(emptyStr), right.Direction	.value_or(emptyStr));
	EXPECT_EQ(left.Restart		.value_or(emptyStr), right.Restart		.value_or(emptyStr));
	EXPECT_EQ(left.Duration		.value_or(emptyInt), right.Duration		.value_or(emptyInt));
	EXPECT_EQ(left.Delay		.value_or(emptyStr), right.Delay		.value_or(emptyStr));
	EXPECT_EQ(left.End			.value_or(emptyStr), right.End			.value_or(emptyStr));
	EXPECT_EQ(left.Fill			.value_or(emptyStr), right.Fill			.value_or(emptyStr));
	EXPECT_EQ(left.ShapeID		.value_or(emptyInt), right.ShapeID		.value_or(emptyInt));
	EXPECT_EQ(left.AttributeName.value_or(emptyStr), right.AttributeName.value_or(emptyStr));
	EXPECT_EQ(left.ToValue		.value_or(emptyStr), right.ToValue		.value_or(emptyStr));
}

void CompareAnimAnimate(const cpdoccore::oox::pptx_animation_context::Impl::_anim& left, const cpdoccore::oox::pptx_animation_context::Impl::_anim& right)
{
	EXPECT_EQ(left.CalcMode		.has_value(), right.CalcMode		.has_value());
	EXPECT_EQ(left.ValueType	.has_value(), right.ValueType		.has_value());
	EXPECT_EQ(left.ShapeID		.has_value(), right.ShapeID			.has_value());
	EXPECT_EQ(left.Duration		.has_value(), right.Duration		.has_value());
	EXPECT_EQ(left.AttributeName.has_value(), right.AttributeName	.has_value());
	EXPECT_EQ(left.From			.has_value(), right.From			.has_value());
	EXPECT_EQ(left.To			.has_value(), right.To				.has_value());
	EXPECT_EQ(left.By			.has_value(), right.By				.has_value());
	EXPECT_EQ(left.Additive		.has_value(), right.Additive		.has_value());
	EXPECT_EQ(left.AutoReverse	.has_value(), right.AutoReverse		.has_value());
	EXPECT_EQ(left.Delay		.has_value(), right.Delay			.has_value());
	EXPECT_EQ(left.KeypointArray.has_value(), right.KeypointArray	.has_value());

	const std::wstring emptyStr = std::wstring();
	const int emptyInt			= 0;

	EXPECT_EQ(left.CalcMode		.value_or(emptyStr)	, right.CalcMode		.value_or(emptyStr));
	EXPECT_EQ(left.ValueType	.value_or(emptyStr)	, right.ValueType		.value_or(emptyStr));
	EXPECT_EQ(left.ShapeID		.value_or(emptyInt)	, right.ShapeID			.value_or(emptyInt));
	EXPECT_EQ(left.Duration		.value_or(emptyInt)	, right.Duration		.value_or(emptyInt));
	EXPECT_EQ(left.AttributeName.value_or(emptyStr)	, right.AttributeName	.value_or(emptyStr));
	EXPECT_EQ(left.From			.value_or(emptyStr)	, right.From			.value_or(emptyStr));
	EXPECT_EQ(left.To			.value_or(emptyStr)	, right.To				.value_or(emptyStr));
	EXPECT_EQ(left.By			.value_or(emptyStr)	, right.By				.value_or(emptyStr));
	EXPECT_EQ(left.Additive		.value_or(emptyStr)	, right.Additive		.value_or(emptyStr));
	EXPECT_EQ(left.AutoReverse	.value_or(false)	, right.AutoReverse		.value_or(false));
	EXPECT_EQ(left.Delay		.value_or(emptyStr)	, right.Delay			.value_or(emptyStr));
	
	EXPECT_EQ(left.KeypointArray->size(), right.KeypointArray->size());
	for (size_t i = 0; i < left.KeypointArray->size(); i++)
	{
		EXPECT_EQ(left.KeypointArray->at(i).Time					, right.KeypointArray->at(i).Time);
		EXPECT_EQ(left.KeypointArray->at(i).Value					, right.KeypointArray->at(i).Value);
		EXPECT_EQ(left.KeypointArray->at(i).Fmla.has_value()		, right.KeypointArray->at(i).Fmla.has_value());
		EXPECT_EQ(left.KeypointArray->at(i).Fmla.value_or(emptyStr)	, right.KeypointArray->at(i).Fmla.value_or(emptyStr));
	}
}

void CompareAnimEffect(const cpdoccore::oox::pptx_animation_context::Impl::_anim_effect& left, const cpdoccore::oox::pptx_animation_context::Impl::_anim_effect& right)
{
	EXPECT_EQ(left.Filter		.has_value(), right.Filter		.has_value());
	EXPECT_EQ(left.Transition	.has_value(), right.Transition	.has_value());
	EXPECT_EQ(left.Duration		.has_value(), right.Duration	.has_value());
	EXPECT_EQ(left.ShapeID		.has_value(), right.ShapeID		.has_value());

	const std::wstring emptyStr = std::wstring();
	const int emptyInt			= 0;

	EXPECT_EQ(left.Filter		.value_or(emptyStr), right.Filter		.value_or(emptyStr));
	EXPECT_EQ(left.Transition	.value_or(emptyStr), right.Transition	.value_or(emptyStr));
	EXPECT_EQ(left.Duration		.value_or(emptyInt), right.Duration		.value_or(emptyInt));
	EXPECT_EQ(left.ShapeID		.value_or(emptyInt), right.ShapeID		.value_or(emptyInt));
}

TEST_F(ODP2OOX_EnterExitAnimationTest, timing_root_par)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_ptr& root = mAnimationContext->get_root_par_animation();

	const std::wstring nodeTypeExp = L"tmRoot";

	EXPECT_EQ(root->NodeType.value(), nodeTypeExp);
}

TEST_F(ODP2OOX_EnterExitAnimationTest, main_sequence)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_seq_animation_ptr& main_seq = GetMainSequence();

	const std::wstring nodeTypeExp = L"mainSeq";

	EXPECT_EQ(main_seq->PresentationNodeType.value(), nodeTypeExp);
	EXPECT_FALSE(main_seq->Duration.has_value());
}

TEST_F(ODP2OOX_EnterExitAnimationTest, main_sequence_par_animations)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& par_animations = mAnimationContext->get_root_par_animation()->AnimSeq->AnimParArray;

	const std::wstring delayExp = L"indefinite";

	for (size_t i = 0; i < par_animations.size(); i++)
	{
		EXPECT_EQ(par_animations[i]->Delay.value(), delayExp);
	}
}

TEST_F(ODP2OOX_EnterExitAnimationTest, main_sequence_inner_par_animation)
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

TEST_F(ODP2OOX_EnterExitAnimationTest, main_sequence_innermost_par_animation)
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

TEST_F(ODP2OOX_EnterExitAnimationTest, entrance_appear)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set = dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	
	const size_t actionsSizeExp = 1;
	const int durationExp = 1;
	const std::wstring fillExp = L"hold";
	const std::wstring delayExp = L"0";
	const size_t spidExp = 2;
	const std::wstring attributeNameExp = L"style.visibility";
	const std::wstring toValueExp = L"visible";

	EXPECT_TRUE(set != nullptr);
	EXPECT_EQ(actions.size(), actionsSizeExp);

	EXPECT_EQ(set->Duration.value()		, durationExp);
	EXPECT_EQ(set->Fill.value()			, fillExp);
	EXPECT_EQ(set->Delay.value()		, delayExp);
	EXPECT_EQ(set->ShapeID.value()		, spidExp);
	EXPECT_EQ(set->AttributeName.value(), attributeNameExp);
	EXPECT_EQ(set->ToValue.value()		, toValueExp);
}

TEST_F(ODP2OOX_EnterExitAnimationTest, entrance_fly_in)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set		= dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const pptx_animation_context::Impl::_anim* animate1 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[1].get());
	const pptx_animation_context::Impl::_anim* animate2 = dynamic_cast<pptx_animation_context::Impl::_anim*>(actions[2].get());

	const size_t actionsSizeExp	= 3;

	pptx_animation_context::Impl::_set setExp;
	setExp.Duration				= 1;
	setExp.Delay				= L"0";
	setExp.Fill					= L"hold";
	setExp.ShapeID				= 2;
	setExp.AttributeName		= L"style.visibility";
	setExp.ToValue				= L"visible";

	pptx_animation_context::Impl::_anim animate1Exp;
	animate1Exp.CalcMode		= L"lin";
	animate1Exp.ValueType		= L"num";
	animate1Exp.ShapeID			= 2;
	animate1Exp.Duration		= 500;
	animate1Exp.AttributeName	= L"ppt_x";
	animate1Exp.Additive		= L"repl";
	animate1Exp.KeypointArray = std::vector<pptx_animation_context::Impl::_anim::_keypoint>();
	animate1Exp.KeypointArray->push_back(pptx_animation_context::Impl::_anim::_keypoint(0, L"#ppt_x", boost::none));
	animate1Exp.KeypointArray->push_back(pptx_animation_context::Impl::_anim::_keypoint(100000, L"#ppt_x", boost::none));

	pptx_animation_context::Impl::_anim animate2Exp;
	animate2Exp.CalcMode		= L"lin";
	animate2Exp.ValueType		= L"num";
	animate2Exp.ShapeID			= 2;
	animate2Exp.Duration		= 500;
	animate2Exp.AttributeName	= L"ppt_y";
	animate2Exp.Additive		= L"repl";
	animate2Exp.KeypointArray = std::vector<pptx_animation_context::Impl::_anim::_keypoint>();
	animate2Exp.KeypointArray->push_back(pptx_animation_context::Impl::_anim::_keypoint(0, L"1+#ppt_h/2", boost::none));
	animate2Exp.KeypointArray->push_back(pptx_animation_context::Impl::_anim::_keypoint(100000, L"#ppt_y", boost::none));

	EXPECT_TRUE(set != nullptr);
	EXPECT_TRUE(animate1 != nullptr);
	EXPECT_TRUE(animate2 != nullptr);
	EXPECT_EQ(actions.size(), actionsSizeExp);

	CompareAnimSet(*set, setExp);
	CompareAnimAnimate(*animate1, animate1Exp);
	CompareAnimAnimate(*animate2, animate2Exp);
}

TEST_F(ODP2OOX_EnterExitAnimationTest, entrance_venetian_blinds)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_set* set					= dynamic_cast<pptx_animation_context::Impl::_set*>(actions[0].get());
	const pptx_animation_context::Impl::_anim_effect* animEffect	= dynamic_cast<pptx_animation_context::Impl::_anim_effect*>(actions[1].get());

	const size_t actionsSizeExp = 2;

	pptx_animation_context::Impl::_set setExp;
	setExp.Duration = 1;
	setExp.Delay = L"0";
	setExp.Fill = L"hold";
	setExp.ShapeID = 2;
	setExp.AttributeName = L"style.visibility";
	setExp.ToValue = L"visible";

	pptx_animation_context::Impl::_anim_effect animEffectExp;
	animEffectExp.Filter = L"blinds(horizontal)";
	animEffectExp.Transition = L"in";
	animEffectExp.Duration = 500;
	animEffectExp.ShapeID = 2;

	EXPECT_TRUE(set != nullptr);
	EXPECT_TRUE(animEffect != nullptr);
	EXPECT_EQ(actions.size(), actionsSizeExp);

	CompareAnimSet(*set, setExp);
	CompareAnimEffect(*animEffect, animEffectExp);
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new TestEnvironment);
	return RUN_ALL_TESTS();
}