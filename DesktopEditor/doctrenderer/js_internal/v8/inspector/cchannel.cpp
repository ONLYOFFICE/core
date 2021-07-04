#include "cchannel.h"
#include "../../../../../DesktopEditor/common/File.h"

namespace
{
    std::string StringViewToUtf8(const v8_inspector::StringView &view)
    {
        if (view.is8Bit())
        {
            return std::string((char*)view.characters8(), view.length());
        }

        NSFile::CStringUtf16 oUtf16;
        oUtf16.Data = (BYTE*)view.characters16();
        oUtf16.Length = (int)(2 * view.length());
        std::wstring sWcharStr = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)view.characters16(), (LONG)view.length());
        return U_TO_UTF8(sWcharStr);
    }
    std::string StringBufferToUtf8(v8_inspector::StringBuffer* buffer)
    {
        return StringViewToUtf8(buffer->string());
    }
}

void CChannel::sendAnything(std::unique_ptr<v8_inspector::StringBuffer> message)
{
    socket->send(StringBufferToUtf8(message.get()));
}

CChannel::CChannel(std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> openedSocket)
    : v8_inspector::V8Inspector::Channel()
    , socket(openedSocket)
{
    //
}

void CChannel::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message)
{
    sendAnything(std::move(message));
}

void CChannel::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message)
{
    sendAnything(std::move(message));
}

void CChannel::flushProtocolNotifications()
{
    //idk, just do nothing here
}
