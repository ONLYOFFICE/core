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

    //3
    //ждём первого сообщения о постановке/снятии брейкпойнта
    //остальные придут на паузе
    //если Runtime.getProperties пришёл до этого, снимаемся с паузы
    if (m_bBreakpointAdded) {
        if (isBreakpointAlterMethod(method)) {
            m_bBreakpointAlterMethodCame = true;
            if (m_bResumeOnBreakpointCame) {
                return resumeDebuggingSession();
            }
            return;
        }
    }

    //1
    //если пришёл Debugger.setBreakpointsActive
    if (activateBreakpointsMethod == method) {
        //и active == true
        if (isActiveTrue(message)) {
            //ставим флаг, что ждём изменения брейкпойнтов
            m_bBreakpointAdded = true;
        }
        return;
    }

    //функции вызываются чаще, первыми проверяем их
    if (funcResumeMessageLate == method) {
        //если ждём изменения брейкпойнтов, запоминаем, что Runtime.getProperties пришёл
        if (m_bBreakpointAdded) {
            m_bResumeOnBreakpointCame = true;
            //если сообщение об изменении брейкпойнтов пришло до этого, снимаемся с паузы
            if (m_bBreakpointAlterMethodCame) {
                return resumeDebuggingSession();
            }
            return;
        } else {
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

void NSJSBase::v8_debug::internal::CInspectorClient::pauseOnNextStatement()
{
    m_bMySessionPause = true;
    m_pSession->schedulePauseOnNextStatement(strToView("other"), {});
}

void NSJSBase::v8_debug::internal::CInspectorClient::resumeDebuggingSession()
{
    //manual pause setting flag
    m_bMySessionPause = false;

    //breakpoint altering flags
    m_bBreakpointAdded = false;
    m_bBreakpointAlterMethodCame = false;
    m_bResumeOnBreakpointCame = false;

    m_pSession->resume();
}

bool NSJSBase::v8_debug::internal::CInspectorClient::isBreakpointAlterMethod(const std::string &method)
{
    return method == setBreakpointMethod || method == removeBreakpointMethod;
}

bool NSJSBase::v8_debug::internal::CInspectorClient::isActiveTrue(const std::string &message)
{
    v8::Local<v8::Value> params = getJsonProperty(m_Context, message, "params");
    if (params.IsEmpty()) {
        return false;
    }
    if (!params->IsObject()) {
        return false;
    }
    v8::MaybeLocal<v8::Object> maybeObj = params->ToObject(m_Context);
    if (maybeObj.IsEmpty()) {
        return false;
    }
    v8::Local<v8::Object> obj = maybeObj.ToLocalChecked();
    v8::Local<v8::Value> active = getJsonPropertyImpl(m_Context, obj, "active");
    if (active.IsEmpty()) {
        return false;
    }
    if (!active->IsBoolean()){
        return false;
    }
    v8::MaybeLocal<v8::Boolean> maybeBool = active->ToBoolean(m_Context);
    if (maybeBool.IsEmpty()) {
        return false;
    }
    v8::Local<v8::Boolean> result = maybeBool.ToLocalChecked();
    return result->IsTrue();
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
