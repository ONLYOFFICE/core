#ifndef CPERCONTEXTINSPECTOR_H
#define CPERCONTEXTINSPECTOR_H

#include <memory>
#include <string>
#include <v8.h>

//forward declarations
namespace NSJSBase {
    class CJSTryCatch;
    class CJSValue;
}
namespace NSCommon {
    template <typename T>
    class smart_ptr;
}

namespace NSJSBase {
namespace v8_debug {

class CPerContextInspector
{
    v8::Local<v8::Context> m_Context{};
    v8::Platform *m_pPlatform{nullptr};

public:
    CPerContextInspector(v8::Local<v8::Context> context, v8::Platform *platform);

    NSCommon::smart_ptr<CJSValue> runScript(
                const std::string &scriptStr
                , NSCommon::smart_ptr<CJSTryCatch> &pException
                , const std::wstring &scriptPath
                );
    NSCommon::smart_ptr<CJSValue> callFunc(
                v8::Local<v8::Object> value
                , const char *name
                , int argc
                , NSCommon::smart_ptr<CJSValue> argv[]
                );

    static void dispose();

    ~CPerContextInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // CPERCONTEXTINSPECTOR_H
