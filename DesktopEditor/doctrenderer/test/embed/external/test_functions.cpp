#include "test_functions.h"

#include <iostream>

#include "embed/Default.h"
#include "js_internal/js_base.h"
#include "Embed.h"

using namespace NSJSBase;

void testMultipleContexts()
{
	// passing `false` when creating CJSContext means that we need to initialize it manually
	JSSmart<CJSContext> context1 = new CJSContext(false);
	context1->Initialize();

	// while creating CJSContext without any arguments will create initialized CJSContext
	JSSmart<CJSContext> context2 = new CJSContext;
	// we don't need it
	// oContext2->Initialize();

	// work with first context
	context1->Enter();
	{
		CJSContextScope scope(context1);
		CJSLocalScope local_scope;
		JSSmart<CJSValue> oResLocal = context1->runScript("function f() { return 'Local scope test'; }; f();");
		std::cout << oResLocal->toStringA() << std::endl;
	}
	JSSmart<CJSObject> oGlobal1 = context1->GetGlobal();
	JSSmart<CJSValue> oVar2 = context1->createString("Hel");
	oGlobal1->set("v1", oVar2.GetPointer());
	context1->runScript("var res = v1 + 'lo'");

	context1->Exit();

	// Work with second context with CJSContextScope usage
	{
		CJSContextScope scope(context2);

		JSSmart<CJSObject> oGlobal2 = context2->GetGlobal();
		JSSmart<CJSValue> oVar4 = context2->createString("Wor");
		oGlobal2->set("v1", oVar4.GetPointer());
		context2->runScript("var res = v1 + 'ld!'");
	}

	// Print result from first context
	context1->Enter();

	JSSmart<CJSValue> oRes1 = context1->runScript("function f() { return res; }; f();");
	std::string strRes1 = oRes1->toStringA();
	std::cout << strRes1 << std::endl;

	// Print second variable
	context2->Enter();

	JSSmart<CJSValue> oRes2 = context1->runScript("function f() { return res; }; f();");
	std::string strRes2 = oRes2->toStringA();
	std::cout << strRes2 << std::endl;

	context2->Exit();
	context1->Exit();

//	oContext1->Dispose();
	context2->Dispose();
}

void testEmbedExternal()
{
	JSSmart<CJSContext> context = new CJSContext();

	// Embedding
	CJSContextScope scope(context);
	CJSContext::Embed<CTestEmbed>();

	JSSmart<CJSValue> res1 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSum(10, 5); })();");
	std::cout << "FunctionSum(10, 5) = " << res1->toInt32() << std::endl;

	JSSmart<CJSValue> res2 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSquare(4); })();");
	std::cout << "FunctionSquare(4) = " << res2->toInt32() << std::endl;

	JSSmart<CJSValue> res3 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionDel(30, 3); })();");
	std::cout << "FunctionDel(30, 3) = " << res3->toInt32() << std::endl;

	JSSmart<CJSValue> res4 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionGet(); })();");
	std::cout << "FunctionGet() = " << res4->toInt32() << std::endl;
}

void testEmbedInternal()
{
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
}

void testHashEmbed()
{
	JSSmart<CJSContext> context = new CJSContext();

	// test hash()
	CJSContextScope scope(context);
	CreateDefaults();
	context->runScript(
		"var oHash = CreateEmbedObject('CHashEmbed');\n"
		"var str = 'test';\n"
		"var hash = oHash.hash(str, str.length, 0);"
	);

	JSSmart<CJSObject> global = context->GetGlobal();
	JSSmart<CJSTypedArray> res_hash = global->get("hash")->toTypedArray();
	std::cout << "\nRESULTED HASH:\n";
	for (int i = 0; i < res_hash->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(res_hash->getData().Data[i]);
	}
	std::cout << std::endl;

	// test hash2()
	context->runScript(
		"var str2 = 'test';\n"
		"var hash2 = oHash.hash2(str2, 'yrGivlyCImiWnryRee1OJw==', 100000, 7);"
	);

	JSSmart<CJSTypedArray> res_hash2 = global->get("hash2")->toTypedArray();
	std::cout << "\nRESULTED HASH2:\n";
	for (int i = 0; i < res_hash2->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(res_hash2->getData().Data[i]);
	}
	std::cout << std::endl;
}

void testEmbedMixed()
{
	JSSmart<CJSContext> context = new CJSContext();
	CJSContextScope scope(context);


	// --- test external embedding with CTestEmbed ---
	// embed with `false` - means we are not able to create the object directly from JavaScript.
	CJSContext::Embed<CTestEmbed>(false);

	// example of incorrect usage:
	JSSmart<CJSValue> res1 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionSum(10, 5); })();");
	if (res1->isNumber())
	{
		// this wouldn't print anything, since `res1` not a number - embedded object was not created in JS and exception was thrown.
		std::cout << "FunctionSum(10, 5) = " << res1->toInt32() << std::endl;
	}

	// example of correct usage:
	JSSmart<CJSObject> embedded_object = CJSContext::createEmbedObject("CTestEmbed");
	JSSmart<CJSValue> args2[1];
	args2[0] = CJSContext::createInt(4);
	JSSmart<CJSValue> res2 = embedded_object->call_func("FunctionSquare", 1, args2);
	if (res2->isNumber())
	{
		// should print: "FunctionSquare(4) = 16"
		std::cout << "FunctionSquare(4) = " << res2->toInt32() << std::endl;
	}

	// another example of correct usage:
	JSSmart<CJSValue> args3[2];
	args3[0] = CJSContext::createInt(30);
	args3[1] = CJSContext::createInt(3);
	JSSmart<CJSValue> oResTestEmbed3 = embedded_object->call_func("FunctionDel", 2, args3);
	if (oResTestEmbed3->isNumber())
	{
		// should print: "FunctionDel(30, 3) = 10"
		std::cout << "FunctionDel(30, 3) = " << oResTestEmbed3->toInt32() << std::endl;
	}

	// another example of incorrect usage:
	JSSmart<CJSValue> oResTestEmbed4 = context->runScript("(function() { var value = CreateEmbedObject('CTestEmbed'); return value.FunctionGet(); })();");
	if (oResTestEmbed4->isNumber())
	{
		// again, this won't be executed and nothing will be printed
		std::cout << "FunctionGet() = " << oResTestEmbed4->toInt32() << std::endl;
	}


	// --- test internal embedding with CHashEmbed ---
	CreateDefaults();
	context->runScript(
		"var oHash = CreateEmbedObject('CHashEmbed');\n"
		"var str = 'test';\n"
		"var hash = oHash.hash(str, str.length, 0);"
	);
	// print hash
	JSSmart<CJSObject> global = context->GetGlobal();
	JSSmart<CJSTypedArray> hash = global->get("hash")->toTypedArray();
	std::cout << "\nRESULTED HASH:\n";
	for (int i = 0; i < hash->getCount(); i++)
	{
		std::cout << std::hex << static_cast<unsigned>(hash->getData().Data[i]);
	}
	std::cout << std::endl;


	// --- test internal embedding with CZipEmbed ---
	context->runScript(
		"var oZip = CreateEmbedObject('CZipEmbed');\n"
		"var files = oZip.open('" CURR_DIR "');\n"
		"oZip.close();"
	);

	// print files
	JSSmart<CJSArray> file_list = global->get("files")->toArray();
	std::cout << "\nFILES IN CURRENT DIRECTORY:\n";
	for (int i = 0; i < file_list->getCount(); i++)
	{
		std::cout << file_list->get(i)->toStringA() << std::endl;
	}
}
