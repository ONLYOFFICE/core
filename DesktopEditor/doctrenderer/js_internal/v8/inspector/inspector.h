#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <functional>

#include "websocket_server.h"
#include "v8_inspector_client.h"
#include "utils.h"

// Inspector code structure based on "V8 Inspector simple examle" from this repository:
// https://github.com/ahmadov/v8_inspector_example

namespace NSJSBase
{

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

}

#endif // INSPECTOR_H
