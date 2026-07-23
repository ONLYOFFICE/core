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

#include "Test44363.h"

TestEnv44363* g_TestEnv44363 = (TestEnv44363*)testing::AddGlobalTestEnvironment(new TestEnv44363);

TestEnv44363::TestEnv44363()
	: ODT2DOCX_ConversionEnvironment(L"ExampleFiles/44363.odt")
{ }

void Test44363::SetUp()
{
	Docx = g_TestEnv44363->GetDocument();
}

TEST_F(Test44363, Docx)
{
	EXPECT_TRUE(Docx);
}

TEST_F(Test44363, document)
{
	EXPECT_TRUE(Docx->m_oMain.document);
}

TEST_F(Test44363, document_elements_size)
{
	uint32_t expected = 5;

	EXPECT_EQ(Docx->m_oMain.document->m_arrItems.size(), expected);
}

TEST_F(Test44363, paragraph)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);
	EXPECT_TRUE(p1);
}

TEST_F(Test44363, paragraph_props)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	EXPECT_TRUE(p1->m_oParagraphProperty);
}

TEST_F(Test44363, paragraph_props_run_props)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
}

TEST_F(Test44363, paragraph_props_run_props_bold)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	ASSERT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr->m_oBold.IsInit());
}

TEST_F(Test44363, paragraph_props_run_props_italic)
{
	OOX::Logic::CParagraph* p1 = dynamic_cast<OOX::Logic::CParagraph*>(Docx->m_oMain.document->m_arrItems[0]);

	ASSERT_TRUE(p1->m_oParagraphProperty->m_oRPr.IsInit());
	EXPECT_TRUE(p1->m_oParagraphProperty->m_oRPr->m_oItalic.IsInit());
}
