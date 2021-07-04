#ifndef CINSPECTORCLIENT_H
#define CINSPECTORCLIENT_H

#include "v8-inspector.h"//base class
#include <atomic>//pause flag must be thread-safe
#include "CAbstractDebugSocketListener.h"//socketListener
#include "../../../../../Common/Network/WebSocket/include/websocket.h"

//wait for message on runMessageLoopOnPause
//stop waiting for quitMessageLoopOnPause on quit_etc
class CInspectorClient : public v8_inspector::V8InspectorClient
{
public:
    //given
    v8::Isolate *m_pIsolate = nullptr;
    v8::Global<v8::Context> mContext{};

    //context name
    std::string m_sContextName{};

    //idk what is it, but it's never used anywhere in v8
    int m_iContextGroupId = 1;

    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector = nullptr;
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel = nullptr;
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession = nullptr;

    // client data
    // CTcpDebugSocket
    std::shared_ptr<CAbstractDebugSocketListener> socketListener;
    std::atomic<bool> pause{false};

public:
    //PORT
    //static uint16_t debuggingPort;

    //ctor
    CInspectorClient(v8::Local<v8::Context> context
                     , const std::string &contextName
                     , std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> socket
                     , std::shared_ptr<CAbstractDebugSocketListener> listener
            );

    //wait for message to dispatch it on session
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    //quit waiting
    virtual void quitMessageLoopOnPause() override;
};

#endif // CINSPECTORCLIENT_H
