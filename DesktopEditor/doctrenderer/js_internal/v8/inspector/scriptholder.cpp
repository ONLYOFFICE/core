#include "scriptholder.h"
#include "../v8_base.h"//compileScript

//static per-thread script holder
NSJSBase::v8_debug::internal::CPerThreadScriptHolder
NSJSBase::v8_debug::internal::CScriptHoldersManager::m_Holder{};

//holds scripts separately
namespace {
    class CScriptHolderSeparated : public NSJSBase::v8_debug::internal::CScriptHolder {
        std::vector<std::string> m_Scripts{};

    public:
        virtual void compileScripts(v8::Local<v8::Context> context) const override;
        virtual void addScript(const std::string &script) override;
        virtual ~CScriptHolderSeparated() = default;
    };

    void CScriptHolderSeparated::compileScripts(
            v8::Local<v8::Context> context) const
    {
        for (const auto &str: m_Scripts) {
            //omit path, because performance in not necessary while debugging
            NSJSBase::compileScript(context, str, L"");
        }
    }

    void CScriptHolderSeparated::addScript(const std::string &script)
    {
        m_Scripts.push_back(script);
    }
}//namespace anonymous

//holds scripts as one joint
namespace {
    class CScriptHolderJoint : public NSJSBase::v8_debug::internal::CScriptHolder {
        std::string m_Scripts{};

    public:
        virtual void compileScripts(v8::Local<v8::Context> context) const override;
        virtual void addScript(const std::string &script) override;
        virtual ~CScriptHolderJoint() = default;
    };

    void CScriptHolderJoint::compileScripts(
            v8::Local<v8::Context> context) const
    {
        if (m_Scripts.empty()) {
            return;
        }
        //omit path, because performance in not necessary while debugging
        NSJSBase::compileScript(context, m_Scripts, L"");
    }

    void CScriptHolderJoint::addScript(const std::string &script)
    {
        m_Scripts.reserve(m_Scripts.length()
                          + script.length()//for script
                          + 1//for newline
                          );
        m_Scripts += '\n';
        m_Scripts += script;
    }
}//namespace anonymous

//different holders
std::unique_ptr<NSJSBase::v8_debug::internal::CScriptHolder>
NSJSBase::v8_debug::internal::CScriptHolder::getSeparated()
{
    return std::make_unique<CScriptHolderSeparated>();
}
std::unique_ptr<NSJSBase::v8_debug::internal::CScriptHolder>
NSJSBase::v8_debug::internal::CScriptHolder::getJoint()
{
    return std::make_unique<CScriptHolderJoint>();
}
std::unique_ptr<NSJSBase::v8_debug::internal::CScriptHolder>
NSJSBase::v8_debug::internal::CScriptHolder::getDoingNothing()
{
    return std::make_unique<CScriptHolder>();
}

NSJSBase::v8_debug::internal::CScriptHolder *
NSJSBase::v8_debug::internal::CPerThreadScriptHolder::addHolder(ASC_THREAD_ID threadId)
{
    //mutex must be locker
    auto insertionResult = m_Holders.emplace(
                threadId
                , CScriptHolder::

                //different types
//                getSeparated
                getDoingNothing
//                getJoint
                ()

                );
    return insertionResult//pair
            .first//iterator
            ->second//unique_ptr
            .get()//raw pointer
            ;
}

NSJSBase::v8_debug::internal::CScriptHolder *
NSJSBase::v8_debug::internal::CPerThreadScriptHolder::getHolder()
{
    ASC_THREAD_ID threadId = NSThreads::GetCurrentThreadId();
    auto iter = m_Holders.find(threadId);
    if (m_Holders.end() == iter) {
        return addHolder(threadId);
    }
    return iter//iterator
            ->second//unique_ptr
            .get()//raw pointer
            ;
}

NSJSBase::v8_debug::internal::CScriptHolder *
NSJSBase::v8_debug::internal::CScriptHoldersManager::getHolder()
{
    return m_Holder.getHolder();
}
