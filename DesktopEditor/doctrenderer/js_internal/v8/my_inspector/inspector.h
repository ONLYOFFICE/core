#ifndef V8_INSPECTOR_H
#define V8_INSPECTOR_H

#include <iostream>
#include <functional>
#include <vector>
#include <list>

#include "../../js_base.h"
#include "websocket_server.h"
#include "v8_inspector_client.h"
#include "utils.h"

class Inspector
{
private:
	// pointer to CJSContext instance in which scripts/functions are executed
	NSJSBase::CJSContext* jscontext_;
	// JS value, returned by script
	JSSmart<NSJSBase::CJSValue> ret_;
	// server port
	int port_;

	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;

private:
	void onMessage(const std::string& message);
	void sendMessage(const std::string& message);
	int waitForFrontendMessage();

public:
	Inspector(NSJSBase::CJSContext* context, int port, int contextGroupId);

	void startAgent();
	bool isScriptRunning();
	JSSmart<NSJSBase::CJSValue> getReturnValue();
};


#endif //V8_INSPECTOR_H
