#include "inspector_pool.h"
#include "portdistributor.h"
#include <iostream>


NSJSBase::v8_debug::internal::CInspectorPool::CInspectorPool(const std::string &contextName)
    : m_iContextGroupId{CInspectorInfo::getContextId()}
    , m_ContextName{contextName}
{
    //
}

NSJSBase::v8_debug::internal::CInspectorPool::~CInspectorPool() {
    std::cout << "POOL DTOR\n";
}

NSJSBase::v8_debug::internal::CInspectorPool::storage_t::iterator
NSJSBase::v8_debug::internal::CInspectorPool::findFreeInspector()
{
    for (auto i = m_Inspectors.begin(); i != m_Inspectors.end(); ++i) {
        if (i->second.free()) {
            return i;
        }
    }
    return m_Inspectors.end();
}

NSJSBase::v8_debug::internal::CInspectorImpl& NSJSBase::v8_debug::internal::CInspectorPool::addInspector()
{
    uint16_t port = CPortDistributor::getPort();
    std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
                std::piecewise_construct
                , std::forward_as_tuple(port)
                , std::forward_as_tuple(
                    m_Context
                    , m_pPlatform
                    , CInspectorInfo{
                        m_bLog
                        , m_iContextGroupId
                        , m_ContextName
                    }
                    , port)
                );
    CInspectorImpl &inspector = result.first->second;
    inspector.prepareServer();
    return inspector;
}

NSJSBase::v8_debug::internal::CInspectorImpl& NSJSBase::v8_debug::internal::CInspectorPool::singleInsp()
{
    if (1 == m_Inspectors.size()) {
        return m_Inspectors.begin()->second;
    }
    return addInspector();
}

NSJSBase::v8_debug::internal::CInspectorImpl& NSJSBase::v8_debug::internal::CInspectorPool::getInspector()
{
//    //tmp
//    return singleInsp();
//    //main variant
//    storage_t::iterator iter = findFreeInspector();
//    if (m_Inspectors.end() == iter) {
//        return addInspector();
//    }
//    return iter->second;
    return *ppp;
}

void NSJSBase::v8_debug::internal::CInspectorPool::maybeSetV8Data(v8::Local<v8::Context> context, v8::Platform *platform)
{
    if (isInitV8Data()) {
        return;
    }
    m_Context = context;
    m_pPlatform = platform;
    if (ppp) {
        std::cout << "init called for second time\n";
        return;
    }
    ppp = std::make_unique<CInspectorImpl>(
                m_Context
                , m_pPlatform
                , CInspectorInfo{
                    m_bLog
                    , m_iContextGroupId
                    , m_ContextName
                }
                , 8080);
    ppp->prepareServer();
}

bool NSJSBase::v8_debug::internal::CInspectorPool::isInitV8Data() const
{
    return m_pPlatform;
}
