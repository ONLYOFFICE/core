#include "per_context_inspector.h"
#include "inspector_pool.h"
//#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include <iostream>

//NSJSBase::v8_debug::CPerContextInspector::CPerContextInspector(
//        v8::Local<v8::Context> context
//        , v8::Platform *platform)
//    : m_Context{context}
//    , m_pPlatform{platform}
//{
//    //
//}

//NSCommon::smart_ptr<NSJSBase::CJSValue>
//NSJSBase::v8_debug::CPerContextInspector::runScript(
//        const std::string &scriptStr
//        , NSCommon::smart_ptr<CJSTryCatch> &pException
//        , const std::wstring &scriptPath
//        )
//{
//    return internal::CInspectorPool::get().getInspector(m_Context, m_pPlatform).runScript({scriptStr, pException, scriptPath});
//}

//NSCommon::smart_ptr<NSJSBase::CJSValue>
//NSJSBase::v8_debug::CPerContextInspector::callFunc(
//        v8::Local<v8::Object> value
//        , const char *name
//        , int argc
//        , NSCommon::smart_ptr<CJSValue> argv[])
//{
//    return internal::CInspectorPool::get().getInspector(m_Context, m_pPlatform).callFunc({value, name, argc, argv});
//}

//void NSJSBase::v8_debug::CPerContextInspector::dispose()
//{
//    internal::CInspectorPool::get().dispose();
//}

//NSJSBase::v8_debug::CPerContextInspector::~CPerContextInspector() = default;




void NSJSBase::v8_debug::beforeScript(v8::Local<v8::Context> context
                                      , v8::Platform *platform
                                      , const std::string &contextName) {
    internal::CInspectorPool::get().getInspector(context, platform, contextName).beforeScript();
}

void NSJSBase::v8_debug::beforeFunc(v8::Local<v8::Context> context, v8::Platform *platform, const std::string &contextName) {
    internal::CInspectorPool::get().getInspector(context, platform, contextName).beforeFunc();
}

void NSJSBase::v8_debug::disposeInspector(v8::Local<v8::Context> context) {
    internal::CInspectorPool::get().disposeInspector(context);
}
