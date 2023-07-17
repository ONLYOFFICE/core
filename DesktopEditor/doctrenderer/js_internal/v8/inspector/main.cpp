#include "../../js_base.h"

#include <iostream>
#include <fstream>

std::string readFileContent(const std::string& sFilename)
{
	std::ifstream oFile(sFilename);
	if (!oFile.is_open())
	{
		std:: cerr << "Could not open the file: " << sFilename << std::endl;
		return "";
	}
	std::string sContent((std::istreambuf_iterator<char>(oFile)), std::istreambuf_iterator<char>());
	return sContent;
}

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext1 = new CJSContext();
	JSSmart<CJSContext> pContext2 = new CJSContext();

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
