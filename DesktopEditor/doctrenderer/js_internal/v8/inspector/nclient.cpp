#include "nclient.h"
#include "nchannel.h"
#include "singlethreadutils.h"
#include <libplatform/libplatform.h>
#include <iostream>
#include <thread>
#include "../../v8/v8_base.h"

namespace {
    //
    class FalseSetter {
        std::atomic<bool> &m_bool;
    public:
        FalseSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = false;}
        ~FalseSetter() {m_bool = true;}
    };
    //
    class TrueSetter {
        std::atomic<bool> &m_bool;
    public:
        TrueSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = true;}
        ~TrueSetter() {m_bool = false;}
    };
}

void NSJSBase::v8_debug::internal::CInspectorClient::setUpDebuggingSession(
        CInspectorChannel::sendDataCallback sendDataCallback)
{
    //client instance is this

    //channel
    m_pChannel = std::make_unique<CInspectorChannel>(
                std::move(sendDataCallback)
                );

    //inspector
    m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);



    //state for session
    v8_inspector::StringView state{};

    //session
    m_pSession = m_pInspector->connect(m_iContextGroupId, m_pChannel.get(), state);

    //context name as string view
    v8_inspector::StringView viewContextName = strToView(m_sContextName);

    //context info
    v8_inspector::V8ContextInfo info{
        m_Context
                , m_iContextGroupId
                , viewContextName
    };

    //register context objects in inspector
    m_pInspector->contextCreated(info);
}

void NSJSBase::v8_debug::internal::CInspectorClient::pumpPlatform()
{
    while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) {
        //just pump until pumped
    }
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(v8::Local<v8::Context> context//for some stuff
        , const std::string &contextName//why not
        , v8::Platform *platform
        , v8::Local<v8::Script> script
        , CInspectorChannel::sendDataCallback sendDataFunc//for channel
        , waitMessageCallback waitIncomingMessage//to deal with incoming messages
        , setScriptRetValCallback setScriptRetVal
        )
    //set isolate
    : m_pIsolate(context->GetIsolate())

    //set platform
    , m_pPlatform(platform)

    //set context in v8::Global
    , m_Context(
//          m_pIsolate,
          context)

    , m_Script(script)

//    , contextScope(m_Context)

    //set context name
    , m_sContextName(contextName)

    //set message callback
    , m_WaitForFrontendMessage(std::move(waitIncomingMessage))

    //
    , m_SetRetVal(std::move(setScriptRetVal))
{
    setUpDebuggingSession(sendDataFunc);
}



void NSJSBase::v8_debug::internal::CInspectorClient::runMessageLoopOnPause(int contextGroupId) {
    if (m_bPause) {
        return;
    }

    TrueSetter f(m_bPause);

    while (m_bPause) {
        if (!m_WaitForFrontendMessage()) {
            break;
        }
        pumpPlatform();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::quitMessageLoopOnPause() {
    m_bPause = false;
}


void NSJSBase::v8_debug::internal::CInspectorClient::startDebugging()
{
    std::cout << "to start dbg" << std::endl;
    v8::Context::Scope contextScope(m_Context);
    m_pSession->schedulePauseOnNextStatement(strToView("debugging"), strToView(""));

    //string with script
    const char rawString[] = "function a()\
                                    {\
                                    return 2 + 3;\
                                    }\
                                    a();";
    v8::Local<v8::String> string =
            v8::String::NewFromUtf8(
                m_Context->GetIsolate()
                , rawString
                , v8::NewStringType::kNormal
                ).ToLocalChecked();

    //compiled script; it can be casted to string - surprising
//    v8::Local<v8::Script> script = v8::Script::Compile(m_Context, string).ToLocalChecked();

    v8::MaybeLocal<v8::Value> result =
//            script//1
//            m_Script//2
                getFileScript(//3
                            m_Context
                            ,"D:/111/work/v8-debug/v8-debug/scripts/sample.js")

                        ->Run(m_Context)
            ;

    m_SetRetVal(result);
}

void NSJSBase::v8_debug::internal::CInspectorClient::processMessageFromFrontend(
        const std::string &message)
{
    m_pSession->dispatchProtocolMessage(strToView(message));
    if (checkForStartDebugging(message)) {
        startDebugging();
    }
}

bool NSJSBase::v8_debug::internal::CInspectorClient::checkForStartDebugging(
        const std::string &json)
{
    return getMethod(m_Context, json) == debugStartMarker;
}

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient()
{
    //
}
