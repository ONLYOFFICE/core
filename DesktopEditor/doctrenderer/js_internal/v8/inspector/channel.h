#ifndef CHANNEL_H
#define CHANNEL_H

#include <v8-inspector.h>//v8_inspector stuff
#include <functional>//std::function

namespace NSJSBase {
namespace v8_debug {
namespace internal {
//class that sends messages to frontend
class CInspectorChannel : public v8_inspector::V8Inspector::Channel
{
public:
    using sendDataCallback = std::function<void(const v8_inspector::StringView&)>;

private:
    sendDataCallback m_SendData{};

public:
    CInspectorChannel(sendDataCallback sendData);

    //send message to frontend through socket
    virtual void sendResponse(int callId,
                              std::unique_ptr<v8_inspector::StringBuffer> message) override;
    virtual void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

    //may do nothing
    virtual void flushProtocolNotifications() override;
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CHANNEL_H
