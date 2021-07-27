#include "inspector_impl.h"
#include <iostream>//std::cout
#include "singlethreadutils.h"//string conversion

bool NSJSBase::v8_debug::internal::CInspectorImpl::initServer()
{
    //set on message callback
    auto messageCallback = [this](const std::string &message) {
        if (!message.empty()) {
            m_Client.processFrontendMessage(message);
        }
    };
    m_Server.setOnMessageCallback(messageCallback);

    //listen
    return m_Server.listen();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::connectServer()
{
    printChromeLaunchHint(std::cout, m_Server.port());
    return m_Server.waitForConnection();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::waitWhileServerReady()
{
    m_Server.run();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::maybeLogOutgoing(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }
    logOutgoingMessage(std::cout, message);
}

void NSJSBase::v8_debug::internal::CInspectorImpl::printChromeLaunchHint(
        std::ostream &out
        , uint16_t port)
{
    out << "chrome "
         << "--remote-debugging-port=9222 "
         << "http://localhost:9222/devtools/inspector.html?ws=localhost:"
         << port
         << std::endl;
}

v8::Local<v8::Object> NSJSBase::v8_debug::internal::CInspectorImpl::getParams(
        const std::string &debuggerPausedMessage
        ) {
    v8::Local<v8::Value> params = getJsonProperty(m_Context, debuggerPausedMessage, "params");
    if (!params->IsObject()) {
        return v8::Local<v8::Object>();
    }
    v8::MaybeLocal<v8::Object> maybeParams = params->ToObject(m_Context);
    if (maybeParams.IsEmpty()) {
        return v8::Local<v8::Object>();
    }
    return maybeParams.ToLocalChecked();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::hasFunction(v8::Local<v8::Object> params)
{
    if (params.IsEmpty()) {
        return false;
    }
    v8::Local<v8::Value> callFrames = getJsonPropertyImpl(m_Context, params, "callFrames");
    if (!callFrames->IsArray()) {
        return false;
    }
    v8::Local<v8::Array> callFramesArr = v8::Local<v8::Array>::Cast(callFrames);
    v8::Local<v8::Value> jsonZero = callFramesArr->Get(0);
    if (!jsonZero->IsObject()) {
        return false;
    }
    v8::MaybeLocal<v8::Object> maybeJson = jsonZero->ToObject(m_Context);
    if (maybeJson.IsEmpty()) {
        return false;
    }
    v8::Local<v8::Object> json = maybeJson.ToLocalChecked();
    v8::Local<v8::Value> functionName = getJsonPropertyImpl(m_Context, json, "functionName");
    return !asString(functionName).empty();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::hasBreakpoint(v8::Local<v8::Object> params)
{
    if (params.IsEmpty()) {
        return false;
    }
    v8::Local<v8::Value> hitBreakpoints = getJsonPropertyImpl(m_Context
                                                              , params, "hitBreakpoints");
    if (!hitBreakpoints->IsArray()) {
        return false;
    }
    v8::Local<v8::Array> hbArr = v8::Local<v8::Array>::Cast(hitBreakpoints);
    return hbArr->Length();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::hasFunction(const std::string &debuggerPausedMessage)
{
    return hasFunction(getParams(debuggerPausedMessage));
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::hasBreakpoint(const std::string &debuggerPausedMessage)
{
    return hasBreakpoint(getParams(debuggerPausedMessage));
}

void NSJSBase::v8_debug::internal::CInspectorImpl::checkOutgoingMessage(const std::string &message)
{
    if (CInspectorClient::debuggerPausedFlag == getMethod(m_Context, message)) {
        m_Client.setAutoResume(!hasBreakpoint(message));
        bool isFunc = hasFunction(message);
    }
}

void NSJSBase::v8_debug::internal::CInspectorImpl::sendData(const v8_inspector::StringView &message)
{
    if (message.length() == 0) {
        return;
    }
    std::string str = internal::viewToStr(
                m_pIsolate
                , message);
    //
    maybeLogOutgoing(str);
    //чекаем прежде, чем отправить, чтобы заранее поставить флаги на клиент
    checkOutgoingMessage(str);
    //
    this->m_Server.sendData(str);
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::waitForMessage()
{
    return m_Server.waitAndProcessMessage();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::onServerReady()
{
    m_Server.pause();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::prepareServer()
{
    if (!connectServer()) {
        return;
    }
    waitWhileServerReady();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::beforeLaunch()
{
    m_Client.pauseOnNextStatement();
}

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(
        //
        v8::Local<v8::Context> context
        //platform to pump
        , v8::Platform *platform
        //
        , uint16_t port
        , int contextGroupId
        , const std::string &contextName
        , bool log
        )
    : m_Server{port}
    , m_pIsolate{context->GetIsolate()}
    , m_Context{context}
    , m_bLog{log}
    , m_Client{context, contextName, contextGroupId, platform, this, log}
{
    if (!initServer()) {
        std::cerr << "server can't listen to incoming connections" << std::endl;
        return;
    }
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() {
    if (m_Server.listening() && m_Server.connected()) {
        m_Server.shutdown();
    }
}
