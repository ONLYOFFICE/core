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
	// we don't need it:
	// oContext2->Initialize();

	// entering the first context
	context1->Enter();
	{
		// entering the first context the second time in scope creation  - it's allowed and the scopes stack correctly
		CJSContextScope scope(context1);
		// allocate new local variables inside a custom local scope instead of
		//  the one that is provided by CJSContextScope by default
		CJSLocalScope local_scope;
		JSSmart<CJSValue> res_local = context1->runScript("(function() { return 'Local scope test'; })();");
		std::cout << res_local->toStringA() << std::endl;
		// the first context is going to be exited at the end of the scope
	}
	// at this moment we are still inside the first context, since we have entered it twices
	JSSmart<CJSObject> global1 = context1->GetGlobal();
	JSSmart<CJSValue> var = context1->createString("Hel");
	global1->set("v1", var);
	// here `res` is set to be "Hello"
	context1->runScript("var res = v1 + 'lo'");
	// exit from the first context
	context1->Exit();

	// now we are going to work with the second context
	{
		// enter the second context via context scope
		CJSContextScope scope(context2);

		JSSmart<CJSObject> global2 = context2->GetGlobal();
		JSSmart<CJSValue> var = context2->createString("Wor");
		global2->set("v1", var);
		// `res` is "World!"
		context2->runScript("var res = v1 + 'ld!'");
		// the second context is going to be exited at the end of the scope
	}

	// enter the first context
	context1->Enter();
	// print `res` variable from the first context
	JSSmart<CJSValue> res1 = context1->runScript("(function() { return res; })();");
	std::string str_res1 = res1->toStringA();
	std::cout << str_res1 << ", ";
	// make new variable `v2` in first context
	// important to note, that accessing previous `global1` object is undefined behaviour and can lead to crashes!
	// that is because when we exited from the first context, every local handle inside CJSValue and CJSObject was invalidated
	global1 = context1->GetGlobal();
	global1->set("v2", CJSContext::createInt(42));

	// enter from the second context (notice, we haven't exited the first context before)
	context2->Enter();
	// print `res` variable from the second context
	JSSmart<CJSValue> res2 = context1->runScript("(function() { return res; })();");
	std::string str_res2 = res2->toStringA();
	std::cout << str_res2 << std::endl;

	// exit from the second context
	context2->Exit();

	// at this moment we are still in the first context
	// we can validate that accessing `v2` variable, which exists only in the first context
	global1 = context1->GetGlobal();
	std::cout << global1->get("v2")->toInt32() << std::endl;
	// exit from the first context
	context1->Exit();

	// manual disposing is not necessary, since it's going to be called in CJSContext's destructor anyway
	// so we don't need to explicitly write:
	// oContext1->Dispose();

	// but still nothing wrong will happen if we do:
	context2->Dispose();
}

void testEmbedExternal()
{
	JSSmart<CJSContext> context = new CJSContext();

	// Embedding
	CJSContextScope scope(context);
	CJSContext::Embed<CTestEmbed>();

	JSSmart<CJSValue> res1 = context->runScript("(function() { let value = CreateEmbedObject('CTestEmbed'); let ret = value.FunctionSum(10, 5); FreeEmbedObject(value); return ret; })();");
	std::cout << "FunctionSum(10, 5) = " << res1->toInt32() << std::endl;

	JSSmart<CJSValue> res2 = context->runScript("(function() { let value = CreateEmbedObject('CTestEmbed'); let ret = value.FunctionSquare(4); FreeEmbedObject(value); return ret; })();");
	std::cout << "FunctionSquare(4) = " << res2->toInt32() << std::endl;

	JSSmart<CJSValue> res3 = context->runScript("(function() { let value = CreateEmbedObject('CTestEmbed'); let ret = value.FunctionDel(30, 3); FreeEmbedObject(value); return ret; })();");
	std::cout << "FunctionDel(30, 3) = " << res3->toInt32() << std::endl;

	JSSmart<CJSValue> res4 = context->runScript("(function() { let value = CreateEmbedObject('CTestEmbed'); let ret = value.FunctionGet(); FreeEmbedObject(value); return ret; })();");
	std::cout << "FunctionGet() = " << res4->toInt32() << std::endl;
}

void testEmbedInternal()
{
	// create both contexts
	JSSmart<CJSContext> context1 = new CJSContext();
	JSSmart<CJSContext> context2 = new CJSContext();

	// work with the first context
	context1->Enter();
	// create embedding info for internally embedded objects (CZipEmbed is the one of them)
	CreateDefaults();
	context1->runScript(
		"var oZip = CreateEmbedObject('CZipEmbed');\n"
		"var files = oZip.open('" CURR_DIR "');\n"
		"oZip.close();"
	);
	context1->Exit();

	// work with the second context via context scope
	{
		CJSContextScope scope(context2);
		// function CJSContext::Embed() is context-independent, so we don't actually need to call it in another context
		// CreateDefaults();
		context2->runScript(
			"var oZip = CreateEmbedObject('CZipEmbed');\n"
			"var files = oZip.open('" CURR_DIR "/../../../embed');\n"
			"oZip.close();"
		);
	}

	// print the files from the first context
	context1->Enter();
	JSSmart<CJSObject> global1 = context1->GetGlobal();
	JSSmart<CJSArray> file_list1 = global1->get("files")->toArray();
	std::cout << "\nRESULT FROM CONTEXT 1:\n";
	for (int i = 0; i < file_list1->getCount(); i++)
	{
		std::cout << file_list1->get(i)->toStringA() << std::endl;
	}

	// print the files from the second result (note, we haven't exited the first context before)
	context2->Enter();
	JSSmart<CJSObject> global2 = context2->GetGlobal();
	JSSmart<CJSArray> file_list2 = global2->get("files")->toArray();
	std::cout << "\nRESULT FROM CONTEXT 2:\n";
	for (int i = 0; i < file_list2->getCount(); i++)
	{
		std::cout << file_list2->get(i)->toStringA() << std::endl;
	}

	// exit the contexts in reverse order
	context2->Exit();
	context1->Exit();
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
