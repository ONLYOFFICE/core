/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
