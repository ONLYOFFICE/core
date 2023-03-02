#ifndef V8_INSPECTOR_H
#define V8_INSPECTOR_H

#include <iostream>
#include <functional>

#include "websocket_server.h"
#include "v8_inspector_client.h"
#include "utils.h"

class Inspector
{
private:
	// pointer to CJSContext instance in which scripts/functions are executed
	v8::Isolate* isolate_;
	// server port
	int port_;
	// flag that is `true` when script is ready to be executed in V8 and `false` otherwise
	bool isReadyToRun_;

	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;

private:
	void onMessage(const std::string& message);
	void sendMessage(const std::string& message);
	int waitForFrontendMessage();

public:
	Inspector(v8::Isolate* isolate, int port, int contextGroupId);

	void startAgent();
	void stopAgent();
	bool isReadyToRun();
};


#endif //V8_INSPECTOR_H
