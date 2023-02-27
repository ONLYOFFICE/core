#include "v8_inspector_client.h"

V8InspectorClientImpl::V8InspectorClientImpl(v8::Platform* platform, const v8::Local<v8::Context> &context, const std::function<void(std::string)> &onResponse, const std::function<int(void)> &onWaitFrontendMessageOnPause) {
	platform_ = platform;
	context_ = context;
	onWaitFrontendMessageOnPause_ = onWaitFrontendMessageOnPause;
	isolate_ = context_->GetIsolate();
	channel_.reset(new V8InspectorChannelImpl(isolate_, onResponse));
	inspector_ = v8_inspector::V8Inspector::create(isolate_, this);
	session_ = inspector_->connect(kContextGroupId, channel_.get(), v8_inspector::StringView());
	context_->SetAlignedPointerInEmbedderData(1, this);

	v8_inspector::StringView contextName = convertToStringView("inspector");
	inspector_->contextCreated(v8_inspector::V8ContextInfo(context, kContextGroupId, contextName));
	terminated_ = true;
	run_nested_loop_ = false;
}

void V8InspectorClientImpl::dispatchProtocolMessage(const v8_inspector::StringView &message_view) {
	session_->dispatchProtocolMessage(message_view);
}

void V8InspectorClientImpl::runMessageLoopOnPause(int contextGroupId) {
	if (run_nested_loop_) {
		return;
	}
	terminated_ = false;
	run_nested_loop_ = true;
	while (!terminated_ && onWaitFrontendMessageOnPause_()) {
		while (v8::platform::PumpMessageLoop(platform_, isolate_)) {}
	}
	terminated_ = true;
	run_nested_loop_ = false;
}

void V8InspectorClientImpl::quitMessageLoopOnPause() {
	terminated_ = true;
}

v8::Local<v8::Context> V8InspectorClientImpl::ensureDefaultContextInGroup(int contextGroupId) {
	return context_;
}

void V8InspectorClientImpl::schedulePauseOnNextStatement(const v8_inspector::StringView &reason) {
	session_->schedulePauseOnNextStatement(reason, reason);
}

void V8InspectorClientImpl::waitFrontendMessageOnPause() {
	terminated_ = false;
}
