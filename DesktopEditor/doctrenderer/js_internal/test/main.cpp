/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "../js_base.h"
#include <iostream>

using namespace NSJSBase;
int main(int argc, char *argv[])
{
	JSSmart<CJSContext> oContext1 = new CJSContext;
	oContext1->Initialize();

	JSSmart<CJSContext> oContext2 = new CJSContext;
	oContext2->Initialize();

	// Create first context
	oContext1->CreateContext();

	// Create second context
	oContext2->CreateContext();

	// Work with first context
	oContext1->Enter();

	JSSmart<CJSObject> oGlobal1 = oContext1->GetGlobal();
	JSSmart<CJSValue> oVar2 = oContext1->createString("Hel");
	oGlobal1->set("v1", oVar2.GetPointer());
	oContext1->runScript("var res = v1 + 'lo'");

	oContext1->Exit();

	// Work with second context
	oContext2->Enter();

	JSSmart<CJSObject> oGlobal2 = oContext2->GetGlobal();
	JSSmart<CJSValue> oVar4 = oContext2->createString("Wor");
	oGlobal2->set("v1", oVar4.GetPointer());
	oContext2->runScript("var res = v1 + 'ld!'");

	oContext2->Exit();

	// Print result from first context
	oContext1->Enter();

	JSSmart<CJSValue> oRes1 = oContext1->runScript("function f() { return res; }; f();");
	std::string strRes1 = oRes1->toStringA();
	std::cout << strRes1 << std::endl;

	oContext1->Exit();

	// Print second variable
	oContext2->Enter();

	JSSmart<CJSValue> oRes2 = oContext1->runScript("function f() { return res; }; f();");
	std::string strRes2 = oRes2->toStringA();
	std::cout << strRes2 << std::endl;

	oContext2->Exit();

	oContext1->Dispose();
	oContext2->Dispose();

	return 0;
}
