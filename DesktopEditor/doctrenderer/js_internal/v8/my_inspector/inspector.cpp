#include "inspector.h"

#include "../v8_base.h"

Inspector::Inspector(v8::Isolate* pIsolate, int nPort, int nContextGroupId)
	: m_pIsolate(pIsolate)
	, m_nPort(nPort)
{
	m_pWebsocketServer.reset(
		new WebSocketServer(
			m_nPort,
			std::bind(&Inspector::onMessage, this, std::placeholders::_1)
		)
	);
	m_pIspectorClient.reset(
		new V8InspectorClientImpl(
			CV8Worker::getInitializer().getPlatform(),
			m_pIsolate,
			nContextGroupId,
			std::bind(&Inspector::sendMessage, this, std::placeholders::_1),
			std::bind(&Inspector::waitForFrontendMessage, this)
		)
	);
	m_pWebsocketServer->connect();
}

void Inspector::onMessage(const std::string& sMessage)
{
//	std::cout << "CDT message: " << message << std::endl;
	v8_inspector::StringView oProtocolMessage = convertToStringView(sMessage);
	m_pIspectorClient->dispatchProtocolMessage(oProtocolMessage);

	v8::Local<v8::Object> oJsonObject = parseJson(m_pIsolate->GetCurrentContext(), sMessage);
	if (!oJsonObject.IsEmpty())
	{
		std::string sMethod = getPropertyFromJson(m_pIsolate, oJsonObject, "method");
		if (sMethod == "Runtime.runIfWaitingForDebugger")
		{
			m_pWebsocketServer->isServerReady_ = true;
		}
	}
}

void Inspector::sendMessage(const std::string& sMessage)
{
//	std::cout << "Message to frontend: " << message << std::endl;
	m_pWebsocketServer->sendMessage(sMessage);
}

void Inspector::startAgent()
{
	m_pIspectorClient->schedulePauseOnNextStatement(convertToStringView("debugging"));
}

bool Inspector::waitForFrontendMessage()
{
	m_pWebsocketServer->waitForFrontendMessageOnPause();
	return true;
}
