#ifndef CTHREADINSPECTORAMOUNT_H
#define CTHREADINSPECTORAMOUNT_H

#include "port_holder_id.h"//InspectorCountType, thread_id.h
#include <unordered_map>//std::unordered_map
#include <mutex>//std::mutex

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CThreadInspectorAmount
{
    std::mutex m_Mutex{};
    std::unordered_map<ASC_THREAD_ID
    , InspectorCountType//no need in atomic 'cause each value is accessed only by one thread
    > m_AmountsPerThread{};

    std::size_t& addThread(ASC_THREAD_ID id);
public:
    std::size_t& getCount();
    std::size_t& getCount(ASC_THREAD_ID id);
};

class CThreadInspectorCounter {
    static CThreadInspectorAmount m_Amounts;
public:
    static std::size_t& getCount();
    static std::size_t& getCount(ASC_THREAD_ID id);
};

class CCountManager {
    InspectorCountType &m_Count;
public:
    CCountManager() = delete;
    CCountManager(const CCountManager&) = delete;
    CCountManager(CCountManager&&) = delete;
    CCountManager& operator=(const CCountManager&) = delete;
    CCountManager& operator=(CCountManager&&) = delete;

    CCountManager(InspectorCountType &count);
    ~CCountManager();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CTHREADINSPECTORAMOUNT_H
