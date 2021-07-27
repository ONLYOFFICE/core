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
        this->processIncomingMessage(message);
    };
    m_Server.setOnMessageCallback(messageCallback);

    //set on resume callback
    m_fOnServerResume = [this]() {
        this->m_Client.startDebugging();
    };

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

void NSJSBase::v8_debug::internal::CInspectorImpl::processIncomingMessage(const std::string &message)
{
    if (message.empty()) {
        return;
    }
    m_Client.processFrontendMessage(message);
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

bool NSJSBase::v8_debug::internal::CInspectorImpl::checkServer() const
{
    if (!m_Server.listening()) {
        std::cerr << "server is not listening" << std::endl;
        return false;
    }
    if (!m_Server.connected()) {
        std::cerr << "server is not connected" << std::endl;
        return false;
    }
    return true;
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::internal::CInspectorImpl::getReturnValue()
{
    //get result, if any
    NSCommon::smart_ptr<CJSValue> result;
    if (m_pScriptResult) {
        result = *m_pScriptResult;
    }

    //reset pointer
    m_pScriptResult.reset(nullptr);

    //return result
    return result;
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
    this->m_Server.sendData(str);
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::waitForMessage()
{
    return m_Server.waitAndProcessMessage();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::setRetVal(const NSCommon::smart_ptr<CJSValue> &val)
{
    m_pScriptResult.reset(new JSSmart<CJSValue>(val));
}

void NSJSBase::v8_debug::internal::CInspectorImpl::pauseServer()
{
    m_Server.pause();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::onServerReady()
{
    pauseServer();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::prepareServer()
{
    if (!connectServer()) {
        return;
    }
    waitWhileServerReady();
}

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(//for client
        v8::Local<v8::Context> context
        //platform to pump
        , v8::Platform *platform
        //other info
        , CInspectorInfo info
        //
        , uint16_t port
        )
    : m_Server{port}
    , m_pIsolate{context->GetIsolate()}
    , m_bLog{info.log}
    , m_Client{context, info.contextName, info.contextGroupId, platform, this, info.log}
{
    if (!initServer()) {
        std::cerr << "server can't listen to incoming connections" << std::endl;
        return;
    }
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::internal::CInspectorImpl::runScript(
        const CScriptExecData &execData
        )
{
    //check for listening
    if (!checkServer()) {
        return JSSmart<CJSValue>();
    }
    //set data to client
    m_Client.setScriptExecData(execData);
    m_Server.run(m_fOnServerResume);
    return getReturnValue();
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::internal::CInspectorImpl::callFunc(
        const CFCallData &callData
        )
{
    //check for listening
    if (!checkServer()) {
        return JSSmart<CJSValue>();
    }
    //set data to client
    m_Client.setFuncCallData(callData);
    //run server
    m_Server.run(m_fOnServerResume);
    return getReturnValue();
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() {
    if (m_Server.listening() && m_Server.connected()) {
        m_Server.shutdown();
    }
}
