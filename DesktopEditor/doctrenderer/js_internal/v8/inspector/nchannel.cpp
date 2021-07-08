#include "nchannel.h"

NChannel::NChannel(sendDataCallback sendData)
    : m_SendData(std::move(sendData))
{
    //
}

void NChannel::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message)
{
    if (message) {
        m_SendData(message->string());
    }
}

void NChannel::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message)
{
    if (message) {
        m_SendData(message->string());
    }
}

void NChannel::flushProtocolNotifications()
{
    //do nothing
}
