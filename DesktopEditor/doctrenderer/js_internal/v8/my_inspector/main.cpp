#include "../../js_base.h"
#include "../v8_base.h"

#include "inspector.h"
#include "v8_inspector_listener_impl.h"

const static int port = 8080;

using namespace NSJSBase;
int main()
{
	JSSmart<CJSContext> pContext = new CJSContext();
	pContext->CreateContext();

	{
		CJSContextScope scope(pContext);
		v8::Local<v8::Context> context = pContext->m_internal->m_context;

		pContext->runScript("var special = 42;\n");

		Inspector inspector(CV8Worker::getInitializer().getPlatform(), context, port);

		inspector.addListener(new BaseV8InspectorListenerImpl());
		inspector.startAgent();

		std::cout << "\n\n\nLAST LINE\n";
	}

	return 0;
}
