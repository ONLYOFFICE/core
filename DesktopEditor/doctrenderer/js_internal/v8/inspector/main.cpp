#include "../v8_base.h"
#include "inspector_impl.h"

int main(int argc, char *argv[])
{
    //V8 INIT
    JSSmart<NSJSBase::CJSContext> pJSContext = new NSJSBase::CJSContext();
    pJSContext->Initialize();

//    v8::V8::InitializeICU();
//    v8::Platform* v8Platform = v8::platform::CreateDefaultPlatform();
//    v8::V8::InitializePlatform(v8Platform);
//    v8::V8::Initialize();

//    v8::Isolate::CreateParams create_params;
//    create_params.array_buffer_allocator =
//            v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//    v8::Isolate* isolate = v8::Isolate::New(create_params);

    {
//        v8::Isolate::Scope isolate_scope(isolate);
//        v8::HandleScope scope(isolate);
//        v8::Handle<v8::Context> context = v8::Context::New(isolate);
//        v8::Context::Scope context_scope(context);
        JSSmart<NSJSBase::CJSIsolateScope> isolate_scope = pJSContext->CreateIsolateScope();
        JSSmart<NSJSBase::CJSLocalScope> handle_scope = pJSContext->CreateLocalScope();

        pJSContext->CreateGlobalForContext();
        pJSContext->CreateContext();

        JSSmart<NSJSBase::CJSContextScope> context_scope = pJSContext->CreateContextScope();
        JSSmart<NSJSBase::CJSTryCatch> try_catch = pJSContext->GetExceptions();
        JSSmart<NSJSBase::CJSObject> global_js = pJSContext->GetGlobal();

//        NSJSBase::v8_debug::internal::CInspectorImpl i{
//            context
////                    , CV8Worker::getInitializer()->getPlatform()
//                    , v8Platform
//                    , {true, 1, ""}
//            , 8080};
//        i.prepareServer();
//        std::cout << "server prepared\n";

        //trial shit
//        v8::Local<v8::Context> context = pJSContext->m_internal->m_context;
//        v8::Isolate *isolate = context->GetIsolate();

//        i.before();
//        std::cout << "before called 1\n";
//        const char *c1 = "function func1() {return 1 + 2;}func1();";
//        v8::Script::Compile(context
//                            , v8::String::NewFromOneByte(isolate, (uint8_t*)c1))
//                .ToLocalChecked()->Run(context).ToLocalChecked();

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
//        i.before();
//        std::cout << "before called 2\n";
//        const char *c2 = "var val = func1();";
//        v8::Script::Compile(context
//                            , v8::String::NewFromOneByte(isolate, (uint8_t*)c2))
//                .ToLocalChecked()->Run(context).ToLocalChecked();
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
        global_js->call_func("function_1");
        std::cout << "after four\n";

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

//    isolate->Dispose();
//    v8::V8::Dispose();
//    v8::V8::ShutdownPlatform();
//    delete create_params.array_buffer_allocator;

    pJSContext->Dispose();
    NSJSBase::CJSContext::ExternalDispose();
    return 0;
}
