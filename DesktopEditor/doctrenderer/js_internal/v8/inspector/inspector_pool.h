#ifndef CINSPECTORPOOL_H
#define CINSPECTORPOOL_H

#include "inspector_impl.h"
#include <map>
#include <v8.h>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CInspectorPool
{
    //using map for piecewise construct
    using storage_t = std::map<uint16_t, CInspectorImpl>;
    storage_t m_Inspectors{};

    v8::Local<v8::Context> m_Context{};
    v8::Platform *m_pPlatform{nullptr};

    static constexpr bool m_bLog{true};
    const int m_iContextGroupId;
    const std::string m_ContextName;


    storage_t::iterator findFreeInspector();
    CInspectorImpl& addInspector();

public:
    CInspectorPool(const std::string &contextName);
    CInspectorImpl& getInspector();
    void maybeSetV8Data(v8::Local<v8::Context> context, v8::Platform *platform);
    bool isInitV8Data() const;
    ~CInspectorPool();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORPOOL_H
