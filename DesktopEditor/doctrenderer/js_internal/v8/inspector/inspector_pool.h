#ifndef CINSPECTORPOOL_H
#define CINSPECTORPOOL_H

#include "inspector_impl.h"
#include <map>
#include <v8.h>
#include <mutex>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CInspectorPool
{
    //
    using storage_t = std::map<v8::Isolate*
    , CInspectorImpl
    >;
    storage_t m_Inspectors{};
    std::mutex m_Mutex{};

    static constexpr bool m_bLog{true};

    CInspectorImpl& addInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName);

    CInspectorPool();
    ~CInspectorPool();

public:
    CInspectorPool(const CInspectorPool&) = delete;
    CInspectorPool(CInspectorPool&&) = delete;
    CInspectorPool& operator=(const CInspectorPool&) = delete;
    CInspectorPool& operator=(CInspectorPool&&) = delete;

    CInspectorImpl& getInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName = "");
    void dispose();
    static CInspectorPool& get();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORPOOL_H
