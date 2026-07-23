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

#include "docbuilder.python/src/docbuilder_functions.h"

#include <string>
#include <iostream>

#include "app_builder_lib/utils.cpp"

#define DECLARE_RAII_DOCBUILDER_FUNC_CLASS( CLASS_NAME )							\
class CLASS_NAME ## F                                                               \
{                                                                                   \
public:                                                                             \
	CLASS_NAME ## F(CLASS_NAME* internal) { m_internal = internal; }				\
	~CLASS_NAME ## F() { CLASS_NAME ## _Destroy(m_internal); }                      \
																					\
	CLASS_NAME* get() { return m_internal; }                                        \
																					\
private:                                                                            \
	CLASS_NAME* m_internal;                                                         \
};

DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilder)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderValue)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderContext)
DECLARE_RAII_DOCBUILDER_FUNC_CLASS(CDocBuilderContextScope)

int main()
{
	std::wstring sWorkDirectory = NSUtils::GetBuilderDirectory();

#if 0
	// Simple test that shows builder version if everything is correct
	CDocBuilder_InitializeWithDirectory(sWorkDirectory.c_str());
	CDocBuilder* pBuilder = CDocBuilder_Create();

	char* sVersion = CDocBuilder_GetVersion(pBuilder);
	std::cout << sVersion << std::endl;
	DeleteCharP(sVersion);

	CDocBuilder_Dispose();
	CDocBuilder_Destroy(pBuilder);
#else
	// Test is identical to app_builder_lib.pro
	// The test uses RAII wrappers - classes with 'F' postfix, which are destroyed automatically
	CDocBuilder_InitializeWithDirectory(sWorkDirectory.c_str());

	CDocBuilderF oBuilder = CDocBuilder_Create();
	CDocBuilder_SetProperty(oBuilder.get(), L"--work-directory", sWorkDirectory.c_str());

	CDocBuilder_CreateFileByExtension(oBuilder.get(), L"docx");

	CDocBuilderContextF oContext = CDocBuilder_GetContext(oBuilder.get());
	CDocBuilderContextScopeF oScope = CDocBuilderContext_CreateScope(oContext.get());

	CDocBuilderValueF oGlobal = CDocBuilderContext_GetGlobal(oContext.get());

	CDocBuilderValueF oApi = CDocBuilderValue_GetProperty(oGlobal.get(), L"Api");
	CDocBuilderValueF oDocument = CDocBuilderValue_Call0(oApi.get(), L"GetDocument");
	CDocBuilderValueF oParagraph = CDocBuilderValue_Call0(oApi.get(), L"CreateParagraph");
	CDocBuilderValue_Call2(oParagraph.get(), L"SetSpacingAfter", CDocBuilderValueF(CDocBuilderValue_CreateWithInt(1000)).get(), CDocBuilderValueF(CDocBuilderValue_CreateWithBool(false)).get());
	CDocBuilderValue_Call1(oParagraph.get(), L"AddText", CDocBuilderValueF(CDocBuilderValue_CreateWithString(L"Hello, world!")).get());
	CDocBuilderValueF oContent = CDocBuilderContext_CreateArray(oContext.get(), 1);
	CDocBuilderValue_SetByIndex(oContent.get(), 0, oParagraph.get());
	CDocBuilderValue_Call1(oDocument.get(), L"InsertContent", oContent.get());

	std::wstring sProcessDirectory = NSUtils::GetProcessDirectory();
	std::wstring sDstPath = sProcessDirectory + L"/result.docx";
	CDocBuilder_SaveFileByExtension(oBuilder.get(), L"docx", sDstPath.c_str());
	CDocBuilder_CloseFile(oBuilder.get());

	CDocBuilder_Dispose();
#endif

	return 0;
}
