#ifndef BASEV8INSPECTORLISTENERIMPL_H
#define BASEV8INSPECTORLISTENERIMPL_H

#include "v8_inspector_listener.h"

class BaseV8InspectorListenerImpl final: public V8InspectorListener {
public:
	void onConnected(JSSmart<NSJSBase::CJSContext> context, const std::string& script) override {
		context->runScript(script);
	}
};

#endif //BASEV8INSPECTORLISTENERIMPL_H
