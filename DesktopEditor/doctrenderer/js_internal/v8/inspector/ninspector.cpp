#include "ninspector.h"
#include "nclient.h"
#include <iostream>
#include "singlethreadutils.h"
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

    m_Server.setOnMessageCallback(messageCallback);

    //callback is set. now listen
    m_Server.listen();


    if (!m_Server.listening()) {
        std::cerr << "server can't listen to CDT connection" << std::endl;
        return false;
    }

    return true;
}

void NSJSBase::v8_debug::CInspector::initClient(v8::Local<v8::Context> context
                            , const std::string &contextName
                            , v8::Platform *platform
                            , v8::Local<v8::Script> script)
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
        this->m_Server.sendData(str);
    };

    //wait for message (for client in runMessageLoopOnPause)
    internal::CInspectorClient::waitMessageCallback waitForMessage = [this]() {
        return m_Server.waitAndProcessMessage();
    };

    //set script result
    internal::CInspectorClient::setScriptRetValCallback setScriptRetVal = [this](
            v8::MaybeLocal<v8::Value> result
            ) {
        this->m_ScriptReturnValue = result;
    };

    //make client
    this->m_pClient = std::make_unique<internal::CInspectorClient>(
                                                    context
                                                    , contextName
                                                    , platform
                                                    , script
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



NSJSBase::v8_debug::CInspector::CInspector(v8::Local<v8::Context> context
                       , v8::Platform *platform
                       , v8::Local<v8::Script> script
                       , bool log
                       , uint16_t port
                       , std::string contextName)
    //server
    : m_Server(port)

    //v8 stuff
    , m_pIsolate(context->GetIsolate())
//    , m_Context(
////          m_pIsolate,
//          context)
//    , m_pPlatform(platform)
//    , m_sContextName(std::move(contextName))

//    , m_Script(script)
    , m_bLog(log)

{
    //
    if (!initServer()) {
        return;
    }
    initClient(context, contextName, platform, script);
    printChromeLaunchHint(std::cout, port);
}

v8::MaybeLocal<v8::Value> NSJSBase::v8_debug::CInspector::run()
{
    if (!m_Server.listening()) {
        return v8::MaybeLocal<v8::Value>();
    }
    m_Server.waitForConnection();
    m_Server.run();
    return m_ScriptReturnValue;
}

NSJSBase::v8_debug::CInspector::~CInspector()
{
    //
}
