#ifndef NCLIENT_H
#define NCLIENT_H

#include <v8-inspector.h>
#include <functional>
#include <atomic>
#include "nchannel.h"

class NClient : public v8_inspector::V8InspectorClient
{
public:
    using waitMessageCallback = std::function<void(void)>;

private:
    const std::string debugStartMarker = "Runtime.runIfWaitingForDebugger";

    //v8 stuff
    v8::Isolate *m_pIsolate = nullptr;//raw pointer
    v8::Platform *m_pPlatform = nullptr;//raw pointer
    v8::Local<v8::Context> m_Context{};


    //context data
    std::string m_sContextName{};
    const int m_iContextGroupId = 1;



    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector = nullptr;
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel = nullptr;
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession = nullptr;

    //message loop stuff
    std::atomic<bool> m_bPause{false};

    waitMessageCallback m_WaitForFrontendMessage{};




    //sets up a debugging session
    void setUpDebuggingSession(NChannel::sendDataCallback channelCallback);
    //
    void pumpPlatform();

public:
    NClient(
            v8::Local<v8::Context> context//for some stuff
            , const std::string &contextName//why not
            , v8::Platform *platform
            , NChannel::sendDataCallback sendDataFunc//for channel
            , waitMessageCallback waitIncomingMessage//to synchronously consume incoming messages
            );

    //wait for incoming message
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    //quit waiting
    virtual void quitMessageLoopOnPause() override;

    //start debugging
    void startDebugging(v8::Local<v8::Script> script);
    //
    void processMessageFromFrontend(const std::string &message);
    bool checkForStartDebugging(const std::string &json);

    ~NClient();
};

#endif // NCLIENT_H
