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

//        NSJSBase::v8_debug::internal::CInspectorImpl i{
//            pJSContext->m_internal->m_context
//                    , CV8Worker::getInitializer()->getPlatform()
//                    , {true, 1, ""}
//            , 8080};
//        i.prepareServer();

        //one
        pJSContext->
//        i.
                runScript(
//                            {
                                "function function_1() { return 1 + 2; }function_1();", try_catch, L""
//                            }
                            );
        try_catch->Check();
        std::cout << "after one\n";

        //two
        pJSContext->
//        i.
                runScript(
//                    {
                        "function function_2() { return 100 + function_1(); }", try_catch, L""
//                    }
                    );
        try_catch->Check();
        std::cout << "after two\n";

        //three

        JSSmart<NSJSBase::CJSValue> js_result =


                global_js->call_func("function_2")


//                i.callFunc({
//                               static_cast<NSJSBase::CJSObjectV8*>(global_js.GetPointer())->value
//                               , "function_2"
//                               , 0
//                               , nullptr
//                           });
                ;
        std::cout << "after three\n";

        //four
//        i.callFunc({
//                       static_cast<NSJSBase::CJSObjectV8*>(global_js.GetPointer())->value
//                       , "function_1"
//                       , 0
//                       , nullptr
//                   });
//        std::cout << "after four\n";

        //check
        if (js_result.IsInit()){
            if (js_result->isNumber()) {
                std::cout << js_result->toInt32() << std::endl;
            } else {
                std::cout << "no result\n";
            }
        }
        else
        {
            std::cout << "result not init\n";
        }
    }

    pJSContext->Dispose();
    NSJSBase::CJSContext::ExternalDispose();
    return 0;
}
