#ifndef V8INSPECTORCHANNELIMPL_H
#define V8INSPECTORCHANNELIMPL_H

#include <functional>
#include <v8.h>
#include <v8-inspector.h>

class V8InspectorChannelImpl final: public v8_inspector::V8Inspector::Channel
{
private:
	v8::Isolate* isolate_;
	std::function<void(std::string)> onResponse_;

public:
	V8InspectorChannelImpl(v8::Isolate* isolate, const std::function<void(std::string)> &onResponse);

	// overriden interface methods
	void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) override;
	void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) override;
	void flushProtocolNotifications() override;
};

#endif // V8INSPECTORCHANNELIMPL_H
