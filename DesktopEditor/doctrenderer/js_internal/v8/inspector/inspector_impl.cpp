#include "inspector_impl.h"
#include <iostream>//std::cout
#include "singlethreadutils.h"//string conversion
#include "singleconnectionserver.h"//server implementation
#include "../v8_base.h"//v8 wrappers and other stuff

bool NSJSBase::v8_debug::internal::CInspectorImpl::initServer()
{
    //
    auto messageCallback = [this](const std::string &message) {
        if (message.empty()) {
            return;
        }
        //logging on client
        m_pClient->processMessageFromFrontend(message);
    };
    m_pServer->setOnMessageCallback(messageCallback);

    //callback is set. now listen
    m_pServer->listen();

    //
    if (!m_pServer->listening()) {
        std::cerr << "server can't listen to CDT connection" << std::endl;
        return false;
    }

    return true;
}

void NSJSBase::v8_debug::internal::CInspectorImpl::initClient(
        v8::Local<v8::Context> context
        , int contextGroupId
        , const std::string &contextName
        , v8::Platform *platform)
{
    //send message to frontend (for channel)
    internal::CInspectorChannel::sendDataCallback sendDataCallback = [this](
            const v8_inspector::StringView &message
            ) {
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
    };

    //wait for message (for client in runMessageLoopOnPause)
    internal::CInspectorClient::waitMessageCallback waitForMessage = [this]() {
        return m_pServer->waitAndProcessMessage();
    };

    //set script result from client
    internal::CInspectorClient::setRetValCallback setScriptRetVal = [this](
            JSSmart<CJSValue> result
            ) {
        this->m_pScriptResult =
                std::make_unique<JSSmart<CJSValue> >(result);
    };

    //make client
    this->m_pClient = std::make_unique<CInspectorClient>(
                //for all v8 stuff
                context
                //context name for cdt
                , contextName
                , contextGroupId
                //
                , platform
                //for channel to send data
                , std::move(sendDataCallback)
                //for client to receive data
                , std::move(waitForMessage)
                //set script result to inspector
                , std::move(setScriptRetVal)
                //logging
                , m_bLog
                );
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
    if (!m_pServer->listening()) {
        std::cerr << "no server for inspector" << std::endl;
        return false;
    }
    return true;
}

void NSJSBase::v8_debug::internal::CInspectorImpl::waitAndRunServer()
{
    //
    printChromeLaunchHint(std::cout, m_pServer->port());
    //blocks until connected
    m_pServer->waitForConnection();
    //blocks until disconnected
    m_pServer->run();
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

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(
        //for client
        v8::Local<v8::Context> context
        //platform to pump
        , v8::Platform *platform
        //other info
        , CInspectorInfo info
        )
    : m_pServer(std::make_unique<internal::SingleConnectionServer>(info.port))
    , m_pIsolate(context->GetIsolate())
    , m_bLog(info.log)
{
    if (!initServer()) {
        std::cerr << "can't initialize server" << std::endl;
        return;
    }
    initClient(context, info.contextGroupId, info.contextName, platform);
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
    m_pClient->setScriptExecData(execData);
    //catch the server up
    waitAndRunServer();
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
    m_pClient->setFuncCallData(callData);
    //catch the server up
    waitAndRunServer();
    return getReturnValue();
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() = default;
