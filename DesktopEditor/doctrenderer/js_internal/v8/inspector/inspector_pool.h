#ifndef CINSPECTORPOOL_H
#define CINSPECTORPOOL_H

#include "inspector_impl.h"
#include <map>
#include <mutex>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CInspectorPool
{
    struct ContextLess
    {
        inline bool operator()(const v8::Local<v8::Context> &lhs, const v8::Local<v8::Context> &rhs) const {
            if (lhs == rhs) {
                return false;
            }
            return std::less<v8::Context*>{}(*lhs, *rhs);
        }
    };
    using storage_t = std::map<v8::Local<v8::Context>, CInspectorImpl, ContextLess>;

    std::mutex m_Mutex{};
    storage_t m_Inspectors{};

    static constexpr bool m_bLog{true};

    CInspectorImpl& addInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName);

    static uint16_t getPort();
    static int getContextGroupId();

    CInspectorPool();
    ~CInspectorPool();

public:
    CInspectorPool(const CInspectorPool&) = delete;
    CInspectorPool(CInspectorPool&&) = delete;
    CInspectorPool& operator=(const CInspectorPool&) = delete;
    CInspectorPool& operator=(CInspectorPool&&) = delete;

    static CInspectorPool& get();

    CInspectorImpl& getInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName);
    void disposeInspector(v8::Local<v8::Context> context);
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORPOOL_H
