#include "../v8_base.h"
#include "inspector_impl.h"

int main(int argc, char *argv[])
{
    //V8 INIT
    JSSmart<NSJSBase::CJSContext> pJSContext = new NSJSBase::CJSContext();
    pJSContext->Initialize();

    {
        JSSmart<NSJSBase::CJSIsolateScope> isolate_scope = pJSContext->CreateIsolateScope();
        JSSmart<NSJSBase::CJSLocalScope> handle_scope = pJSContext->CreateLocalScope();

        pJSContext->CreateGlobalForContext();
        pJSContext->CreateContext();

        JSSmart<NSJSBase::CJSContextScope> context_scope = pJSContext->CreateContextScope();
        JSSmart<NSJSBase::CJSTryCatch> try_catch = pJSContext->GetExceptions();
        JSSmart<NSJSBase::CJSObject> global_js = pJSContext->GetGlobal();

        //zero
        pJSContext->
                runScript(
                                "var a = 1;\nvar b = 2;\nvar c = 3;\nvar d = 4;", try_catch
                            );
        try_catch->Check();
        std::cout << "after zero\n";


        //one
        pJSContext->
                runScript(
                                "function function_1() \n { \n return 1 + 2;\n }", try_catch
                            );
        try_catch->Check();
        std::cout << "after one\n";

        //two
        pJSContext->
                runScript(
                        "function function_2()\n {\n return 100 + function_1(); \n}", try_catch
                    );
        try_catch->Check();
        std::cout << "after two\n";

        //three
        JSSmart<NSJSBase::CJSValue> js_result =
                global_js->call_func("function_1")
                ;
        std::cout << "after three\n";

        //four
        JSSmart<NSJSBase::CJSValue> js_result2 = global_js->call_func("function_2");
        std::cout << "after four\n";

        std::cout << "function_1 = ";
        //check
        if (js_result.IsInit()){
            if (js_result->isNumber()) {
                std::cout << js_result->toInt32() << std::endl;
            } else {
                std::cout << "no result\n";
            }
        } else {
            std::cout << "result not init\n";
        }


        std::cout << "function_2 = ";
        //check
        if (js_result.IsInit()){
            if (js_result2->isNumber()) {
                std::cout << js_result2->toInt32() << std::endl;
            } else {
                std::cout << "no result\n";
            }
        } else {
            std::cout << "result not init\n";
        }
    }

    pJSContext->Dispose();
    NSJSBase::CJSContext::ExternalDispose();
    return 0;
}
