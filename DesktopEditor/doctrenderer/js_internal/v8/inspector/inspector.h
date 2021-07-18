#ifndef CINSPECTOR_H
#define CINSPECTOR_H

#include <v8.h>//context and platform
#include <string>//strings

//forward declarations
namespace NSJSBase {
    class CJSTryCatch;
    class CJSValue;
}
namespace NSCommon {
    template <typename T>
    class smart_ptr;
}

//
namespace NSJSBase {
namespace v8_debug {

//inspector impl forward declaration
namespace internal {
    class CInspectorImpl;
}

//just a wrap around inspector implementation
class CInspector
{
    std::unique_ptr<internal::CInspectorImpl> pImpl;
public:
    CInspector(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            );
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
    ~CInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTOR_H
