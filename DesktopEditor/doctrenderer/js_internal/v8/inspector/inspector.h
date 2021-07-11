#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <stdint.h>
#include <v8.h>
#include "client.h"

namespace NSJSBase {
namespace v8_debug {

//server forward declaration
namespace internal {
    class SingleConnectionServer;
}

class CInspector
{
    //server
    std::unique_ptr<internal::SingleConnectionServer> m_pServer{};

    //to convert v8 string view to string
    v8::Isolate *m_pIsolate{nullptr};

    //logging protocol messages
    bool m_bLog{false};

    //using pointer to limit with forward declaration of smart_ptr
    std::unique_ptr<
    NSCommon::smart_ptr<CJSValue>
    > m_pScriptResult{nullptr};

    //using pointer to initialize client out of constructor
    std::unique_ptr<internal::CInspectorClient> m_pClient{nullptr};



    //
    bool initServer();
    void initClient(
            //for debugging session
            CJSContextPrivate *pContextPrivate
                    //for cdt
                    , const std::string &contextName
                    //script
                    , const std::string &scriptStr
                    //exception catcher
                    , NSCommon::smart_ptr<CJSTryCatch> &pException
                    //script origin
                    , const std::wstring &scriptPath);

    //
    enum class msgType : bool {
        incoming
        , outgoing
    };
    void maybeLog(const std::string &message, msgType type) const;

    //
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

public:
//    CInspector(v8::Local<v8::Context> context
//               , v8::Platform *platform
//               , v8::Local<v8::Script> script
//               , bool log = false
//               , uint16_t port = 8080
//            , std::string contextName = "");
    CInspector(
            //stript executor
            NSJSBase::CJSContextPrivate *pContextPrivate
            //script source
               , const std::string &scriptStr
            //v8::TryCatch wrapper
               , NSCommon::smart_ptr<CJSTryCatch> &pException
            //to cache script
               , const std::wstring &scriptPath
            //to log communication between app and cdt
               , bool log = false
            //
               , uint16_t port = 8080
            //for cdt
               , const std::string &contextName = "");

    NSCommon::smart_ptr<CJSValue> run();

    ~CInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_H
