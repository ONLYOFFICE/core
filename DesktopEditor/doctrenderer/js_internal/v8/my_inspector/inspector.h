#ifndef V8_INSPECTOR_EXAMPLE_INSPECTOR_H
#define V8_INSPECTOR_EXAMPLE_INSPECTOR_H

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
	// TODO: make static and increment for all next contexts
	int port_ = 8080;
	// pointer to CJSContext instance in which scripts/functions are executed
	NSJSBase::CJSContext* jscontext_;
	// reference to executed script
	const std::string& script_;
	// V8 context
	v8::Handle<v8::Context> context_;
	// JS value, returned by script
	JSSmart<NSJSBase::CJSValue> ret_;

	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;

private:
	void onMessage(const std::string& message);
	void sendMessage(const std::string& message);
	int waitForFrontendMessage();

public:
	Inspector(NSJSBase::CJSContext* context, const std::string& script);

	void startAgent();
	bool isScriptRunning();
	JSSmart<NSJSBase::CJSValue> getReturnValue();
};


#endif //V8_INSPECTOR_EXAMPLE_INSPECTOR_H
