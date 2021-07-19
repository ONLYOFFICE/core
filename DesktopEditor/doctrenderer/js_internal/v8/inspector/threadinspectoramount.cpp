#include "threadinspectoramount.h"

NSJSBase::v8_debug::internal::CThreadInspectorAmount
NSJSBase::v8_debug::internal::CThreadInspectorCounter::m_Amounts{};

std::size_t& NSJSBase::v8_debug::internal::CThreadInspectorAmount::addThread(ASC_THREAD_ID id)
{
    //mutex should be locked
    auto result//pair
            = m_AmountsPerThread.emplace(
                id
                , 0//0 by default
                );
    return result
            .first//iterator
            ->second//value
            ;
}

std::size_t& NSJSBase::v8_debug::internal::CThreadInspectorAmount::getCount()
{
    return getCount(NSThreads::GetCurrentThreadId());
}

std::size_t& NSJSBase::v8_debug::internal::CThreadInspectorAmount::getCount(ASC_THREAD_ID id)
{
    std::lock_guard<std::mutex> lock{m_Mutex};
    auto iter = m_AmountsPerThread.find(id);
    if (m_AmountsPerThread.end() == iter) {
        return addThread(id);
    }
    return iter->second;
}

std::size_t& NSJSBase::v8_debug::internal::CThreadInspectorCounter::getCount()
{
    return m_Amounts.getCount();
}

std::size_t& NSJSBase::v8_debug::internal::CThreadInspectorCounter::getCount(ASC_THREAD_ID id)
{
    return m_Amounts.getCount(id);
}

NSJSBase::v8_debug::internal::CCountManager::CCountManager(InspectorCountType &count)
    : m_Count{count}
{
    ++m_Count;
}

NSJSBase::v8_debug::internal::CCountManager::~CCountManager()
{
    --m_Count;
}
