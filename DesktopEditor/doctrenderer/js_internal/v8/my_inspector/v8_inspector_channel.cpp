#include <functional>
#include "v8_inspector_channel.h"
#include "utils.h"

V8InspectorChannelImpl::V8InspectorChannelImpl(v8::Isolate* pIsolate, const std::function<void(std::string)>& fOnResponse)
	: m_pIsolate(pIsolate)
	, m_fOnResponce(fOnResponse)
{
}

void V8InspectorChannelImpl::sendResponse(int nCallId, std::unique_ptr<v8_inspector::StringBuffer> pMessage)
{
	const std::string response = convertToString(m_pIsolate, pMessage->string());
	m_fOnResponce(response);
}

void V8InspectorChannelImpl::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> pMessage)
{
	const std::string notification = convertToString(m_pIsolate, pMessage->string());
	m_fOnResponce(notification);
}

void V8InspectorChannelImpl::flushProtocolNotifications()
{
	// does nothing
}
