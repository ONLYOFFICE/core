#ifndef ISCRIPTHOLDER_H
#define ISCRIPTHOLDER_H

#include <v8.h>//v8::Context
#include "../../../../../../DesktopEditor/graphics/BaseThread.h"//GetCurrentThreadId
#include <mutex>//
#include <unordered_map>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CScriptHolder
{
public:
    virtual void compileScripts(v8::Local<v8::Context>) const {};
    virtual void addScript(const std::string &) {};
    virtual ~CScriptHolder() = default;

    static std::unique_ptr<CScriptHolder> getDoingNothing();
    static std::unique_ptr<CScriptHolder> getSeparated();
    static std::unique_ptr<CScriptHolder> getJoint();
};

class CPerThreadScriptHolder {
    std::mutex m_Mutex{};
    std::unordered_map<
    ASC_THREAD_ID
    , std::unique_ptr<CScriptHolder>
    > m_Holders{};

    CScriptHolder* addHolder(ASC_THREAD_ID threadId);
public:
    CScriptHolder* getHolder();
};

class CScriptHoldersManager {
    static CPerThreadScriptHolder m_Holder;
public:
    static CScriptHolder* getHolder();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // ISCRIPTHOLDER_H
