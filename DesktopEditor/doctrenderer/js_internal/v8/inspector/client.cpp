#include "client.h"
#include "channel.h"//client holds channel
#include "singlethreadutils.h"//converting strings
#include <libplatform/libplatform.h>//v8::Platform
#include "inspector_impl.h"//to interact with inspector
#include <iostream>

namespace {
    //use them in loop
    class TrueSetter {
        bool &m_bool;
    public:
        TrueSetter(bool &b) : m_bool(b) {m_bool = true;}
        ~TrueSetter() {m_bool = false;}
    };
}

void NSJSBase::v8_debug::internal::CInspectorClient::setUpDebuggingSession(
        const std::string &contextName
        )
{
    //client instance is this

    //channel
    m_pChannel = std::make_unique<CInspectorChannel>(
                [this](const v8_inspector::StringView &message) {
                        m_pInspectingWrapper->sendData(message);
                    }
                );

    //inspector
    m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);

    //state for session
    v8_inspector::StringView state{};

    //session
    m_pSession = m_pInspector->connect(m_iContextGroupId, m_pChannel.get(), state);

    //
    m_pInspector->contextCreated({m_Context, m_iContextGroupId, strToView(contextName)});
}

void NSJSBase::v8_debug::internal::CInspectorClient::pumpPlatform()
{
    while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) {
        //just pump until pumped
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::runMessageLoopOnPause(
        int
        ) {
    //no nested calls
    if (m_bPause) {
        return;
    }

    TrueSetter f(m_bPause);

    while (m_bPause) {
        //while not paused by cdt or cdt disconnected
        if (
                !m_pInspectingWrapper->waitForMessage()
                ) {
            break;
        }
        //
        pumpPlatform();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::quitMessageLoopOnPause() {
    m_bPause = false;
}

void NSJSBase::v8_debug::internal::CInspectorClient::processFrontendMessage(
        const std::string &message
        )
{
    //send message to V8 internals
    dispatchProtocolMessage(message);
    //
    checkFrontendMessage(message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::setAutoResume(bool how)
{
//    autoResume = how;
    int check_here;
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessage(const std::string &message)
{
    //если не нужно запускать сессию автоматически, ничего не смотрим
//    if (!autoResume) {
//        return;
//    }
    int check_here;

    std::string method = getMethod(m_Context, message);

    //если сервер не готов - смотрим сообщение о готовности
    if (!serverReady) {
        if (serverReadyMessage == method) {
            serverReady = true;
            m_pInspectingWrapper->onServerReady();
        }
        return;
    }

    if (!myPause) {
        return;
    }

    //функции вызываются чаще, первыми проверяем их
    if (funcResumeFlag_2 == method) {
        return resumeDebuggingSession();
    }

    //проверяем скрипт
    if (scriptResumeFlag_1 == method) {
        return resumeDebuggingSession();
    }
}

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient() {
    //tmp
    //если не релизить, она дважды удаляется
    m_pSession.release();
}

void NSJSBase::v8_debug::internal::CInspectorClient::maybeLogIncoming(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }
    //log incoming message
    logCdtMessage(std::cout, message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::pauseOnNextStatement()
{
    myPause = true;
    m_pSession->schedulePauseOnNextStatement({}, {});
}

void NSJSBase::v8_debug::internal::CInspectorClient::resumeDebuggingSession()
{
    myPause = false;
    m_pSession->resume();
}

void NSJSBase::v8_debug::internal::CInspectorClient::dispatchProtocolMessage(
        const std::string &message)
{
    maybeLogIncoming(message);
    m_pSession->dispatchProtocolMessage(strToView(message));
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(
        v8::Local<v8::Context> context,
        //for cdt
        const std::string &contextName
        //somewhat
        , int contextGroupId
        //to pump it
        , v8::Platform *platform
        //to interact with inspector, which holds client
        , CInspectorImpl *inspector
        //
        , bool log
        )

    //v8 stuff
    : m_Context{context}
    , m_pIsolate{m_Context->GetIsolate()}
    , m_pPlatform{platform}

    //callbacks
    , m_pInspectingWrapper{inspector}

    //logging
    , m_bLog(log)

    //
    , m_iContextGroupId{contextGroupId}
{
    setUpDebuggingSession(contextName);
}
