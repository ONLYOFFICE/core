#include "cwebsocketlistener.h"
#include <iostream>//wclog

namespace {
    //
    v8_inspector::StringView sviewFromStr(const std::string &str) {
        return v8_inspector::StringView(
                    reinterpret_cast<const uint8_t*>(str.data())
                    , str.size()
                    )
                ;
    }
}

void CWebSocketListener::onMessage(const std::string &message)
{
    acceptMessage(message);
    std::clog << "message " << message << std::endl;
}

void CWebSocketListener::onOpen()
{
    clearInternalData();
    std::clog << "websocket opened" << std::endl;
}

void CWebSocketListener::onError(const std::string &error)
{
    std::clog << "error: " << error.c_str() << std::endl;
}

void CWebSocketListener::onClose(int code, const std::string &reason)
{
    clearInternalData();
    std::clog << "websocket closed" << std::endl;
}

bool CWebSocketListener::readSingleMessage()
{
    //
    if (unreadMessages.empty()) {
        return false;
    }

    //
    std::string pendingMessage = unreadMessages.front();
    unreadMessages.pop_front();

    //
    readMessages.emplace_back(std::move(pendingMessage));

    //
    return true;
}

void CWebSocketListener::clearInternalData()
{
    unreadMessages.clear();
    readMessages.clear();
}

void CWebSocketListener::acceptMessage(const std::string &message)
{
    unreadMessages.push_back(message);
}

CWebSocketListener::CWebSocketListener()
{
    //
}

bool CWebSocketListener::hasMessage() const
{
    return !unreadMessages.empty();
}

v8_inspector::StringView CWebSocketListener::getMessage()
{
    //if no message was read, return empty string view
    if (!readSingleMessage()) {
        return v8_inspector::StringView();
    }

    //read message is back one of readMessages
    return sviewFromStr(readMessages.back());
}
