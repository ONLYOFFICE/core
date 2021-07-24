#ifndef INSPECTOR_IMPL_H
#define INSPECTOR_IMPL_H

#include <stdint.h>//uintxx_t
#include <v8.h>//v8 stuff
#include "client.h"//inspector is what holds client
#include "inspector_info.h"//info for constructing inspector
#include "serverholder.h"

namespace NSJSBase {
namespace v8_debug {
namespace internal {

//server forward declaration
class CSingleConnectionServer;

class CInspectorImpl
{
    //server
    CSingleConnectionServer *m_pServer{nullptr};

    //to convert v8 string view to string
    v8::Isolate *m_pIsolate{nullptr};

    //logging protocol messages
    bool m_bLog{false};

    //using pointer to limit with forward declaration of smart_ptr
    std::unique_ptr<
    NSCommon::smart_ptr<CJSValue>
    > m_pScriptResult{nullptr};

    //using pointer to initialize client out of constructor
//    std::unique_ptr<CInspectorClient>
    CInspectorClient
    m_Client;

    //to set that the server is in use
    CServerHolder::shared_flag_t &m_bServerInUse;
    CServerHolder::shared_flag_t &m_bServerReady;





    //initialize methods
    void initServer();

    //
    void processIncomingMessage(const std::string &message);

    //logging and hints
    void maybeLogOutgoing(const std::string &message) const;
    //
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

    //server and run stuff
    bool checkServer() const;
    NSCommon::smart_ptr<CJSValue> getReturnValue();

public:
    //explicitly delete all the stuff
    CInspectorImpl(const CInspectorImpl&) = delete;
    CInspectorImpl(CInspectorImpl&&) = delete;
    CInspectorImpl& operator=(const CInspectorImpl&) = delete;
    CInspectorImpl& operator=(CInspectorImpl&&) = delete;

    //only allowed ctor
    CInspectorImpl(
            //stript executor
            v8::Local<v8::Context> context
            //platform to pump
            , v8::Platform *platform
            //
            , CInspectorInfo info
            //
            , CServerHolder::CUseData useData
    );

    //api for inspector client
    void sendData(const v8_inspector::StringView &message);
    bool waitForMessage();
    void setRetVal(const NSCommon::smart_ptr<CJSValue> &val);
    void shutServerDown();
    void pauseServer();

    //running api
    NSCommon::smart_ptr<CJSValue> runScript(
            const CScriptExecData &execData
            );
    NSCommon::smart_ptr<CJSValue> callFunc(
            const CFCallData &callData
            );

    ~CInspectorImpl();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_IMPL_H
