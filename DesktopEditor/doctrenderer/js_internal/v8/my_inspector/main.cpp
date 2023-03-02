#include "../../js_base.h"

#include "utils.h"	// for readFileContent()

/** TODO:
 * - implement InspectorInterface ???
 * - wrap all inspector code in a namespace ???
 * - reformat to satisfy codestyle
 * - rename `my_inspector` to `inspector` and remove all old inspector stuff
 * - dispose inspector in CJSContext::Exit() ???
 * - test for old version of V8
 */

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext1 = new CJSContext();
	pContext1->CreateContext();


	JSSmart<CJSContext> pContext2 = new CJSContext();
	pContext2->CreateContext();

	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSValue> pRet = pContext1->runScript("var special = 42;\n"
													  "special;\n");

		{
			CJSContextScope oScope(pContext2);


			JSSmart<CJSValue> pRet = pContext2->runScript(readFileContent("../example/code.js"));

			std::cout << "RESULT: ";
			if (pRet->isString())
			{
				std::cout << pRet->toStringA() << std::endl;
			}
			else
			{
				std::cout << "ERROR!" << std::endl;
			}
		}

		std::cout << "RESULT: ";
		if (pRet->isNumber())
		{
			std::cout << pRet->toInt32() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}


	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSValue> pRet = pContext1->runScript("function sayHi(name) {\n"
													  "  var a = special;\n"
													  "  var msg = 'Hi, ' + name;\n"
													  "  return msg;\n"
													  "}\n"
													  "sayHi('Foo');\n");
		pContext1->runScript("sayHi('Foo 2');\n");
		std::cout << "RESULT: ";
		if (pRet->isString())
		{
			std::cout << pRet->toStringA() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}

	{
		CJSContextScope oScope(pContext1);

		JSSmart<CJSObject> pGlobal = pContext1->GetGlobal();
		JSSmart<CJSValue> argv[1];
		argv[0] = pContext1->createString("Bar");
		JSSmart<CJSValue> pRet = pGlobal->call_func("sayHi", 1, argv);

		std::cout << "RESULT: ";
		if (pRet->isString())
		{
			std::cout << pRet->toStringA() << std::endl;
		}
		else
		{
			std::cout << "ERROR!" << std::endl;
		}
	}

	pContext1->Dispose();
	std::cout << "AFTER CONTEXT1 DISPOSE" << std::endl;
	pContext2->Dispose();
	std::cout << "AFTER CONTEXT2 DISPOSE" << std::endl;

	return 0;
}
