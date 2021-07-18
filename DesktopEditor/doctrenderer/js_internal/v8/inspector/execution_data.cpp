#include "execution_data.h"

NSJSBase::v8_debug::internal::CFCallData::CFCallData(
        v8::Local<v8::Object> val
        , const char *funcName
        , int argCount
        , NSCommon::smart_ptr<CJSValue> *argVal)
    : value(val)
    , name(funcName)
    , argc(argCount)
    , argv(argVal)
{
    //
}

void NSJSBase::v8_debug::internal::CFCallData::dispose() {
    value.Clear();
    name = nullptr;
    argc = 0;
    argv = nullptr;
}

NSJSBase::v8_debug::internal::CScriptExecData::CScriptExecData(const std::string &script
        , NSCommon::smart_ptr<CJSTryCatch> &exception
        , const std::wstring sPath)
    : scriptSource(script)
    , pException(&exception)
    , scriptPath(sPath)
{
    //
}

void NSJSBase::v8_debug::internal::CScriptExecData::dispose() {
    scriptSource.clear();
    pException = nullptr;
    scriptPath.clear();
}
