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

    //v8 stuff
    v8::Isolate *m_pIsolate{nullptr};
    //stored on client
//    v8::Local<v8::Context> m_Context{};
//    v8::Platform *m_pPlatform = nullptr;
//    std::string m_sContextName{};

    //stored on client
//    v8::Local<v8::Script> m_Script;

    //logging protocol messages
    bool m_bLog{false};

    //
    std::unique_ptr<
    NSCommon::smart_ptr<CJSValue>
    > m_pScriptResult{nullptr};

    //
//    v8::MaybeLocal<v8::Value> m_ScriptReturnValue{};


    //inspector client
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
    void processIncomingMessage(const std::string &message);

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
    CInspector(NSJSBase::CJSContextPrivate *pContextPrivate
               , const std::string &scriptStr
               , NSCommon::smart_ptr<CJSTryCatch> &pException
               , const std::wstring &scriptPath
               , bool log = false
               , uint16_t port = 8080
               , const std::string &contextName = "");

    NSCommon::smart_ptr<CJSValue> run();

    ~CInspector();
};

}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_H
