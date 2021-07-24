#ifndef CPERCONTEXTSERVERMANAGER_H
#define CPERCONTEXTSERVERMANAGER_H

#include <vector>//std::vector
#include "serverholder.h"//CServerHolder
#include <v8.h>//v8 stuff

//forward declarations
namespace NSJSBase {
    class CJSTryCatch;
    class CJSValue;
}
namespace NSCommon {
    template <typename T>
    class smart_ptr;
}

namespace NSJSBase {
namespace v8_debug {

class CPerContextInspector
{
    using storage_t = std::vector<
    std::unique_ptr<
    internal::CServerHolder
    >
    >;
    storage_t m_Holders{};
    v8::Local<v8::Context> m_Context{};
    v8::Platform *m_pPlatform{nullptr};

    //getting servers helpers
    storage_t::iterator findFreeHolder();
    internal::CServerHolder* addHolder();
    internal::CServerHolder* getFreeHolder();

    //getting server data for inspector
    internal::CServerHolder::CUseData getServer();

public:
    CPerContextInspector(
            v8::Local<v8::Context> context
            , v8::Platform *platform
            );

    NSCommon::smart_ptr<CJSValue> runScript(
                const std::string &scriptStr
                , NSCommon::smart_ptr<CJSTryCatch> &pException
                , const std::wstring &scriptPath
                );
    NSCommon::smart_ptr<CJSValue> callFunc(
                v8::Local<v8::Object> value
                , const char *name
                , int argc
                , NSCommon::smart_ptr<CJSValue> argv[]
                );

    ~CPerContextInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // CPERCONTEXTSERVERMANAGER_H
