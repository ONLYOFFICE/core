#ifndef V8INSPECTORCLIENTIMPL_H
#define V8INSPECTORCLIENTIMPL_H

#include <iostream>
#include <v8.h>
#include <v8-inspector.h>
#include <libplatform/libplatform.h>
#include "v8_inspector_channel.h"
#include "utils.h"

class V8InspectorClientImpl final: public v8_inspector::V8InspectorClient {
public:
  V8InspectorClientImpl(v8::Platform* platform, const v8::Local<v8::Context>& context, const std::function<void(std::string)> &onResponse, const std::function<int(void)> &onWaitFrontendMessageOnPause);

  void dispatchProtocolMessage(const v8_inspector::StringView &message_view);

  void runMessageLoopOnPause(int contextGroupId) override;

  void quitMessageLoopOnPause() override;

  void schedulePauseOnNextStatement(const v8_inspector::StringView &reason);

  void waitFrontendMessageOnPause();
private:
  v8::Local<v8::Context> ensureDefaultContextInGroup(int contextGroupId) override;

  static const int kContextGroupId = 1;
  v8::Platform* platform_;
  std::unique_ptr<v8_inspector::V8Inspector> inspector_;
  std::unique_ptr<v8_inspector::V8InspectorSession> session_;
  std::unique_ptr<V8InspectorChannelImpl> channel_;
  v8::Isolate* isolate_;
  v8::Handle<v8::Context> context_;
  std::function<int(void)> onWaitFrontendMessageOnPause_;
  uint8_t terminated_ = 0;
  uint8_t run_nested_loop_ = 0;
};

#endif // V8INSPECTORCLIENTIMPL_H
