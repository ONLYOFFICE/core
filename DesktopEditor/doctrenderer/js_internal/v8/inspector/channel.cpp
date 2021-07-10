#include "channel.h"

NSJSBase::v8_debug::internal::CInspectorChannel::CInspectorChannel(sendDataCallback sendData)
    : m_SendData(std::move(sendData))
{
    //
}

void NSJSBase::v8_debug::internal::CInspectorChannel::sendResponse(int callId
                 , std::unique_ptr<v8_inspector::StringBuffer> message)
{
    if (message) {
        m_SendData(message->string());
    }
}

void NSJSBase::v8_debug::internal::CInspectorChannel::sendNotification(
        std::unique_ptr<v8_inspector::StringBuffer> message)
{
    if (message) {
        m_SendData(message->string());
    }
}

void NSJSBase::v8_debug::internal::CInspectorChannel::flushProtocolNotifications()
{
    //do nothing
}
