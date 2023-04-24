#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "websocket_server.h"
#include "v8_inspector_client.h"

#include <functional>

// To use inspector set V8_USE_INSPECTOR environment variable to any non-zero value
// Unset V8_USE_INSPECTOR or set it to '0' for disabling the inspector


// Inspector code structure based on "V8 Inspector simple examle" from this repository:
// https://github.com/ahmadov/v8_inspector_example

namespace NSJSBase
{

	class CInspector
	{
	private:
		// pointer to V8 isolate in which scripts/functions are executed
		v8::Isolate* m_pIsolate;

		std::unique_ptr<CWebSocketServer> m_pWebsocketServer;
		std::unique_ptr<CV8InspectorClientImpl> m_pIspectorClient;

	private:
		void onMessage(std::string& sMessage);
		void sendMessage(const std::string& sMessage);
		bool waitForFrontendMessage();

	public:
		CInspector(v8::Isolate* pIsolate, int nContextGroupId);

		void startAgent(bool bIsBreakOnStart = true);
	};

}

#endif // INSPECTOR_H
