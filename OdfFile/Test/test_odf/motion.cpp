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

#include "motion.h"

boost::shared_ptr<cpdoccore::odf_reader::odf_document>		ODP2OOX_AnimationMotionEnvironment::sInputOdf;
boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	ODP2OOX_AnimationMotionEnvironment::sConverionContext;

ODP2OOX_AnimationMotionEnvironment::ODP2OOX_AnimationMotionEnvironment()
	: ODP2OOX_AnimationEnvironment(L"ExampleFiles" FILE_SEPARATOR_STR "motion.odp",
		&sInputOdf,
		&sConverionContext)
{
}

const cpdoccore::oox::pptx_animation_context& ODP2OOX_AnimationMotionEnvironment::GetAnimationContext()
{
	return sConverionContext->get_slide_context().get_animation_context();
}

void ODP2OOX_MotionAnimationTest::SetUp()
{
	mAnimationContext = &ODP2OOX_AnimationMotionEnvironment::GetAnimationContext();
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& ODP2OOX_MotionAnimationTest::GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimParArray[0];
}

const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr& ODP2OOX_MotionAnimationTest::GetMainSequence()
{
	if(mAnimationContext->get_root_par_animation()->AnimSeq.size())
		return mAnimationContext->get_root_par_animation()->AnimSeq[0];
	return nullptr;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array& ODP2OOX_MotionAnimationTest::GetMainSequenceArray()
{
	return GetMainSequence()->AnimParArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_MotionAnimationTest::GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	return par->AnimationActionArray;
}

const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr ODP2OOX_MotionAnimationTest::GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par)
{
	using namespace cpdoccore::oox;
	cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr inner = par;

	while (inner->AnimParArray.size())
		inner = GetInnerPar(inner);

	return inner;
}

const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array& ODP2OOX_MotionAnimationTest::GetAnimationActionsByIndex(size_t index)
{
	using namespace cpdoccore::oox;
	const pptx_animation_context::Impl::_par_animation_array& main_seq = GetMainSequenceArray();
	const pptx_animation_context::Impl::_animation_element_array& actions = GetInnermostPar(main_seq[index])->AnimationActionArray;

	return actions;
}

const std::vector<svg_path::_polyline> ODP2OOX_MotionAnimationTest::ParseSvgPath(const std::wstring& path)
{
	std::vector<svg_path::_polyline> polylines;

	bool closed, stroked;
	svg_path::parseSvgD(polylines, path, false, closed, stroked);

	return polylines;
}

