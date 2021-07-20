#ifndef CINSPECTORMANAGER_H
#define CINSPECTORMANAGER_H

#include <map>//std::map
#include <mutex>//std::mutex
#include "port_holder_id.h"//GetCurrentThreadId, CPortHolderId
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
    //might use unordered map, but it's easier to implement compare than hash for port holder id class
    std::map<CPortHolderId, CInspectorInfo, CPHIdLess> m_Inspectors{};
    static constexpr uint16_t startPort = 8080;

    std::unique_ptr<CInspectorImpl> makeNewInspectorInfo(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            , const CPortHolderId &id
            , bool needToDebug
            );
public:
    std::unique_ptr<CInspectorImpl> getInspector(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            , bool needToDebug
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
            , bool needToDebug
            );
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORMANAGER_H
