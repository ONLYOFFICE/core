#ifndef NINSPECTOR_H
#define NINSPECTOR_H

#include <stdint.h>
#include <v8.h>
#include "nclient.h"
#include "singleconnectionserver.h"

class NInspector
{

    //server
    SingleConnectionServer m_Server{};

    //is server listening
    bool m_bListening = false;

    //v8 stuff
    v8::Isolate *m_pIsolate = nullptr;
    v8::Local<v8::Context> m_Context{};
    v8::Platform *m_pPlatform = nullptr;
    std::string m_sContextName{};

    //
//    v8::Local<v8::Script> m_Script;


    //inspector client
    std::unique_ptr<NClient> m_pClient{nullptr};

    void initServer();
    void initClient();

    void processIncomingMessage(const std::string &message);

public:
    NInspector(v8::Local<v8::Context> context
               , v8::Platform *platform
               , uint16_t port = 8080
            , std::string contextName = "");

    void run();

    ~NInspector();
};

#endif // NINSPECTOR_H
