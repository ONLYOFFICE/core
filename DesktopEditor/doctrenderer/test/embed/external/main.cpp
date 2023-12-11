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

#include <iostream>

#include "embed/Default.h"
#include "js_internal/js_base.h"
#include "Embed.h"

using namespace NSJSBase;
int main(int argc, char *argv[])
{
#if 0
	// Primitives example

	JSSmart<CJSContext> oContext1 = new CJSContext(false);
	oContext1->Initialize();

	JSSmart<CJSContext> oContext2 = new CJSContext;
//	oContext2->Initialize();

	// Work with first context
	oContext1->Enter();

	{
		CJSContextScope scope(oContext1);
		CJSLocalScope local_scope;
		JSSmart<CJSValue> oResLocal = oContext1->runScript("function f() { return 'Local scope test'; }; f();");
		std::cout << oResLocal->toStringA() << std::endl;
	}
	JSSmart<CJSObject> oGlobal1 = oContext1->GetGlobal();
	JSSmart<CJSValue> oVar2 = oContext1->createString("Hel");
	oGlobal1->set("v1", oVar2.GetPointer());
	oContext1->runScript("var res = v1 + 'lo'");

	oContext1->Exit();

	// Work with second context with CJSContextScope usage
	{
		CJSContextScope scope(oContext2);

		JSSmart<CJSObject> oGlobal2 = oContext2->GetGlobal();
		JSSmart<CJSValue> oVar4 = oContext2->createString("Wor");
		oGlobal2->set("v1", oVar4.GetPointer());
		oContext2->runScript("var res = v1 + 'ld!'");
	}

	// Print result from first context
	oContext1->Enter();

	JSSmart<CJSValue> oRes1 = oContext1->runScript("function f() { return res; }; f();");
	std::string strRes1 = oRes1->toStringA();
	std::cout << strRes1 << std::endl;

	// Print second variable
	oContext2->Enter();

	JSSmart<CJSValue> oRes2 = oContext1->runScript("function f() { return res; }; f();");
	std::string strRes2 = oRes2->toStringA();
	std::cout << strRes2 << std::endl;

	oContext2->Exit();
	oContext1->Exit();

//	oContext1->Dispose();
	oContext2->Dispose();

#endif

#if 0
	// External embed example

	JSSmart<CJSContext> oContext1 = new CJSContext();

	// Embedding
	CJSContextScope scope(oContext1);
	CJSContext::Embed<CTestEmbed>();

	JSSmart<CJSValue> oResTestEmbed1 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSum(10, 5); })();");
	std::cout << "FunctionSum(10, 5) = " << oResTestEmbed1->toInt32() << std::endl;

	JSSmart<CJSValue> oResTestEmbed2 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSquare(4); })();");
	std::cout << "FunctionSquare(4) = " << oResTestEmbed2->toInt32() << std::endl;

	JSSmart<CJSValue> oResTestEmbed3 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionDel(30, 3); })();");
	std::cout << "FunctionDel(30, 3) = " << oResTestEmbed3->toInt32() << std::endl;

	JSSmart<CJSValue> oResTestEmbed4 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionGet(); })();");
	std::cout << "FunctionGet() = " << oResTestEmbed4->toInt32() << std::endl;

#endif

#if 0
	// CZipEmbed example

	JSSmart<CJSContext> oContext1 = new CJSContext();
	JSSmart<CJSContext> oContext2 = new CJSContext();

	// Work with first context
	oContext1->Enter();
	CreateDefaults();
	JSSmart<CJSValue> oRes1 = oContext1->runScript(
		"var oZip = CreateEmbedObject('CZipEmbed');\n"
		"var files = oZip.open('" CURR_DIR "');\n"
		"oZip.close();");
	oContext1->Exit();

	// Work with second context
	{
		CJSContextScope scope(oContext2);
//		CreateDefaults();
		JSSmart<CJSValue> oRes2 = oContext2->runScript(
			"var oZip = CreateEmbedObject('CZipEmbed');\n"
			"var files = oZip.open('" CURR_DIR "/../embed');\n"
			"oZip.close();");
	}

	// Print first result
	oContext1->Enter();
	JSSmart<CJSObject> oGlobal1 = oContext1->GetGlobal();
	JSSmart<CJSArray> oFiles1 = oGlobal1->get("files")->toArray();
	std::cout << "\nRESULT FROM CONTEXT 1:\n";
	for (int i = 0; i < oFiles1->getCount(); i++)
	{
		std::cout << oFiles1->get(i)->toStringA() << std::endl;
	}

	// Print second result
	oContext2->Enter();
	JSSmart<CJSObject> oGlobal2 = oContext2->GetGlobal();
	JSSmart<CJSArray> oFiles2 = oGlobal2->get("files")->toArray();
	std::cout << "\nRESULT FROM CONTEXT 2:\n";
	for (int i = 0; i < oFiles2->getCount(); i++)
	{
		std::cout << oFiles2->get(i)->toStringA() << std::endl;
	}
	oContext2->Exit();
	oContext1->Exit();

