#ifndef CPERCONTEXTINSPECTOR_H
#define CPERCONTEXTINSPECTOR_H

//#include <memory>
//#include <string>
#include <v8.h>

namespace NSJSBase {
namespace v8_debug {

void beforeScript(v8::Local<v8::Context> context, v8::Platform *platform, const std::string &contextName = "");
void beforeFunc(v8::Local<v8::Context> context, v8::Platform *platform, const std::string &contextName = "");
void disposeInspector(v8::Local<v8::Context> context);
//class CPerContextInspector
//{
//    v8::Local<v8::Context> m_Context{};
//    v8::Platform *m_pPlatform{nullptr};

//public:
//    CPerContextInspector(v8::Local<v8::Context> context, v8::Platform *platform);

//    NSCommon::smart_ptr<CJSValue> runScript(
//                const std::string &scriptStr
//                , NSCommon::smart_ptr<CJSTryCatch> &pException
//                , const std::wstring &scriptPath
//                );
//    NSCommon::smart_ptr<CJSValue> callFunc(
//                v8::Local<v8::Object> value
//                , const char *name
//                , int argc
//                , NSCommon::smart_ptr<CJSValue> argv[]
//                );

//    static void disposeContext(v8::Local<v8::Context> context);

//    ~CPerContextInspector();
//};

}//namespace v8_debug
}//namespace NSJSBase

#endif // CPERCONTEXTINSPECTOR_H
