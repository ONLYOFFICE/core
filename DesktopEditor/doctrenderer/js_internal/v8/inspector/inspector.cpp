#include "inspector.h"
#include "inspectormanager.h"//for getting inspector
#include "../v8_base.h"//v8 wrappers and smart_ptr
#include "inspector_impl.h"//inspector implementation

NSJSBase::v8_debug::CInspector::CInspector(
        v8::Local<v8::Context> context
        , v8::Platform *platform)
    : pImpl(internal::CInspectorManager::getInspector(context, platform))
{
    //
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CInspector::runScript(
        const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        )
{
    return pImpl->runScript(internal::CScriptExecData{
                              scriptStr
                              , pException
                              , scriptPath
                          });
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CInspector::callFunc(
        v8::Local<v8::Object> value
        , const char *name
        , int argc
        , NSCommon::smart_ptr<CJSValue> argv[]
        )
{
    return pImpl->callFunc(internal::CFCallData{
                             value
                             , name
                             , argc
                             , argv
                           });
}

//must move dtor here, because here CInspectorImpl is complete class
NSJSBase::v8_debug::CInspector::~CInspector() = default;
