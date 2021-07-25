//this file contains boost includes, and is should be included before any others
#include "singleconnectionserver.h"//server implementation
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
    m_pServer->setOnMessageCallback(messageCallback);

    //set on resume callback
    auto resumeCallback = [this]() {
        this->m_Client.startDebugging();
    };
    m_pServer->setOnResumeCallback(resumeCallback);

    //listen
    return m_pServer->listen();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::connectServer()
{
    printChromeLaunchHint(std::cout, m_pServer->port());
    return m_pServer->waitForConnection();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::waitWhileServerReady()
{
    m_pServer->run();
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
    if (!m_pServer) {
        std::cerr << "no server for inspector" << std::endl;
        return false;
    }
    if (!m_pServer->listening()) {
        std::cerr << "server is not listening" << std::endl;
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
    if (!m_pServer) {
        return;
    }
    if (message.length() == 0) {
        return;
    }
    std::string str = internal::viewToStr(
                m_pIsolate
                , message);
    //
    maybeLogOutgoing(str);
    //
    this->m_pServer->sendData(str);
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::waitForMessage()
{
    if (!m_pServer) {
        return false;
    }
    return m_pServer->waitAndProcessMessage();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::setRetVal(const NSCommon::smart_ptr<CJSValue> &val)
{
    m_pScriptResult = std::make_unique<JSSmart<CJSValue> >(val);
}

void NSJSBase::v8_debug::internal::CInspectorImpl::shutServerDown()
{
    m_pServer->shutdown();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::pauseServer()
{
    m_pServer->pause();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::free() const
{
    return !m_pServer->busy();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::onServerReady()
{
    std::cout << "THAT START DEBUGGING MESSAGE HAPPENED\n";
    pauseServer();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::prepareServer()
{
    if (!connectServer()) {
        std::cerr << "server can't connect" << std::endl;
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
    : m_pServer{std::make_unique<CSingleConnectionServer>(port)}
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
    std::cout << "BEFORE RESUME SERVER: RS\n";
    m_pServer->resume();
    std::cout << "AFTER RESUME SERVER: RS\n";
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
    std::cout << "BEFORE RESUME SERVER: FC\n";
    m_pServer->resume();
    std::cout << "AFTER RESUME SERVER: FC\n";
    return getReturnValue();
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() {
    std::cout << "INSPECTOR DTOR\n";
    if (m_pServer->listening()) {
        m_pServer->shutdown();
    }
}
