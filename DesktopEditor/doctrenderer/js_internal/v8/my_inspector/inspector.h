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
	v8::Isolate* m_pIsolate;
	// server port
	const int m_nPort;

	std::unique_ptr<WebSocketServer> m_pWebsocketServer;
	std::unique_ptr<V8InspectorClientImpl> m_pIspectorClient;

private:
	void onMessage(const std::string& sMessage);
	void sendMessage(const std::string& sMessage);
	bool waitForFrontendMessage();

public:
	Inspector(v8::Isolate* pIsolate, int nPort, int nContextGroupId);

	void startAgent();
};


#endif //V8_INSPECTOR_H
