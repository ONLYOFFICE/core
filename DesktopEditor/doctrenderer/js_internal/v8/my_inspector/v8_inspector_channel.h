#ifndef V8INSPECTORCHANNELIMPL_H
#define V8INSPECTORCHANNELIMPL_H

#include <functional>
#include <v8.h>
#include <v8-inspector.h>

class V8InspectorChannelImpl final: public v8_inspector::V8Inspector::Channel
{
private:
	v8::Isolate* m_pIsolate;
	std::function<void(std::string)> m_fOnResponce;

public:
	V8InspectorChannelImpl(v8::Isolate* pIsolate, const std::function<void(std::string)>& fOnResponse);

	// overriden interface methods
	void sendResponse(int nCallId, std::unique_ptr<v8_inspector::StringBuffer> pMessage) override;
	void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> pMessage) override;
	void flushProtocolNotifications() override;
};

#endif // V8INSPECTORCHANNELIMPL_H
