#include "inspector.h"

#include "../v8_base.h"

Inspector::Inspector(v8::Isolate* isolate, int port, int contextGroupId)
	: isolate_(isolate)
	, port_(port)
{
	websocket_server_.reset(
		new WebSocketServer(
			port_,
			std::bind(&Inspector::onMessage, this, std::placeholders::_1)
		)
	);
	inspector_client_.reset(
		new V8InspectorClientImpl(
			CV8Worker::getInitializer().getPlatform(),
			isolate_,
			contextGroupId,
			std::bind(&Inspector::sendMessage, this, std::placeholders::_1),
			std::bind(&Inspector::waitForFrontendMessage, this)
		)
	);
	websocket_server_->connect();
}

void Inspector::onMessage(const std::string& message)
{
//	std::cout << "CDT message: " << message << std::endl;
	v8_inspector::StringView protocolMessage = convertToStringView(message);
	inspector_client_->dispatchProtocolMessage(protocolMessage);

	v8::Local<v8::Object> jsonObject = parseJson(isolate_->GetCurrentContext(), message);
	if (!jsonObject.IsEmpty())
	{
		std::string method = getPropertyFromJson(isolate_, jsonObject, "method");
		if (method == "Runtime.runIfWaitingForDebugger")
		{
			websocket_server_->isServerReady_ = true;
		}
	}
}

void Inspector::sendMessage(const std::string& message)
{
//	std::cout << "Message to frontend: " << message << std::endl;
	websocket_server_->sendMessage(message);
}

void Inspector::startAgent()
{
	inspector_client_->schedulePauseOnNextStatement(convertToStringView("debugging"));
}

int Inspector::waitForFrontendMessage()
{
	websocket_server_->waitForFrontendMessageOnPause();
	return 1;
}
