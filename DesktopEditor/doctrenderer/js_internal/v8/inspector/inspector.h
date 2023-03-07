#ifndef V8_INSPECTOR_H
#define V8_INSPECTOR_H

#include <iostream>
#include <functional>

#include "websocket_server.h"
#include "v8_inspector_client.h"
#include "utils.h"

class CInspector
{
private:
	// pointer to V8 isolate in which scripts/functions are executed
	v8::Isolate* m_pIsolate;
	// server port
	const int m_nPort;

	std::unique_ptr<CWebSocketServer> m_pWebsocketServer;
	std::unique_ptr<CV8InspectorClientImpl> m_pIspectorClient;

private:
	void onMessage(std::string& sMessage);
	void sendMessage(const std::string& sMessage);
	bool waitForFrontendMessage();

public:
	CInspector(v8::Isolate* pIsolate, int nPort, int nContextGroupId);

	void startAgent(bool bIsBreakOnStart = true);
};


#endif //V8_INSPECTOR_H
