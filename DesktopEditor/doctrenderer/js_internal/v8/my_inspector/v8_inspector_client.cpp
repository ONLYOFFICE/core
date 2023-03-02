#include "v8_inspector_client.h"

#include <iostream>
#include <libplatform/libplatform.h>

#include "utils.h"

V8InspectorClientImpl::V8InspectorClientImpl(
	v8::Platform* platform,
	v8::Isolate* isolate,
	int contextGroupId,
	const std::function<void(std::string)>& onResponse,
	const std::function<int(void)>& onWaitFrontendMessageOnPause)
	: platform_(platform)
	, isolate_(isolate)
	, contextGroupId_(contextGroupId)
	, onWaitFrontendMessageOnPause_(std::move(onWaitFrontendMessageOnPause))
{
	v8::Local<v8::Context> context = isolate_->GetCurrentContext();
	// initialize all V8 inspector stuff
	channel_.reset(new V8InspectorChannelImpl(isolate_, onResponse));
	inspector_ = v8_inspector::V8Inspector::create(isolate_, this);
	session_ = inspector_->connect(contextGroupId_, channel_.get(), v8_inspector::StringView());
	context->SetAlignedPointerInEmbedderData(1, this);

	v8_inspector::StringView contextName = convertToStringView("inspector" + std::to_string(contextGroupId));
	inspector_->contextCreated(v8_inspector::V8ContextInfo(context, contextGroupId_, contextName));
}

void V8InspectorClientImpl::dispatchProtocolMessage(const v8_inspector::StringView &message_view)
{
	session_->dispatchProtocolMessage(message_view);
}

void V8InspectorClientImpl::runMessageLoopOnPause(int contextGroupId)
{
	if (run_nested_loop_)
	{
		return;
	}
	terminated_ = false;
	run_nested_loop_ = true;
	while (!terminated_ && onWaitFrontendMessageOnPause_())
	{
		while (v8::platform::PumpMessageLoop(platform_, isolate_)) {}
	}
	terminated_ = true;
	run_nested_loop_ = false;
}

void V8InspectorClientImpl::quitMessageLoopOnPause()
{
	terminated_ = true;
}

v8::Local<v8::Context> V8InspectorClientImpl::ensureDefaultContextInGroup(int contextGroupId)
{
	return isolate_->GetCurrentContext();
}

void V8InspectorClientImpl::schedulePauseOnNextStatement(const v8_inspector::StringView &reason)
{
	session_->schedulePauseOnNextStatement(reason, {});
}

void V8InspectorClientImpl::waitFrontendMessageOnPause()
{
	terminated_ = false;
}
