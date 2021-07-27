#ifndef CPERCONTEXTINSPECTOR_H
#define CPERCONTEXTINSPECTOR_H

#include <v8.h>

namespace NSJSBase {
namespace v8_debug {

void before(const v8::Local<v8::Context> &context, v8::Platform *platform, const std::string &contextName = "");
void disposeInspector(const v8::Local<v8::Context> &context);

}//namespace v8_debug
}//namespace NSJSBase

#endif // CPERCONTEXTINSPECTOR_H
