#include "client.h"
#include "channel.h"//client holds channel
#include "singlethreadutils.h"//converting strings
#include <libplatform/libplatform.h>//v8::Platform
#include "inspector_impl.h"//to interact with inspector
#include <iostream>//std::cout

namespace {
    //use it in loop
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

    //register context in v8 inspector
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
    //check it for notable stuff
    checkFrontendMessage(message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessage(const std::string &message)
{
    std::string method = getMethod(m_Context, message);

    //если сервер не готов - смотрим сообщение о готовности
    if (!m_bServerReady) {
        if (serverReadyMessage == method) {
            m_bServerReady = true;
            m_pInspectingWrapper->onServerReady();
        }
        return;
    }

    //если пауза поставлена из v8, то ничего не смотрим
    if (!m_bMySessionPause) {
        return;
    }

    if (m_bp == bp::afterRuntime) {//3
        if (message.find(R"("method":"Debugger.setBreakpoint")") != std::string::npos
                || message.find(R"("method":"Debugger.removeBreakpoint")") != std::string::npos) {
            std::cout << "after runtime bp flag check, to resume\n";
            m_bp = bp::no;
            return resumeDebuggingSession();
        }
    }

    //1
    if (message.find(R"("method":"Debugger.setBreakpointsActive","params":{"active":true})") != std::string::npos) {
        std::cout << "bp check\n";
        m_bp = bp::beforeRuntime;//1
        return;
    }
    //функции вызываются чаще, первыми проверяем их
    if (funcResumeMessageLate == method) {
        //2
        if (m_bp == bp::beforeRuntime) {
            std::cout << "runtime with bp check\n";
            m_bp = bp::afterRuntime;//2
            return;
        }

        if (m_bp == bp::no){
            std::cout << "to plain resume after runtime\n";
            return resumeDebuggingSession();
        }
    }

    //проверяем скрипт
    if (scriptResumeMessage == method) {
        return resumeDebuggingSession();
    }
}

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient() {
    //если не релизить, она дважды удаляется, и программа падает на выходе
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

void NSJSBase::v8_debug::internal::CInspectorClient::pauseOnNextStatement(const char *fname)
{
//    if (std::string(fname) != "offline_mouse_move")
    m_bMySessionPause = true;
    m_pSession->schedulePauseOnNextStatement(strToView("other"), {});
}

void NSJSBase::v8_debug::internal::CInspectorClient::resumeDebuggingSession()
{
    m_bMySessionPause = false;
    m_pSession->resume();
}

void NSJSBase::v8_debug::internal::CInspectorClient::dispatchProtocolMessage(
        const std::string &message)
{
    maybeLogIncoming(message);
    m_pSession->dispatchProtocolMessage(strToView(message));
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(
        //for v8 inspector
        v8::Local<v8::Context> context,
        //for cdt
        const std::string &contextName
        //somewhat
        , int contextGroupId
        //to pump platform on pause
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

    //somewhat
    , m_iContextGroupId{contextGroupId}
{
    setUpDebuggingSession(contextName);
}
