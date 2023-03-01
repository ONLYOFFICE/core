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
	}

	std::cout << "AFTER INSPECTOR" << std::endl;

	return 0;
}
