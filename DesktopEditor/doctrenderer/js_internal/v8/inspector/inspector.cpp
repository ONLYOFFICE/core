#include "inspector.h"
#include "client.h"
#include <iostream>
#include "singlethreadutils.h"
#include "singleconnectionserver.h"
#include "../../v8/v8_base.h"

bool NSJSBase::v8_debug::CInspector::initServer()
{
    auto messageCallback = [this](const std::string &message) {
        if (message.empty()) {
            return;
        }
        maybeLog(message, msgType::incoming);
        m_pClient->processMessageFromFrontend(message);
    };

    m_pServer->setOnMessageCallback(messageCallback);

    //callback is set. now listen
    m_pServer->listen();


    if (!m_pServer->listening()) {
        std::cerr << "server can't listen to CDT connection" << std::endl;
        return false;
    }

    return true;
}

void NSJSBase::v8_debug::CInspector::initClient(
                            CJSContextPrivate *pContextPrivate
                            , const std::string &contextName
                            , const std::string &scriptStr
                            , NSCommon::smart_ptr<CJSTryCatch> &pException
                            , const std::wstring &scriptPath)
{
    //send message to frontend (for channel)
    internal::CInspectorChannel::sendDataCallback sendDataCallback = [this](
            const v8_inspector::StringView &message
            ) {
        if (message.length() == 0) {
            return;
        }
        std::string str = internal::viewToStr(m_pIsolate, message);
        maybeLog(str, msgType::outgoing);
        this->m_pServer->sendData(str);
    };

    //wait for message (for client in runMessageLoopOnPause)
    internal::CInspectorClient::waitMessageCallback waitForMessage = [this]() {
        return m_pServer->waitAndProcessMessage();
    };

    //set script result
    internal::CInspectorClient::setScriptRetValCallback setScriptRetVal = [this](
            JSSmart<CJSValue> result
            ) {
        this->m_pScriptResult =
                std::make_unique<JSSmart<CJSValue> >(result);
    };

    //make client
    this->m_pClient = std::make_unique<internal::CInspectorClient>(
                                                //for all v8 stuff
                                                    pContextPrivate
                                                //context name for cdt
                                                    , contextName
                                                //script running trio
                                                    , scriptStr
                                                    , pException
                                                    , scriptPath
                                                //for channel to send data
                                                    , sendDataCallback
                                                //for client to receive data
                                                    , waitForMessage
                                                //
                                                    , setScriptRetVal
                );
}

void NSJSBase::v8_debug::CInspector::maybeLog(const std::string &message, msgType type) const
{
    if (!m_bLog) {
        return;
    }

    switch (type) {
    case msgType::incoming: {
        internal::logCdtMessage(std::clog, message);
        return;
    }
    case msgType::outgoing: {
        internal::logOutgoingMessage(std::clog, message);
        return;
    }
    }
}

void NSJSBase::v8_debug::CInspector::printChromeLaunchHint(std::ostream &out, uint16_t port)
{
    out << "open chrome dev tools with the following command" << std::endl;
    out << "chrome "
         << "--remote-debugging-port=9222 "
         << "http://localhost:9222/devtools/inspector.html?ws=localhost:"
         << port
         << std::endl;
}

NSJSBase::v8_debug::CInspector::CInspector(CJSContextPrivate *pContextPrivate
        , const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        , bool log
        , uint16_t port
        , const std::string &contextName
        )
    : m_pServer(std::make_unique<internal::SingleConnectionServer>())
{
    if (!initServer()) {
        return;
    }
    initClient(pContextPrivate, contextName, scriptStr, pException, scriptPath);
    printChromeLaunchHint(std::cout, port);
}



//NSJSBase::v8_debug::CInspector::CInspector(v8::Local<v8::Context> context
//                       , v8::Platform *platform
//                       , v8::Local<v8::Script> script
//                       , bool log
//                       , uint16_t port
//                       , std::string contextName)
//    //server
//    : m_Server(port)

//    //v8 stuff
//    , m_pIsolate(context->GetIsolate())
////    , m_Context(
//////          m_pIsolate,
////          context)
////    , m_pPlatform(platform)
////    , m_sContextName(std::move(contextName))

////    , m_Script(script)
//    , m_bLog(log)

//{
//    //
//    if (!initServer()) {
//        return;
//    }
//    initClient(context, contextName, platform, script);
//    printChromeLaunchHint(std::cout, port);
//}

JSSmart<NSJSBase::CJSValue> NSJSBase::v8_debug::CInspector::run()
{
    if (!m_pServer->listening()) {
        return JSSmart<CJSValue>();
    }
    m_pServer->waitForConnection();
    m_pServer->run();
    if (m_pScriptResult) {
        return *(m_pScriptResult.get());
    }
    return JSSmart<CJSValue>();
}

NSJSBase::v8_debug::CInspector::~CInspector()
{
    //
}
