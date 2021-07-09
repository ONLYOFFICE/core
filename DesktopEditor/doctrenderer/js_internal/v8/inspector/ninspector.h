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
    bool m_bListening{false};

    //v8 stuff
    v8::Isolate *m_pIsolate{nullptr};
    //stored on client
//    v8::Local<v8::Context> m_Context{};
//    v8::Platform *m_pPlatform = nullptr;
//    std::string m_sContextName{};

    //stored on client
//    v8::Local<v8::Script> m_Script;

    //logging protocol messages
    bool m_bLog{false};

    //
    v8::MaybeLocal<v8::Value> m_ScriptReturnValue{};


    //inspector client
    std::unique_ptr<NClient> m_pClient{nullptr};

    //
    bool initServer();
    void initClient(v8::Local<v8::Context> context
                    , const std::string &contextName
                    , v8::Platform *platform
                    , v8::Local<v8::Script> script);

    //
    void processIncomingMessage(const std::string &message);

    //
    enum class msgType : bool {
        incoming
        , outgoing
    };
    void maybeLog(const std::string &message, msgType type) const;

    //
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

public:
    NInspector(v8::Local<v8::Context> context
               , v8::Platform *platform
               , v8::Local<v8::Script> script
               , bool log = false
               , uint16_t port = 8080
            , std::string contextName = "");

    v8::MaybeLocal<v8::Value> run();

    ~NInspector();
};

#endif // NINSPECTOR_H
