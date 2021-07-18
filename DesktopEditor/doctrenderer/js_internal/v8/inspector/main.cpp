#include <v8-inspector.h>//V8InspectorClient
#include <v8.h>//context
#include <libplatform/libplatform.h>//for v8 stuff

#include "../../js_base.h"
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

        //possibly own isolate by this thread
//        v8::Locker lock(isolate);
        //all in current isolate
//        v8::Isolate::Scope isolateScope(isolate);

        //all stack-allocated in current isolate
//        v8::HandleScope handleScope(isolate);

        //stack-allocated in current isolate context of current isolate
//        v8::Local<v8::Context> context = v8::Context::New(isolate);

        //enter the context
//        v8::Context::Scope contextScope(context);


        //string with script
        std::string rawString = "function a()\n\
                                        {\n\
                                        var k = 1;\n\
                                        var kk = 11;\n\
                                        return k + kk;\n\
                                        }";
        std::string f2 =  "function b() {var kkk = 111; return a() + kkk;}";

        //make js context
//        JSSmart<NSJSBase::CJSTryCatch> tc(new NSJSBase::CV8TryCatch);

        //run
        pJSContext->runScript(rawString);//calls inspector inside
        pJSContext->runScript(f2);
        //to inspect
        JSSmart<NSJSBase::CJSValue> js_result = global_js->call_func("b");

        //check
        if (js_result->isNumber()) {
            std::cout << js_result->toInt32() << std::endl;
        } else {
            std::cout << "nope\n";
        }
    }
    pJSContext->Dispose();
    NSJSBase::CJSContext::ExternalDispose();
    return 0;
}
