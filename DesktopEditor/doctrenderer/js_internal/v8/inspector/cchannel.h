#ifndef CCHANNEL_H
#define CCHANNEL_H

#include "v8-inspector.h"//base class
#include "../../../../../Common/Network/WebSocket/include/websocket.h"


//send message to frontend (possibly using socket)
class CChannel : public v8_inspector::V8Inspector::Channel
{
    // DebugSocket
    std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> socket;

    void sendAnything(std::unique_ptr<v8_inspector::StringBuffer> message);

public:
    CChannel(std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> openedSocket);

    // send message to frontend through socket
    virtual void sendResponse(int callId,
                              std::unique_ptr<v8_inspector::StringBuffer> message) override;
    virtual void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;
    // may do nothing
    virtual void flushProtocolNotifications() override;
};



#endif // CCHANNEL_H
