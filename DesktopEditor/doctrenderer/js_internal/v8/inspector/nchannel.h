#ifndef NCHANNEL_H
#define NCHANNEL_H

#include <v8-inspector.h>//v8_inspector namespace
#include <functional>//std::function

class NChannel : public v8_inspector::V8Inspector::Channel
{
public:
    using sendDataCallback = std::function<void(const v8_inspector::StringView&)>;

private:
    sendDataCallback m_SendData{};

public:
    NChannel(sendDataCallback sendData);

    //send message to frontend through socket
    virtual void sendResponse(int callId,
                              std::unique_ptr<v8_inspector::StringBuffer> message) override;
    virtual void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;

    //may do nothing
    virtual void flushProtocolNotifications() override;
};

#endif // NCHANNEL_H
