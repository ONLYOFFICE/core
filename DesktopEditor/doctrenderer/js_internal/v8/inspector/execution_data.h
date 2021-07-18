#ifndef EXECUTION_DATA_H
#define EXECUTION_DATA_H

#include <v8.h>//v8 stuff

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

//script execution data
struct CScriptExecData {
    std::string scriptSource{};
    NSCommon::smart_ptr<CJSTryCatch> *pException = nullptr;
    std::wstring scriptPath{};
    //
    CScriptExecData() = default;
    CScriptExecData(
            const std::string &script
          , NSCommon::smart_ptr<CJSTryCatch> &exception
          , const std::wstring sPath
            );
    void dispose();
};

//function call data
struct CFCallData {
    v8::Local<v8::Object> value{};
    const char *name = nullptr;
    int argc{0};
    NSCommon::smart_ptr<CJSValue> *argv = nullptr;
    //
    CFCallData() = default;
    CFCallData(
            v8::Local<v8::Object> val
            , const char *funcName
            , int argCount
            , NSCommon::smart_ptr<CJSValue> *argVal
            );
    void dispose();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // EXECUTION_DATA_H
