#ifndef CWEBSOCKETLISTENER_H
#define CWEBSOCKETLISTENER_H

//
#include "../../../../../Common/Network/WebSocket/include/websocket.h"
#include "CAbstractDebugSocketListener.h"//CAbstractDebugSocketListener
#include <deque>//unread messages
#include <vector>//read messages

class CWebSocketListener
        : public NSNetwork::NSWebSocket::IListener
        , public CAbstractDebugSocketListener
{
    //
    std::deque<std::string> unreadMessages{};
    std::vector<std::string> readMessages{};

    //
    virtual void onMessage(const std::string& message) override;
    virtual void onOpen() override;
    virtual void onError(const std::string& error) override;
    virtual void onClose(int code, const std::string& reason) override;

    //returns true if the message was read
    bool readSingleMessage();
    //
    void clearInternalData();
    //
    void acceptMessage(const std::string &message);

public:
    CWebSocketListener();
    virtual bool hasMessage() const override;
    virtual v8_inspector::StringView getMessage() override;
};

#endif // CWEBSOCKETLISTENER_H
