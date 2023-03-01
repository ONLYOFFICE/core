#ifndef V8INSPECTORCLIENTIMPL_H
#define V8INSPECTORCLIENTIMPL_H

#include <iostream>
#include <v8.h>
#include <v8-inspector.h>
#include <libplatform/libplatform.h>
#include "v8_inspector_channel.h"
#include "utils.h"

class V8InspectorClientImpl final: public v8_inspector::V8InspectorClient
{
private:
	static const int kContextGroupId = 1;
	v8::Platform* platform_;
	// V8 Inspector stuff
	std::unique_ptr<v8_inspector::V8Inspector> inspector_;
	std::unique_ptr<v8_inspector::V8InspectorSession> session_;
	std::unique_ptr<V8InspectorChannelImpl> channel_;
	// V8 isolate and context
	v8::Isolate* isolate_;
	v8::Handle<v8::Context> context_;
	// callbacks
	std::function<int(void)> onWaitFrontendMessageOnPause_;
	// server message loop terminate flag
	bool terminated_ = false;
	// flag for preventing server from falling into two loops
	bool run_nested_loop_ = false;

private:
	v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

public:
	V8InspectorClientImpl(v8::Platform* platform, const v8::Local<v8::Context>& context, const std::function<void(std::string)> &onResponse, const std::function<int(void)> &onWaitFrontendMessageOnPause);
	// send message to V8 internals
	void dispatchProtocolMessage(const v8_inspector::StringView &message_view);
	// overriden interface methods
	void runMessageLoopOnPause(int contextGroupId) override;
	void quitMessageLoopOnPause() override;
	// pause on next statement
	void schedulePauseOnNextStatement(const v8_inspector::StringView &reason);
	// wait for message from frontend
	void waitFrontendMessageOnPause();

};

#endif // V8INSPECTORCLIENTIMPL_H
