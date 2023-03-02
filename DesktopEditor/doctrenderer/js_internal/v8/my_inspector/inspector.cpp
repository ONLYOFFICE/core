#include "inspector.h"

#include "../v8_base.h"

Inspector::Inspector(NSJSBase::CJSContext* context, int port, int contextGroupId)
	: jscontext_(context)
	, port_(port)
{
	websocket_server_.reset(
		new WebSocketServer(
			port_,
			std::bind(&Inspector::onMessage, this, std::placeholders::_1),
			std::bind(&Inspector::isScriptRunning, this)
		)
	);
	inspector_client_.reset(
		new V8InspectorClientImpl(
			CV8Worker::getInitializer().getPlatform(),
			jscontext_->m_internal->m_isolate,
			contextGroupId,
			std::bind(&Inspector::sendMessage, this, std::placeholders::_1),
			std::bind(&Inspector::waitForFrontendMessage, this)
		)
	);
}

void Inspector::setScriptSource(const std::string& script)
{
	script_ = script;
}

void Inspector::onMessage(const std::string& message)
{
//	std::cout << "CDT message: " << message << std::endl;
	v8_inspector::StringView protocolMessage = convertToStringView(message);
	inspector_client_->dispatchProtocolMessage(protocolMessage);

	v8::Local<v8::Object> jsonObject = parseJson(jscontext_->m_internal->m_context, message);
	if (!jsonObject.IsEmpty())
	{
		std::string method = getPropertyFromJson(jscontext_->m_internal->m_isolate, jsonObject, "method");
		if (method == "Runtime.runIfWaitingForDebugger")
		{
			inspector_client_->schedulePauseOnNextStatement(convertToStringView("For testing purpose!"));
			ret_ = jscontext_->runScript(script_);
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
	websocket_server_->run();
}

bool Inspector::isScriptRunning()
{
	return jscontext_->m_internal->m_bRunningInInspector;
}

JSSmart<NSJSBase::CJSValue> Inspector::getReturnValue()
{
	return ret_;
}

int Inspector::waitForFrontendMessage()
{
	websocket_server_->waitForFrontendMessageOnPause();
	return 1;
}