void ODP2OOX_MotionAnimationTest::CompareSvgPath(const std::vector<svg_path::_polyline>& left, const std::vector<svg_path::_polyline>& right)
{
	ASSERT_EQ(left.size(), right.size());
	for (size_t i = 0; i < left.size(); i++)
	{
		EXPECT_EQ(left[i].command, right[i].command);
		EXPECT_EQ(left[i].points.size(), right[i].points.size());
		for (size_t p = 0; p < left[i].points.size(); p++)
		{
			EXPECT_EQ(left[i].points[p].x.has_value(), right[i].points[p].x.has_value());
			EXPECT_EQ(left[i].points[p].y.has_value(), right[i].points[p].y.has_value());

			double x = left[i].points[p].x.value_or(0.0);
			double y = left[i].points[p].y.value_or(0.0);
			double xExp = right[i].points[p].x.value_or(0.0);
			double yExp = right[i].points[p].y.value_or(0.0);

			EXPECT_NEAR(x, xExp, mEpsilon);
			EXPECT_NEAR(y, yExp, mEpsilon);
		}
	}
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_polygon_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const int durationExp = 2000;

	EXPECT_EQ(motion->SmilDurMs.value(), durationExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_polygon_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring fillExp = L"hold";

	EXPECT_EQ(motion->SmilFill.value(), fillExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_polygon_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 0;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M -0.00978571428571429 0.000253968253968254 L 0.311285714285715 -0.423111111111111 L 0.299714285714286 -0.390539682539682 L 0.625607142857143 0.262539682539683 L 0.0596071428571431 0.355111111111111 L 0.282357142857143 -0.0340317460317456 E";

	std::vector<::svg_path::_polyline> polylines	= ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_curve_duration)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const int durationExp = 2000;

	EXPECT_EQ(motion->SmilDurMs.value(), durationExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_curve_fill)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring fillExp = L"hold";

	EXPECT_EQ(motion->SmilFill.value(), fillExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_curve_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 1;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0.0152857142857143 -0.0443174603174603 "
		"C 0.571642857142857 -0.243174603174603 0.416392857142857 0.360253968253969 0.418321428571428 0.358539682539683 "
		"C 0.420249999999999 0.356825396825397 0.214857142857142 0.392825396825397 0.116499999999999 0.205968253968254 "
		"C 0.0181428571428562 0.0191111111111104 0.286214285714285 -0.0408888888888893 0.286214285714285 -0.0408888888888893 E";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_freeform_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 2;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0.212928571428571 -0.241460317460317 "
		"C 0.278928571428571 -0.249142857142857 0.342073306897614 -0.198873917257616 0.405785714285714 -0.17631746031746 "
		"C 0.44232330881459 -0.163381853086875 0.477388383258405 -0.136999684267748 0.510892857142857 -0.107746031746031 "
		"C 0.543852671468436 -0.0789679360568993 0.580071048835442 -0.0793628025871184 0.614035714285714 -0.061460317460317 "
		"C 0.634999620327278 -0.0504104216013913 0.666289460411191 -0.0693373484432591 0.677678571428571 -0.0357460317460313 "
		"C 0.686610889491912 -0.00940083900463148 0.705793372667082 -0.0133203379587745 0.690214285714285 0.0156825396825401 "
		"C 0.674757654936194 0.0444574462327262 0.67042353068495 0.0813244939972394 0.654535714285714 0.109968253968254 "
		"C 0.640066386196015 0.136054655669317 0.624280246774309 0.157957406936567 0.609214285714285 0.181968253968254 "
		"C 0.595994529846027 0.203036776227432 0.577179663378154 0.20064283022504 0.561035714285714 0.20768253968254 "
		"C 0.546893947779404 0.21384918000395 0.532679106525073 0.21676565399386 0.518607142857143 0.223111111111112 "
		"C 0.500857677670313 0.231114860470829 0.482430590382609 0.234300531367829 0.464607142857142 0.241968253968254 "
		"C 0.440073444160649 0.25252275503761 0.414464853047089 0.254479463994283 0.390357142857142 0.26768253968254 "
		"C 0.370179138607032 0.27873343233278 0.349960266692982 0.286387897422403 0.329607142857142 0.296825396825397 "
		"C 0.301674544144156 0.3111498064218 0.272359084390281 0.315003059917242 0.243785714285714 0.324253968253969 "
		"C 0.217644794521686 0.332717346976526 0.191821454417738 0.340186097907613 0.164714285714285 0.33968253968254 "
		"C 0.1234643113752 0.338916256634328 0.0825701269502042 0.323878368626136 0.0451428571428562 0.295111111111112 "
		"C 0.0258201265945204 0.280259320733704 0.0105099770831932 0.252181552551216 0.000821428571427633 0.217968253968254 "
		"C -0.0287757389302364 0.113451387719954 0.108240869954766 0.223417983073443 0.132892857142857 0.156253968253969 "
		"C 0.149419443176705 0.111227501996963 0.160327960399085 0.0562104066324277 0.187857142857142 0.0276825396825397 "
		"C 0.223006535897276 -0.00874197853720813 0.26042803119238 -0.0400037203455829 0.299714285714285 -0.058031746031746 "
		"C 0.317570888513603 -0.066225942649655 0.346455930353361 -0.113453411344393 0.354678571428571 -0.082031746031746 "
		"C 0.364848789157456 -0.0431676907104916 0.321427082018808 -0.0417237090720452 0.302607142857142 -0.0323174603174603 "
		"C 0.291319937170731 -0.0266760890293527 0.27925 -0.0337142857142857 0.267892857142857 -0.0271746031746032 "
		"L 0.256321428571428 -0.0203174603174603 E";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_4_point_star_preset_id)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 3;
	const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(GetMainSequenceArray()[animationIndex]);

	const int presetIdExp = 16;

	EXPECT_EQ(par->PresetID.value(), presetIdExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_4_point_star_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 3;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0 0 L 0.091 -0.0453 L 0.125 -0.16655 L 0.158 -0.0453 L 0.249 0 L 0.158 0.0453 L 0.125 0.16655 L 0.091 0.0453 L 0 0 Z";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_diamond_preset_id)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 4;
	const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(GetMainSequenceArray()[animationIndex]);

	const int presetIdExp = 3;

	EXPECT_EQ(par->PresetID.value(), presetIdExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_diamond_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 4;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0 0 L 0.125 -0.11192 L 0.25 0 L 0.125 0.11192 L 0 0 Z";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_buzz_saw_preset_id)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 5;
	const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(GetMainSequenceArray()[animationIndex]);

	const int presetIdExp = 25;

	EXPECT_EQ(par->PresetID.value(), presetIdExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_buzz_saw_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 5;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0 0 C -0.022 -0.02265 -0.033 -0.06129 -0.027 -0.09993 "
		"C -0.024 -0.11325 -0.02 -0.12658 -0.014 -0.13724 "
		"C -0.01 -0.10659 0.004 -0.07861 0.025 -0.06129 "
		"C 0.025 -0.0986 0.041 -0.13457 0.068 -0.15056 "
		"C 0.077 -0.15722 0.087 -0.15989 0.097 -0.16122 "
		"C 0.082 -0.13857 0.074 -0.10659 0.077 -0.07328 "
		"C 0.099 -0.09727 0.13 -0.1026 0.157 -0.08527 "
		"C 0.166 -0.07994 0.175 -0.07062 0.181 -0.06129 "
		"C 0.158 -0.06396 0.134 -0.05196 0.117 -0.02798 "
		"C 0.144 -0.01999 0.167 0.00799 0.174 0.04663 "
		"C 0.176 0.05996 0.176 0.07328 0.174 0.08661 "
		"C 0.161 0.06129 0.139 0.04397 0.115 0.0413 "
		"C 0.127 0.07461 0.124 0.11592 0.106 0.14656 "
		"C 0.099 0.15722 0.091 0.16655 0.082 0.17188 "
		"C 0.089 0.14257 0.085 0.10926 0.072 0.08261 "
		"C 0.06 0.11592 0.034 0.13857 0.004 0.13857 "
		"C -0.007 0.13857 -0.017 0.13591 -0.026 0.13058 "
		"C -0.004 0.11992 0.013 0.0946 0.021 0.06396 "
		"C -0.007 0.07195 -0.036 0.05996 -0.055 0.02931 "
		"C -0.062 0.01732 -0.066 0.00533 -0.069 -0.00799 "
		"C -0.049 0.00933 -0.023 0.01199 0 0 Z";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_spring_preset_id)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 6;
	const pptx_animation_context::Impl::_par_animation_ptr& par = GetInnermostPar(GetMainSequenceArray()[animationIndex]);

	const int presetIdExp = 53;

	EXPECT_EQ(par->PresetID.value(), presetIdExp);
}

TEST_F(ODP2OOX_MotionAnimationTest, motionpath_spring_path)
{
	using namespace cpdoccore::oox;
	const size_t animationIndex = 6;
	const pptx_animation_context::Impl::_animation_element_array& actions = GetAnimationActionsByIndex(animationIndex);

	const pptx_animation_context::Impl::_animate_motion* motion = dynamic_cast<pptx_animation_context::Impl::_animate_motion*>(actions[0].get());
	const std::wstring pathExp = L"M 0 0 C -0.066 0.00799 -0.115 0.02798 -0.115 0.04397 "
		"C -0.115 0.05863 -0.067 0.06929 -0.003 0.06929 "
		"C 0.061 0.06929 0.115 0.05863 0.115 0.04397 "
		"C 0.115 0.02798 0.059 0.02398 -0.005 0.03464 "
		"C -0.068 0.04663 -0.115 0.06662 -0.115 0.08128 "
		"C -0.115 0.09593 -0.066 0.10793 -0.003 0.10793 "
		"C 0.061 0.10793 0.115 0.09593 0.115 0.08128 "
		"C 0.115 0.06662 0.059 0.06262 -0.004 0.07328 "
		"C -0.068 0.08394 -0.115 0.10393 -0.115 0.11858 "
		"C -0.115 0.13457 -0.066 0.14656 -0.002 0.14656 "
		"C 0.061 0.14656 0.115 0.13457 0.115 0.11858 "
		"C 0.115 0.10526 0.059 0.10126 -0.004 0.11059 "
		"C -0.067 0.12125 -0.115 0.14257 -0.115 0.15722 "
		"C -0.115 0.17188 -0.065 0.18387 -0.002 0.18387 "
		"C 0.063 0.18387 0.115 0.17188 0.115 0.15722 "
		"C 0.115 0.14257 0.06 0.13857 -0.003 0.14923 "
		"C -0.066 0.15989 -0.115 0.17988 -0.115 0.19453 "
		"C -0.115 0.21052 -0.065 0.22118 -0.001 0.22118 "
		"C 0.063 0.22118 0.115 0.20919 0.115 0.19453 "
		"C 0.115 0.17988 0.06 0.17588 -0.003 0.18654 "
		"C -0.066 0.1972 -0.115 0.21851 -0.115 0.23184 "
		"C -0.115 0.2465 -0.064 0.25849 -0.001 0.25849 "
		"C 0.063 0.25849 0.115 0.2465 0.115 0.23184 "
		"C 0.115 0.21851 0.061 0.21452 -0.003 0.22384 "
		"C -0.066 0.2345 -0.115 0.25582 -0.115 0.27048 "
		"C -0.115 0.2838 -0.064 0.29713 0 0.29713 "
		"C 0.064 0.29713 0.115 0.28514 0.115 0.27048 "
		"C 0.115 0.25582 0.061 0.25183 -0.002 0.26248 "
		"C -0.065 0.27314 -0.116 0.29313 -0.115 0.30779 "
		"C -0.114 0.32244 -0.064 0.3331 0 0.3331 "
		"C 0.064 0.3331 0.115 0.32111 0.115 0.30645 "
		"C 0.115 0.29313 0.063 0.28913 0 0.30112 E";

	std::vector<::svg_path::_polyline> polylines = ParseSvgPath(motion->SvgPath.value());
	std::vector<::svg_path::_polyline> polylinesExp = ParseSvgPath(pathExp);

	CompareSvgPath(polylines, polylinesExp);
}
