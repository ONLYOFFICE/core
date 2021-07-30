#ifndef CINSPECTORPOOL_H
#define CINSPECTORPOOL_H

#include "inspector_impl.h"//inspector itself
#include <map>//to hold inspectors
#include <mutex>//may be used in multithread environment

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

    static constexpr bool m_bLog{false};

    //добавить новый инспектор
    CInspectorImpl& addInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName);

    //порт для сервака, id для v8
    static uint16_t getPort();
    static int getContextGroupId();

    //всё приватное, синглтон по классике
    CInspectorPool();
    ~CInspectorPool();

public:
    CInspectorPool(const CInspectorPool&) = delete;
    CInspectorPool(CInspectorPool&&) = delete;
    CInspectorPool& operator=(const CInspectorPool&) = delete;
    CInspectorPool& operator=(CInspectorPool&&) = delete;

    //инстанс пула
    static CInspectorPool& get();

    //достать инспектор для контекста
    CInspectorImpl& getInspector(v8::Local<v8::Context> context
                                 , v8::Platform *platform
                                 , const std::string &contextName);
    //удалить инспектор для контекста
    void disposeInspector(v8::Local<v8::Context> context);
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORPOOL_H
