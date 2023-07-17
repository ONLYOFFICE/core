#include "inspector.h"

#include "../v8_base.h"
#include "utils.h"

namespace NSJSBase
{

	CInspector::CInspector(v8::Isolate* pIsolate, int nContextGroupId)
		: m_pIsolate(pIsolate)
	{
		m_pWebsocketServer.reset(
			new CWebSocketServer(
				std::bind(&CInspector::onMessage, this, std::placeholders::_1)
			)
		);
		m_pIspectorClient.reset(
			new CV8InspectorClientImpl(
				CV8Worker::getInitializer().getPlatform(),
				m_pIsolate,
				nContextGroupId,
				std::bind(&CInspector::sendMessage, this, std::placeholders::_1),
				std::bind(&CInspector::waitForFrontendMessage, this)
			)
		);
		m_pWebsocketServer->connect();
	}

	void CInspector::onMessage(std::string& sMessage)
	{
		v8::Local<v8::Object> oJsonObject = parseJson(m_pIsolate->GetCurrentContext(), sMessage);

		if (!oJsonObject.IsEmpty())
		{
			std::string sMethod = getPropertyFromJson(m_pIsolate, oJsonObject, "method");
			if (sMethod == "Runtime.runIfWaitingForDebugger")
			{
				m_pWebsocketServer->isServerReady_ = true;
			}
#ifdef V8_VERSION_89_PLUS

#else
			// In older version of V8 there is an eror "Either url or urlRegex must be specified." on trying to set a breakpoint in CDT.
			// So we have to specify it explicitly before "lineNumber" param
			else if (sMethod == "Debugger.setBreakpointByUrl" && sMessage.find("\"url") == sMessage.npos)
			{
				int pos = sMessage.find("\"lineNumber\"");
				sMessage.insert(pos, "\"url\":\"\",");
			}
#endif
		}

		v8_inspector::StringView oProtocolMessage = convertToStringView(sMessage);
		m_pIspectorClient->dispatchProtocolMessage(oProtocolMessage);
	}

	void CInspector::sendMessage(const std::string& sMessage)
	{
		m_pWebsocketServer->sendMessage(sMessage);
	}

	void CInspector::startAgent(bool bIsBreakOnStart)
	{
		if (bIsBreakOnStart)
		{
			m_pIspectorClient->schedulePauseOnNextStatement(convertToStringView("debugging"));
		}
	}

	bool CInspector::waitForFrontendMessage()
	{
		m_pWebsocketServer->waitForFrontendMessageOnPause();
		return true;
	}

}
