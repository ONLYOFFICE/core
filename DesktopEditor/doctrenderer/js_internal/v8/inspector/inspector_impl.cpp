//this file contains boost includes, and is should be included before any others
#include "singleconnectionserver.h"//server implementation
#include "inspector_impl.h"
#include <iostream>//std::cout
#include "singlethreadutils.h"//string conversion
#include "../v8_base.h"//v8 wrappers and other stuff

void NSJSBase::v8_debug::internal::CInspectorImpl::initServer()
{
    //in any case
    m_pServer->pause();

    //set callback
    auto messageCallback = [this](const std::string &message) {
        this->processIncomingMessage(message);
    };
    m_pServer->setOnMessageCallback(messageCallback);
}

void NSJSBase::v8_debug::internal::CInspectorImpl::processIncomingMessage(const std::string &message)
{
    if (message.empty()) {
        return;
    }
    m_Client.processMessageFromFrontend(message);
}

void NSJSBase::v8_debug::internal::CInspectorImpl::maybeLogOutgoing(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }
    logOutgoingMessage(std::clog, message);
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

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(//for client
        v8::Local<v8::Context> context
        //platform to pump
        , v8::Platform *platform
        //other info
        , CInspectorInfo info
        //server and use flag
        , CServerHolder::CUseData useData
        )
    : m_pServer{useData.pServer}
    , m_pIsolate{context->GetIsolate()}
    , m_bLog{info.log}
    , m_Client{context, info.contextName, info.contextGroupId, platform, this, info.log, useData.ready}
    , m_bServerInUse{useData.inUse}
    , m_bServerReady{useData.ready}
{
    m_bServerInUse = true;
    initServer();
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
//    m_Client.pauseOnNextStatement();
//    setRetVal(m_Client.runScript());
    //run server

    if (m_bServerReady) {
        std::cout << "BEFORE PROCESSING FAKE MESSAGE: RS\n";
        m_Client.processMessageFromFrontend("kkk");
    }
    std::cout << "BEFORE RUN SERVER: RS\n";
    m_pServer->run();
    std::cout << "AFTER RUN SERVER: RS\n";
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
//    m_Client.pauseOnNextStatement();
//    setRetVal(m_Client.callFunc());
    if (m_bServerReady) {
        std::cout << "BEFORE PROCESSING FAKE MESSAGE: FC\n";
        m_Client.processMessageFromFrontend("kkk");
    }
    //run server
    std::cout << "BEFORE RUN SERVER: FC\n";
    m_pServer->run();
    std::cout << "AFTER RUN SERVER: FC\n";
    return getReturnValue();
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() {
    m_bServerInUse = false;
}
