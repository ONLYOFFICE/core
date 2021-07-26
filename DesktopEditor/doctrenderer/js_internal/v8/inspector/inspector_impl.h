#ifndef INSPECTOR_IMPL_H
#define INSPECTOR_IMPL_H

#include "singleconnectionserver.h"
#include <stdint.h>//uintxx_t
#include <v8.h>//v8 stuff
#include "client.h"//inspector is what holds client
#include "inspector_info.h"//info for constructing inspector

namespace NSJSBase {
namespace v8_debug {
namespace internal {

//server forward declaration
class CSingleConnectionServer;

class CInspectorImpl
{
    //server
//    std::unique_ptr<CSingleConnectionServer>
    CSingleConnectionServer
    m_pServer
//    {nullptr}
    ;

    CSingleConnectionServer::onResumeCallback m_fOnServerResume{};

    //to convert v8 string view to string
    v8::Isolate *m_pIsolate{nullptr};

    //logging protocol messages
    bool m_bLog{false};

    //using pointer to limit with forward declaration of smart_ptr
    std::unique_ptr<
    NSCommon::smart_ptr<CJSValue>
    > m_pScriptResult{nullptr};

    //
    CInspectorClient m_Client;

    //
//    int m_iContextGroupId;




    //
    bool initServer();
    bool connectServer();
    void waitWhileServerReady();

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
            , uint16_t port
    );

    //api for inspector client
    void sendData(const v8_inspector::StringView &message);
    bool waitForMessage();
    void setRetVal(const NSCommon::smart_ptr<CJSValue> &val);
    void shutServerDown();
    void pauseServer();
    void onServerReady();

    //about server busyness
    bool free() const;
//    int contextGroupId() const;

    //necessary to call before using inspector
    void prepareServer();

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
