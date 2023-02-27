#ifndef BASEV8INSPECTORLISTENERIMPL_H
#define BASEV8INSPECTORLISTENERIMPL_H

#include "v8_inspector_listener.h"
#include "utils.h"

class BaseV8InspectorListenerImpl final: public V8InspectorListener {
public:
	void onConnected(const v8::Local<v8::Context>& context) override {
		// Absolute or relative path of JavaScript source file to debug.
		// V8 Inspector is very powerful, it has the source map support!
		// The TypeScript source file has to be inline source mapped in the JavaScript file.
		const std::string filePath = "../example/code.js";
		runScript(context, filePath);
	}
};

#endif //BASEV8INSPECTORLISTENERIMPL_H
