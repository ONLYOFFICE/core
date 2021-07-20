#include "inspectormanager.h"
#include "inspector_impl.h"//inspector implementation
#include "threadinspectoramount.h"//get amount of inspectors that already exist in current thread

NSJSBase::v8_debug::internal::CInspectorHolder
NSJSBase::v8_debug::internal::CInspectorManager::m_Holder{};

std::unique_ptr<NSJSBase::v8_debug::internal::CInspectorImpl>
NSJSBase::v8_debug::internal::CInspectorHolder::makeNewInspectorInfo(
        v8::Local<v8::Context> context
        , v8::Platform *platform
        , const CPortHolderId &id
        , bool needToDebug
        )
{
    //mutex should be already locked

    auto //pair of iterator and bool
            insertionResult = m_Inspectors.emplace(
                id
                , CInspectorInfo{
                    //logging
                    false//should be false on prod
                    //port
                    , uint16_t(startPort + m_Inspectors.size())
                    //context group id
                    , int(m_Inspectors.size() + 1)
                    //name
                    , ""
                }
                );
    //get new info
    const CInspectorInfo &newInfo = insertionResult
                                .first//iterator
                                ->second//value
                                ;
    //get inspector
    return std::make_unique<CInspectorImpl>(
                context
                , platform
                , newInfo
                , id.threadId
                , needToDebug
                );
}

std::unique_ptr<NSJSBase::v8_debug::internal::CInspectorImpl>
NSJSBase::v8_debug::internal::CInspectorHolder::getInspector(
        v8::Local<v8::Context> context
        , v8::Platform *platform
        , bool needToDebug
        )
{
    //lock
    std::lock_guard<std::mutex> locker{m_Mutex};
    //get thread id and amount of existing inspectors
    ASC_THREAD_ID threadId = NSThreads::GetCurrentThreadId();
    InspectorCountType count = CThreadInspectorCounter::getCount(threadId);
    CPortHolderId id{threadId, count};
    //check for inspector for current thread
    auto inspectorIter = m_Inspectors.find(id);
    if (m_Inspectors.end() == inspectorIter) {
        //make inspector with new info
        return makeNewInspectorInfo(context
                                    , platform
                                    , id
                                    , needToDebug);
    }
    //make inspector with existing info
    return std::make_unique<CInspectorImpl>(
                context
                , platform
                , inspectorIter->second
                , threadId
                , needToDebug
                );
}

std::unique_ptr<NSJSBase::v8_debug::internal::CInspectorImpl>
NSJSBase::v8_debug::internal::CInspectorManager::getInspector(
        v8::Local<v8::Context> context
        , v8::Platform *platform
        , bool needToDebug
        )
{
    return m_Holder.getInspector(context
                                 , platform
                                 , needToDebug);
}
