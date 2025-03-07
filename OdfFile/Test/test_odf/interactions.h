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

class OOX2ODP_InteractionAnimationEnvironment : public OOX2ODP_AnimationEnvironment
{
public:
	OOX2ODP_InteractionAnimationEnvironment();

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext();

private:
	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_InteractionAnimationTest : public OOX2ODP_AnimationTest
{
public:
	void SetUp() override;
	void TearDown() override {}
};

//////////////////////////////////////////////////////////////////////////

class ODP2OOX_AnimationPlayAudioEnvironment : public ODP2OOX_AnimationEnvironment
{
public:
	ODP2OOX_AnimationPlayAudioEnvironment();

	static cpdoccore::oox::pptx_conversion_context& GetConversionContext();

private:
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document>		sInputOdf;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	sConverionContext;
};

class ODP2OOX_AnimationPlayAudioTest : public testing::Test
{
public:
	void SetUp() override;
	void TearDown() override
	{ }

public:
	cpdoccore::oox::pptx_conversion_context* mConversionContext;
};

//////////////////////////////////////////////////////////////////////////

class ODP2OOX_AnimationOpenDocumentEnvironment : public ODP2OOX_AnimationEnvironment
{
public:
	ODP2OOX_AnimationOpenDocumentEnvironment();

	static cpdoccore::oox::pptx_conversion_context& GetConversionContext();

private:
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document>		sInputOdf;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	sConverionContext;
};

class ODP2OOX_AnimationOpenDocumentTest : public testing::Test
{
public:
	void SetUp() override;
	void TearDown() override
	{ }

public:
	cpdoccore::oox::pptx_conversion_context* mConversionContext;
};

//////////////////////////////////////////////////////////////////////////

class ODP2OOX_AnimationRunProgramEnvironment : public ODP2OOX_AnimationEnvironment
{
public:
	ODP2OOX_AnimationRunProgramEnvironment();

	static cpdoccore::oox::pptx_conversion_context& GetConversionContext();

private:
	static boost::shared_ptr<cpdoccore::odf_reader::odf_document>		sInputOdf;
	static boost::shared_ptr<cpdoccore::oox::pptx_conversion_context>	sConverionContext;
};

class ODP2OOX_AnimationRunProgramTest : public testing::Test
{
public:
	void SetUp() override;
	void TearDown() override
	{ }

public:
	cpdoccore::oox::pptx_conversion_context* mConversionContext;
};

//////////////////////////////////////////////////////////////////////////

class OOX2ODP_PlayAudioAnimationEnvironment : public OOX2ODP_AnimationEnvironment
{
public:
	OOX2ODP_PlayAudioAnimationEnvironment();

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext();

private:
	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_PlayAudioAnimationTest : public OOX2ODP_AnimationTest
{
public:
	void SetUp() override;
	void TearDown() override {}
};

//////////////////////////////////////////////////////////////////////////

class OOX2ODP_OpenDocumentAnimationEnvironment : public OOX2ODP_AnimationEnvironment
{
public:
	OOX2ODP_OpenDocumentAnimationEnvironment();

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext();

private:
	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_OpenDocumentAnimationTest : public OOX2ODP_AnimationTest
{
public:
	void SetUp() override;
	void TearDown() override {}
};

//////////////////////////////////////////////////////////////////////////

class OOX2ODP_RunProgramAnimationEnvironment : public OOX2ODP_AnimationEnvironment
{
public:
	OOX2ODP_RunProgramAnimationEnvironment();

public:
	static cpdoccore::odf_writer::odp_conversion_context* GetContext();

private:
	static boost::shared_ptr<Oox2Odf::Converter> mConverter;
	static cpdoccore::odf_writer::odp_conversion_context* mContext;
};

class OOX2ODP_RunProgramAnimationTest : public OOX2ODP_AnimationTest
{
public:
	void SetUp() override;
	void TearDown() override {}
};
