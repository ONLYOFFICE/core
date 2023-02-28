#include "../../js_base.h"
#include "../v8_base.h"

#include "utils.h"

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	pContext->CreateContext();

	{
		CJSContextScope scope(pContext);

		pContext->runScript(readFileContent("../example/code.js"));

		std::cout << "LAST LINE" << std::endl;

		// TODO: for now programm crashes on deleting WebsocketServer. Need to fix it somehow.
	}

	return 0;
}
