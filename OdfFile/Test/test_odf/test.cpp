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

#include "gtest/gtest.h"

#include "common.h"

//#include "entrance.h"
//#include "motion.h"
//#include "audio.h"
//#include "interactions.h"

class TestEnv : public ODT2DOCX_ConversionEnvironment
{
public:
	TestEnv()
		: ODT2DOCX_ConversionEnvironment(L"ExampleFiles/motion.odp")
	{
	}

	void SetUp() override
	{
		ODT2DOCX_ConversionEnvironment::SetUp();
	}
};



int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	::testing::AddGlobalTestEnvironment(new TestEnv);

	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationEntranceEnvironment);
	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationMotionEnvironment);
	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationAudioEnvironment);
	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationPlayAudioEnvironment);	
	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationOpenDocumentEnvironment);
	//::testing::AddGlobalTestEnvironment(new ODP2OOX_AnimationRunProgramEnvironment);
	//
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_EntranceAnimationEnvironment);
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_AudioAnimationEnvironment);
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_InteractionAnimationEnvironment);
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_PlayAudioAnimationEnvironment);
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_OpenDocumentAnimationEnvironment);
	//::testing::AddGlobalTestEnvironment(new OOX2ODP_RunProgramAnimationEnvironment);
	
	return RUN_ALL_TESTS();
}