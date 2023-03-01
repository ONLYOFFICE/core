#include "../../js_base.h"

#include "utils.h"	// for readFileContent()

/** TODO:
 * - implement InspectorPool
 * - implement InspectorInterface ???
 * - make work two consecutive runScript() methods
 * - implement inspector in CJSObjectV8::call_func() method
 * - test in two running contexts
 */

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	pContext->CreateContext();

	{
		CJSContextScope oScope(pContext);

		JSSmart<CJSValue> pRet = pContext->runScript(readFileContent("../example/code.js"));

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

	std::cout << "AFTER INSPECTOR" << std::endl;

	return 0;
}
