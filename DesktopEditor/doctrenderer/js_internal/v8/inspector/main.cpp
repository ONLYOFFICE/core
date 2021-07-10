#include <v8-inspector.h>//V8InspectorClient
#include <v8.h>//context
#include <libplatform/libplatform.h>//for v8 stuff
#include "ninspector.h"
#include "singlethreadutils.h"
//#include "../../js_base.h"

int main(int argc, char *argv[])
{
    //V8 INIT

    //init ICU in current program's location
    const char *execLocation = argv[0];
    v8::V8::InitializeICUDefaultLocation(execLocation);
    //some external startup data is build in (what exactly?)
    v8::V8::InitializeExternalStartupData(execLocation);
    //
    std::unique_ptr<v8::Platform> platform(v8::platform::CreateDefaultPlatform());
    //init platform
    v8::V8::InitializePlatform(platform.get());
    //init v8
    v8::V8::Initialize();
    //ISOLATE
    //create params for isolate
    v8::Isolate::CreateParams createParams;
    //
    createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    //isolate itself
    v8::Isolate *isolate = v8::Isolate::New(createParams);




    {
        //own isolate by this thread
        v8::Locker lock(isolate);
        //all in current isolate
        v8::Isolate::Scope isolateScope(isolate);

        //all stack-allocated in current isolate
        v8::HandleScope handleScope(isolate);

        //stack-allocated in current isolate context of current isolate
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        //enter the context
        v8::Context::Scope contextScope(context);





        //string with script
        const char rawString[] = "function a()\
                                        {\
                                        return 2 + 3;\
                                        }\
                                        a();";
        v8::Local<v8::String> string =
                v8::String::NewFromUtf8(
                    isolate
                    , rawString
                    , v8::NewStringType::kNormal
                    ).ToLocalChecked();

        //compiled script; it can be casted to string - surprising
        v8::Local<v8::Script> script = v8::Script::Compile(context, string).ToLocalChecked();

        //make inspector
        NSJSBase::v8_debug::CInspector inspector(context
                             , platform.get()
                             , NSJSBase::v8_debug::internal::getFileScript(
                                                 context
                                                 ,"D:/111/work/v8-debug/v8-debug/scripts/sample.js"
                                            )
                             , true
                             );
        inspector.run();

    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();


    //
    delete createParams.array_buffer_allocator;
    return 0;
}
