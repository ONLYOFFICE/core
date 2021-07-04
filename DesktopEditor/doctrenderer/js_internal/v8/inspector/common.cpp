#include "common.h"
#include <sstream>//convert file contents to string
#include <fstream>//get file contents
#include "cwebsocketlistener.h"//make listener
#include "cinspectorclient.h"//make inspector client
#include "v8.h"//for v8 stuff
#include "libplatform/libplatform.h"//for v8 stuff
#include <chrono>
#include <thread>

namespace {
    //
    std::string makeUrlString(uint16_t port) {
        std::string address =
                "wss://localhost:"
//                "wss://echo.websocket.org"
                ;
        std::ostringstream stream;
        stream << port;
        std::string port_s = stream.str();
        std::string result = address
                + port_s
                + '/'
                ;
        std::clog << "url: " << result << std::endl;
        return result;
    }

    //
    std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> makeDebugSocket(uint16_t port,
                                                                        std::shared_ptr<NSNetwork::NSWebSocket::IListener> listener)
    {
        std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> pSocket =
                NSNetwork::NSWebSocket::createWebsocket("ixwebsocket", listener, makeUrlString(port));
        pSocket->open();
        return pSocket;
    }
}

std::string getContents(std::istream &in)
{
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string getFileData(const std::string &filename)
{
    std::ifstream f(filename);
    return getContents(f);
}

std::unique_ptr<v8_inspector::V8InspectorClient> makeInspectorClient(v8::Local<v8::Context> context, const std::string &contextName, uint16_t port)
{
    std::shared_ptr<CWebSocketListener> listener = std::make_shared<CWebSocketListener>();
    std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> psocket = makeDebugSocket(port, listener);
    return std::make_unique<CInspectorClient>(context, contextName, psocket, listener);
}

void makeV8Stuff(const char *execLocation)
{
    //V8 INIT

    //init ICU in current program's location
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
        //all in current isolate
        v8::Isolate::Scope isolateScope(isolate);

        //all stack-allocated in current isolate
        v8::HandleScope handleScope(isolate);

        //stack-allocated in current isolate context of current isolate
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        //enter the context
        v8::Context::Scope contextScope(context);


        //make inspector
        std::unique_ptr<v8_inspector::V8InspectorClient> client =
                makeInspectorClient(context, "kkk", 8080)
                ;

        //string with script
//        std::string rawString = getFileData("D:/js_sample.js");
        const char rawString[] = "function a()\
                                        {\
                                        return 2 + 3;\
                                        }\
                                        a();";
        v8::Local<v8::String> string =
                v8::String::NewFromUtf8(
                    isolate
                    , rawString
//                    .data()
                    , v8::NewStringType::kNormal
//                    , rawString.length()
                    ).ToLocalChecked();

        //compiled script; it can be casted to string - surprising
        v8::Local<v8::Script> script = v8::Script::Compile(context, string).ToLocalChecked();



        uint64_t count = 0;
        while (true) {
            //get the result
            v8::Local<v8::Value> result =
                    script->Run(context).ToLocalChecked();


            int64_t val = result->ToInt32(context).ToLocalChecked()->IntegerValue(context).ToChecked();
            std::cout << "iteration " << count++ << " result = " << val << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }

    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    //
    delete createParams.array_buffer_allocator;
}
