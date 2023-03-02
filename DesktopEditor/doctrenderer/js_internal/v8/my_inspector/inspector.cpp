#include "inspector.h"

#include "../v8_base.h"

Inspector::Inspector(v8::Isolate* isolate, int port, int contextGroupId)
	: isolate_(isolate)
	, port_(port)
{
	websocket_server_.reset(
		new WebSocketServer(
			port_,
			std::bind(&Inspector::onMessage, this, std::placeholders::_1),
			std::bind(&Inspector::isReadyToRun, this)
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
			inspector_client_->schedulePauseOnNextStatement(convertToStringView("For testing purpose!"));
			isReadyToRun_ = true;
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
	isReadyToRun_ = false;
	websocket_server_->run();
}

void Inspector::stopAgent()
{
	websocket_server_->stop();
}

bool Inspector::isReadyToRun()
{
	return isReadyToRun_;
}

int Inspector::waitForFrontendMessage()
{
	websocket_server_->waitForFrontendMessageOnPause();
	return 1;
}
