#include "inspector_pool.h"

NSJSBase::v8_debug::internal::CInspectorPool::CInspectorPool() = default;

NSJSBase::v8_debug::internal::CInspectorPool::~CInspectorPool() = default;

NSJSBase::v8_debug::internal::CInspectorImpl&
NSJSBase::v8_debug::internal::CInspectorPool::addInspector(v8::Local<v8::Context> context
                                                           , v8::Platform *platform
                                                           , const std::string &contextName)
{
    std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
                std::piecewise_construct
                , std::forward_as_tuple(context)
                , std::forward_as_tuple(
                    context
                    , platform
                    , getPort()
                    , getContextGroupId()
                    , contextName
                    , m_bLog
                    )
                );
    CInspectorImpl &inspector = result.first->second;
    inspector.prepareServer();
    return inspector;
}

uint16_t NSJSBase::v8_debug::internal::CInspectorPool::getPort()
{
    static std::atomic<uint16_t> initialPort{8080};
    return initialPort++;
}

int NSJSBase::v8_debug::internal::CInspectorPool::getContextGroupId()
{
    static std::atomic<int> initialId{1};
    return initialId++;
}

NSJSBase::v8_debug::internal::CInspectorImpl&
NSJSBase::v8_debug::internal::CInspectorPool::getInspector(v8::Local<v8::Context> context
                                                           , v8::Platform *platform
                                                           , const std::string &contextName)
{
    std::lock_guard<std::mutex> lock{m_Mutex};
    storage_t::iterator iter = m_Inspectors.find(context);
    if (m_Inspectors.end() == iter) {
        return addInspector(context, platform, contextName);
    }
    return iter->second;
}

void NSJSBase::v8_debug::internal::CInspectorPool::disposeInspector(v8::Local<v8::Context> context)
{
    std::lock_guard<std::mutex> lock{m_Mutex};
    storage_t::iterator iter = m_Inspectors.find(context);
    if (m_Inspectors.end() != iter) {
        m_Inspectors.erase(iter);
    }
}

NSJSBase::v8_debug::internal::CInspectorPool&
NSJSBase::v8_debug::internal::CInspectorPool::get()
{
    static CInspectorPool pool;
    return pool;
}
