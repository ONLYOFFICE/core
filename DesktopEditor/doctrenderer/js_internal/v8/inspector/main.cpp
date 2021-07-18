#include "../v8_base.h"

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

        //one
        pJSContext->runScript("function function_1() { return 1 + 2; }", try_catch);
        try_catch->Check();

        //two
        pJSContext->runScript("function function_2() { return 100 + function_1(); }", try_catch);
        try_catch->Check();

        //three
        JSSmart<NSJSBase::CJSValue> js_result = global_js->call_func("function_2");

        //check
        if (js_result->isNumber()) {
            std::cout << js_result->toInt32() << std::endl;
        } else {
            std::cout << "no result\n";
        }
    }

    pJSContext->Dispose();
    NSJSBase::CJSContext::ExternalDispose();
    return 0;
}
