#include "nclient.h"
#include "nchannel.h"
#include "utils.h"
#include <libplatform/libplatform.h>
#include <iostream>
#include <thread>

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

void NClient::setUpDebuggingSession(NChannel::sendDataCallback channelCallback)
{
    //client instance is this

    //channel
    m_pChannel = std::make_unique<NChannel>(
                std::move(channelCallback)
                );

    //inspector
    m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);



    //state for session
    v8_inspector::StringView state{};

    //session
    m_pSession = m_pInspector->connect(this->m_iContextGroupId, m_pChannel.get(), state);

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

void NClient::pumpPlatform()
{
    while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) {
        //just pump until pumped
    }
}

NClient::NClient(v8::Local<v8::Context> context//for some stuff
        , const std::string &contextName//why not
        , v8::Platform *platform
        , NChannel::sendDataCallback sendDataFunc//for channel
        , waitMessageCallback waitIncomingMessage//to deal with incoming messages
        )
    //set isolate
    : m_pIsolate(context->GetIsolate())

    //set platform
    , m_pPlatform(platform)

    //set context in v8::Global
    , m_Context(
//          m_pIsolate,
          context)

//    , contextScope(m_Context)

    //set context name
    , m_sContextName(contextName)

    //set message callback
    , m_WaitForFrontendMessage(std::move(waitIncomingMessage))
{
    setUpDebuggingSession(sendDataFunc);
}



void NClient::runMessageLoopOnPause(int contextGroupId) {
    if (m_bPause) {
        return;
    }

    TrueSetter f(m_bPause);

    while (m_bPause ) {
        m_WaitForFrontendMessage();
        pumpPlatform();
    }
}

void NClient::quitMessageLoopOnPause() {
    m_bPause = false;
}


void NClient::startDebugging(v8::Local<v8::Script> script)
{
    m_pSession->schedulePauseOnNextStatement(strToView(""), strToView(""));
    getFileScript(
                m_Context
                ,"D:/111/work/v8-debug/v8-debug/scripts/sample.js")
            ->Run(m_Context).ToLocalChecked();
}

void NClient::processMessageFromFrontend(const std::string &message)
{
    m_pSession->dispatchProtocolMessage(strToView(message));
    if (checkForStartDebugging(message)) {
        std::cout << "TO START DEBUGGING" << std::endl;
        startDebugging(makeTrialScript(m_Context
                                                  ));
    }
}

bool NClient::checkForStartDebugging(const std::string &json)
{
    return getMethod(m_Context, json) == debugStartMarker;
}

NClient::~NClient()
{
    //
}
