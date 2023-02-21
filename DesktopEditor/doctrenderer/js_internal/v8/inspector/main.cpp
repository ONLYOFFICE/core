#include "../../js_base.h"
#include "inspector_impl.h"

#include <boost/asio.hpp>

#include <iostream>

using namespace NSJSBase;
int main(int argc, char *argv[])
{

	// V8 INIT
	JSSmart<CJSContext> pJSContext = new CJSContext();
	pJSContext->CreateContext();

	{
		CJSContextScope scope(pJSContext);
		JSSmart<CJSTryCatch> try_catch = pJSContext->GetExceptions();

		JSSmart<CJSObject> global_js = pJSContext->GetGlobal();

		// zero
		pJSContext->runScript("var a = 1;\n"
							  "var b = 2;\n"
							  "var c = 3;\n"
							  "var d = 4;",
							  try_catch);
		try_catch->Check();
		std::cout << "after zero\n";

#if 0
		// one
		pJSContext->runScript("function f1()\n"
							  "{\n"
							  "  return 1 + 2;\n"
							  "}"
							  , try_catch);
		try_catch->Check();
		std::cout << "after one\n";

		// two
		pJSContext->runScript("function f2()\n"
							  "{\n"
							  "  return 100 + f1();\n"
							  "}"
							  , try_catch);
		try_catch->Check();
		std::cout << "after two\n";

		// three
		JSSmart<CJSValue> js_result = global_js->call_func("f1");
		std::cout << "after three\n";

		// four
		JSSmart<CJSValue> js_result2 = global_js->call_func("f2");
		std::cout << "after four\n";

		std::cout << "result of calling function f1: ";
		// check
		if (js_result.IsInit())
		{
			if (js_result->isNumber())
			{
				std::cout << js_result->toInt32() << std::endl;
			}
			else
			{
				std::cout << "no result\n";
			}
		}
		else
		{
			std::cout << "result is not init\n";
		}


		std::cout << "result of calling function f1: ";
		//check
		if (js_result.IsInit())
		{
			if (js_result2->isNumber())
			{
				std::cout << js_result2->toInt32() << std::endl;
			}
			else
			{
				std::cout << "no result\n";
			}
		}
		else
		{
			std::cout << "result is not init\n";
		}
#endif
	}

	return 0;
}
