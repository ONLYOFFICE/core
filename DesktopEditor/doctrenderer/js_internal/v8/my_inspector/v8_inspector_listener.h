#ifndef V8_INSPECTOR_LISTENER_H
#define V8_INSPECTOR_LISTENER_H

#include <v8.h>

#include "../../js_base.h"

struct V8InspectorListener {
	virtual void onConnected(const v8::Local<v8::Context>& context) = 0;
};

#endif //V8_INSPECTOR_LISTENER_H
