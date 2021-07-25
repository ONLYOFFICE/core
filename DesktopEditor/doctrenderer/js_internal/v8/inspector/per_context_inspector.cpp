#include "per_context_inspector.h"
#include "inspector_pool.h"
#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include <iostream>

std::unique_ptr<NSJSBase::v8_debug::internal::CInspectorPool>
NSJSBase::v8_debug::CPerContextInspector::m_pPool{nullptr};


NSJSBase::v8_debug::CPerContextInspector::CPerContextInspector(const std::string &contextName)
//    : m_pPool{std::make_unique<internal::CInspectorPool>(contextName)}
{
    if (!m_pPool) {
        m_pPool = std::make_unique<internal::CInspectorPool>("");
    }
}

NSJSBase::v8_debug::CPerContextInspector&
NSJSBase::v8_debug::CPerContextInspector::maybeInit(v8::Local<v8::Context> context, v8::Platform *platform)
{
    m_pPool->maybeSetV8Data(context, platform);
    return *this;
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CPerContextInspector::runScript(
        const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        )
{
    return m_pPool->getInspector().runScript({scriptStr, pException, scriptPath});
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CPerContextInspector::callFunc(
        v8::Local<v8::Object> value
        , const char *name
        , int argc
        , NSCommon::smart_ptr<CJSValue> argv[])
{
    return m_pPool->getInspector().callFunc({value, name, argc, argv});
}

void NSJSBase::v8_debug::CPerContextInspector::dispose()
{
    m_pPool.reset(nullptr);
}

NSJSBase::v8_debug::CPerContextInspector::~CPerContextInspector() {
    std::cout << "PER CONTEXT STUFF DTOR CALLED\n";
//    m_pPool.release();//tmp
}
