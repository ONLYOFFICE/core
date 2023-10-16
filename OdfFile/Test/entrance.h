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
#include "common.h"

#include <boost/shared_ptr.hpp>

#include "Reader/Format/odf_document.h"
#include "Reader/Converter/pptx_conversion_context.h"

#include "Writer/Converter/Oox2OdfConverter.h"
#include "Writer/Format/odp_conversion_context.h"
#include "Writer/Format/anim_elements.h"
#include "Writer/Format/draw_page.h"

class ODP2OOX_AnimationEtranceEnvironment : public ODP2OOX_AnimationEnvironment
{
public:
	ODP2OOX_AnimationEtranceEnvironment();

	static const cpdoccore::oox::pptx_animation_context& GetAnimationContext();

private:
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context> sConverionContext;
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document> sInputOdf;
};

class ODP2OOX_EntranceAnimationTest : public testing::Test
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

public:
	const cpdoccore::oox::pptx_animation_context* mAnimationContext;
};

//////////////////////////////////////////////////////////////////////////
// OOX2ODP

class OOX2ODP_EntranceAnimationTestEnvironment : public ::testing::Environment
{
	~OOX2ODP_EntranceAnimationTestEnvironment() override {}

	void SetUp() override;
	void TearDown() override;

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext()
	{
		return mContext;
	}

private:
	std::wstring sExampleFilename;
	std::wstring sFrom;
	std::wstring sTemp;
	std::wstring sTempUnpackedOox;

	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_EntranceAnimationTest : public testing::Test
{
public:
	void SetUp() override;
	void TearDown() override {}

	const cpdoccore::odf_writer::anim_par* GetTimingRoot()
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

	const cpdoccore::odf_writer::anim_seq* GetMainSequence()
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

	const cpdoccore::odf_writer::anim_par* GetMainSequenceParByIndex(size_t index)
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

	const cpdoccore::odf_writer::anim_par* GetInnerPar(const cpdoccore::odf_writer::anim_par* par)
	{
		using namespace cpdoccore::odf_writer;

		if (!par)
			return nullptr;

		const anim_par* inner_par = dynamic_cast<anim_par*>(par->anim_par_.get());

		if (!inner_par)
			return nullptr;

		return inner_par;
	}

	const cpdoccore::odf_writer::anim_par* GetInnermostPar(const cpdoccore::odf_writer::anim_par* par)
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

	template<typename T>
	const T* GetAnimationBehaviourByIndex(const cpdoccore::odf_writer::anim_par* par, size_t index)
	{
		using namespace cpdoccore::odf_writer;

		if (!par)
			return nullptr;

		T* behaviour = dynamic_cast<T*>(par->content_[index].get());
		if (!behaviour)
			return nullptr;

		return behaviour;
	}

public:
	const Oox2Odf::Converter* mConverter;
	cpdoccore::odf_writer::odp_conversion_context* mContext;
};