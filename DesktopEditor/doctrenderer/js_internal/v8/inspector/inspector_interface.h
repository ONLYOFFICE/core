#ifndef INSPECTOR_INTERFACE_H
#define INSPECTOR_INTERFACE_H

#include <v8.h>

//simplified to the limit interface
//also required to avoid cyclic dependency

namespace NSJSBase {
namespace v8_debug {

void before(const v8::Local<v8::Context> &context, v8::Platform *platform, const std::string &contextName);
void disposeInspector(const v8::Local<v8::Context> &context);

}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_INTERFACE_H
