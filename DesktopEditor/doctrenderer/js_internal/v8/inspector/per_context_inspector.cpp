//без этого считает, что CServerHolder неполный класс
#include "singleconnectionserver.h"//бустовый хедер должен лежать выше всех
#include "per_context_inspector.h"
#include "portdistributor.h"//CPortDistributor for getting free ports
#include "../v8_base.h"//v8 wrappers
#include "inspector_impl.h"//inspector itself

using namespace NSJSBase::v8_debug::internal;

NSJSBase::v8_debug::CPerContextInspector::storage_t::iterator
NSJSBase::v8_debug::CPerContextInspector::findFreeHolder()
{
    for (auto i = m_Holders.begin(); i != m_Holders.end(); ++i) {
        //если есть свободный сервак, кидаем его
        if (i->get()->free()) {
            return i;
        }
    }
    return m_Holders.end();
}

NSJSBase::v8_debug::internal::CServerHolder* NSJSBase::v8_debug::CPerContextInspector::addHolder()
{
    m_Holders.push_back(
                std::make_unique<CServerHolder>(
                    CPortDistributor::getPort()
                    )
                );
    return m_Holders.back().get();
}

NSJSBase::v8_debug::internal::CServerHolder* NSJSBase::v8_debug::CPerContextInspector::getFreeHolder()
{
    auto iter = findFreeHolder();
    if (m_Holders.end() == iter) {
        return addHolder();
    }
    return iter->get();
}

NSJSBase::v8_debug::CPerContextInspector::CPerContextInspector(
        v8::Local<v8::Context> context
        , v8::Platform *platform
        )
    : m_Context{context}
    , m_pPlatform{platform}
{
    //
}

NSJSBase::v8_debug::CPerContextInspector::~CPerContextInspector() = default;

NSJSBase::v8_debug::internal::CServerHolder::CUseData NSJSBase::v8_debug::CPerContextInspector::getServer()
{
    return getFreeHolder()->getServer();
}


NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CPerContextInspector::runScript(
        const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        )
{
    CInspectorImpl inspector{
                m_Context
                , m_pPlatform
                //debug info
                , CInspectorInfo{
                    //log
                    true
                    //context id
                    , CInspectorInfo::getContextId()
                    //context name
                    , ""
                }
                //server data
                , this->getServer()};
    std::cout << "BEFORE RUN SCRIPT\n";
    return inspector.runScript(
                CScriptExecData{
                    scriptStr
                    , pException
                    , scriptPath
                }
                );
}

NSCommon::smart_ptr<NSJSBase::CJSValue>
NSJSBase::v8_debug::CPerContextInspector::callFunc(
        v8::Local<v8::Object> value
        , const char *name
        , int argc
        , NSCommon::smart_ptr<CJSValue> argv[])
{
    CInspectorImpl inspector{
                m_Context
                , m_pPlatform
                //debug info
                , CInspectorInfo{
                    //log
                    true
                    //context id
                    , CInspectorInfo::getContextId()
                    //context name
                    , ""
                }
                //server data
                , this->getServer()};
    return inspector.callFunc(
                CFCallData{
                    value
                    , name
                    , argc
                    , argv
                }
                );
}
