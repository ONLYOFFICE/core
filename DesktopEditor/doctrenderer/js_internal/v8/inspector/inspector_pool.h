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

//    v8::Local<v8::Context> m_Context{};
//    v8::Platform *m_pPlatform{nullptr};

    static constexpr bool m_bLog{true};
//    const int m_iContextGroupId;
//    const std::string m_ContextName;


//    storage_t::iterator findInspector(v8::Isolate *isolate);
    CInspectorImpl& addInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName = "");

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
//    void maybeSetV8Data(v8::Local<v8::Context> context, v8::Platform *platform);
//    bool isInitV8Data() const;
    static CInspectorPool& get();

};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORPOOL_H