#endif

#if 0
	// CHashEmbed example

	JSSmart<CJSContext> oContext1 = new CJSContext();

	// Call hash() on first context
	CJSContextScope scope(oContext1);
	CreateDefaults();
	JSSmart<CJSValue> oRes1 = oContext1->runScript(
		"var oHash = CreateEmbedObject('CHashEmbed');\n"
		"var str = 'test';\n"
		"var hash = oHash.hash(str, str.length, 0);");

	// Print first result
	JSSmart<CJSObject> oGlobal1 = oContext1->GetGlobal();
	JSSmart<CJSTypedArray> oHash = oGlobal1->get("hash")->toTypedArray();
	std::cout << "\nRESULTED HASH:\n";
	for (int i = 0; i < oHash->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(oHash->getData().Data[i]);
	}
	std::cout << std::endl;

	// Call hash2() on first context
	JSSmart<CJSValue> oRes2 = oContext1->runScript(
		"var str2 = 'test';\n"
		"var hash2 = oHash.hash2(str2, 'yrGivlyCImiWnryRee1OJw==', 100000, 7);");

	// Print first result
	JSSmart<CJSTypedArray> oHash2 = oGlobal1->get("hash2")->toTypedArray();
	std::cout << "\nRESULTED HASH2:\n";
	for (int i = 0; i < oHash2->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(oHash2->getData().Data[i]);
	}
	std::cout << std::endl;

#endif

#if 1
	// Mixed embed example

	JSSmart<CJSContext> oContext1 = new CJSContext();

	// External CTestEmbed
	CJSContextScope scope(oContext1);
	CJSContext::Embed<CTestEmbed>(false);

	JSSmart<CJSValue> oResTestEmbed1 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSum(10, 5); })();");
	if (oResTestEmbed1->isNumber())
		std::cout << "FunctionSum(10, 5) = " << oResTestEmbed1->toInt32() << std::endl;

	JSSmart<CJSObject> oTestEmbed = CJSContext::createEmbedObject("CTestEmbed");

//	JSSmart<CJSValue> oResTestEmbed2 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSquare(4); })();");
	JSSmart<CJSValue> args2[1];
	args2[0] = CJSContext::createInt(4);
	JSSmart<CJSValue> oResTestEmbed2 = oTestEmbed->call_func("FunctionSquare", 1, args2);
	if (oResTestEmbed2->isNumber())
		std::cout << "FunctionSquare(4) = " << oResTestEmbed2->toInt32() << std::endl;

//	JSSmart<CJSValue> oResTestEmbed3 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionDel(30, 3); })();");
	JSSmart<CJSValue> args3[2];
	args3[0] = CJSContext::createInt(30);
	args3[1] = CJSContext::createInt(3);
	JSSmart<CJSValue> oResTestEmbed3 = oTestEmbed->call_func("FunctionDel", 2, args3);
	if (oResTestEmbed3->isNumber())
		std::cout << "FunctionDel(30, 3) = " << oResTestEmbed3->toInt32() << std::endl;

	JSSmart<CJSValue> oResTestEmbed4 = oContext1->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionGet(); })();");
	if (oResTestEmbed4->isNumber())
		std::cout << "FunctionGet() = " << oResTestEmbed4->toInt32() << std::endl;

	// Internal CHashEmbed
	CreateDefaults();
	oContext1->runScript(
		"var oHash = CreateEmbedObject('CHashEmbed');\n"
		"var str = 'test';\n"
		"var hash = oHash.hash(str, str.length, 0);");

	// Print hash
	JSSmart<CJSObject> oGlobal = oContext1->GetGlobal();
	JSSmart<CJSTypedArray> oHash = oGlobal->get("hash")->toTypedArray();
	std::cout << "\nRESULTED HASH:\n";
	for (int i = 0; i < oHash->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(oHash->getData().Data[i]);
	}
	std::cout << std::endl;

	// Internal CZipEmbed
	oContext1->runScript(
		"var oZip = CreateEmbedObject('CZipEmbed');\n"
		"var files = oZip.open('" CURR_DIR "');\n"
		"oZip.close();");

	// Print files
	JSSmart<CJSArray> oFiles1 = oGlobal->get("files")->toArray();
	std::cout << "\nFILES IN CURRENT DIRECTORY:\n";
	for (int i = 0; i < oFiles1->getCount(); i++)
	{
		std::cout << oFiles1->get(i)->toStringA() << std::endl;
	}

#endif

	return 0;
}
