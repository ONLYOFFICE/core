#include <functional>
#include "v8_inspector_channel.h"
#include "utils.h"

V8InspectorChannelImpl::V8InspectorChannelImpl(v8::Isolate *isolate, const std::function<void(std::string)> &onResponse)
{
	isolate_ = isolate;
	onResponse_ = onResponse;
}

void V8InspectorChannelImpl::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message)
{
	const std::string response = convertToString(isolate_, message->string());
	onResponse_(response);
}

void V8InspectorChannelImpl::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message)
{
	const std::string notification = convertToString(isolate_, message->string());
	onResponse_(notification);
}

void V8InspectorChannelImpl::flushProtocolNotifications()
{
	// flush protocol notification
}
