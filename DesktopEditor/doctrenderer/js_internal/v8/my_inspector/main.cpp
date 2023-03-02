#include "../../js_base.h"

#include "utils.h"	// for readFileContent()

/** TODO:
 * - implement InspectorInterface ???
 * - implement inspector in CJSObjectV8::call_func() method
 * - wrap all inspector code in a namespace ???
 * - reformat to satisfy codestyle
 * - close websocket in ~WebsocketServer()
 * - store pointer to string in Inspector instead of whole string itself
 * - try to continue C++ runScript() method code when Runtime.runIfWaitingForDebugger() recieved (like in older code)
 * - rename `my_inspector` to `inspector`
 * - dispose inspector on CJSContext::Exit() ???
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

		JSSmart<CJSValue> pRet = pContext1->runScript("var special = 42;\nspecial;");

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

	pContext1->Dispose();
	std::cout << "AFTER CONTEXT1 DISPOSE" << std::endl;
	pContext2->Dispose();
	std::cout << "AFTER CONTEXT2 DISPOSE" << std::endl;

	return 0;
}
