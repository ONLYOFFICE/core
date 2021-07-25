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

namespace internal {
class CInspectorPool;
}

class CPerContextInspector
{
    std::unique_ptr<internal::CInspectorPool> m_pPool{nullptr};

public:
    CPerContextInspector(const std::string &contextName = "");

    CPerContextInspector& maybeInit(v8::Local<v8::Context> context, v8::Platform *platform);
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
    void dispose();

    ~CPerContextInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // CPERCONTEXTINSPECTOR_H
