#include "inspector_pool.h"
#include "portdistributor.h"
#include <iostream>


NSJSBase::v8_debug::internal::CInspectorPool::CInspectorPool()
//    : m_iContextGroupId{CInspectorInfo::getContextId()}
//    , m_ContextName{contextName}
{
    //
}

NSJSBase::v8_debug::internal::CInspectorPool::~CInspectorPool() {
    std::cout << "POOL DTOR\n";
}

//NSJSBase::v8_debug::internal::CInspectorPool::storage_t::iterator
//NSJSBase::v8_debug::internal::CInspectorPool::findInspector(v8::Isolate *isolate)
//{
//    for (auto i = m_Inspectors.begin(); i != m_Inspectors.end(); ++i) {
//        if (i->second.free()) {
//            return i;
//        }
//    }
//    return m_Inspectors.end();
//}

NSJSBase::v8_debug::internal::CInspectorImpl&
NSJSBase::v8_debug::internal::CInspectorPool::addInspector(v8::Local<v8::Context> context
                                                           , v8::Platform *platform
                                                           , const std::string &contextName)
{
    uint16_t port = CPortDistributor::getPort();
    std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
                std::piecewise_construct
                , std::forward_as_tuple(context->GetIsolate())
                , std::forward_as_tuple(
                    context
                    , platform
                    , CInspectorInfo{
                        m_bLog
                        , CInspectorInfo::getContextId()
                        , contextName
                    }
                    , port)
                );
    CInspectorImpl &inspector = result.first->second;
    inspector.prepareServer();
    return inspector;
}

//NSJSBase::v8_debug::internal::CInspectorImpl& NSJSBase::v8_debug::internal::CInspectorPool::singleInsp()
//{
//    if (1 == m_Inspectors.size()) {
//        return m_Inspectors.begin()->second;
//    }
//    return addInspector();
//}

NSJSBase::v8_debug::internal::CInspectorImpl&
NSJSBase::v8_debug::internal::CInspectorPool::getInspector(v8::Local<v8::Context> context
                                                           , v8::Platform *platform
                                                           , const std::string &contextName)
{
//    //main variant
//    storage_t::iterator iter = findFreeInspector();
//    if (m_Inspectors.end() == iter) {
//        return addInspector();
//    }
//    return iter->second;
    int todo_mutex;
    auto iter = m_Inspectors.find(context->GetIsolate());
    if (m_Inspectors.end() == iter) {
        return addInspector(context, platform, contextName);
    }
    return iter->second;
}

NSJSBase::v8_debug::internal::CInspectorPool&
NSJSBase::v8_debug::internal::CInspectorPool::get()
{
    static CInspectorPool pool;
    return pool;
}

//void NSJSBase::v8_debug::internal::CInspectorPool::maybeSetV8Data(v8::Local<v8::Context> context, v8::Platform *platform)
//{
//    if (isInitV8Data()) {
//        return;
//    }
//    m_Context = context;
//    m_pPlatform = platform;
//    if (ppp) {
//        std::cout << "init called for second time\n";
//        return;
//    }
//    ppp = std::make_unique<CInspectorImpl>(
//                m_Context
//                , m_pPlatform
//                , CInspectorInfo{
//                    m_bLog
//                    , m_iContextGroupId
//                    , m_ContextName
//                }
//                , 8080);
//    ppp->prepareServer();
//}

//bool NSJSBase::v8_debug::internal::CInspectorPool::isInitV8Data() const
//{
//    return m_pPlatform;
//}
