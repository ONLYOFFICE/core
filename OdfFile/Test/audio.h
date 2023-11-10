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
#pragma once

#include "common.h"

#include "gtest/gtest.h"

#include "Writer/Converter/Oox2OdfConverter.h"
#include "Writer/Format/odp_conversion_context.h"
#include "Writer/Format/anim_elements.h"
#include "Writer/Format/draw_page.h"

#include "Reader/Format/odf_document.h"
#include "Reader/Converter/pptx_conversion_context.h"

class ODP2OOX_AnimationAudioEnvironment : public ODP2OOX_AnimationEnvironment
{
public:
	ODP2OOX_AnimationAudioEnvironment();

	static const cpdoccore::oox::pptx_animation_context& GetAnimationContext();

private:
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document>		sInputOdf;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	sConverionContext;
};

class ODP2OOX_AnimationAudioTest : public testing::Test
{
public:
	void SetUp() override;
	void TearDown() override
	{ }

	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr&				GetInnerPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_seq_animation_ptr&				GetMainSequence();
	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_array&			GetMainSequenceArray();
	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array&		GetActionArray(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr				GetInnermostPar(const cpdoccore::oox::pptx_animation_context::Impl::_par_animation_ptr& par);
	const cpdoccore::oox::pptx_animation_context::Impl::_animation_element_array&		GetAnimationActionsByIndex(size_t index);


public:
	const cpdoccore::oox::pptx_animation_context* mAnimationContext;
};

class OOX2ODP_AudioAnimationEnvironment : public OOX2ODP_AnimationEnvironment
{
public:
	OOX2ODP_AudioAnimationEnvironment();

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext();

private:
	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_AudioAnimationTest : public OOX2ODP_AnimationTest
{
public:
	void SetUp() override;
	void TearDown() override {}
};