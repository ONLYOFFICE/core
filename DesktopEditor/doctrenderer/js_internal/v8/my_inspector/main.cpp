#include "../../js_base.h"
#include "../v8_base.h"

#include "inspector.h"
#include "v8_inspector_listener_impl.h"

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	pContext->CreateContext();

	{
		CJSContextScope scope(pContext);

		pContext->runScript("var special = 42;\n");

		Inspector inspector(pContext, readFileContent("../example/code.js"));
		inspector.addListener(new BaseV8InspectorListenerImpl());
		inspector.startAgent();

		std::cout << "LAST LINE" << std::endl;

		// NOTE: now programm crashes if you close/refresh CDT tab in browser
	}

	return 0;
}
