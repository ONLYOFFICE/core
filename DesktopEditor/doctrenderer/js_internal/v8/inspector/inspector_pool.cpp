#include "inspector_pool.h"
#include "portdistributor.h"
#include <iostream>


NSJSBase::v8_debug::internal::CInspectorPool::CInspectorPool() = default;

NSJSBase::v8_debug::internal::CInspectorPool::~CInspectorPool() = default;

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

NSJSBase::v8_debug::internal::CInspectorImpl&
NSJSBase::v8_debug::internal::CInspectorPool::getInspector(v8::Local<v8::Context> context
                                                           , v8::Platform *platform
                                                           , const std::string &contextName)
{
    std::lock_guard<std::mutex> lock{m_Mutex};
    auto iter = m_Inspectors.find(context->GetIsolate());
    if (m_Inspectors.end() == iter) {
        return addInspector(context, platform, contextName);
    }
    return iter->second;
}

void NSJSBase::v8_debug::internal::CInspectorPool::dispose()
{
    m_Inspectors.clear();
}

NSJSBase::v8_debug::internal::CInspectorPool&
NSJSBase::v8_debug::internal::CInspectorPool::get()
{
    static CInspectorPool pool;
    return pool;
}
