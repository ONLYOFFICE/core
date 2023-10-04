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
#include <vector>

#include "Reader/Format/odf_document.h"
#include "Reader/Converter/pptx_conversion_context.h"
#include "Reader/Format/svg_parser.h"

class MotionTestEnvironment : public ::testing::Environment {
public:

	~MotionTestEnvironment() override {}

	void SetUp() override;

	void TearDown() override;

	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> GetConversionContext()
	{
		return mConverionContext;
	}

	static const cpdoccore::oox::pptx_animation_context& GetAnimationContext()
	{
		return mConverionContext->get_slide_context().get_animation_context();
	}

private:
	std::wstring sExampleFilename;
	std::wstring sFrom;
	std::wstring sTemp;
	std::wstring sTempUnpackedOdf;

	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> mConverionContext;
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document> mInputOdf;
};

class ODP2OOX_MotionAnimationTest : public testing::Test
{
public:
	void SetUp() override;

	void TearDown() override
	{ }

	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr&			GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr&			GetMainSequence();
	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array&		GetMainSequenceArray();
	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array&	GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr			GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array&	GetAnimationActionsByIndex(size_t index);
	const std::vector<svg_path::_polyline>											ParseSvgPath(const std::wstring& path);

	void CompareSvgPath(const std::vector<svg_path::_polyline>& left, const std::vector<svg_path::_polyline>& right);

public:
	const cpdoccore::oox::pptx_animation_context* mAnimationContext;
	const double mEpsilon = 0.00001;
};
