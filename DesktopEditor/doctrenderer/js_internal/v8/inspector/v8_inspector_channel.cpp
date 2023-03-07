#include <functional>
#include "v8_inspector_channel.h"
#include "utils.h"

namespace NSJSBase
{

	CV8InspectorChannelImpl::CV8InspectorChannelImpl(v8::Isolate* pIsolate, const std::function<void(std::string)>& fOnResponse)
		: m_pIsolate(pIsolate)
		, m_fOnResponce(fOnResponse)
	{
	}

	void CV8InspectorChannelImpl::sendResponse(int nCallId, std::unique_ptr<v8_inspector::StringBuffer> pMessage)
	{
		const std::string response = convertToString(m_pIsolate, pMessage->string());
		m_fOnResponce(response);
	}

	void CV8InspectorChannelImpl::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> pMessage)
	{
		const std::string notification = convertToString(m_pIsolate, pMessage->string());
		m_fOnResponce(notification);
	}

	void CV8InspectorChannelImpl::flushProtocolNotifications()
	{
		// does nothing
	}

}
