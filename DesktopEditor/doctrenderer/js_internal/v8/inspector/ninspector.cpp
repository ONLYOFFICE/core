#include "ninspector.h"
#include "nclient.h"
#include <iostream>
#include "utils.h"

void NInspector::initServer()
{
    auto messageCallback = [this](const std::string &message) {
        std::cout << "cdt: " << message << std::endl;
        m_pClient->processMessageFromFrontend(message);
    };

    m_Server.setOnMessageCallback(messageCallback);

    //callbacks are set. now listen
    if (!m_Server.listen()) {
        std::clog << "can't listen" << std::endl;
        return;
    }

    //
}

void NInspector::initClient()
{
    //send message to frontend (for channel)
    NChannel::sendDataCallback sendDataCallback = [this](
            const v8_inspector::StringView &message
            ) {
        std::string str = viewToStr(m_pIsolate, message);
        std::cout << "responce: " << str << std::endl;
        this->m_Server.sendData(str);
    };
    NClient::waitMessageCallback waitForMessage = [this]() {
        m_Server.waitAndProcessMessage();
    };

    //make client
    this->m_pClient = std::make_unique<NClient>(
                                                    this->m_Context
                                                    , m_sContextName
                                                    , this->m_pPlatform
                                                //for channel to send data
                                                    , sendDataCallback
                                                //for client to receive data
                                                    , waitForMessage
                );
}



NInspector::NInspector(v8::Local<v8::Context> context
                       , v8::Platform *platform
                       , uint16_t port
                       , std::string contextName)
    //server
    : m_Server(port)

    //v8 stuff
    , m_pIsolate(context->GetIsolate())
    , m_Context(
//          m_pIsolate,
          context)
    , m_pPlatform(platform)
    , m_sContextName(contextName)

{
    //
    initServer();
    initClient();
}

void NInspector::run()
{
    m_Server.waitForConnection();
    m_Server.run();
}

NInspector::~NInspector()
{
//    m_Context.Reset();
}
