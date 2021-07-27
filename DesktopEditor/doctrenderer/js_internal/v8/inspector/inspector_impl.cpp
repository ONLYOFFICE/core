//this file contains boost includes, and is should be included before any others
//#include "singleconnectionserver.h"//server implementation
#include "inspector_impl.h"
#include <iostream>//std::cout
#include "singlethreadutils.h"//string conversion
#include "../v8_base.h"//v8 wrappers and other stuff

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

std::string NSJSBase::v8_debug::internal::CInspectorImpl::getFunctionName(const std::string &json) {
    std::cout << "CALL FRAMES: ";
    v8::Local<v8::Value> params = getJsonProperty(m_Context, json, "params");
//    std::cout << "!!! params: " << params << std::endl;
    if (!params->IsObject()) {
        std::cout << "params not object\n";
        return std::string();
    }
    int check_before_cast_all;
    v8::Local<v8::Value> hitBreakpoints = getJsonPropertyImpl(m_Context
                                                              , params->ToObject(m_Context).ToLocalChecked(), "hitBreakpoints");
    if (!hitBreakpoints->IsArray()) {
        std::cout << "not array\n";
        return std::string();
    }
    v8::Local<v8::Array> arrr = v8::Local<v8::Array>::Cast(hitBreakpoints);
    uint32_t len = arrr->Length();
    if (len) {
        m_Client.setAutoResume(false);
    } else {
        m_Client.setAutoResume(true);
    }
    std::cout << "LOOK LEN = " << len << std::endl;
    return std::string();
    v8::Local<v8::Value> callFrames = getJsonPropertyImpl(m_Context, params->ToObject(m_Context).ToLocalChecked(), "callFrames");
    if (!callFrames->IsArray()) {
        std::cout << "frames not array\n";
        return std::string();
    }
    v8::Local<v8::Array> a = v8::Local<v8::Array>::Cast(callFrames);
    v8::Local<v8::Value> json0 = a->Get(0);
    v8::Local<v8::Value> fname = getJsonPropertyImpl(m_Context, json0->ToObject(m_Context).ToLocalChecked(), "functionName");
//    v8::MaybeLocal<v8::String> s = v8::JSON::Stringify(m_Context, callFrames->ToObject(m_Context).ToLocalChecked());
    std::string result = asString(fname);
    std::cout << asString(fname) << std::endl << "LOOK" << std::endl;
    return result;

//    if (s.IsEmpty()) {
//        std::cout << "empty\n";
//        return std::string();
//    } else {
//        std::cout << asString(s.ToLocalChecked());
//    }
//    v8::Array::n
    std::cout << std::endl << "END FRAMES" << std::endl;
//    std::string object = getJsonProperty(m_Context, params, "object");
//    std::cout << "!!! object: " << object << std::endl;
    return std::string();
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
    //
    if (CInspectorClient::funcResumeFlag_1 == getMethod(m_Context, str)) {
        if (!getFunctionName(str).empty()) {
            int todo;
        }
    }
    this->m_Server.sendData(str);
    //

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

void NSJSBase::v8_debug::internal::CInspectorImpl::beforeScript()
{
    m_Client.setScript();
    beforeLaunch();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::beforeFunc()
{
    m_Client.setFunc();
    beforeLaunch();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::beforeLaunch()
{
    m_Client.pauseOnNextStatement();
}

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(//for client
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
