#include <v8-inspector.h>//V8InspectorClient
#include <v8.h>//context
#include <libplatform/libplatform.h>//for v8 stuff
#include "inspector.h"
#include "singlethreadutils.h"

#include "../../js_base.h"
#include "../v8_base.h"

int main(int argc, char *argv[])
{
    //V8 INIT
    CV8Initializer *init = CV8Worker::getInitializer();//here throws an exception
    v8::Isolate *isolate = init->CreateNew();

    {
        //own isolate by this thread
//        v8::Locker lock(isolate);
        //all in current isolate
        v8::Isolate::Scope isolateScope(isolate);

        //all stack-allocated in current isolate
        v8::HandleScope handleScope(isolate);

        //stack-allocated in current isolate context of current isolate
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        //enter the context
        v8::Context::Scope contextScope(context);

        //string with script
        std::string rawString = "function a()\n\
                                        {\n\
                                        var k = 1;\n\
                                        return k + 3;\n\
                                        }\n\
                                        a();";

        std::string rawString1 = "(function(){return 2 + 3;})();";

        //make js context
        NSJSBase::CJSContext ctx;
        ctx.m_internal->m_isolate = isolate;
        ctx.m_internal->m_context = context;
//        JSSmart<NSJSBase::CJSTryCatch> tc(new NSJSBase::CV8TryCatch);

        //run
        ctx.runScript(rawString1);//calls inspector inside
    }

    return 0;
}
