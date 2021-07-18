#ifndef CINSPECTORMANAGER_H
#define CINSPECTORMANAGER_H

#include <unordered_map>//std::unordered_map
#include <mutex>//std::mutex
#include "../../../../../../DesktopEditor/graphics/BaseThread.h"//GetCurrentThreadId
#include "inspectorinfo.h"//CInspectorInfo
#include <v8.h>//v8 stuff

namespace NSJSBase {
namespace v8_debug {
namespace internal {

//inspector impl forward declaration
class CInspectorImpl;

//one inspector per thread
class CInspectorHolder {
    std::mutex m_Mutex{};
    std::unordered_map<ASC_THREAD_ID, CInspectorInfo> m_Inspectors{};
    static constexpr uint16_t startPort = 8080;

    std::unique_ptr<CInspectorImpl> makeNewInspectorInfo(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            , ASC_THREAD_ID threadId
            );
public:
    std::unique_ptr<CInspectorImpl> getInspector(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            );
};

//wrapper for static holder
class CInspectorManager
{
    static CInspectorHolder m_Holder;
public:
    static std::unique_ptr<CInspectorImpl> getInspector(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            );
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORMANAGER_H
