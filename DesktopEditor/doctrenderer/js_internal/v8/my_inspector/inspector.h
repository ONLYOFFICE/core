#ifndef V8_INSPECTOR_EXAMPLE_INSPECTOR_H
#define V8_INSPECTOR_EXAMPLE_INSPECTOR_H

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include "websocket_server.h"
#include "v8_inspector_client.h"
#include "v8_inspector_listener.h"
#include "utils.h"

class Inspector {
public:
	Inspector(v8::Platform* platform, const v8::Local<v8::Context> &context, int webSocketPort);

	void addListener(V8InspectorListener* listener);
	void startAgent();
private:
	void onMessage(const std::string& message);
	void sendMessage(const std::string& message);
	int waitForFrontendMessage();

	v8::Handle<v8::Context> context_;
	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;
	std::vector<std::string> scripts = {};
	std::list<V8InspectorListener*> listeners_;
};


#endif //V8_INSPECTOR_EXAMPLE_INSPECTOR_H
