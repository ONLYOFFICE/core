#ifndef INSPECTOR_IMPL_H
#define INSPECTOR_IMPL_H

#include "singleconnectionserver.h"//CSingleConnectionServer; onResumeCallback
#include <stdint.h>//uintxx_t
#include <v8.h>//v8 stuff
#include "client.h"//inspector is what holds client
//#include "inspector_info.h"//info for constructing inspector

namespace NSJSBase {
namespace v8_debug {
namespace internal {

//server forward declaration
class CSingleConnectionServer;

class CInspectorImpl
{
    //server
    CSingleConnectionServer m_Server;
//    CSingleConnectionServer::onResumeCallback m_fOnServerResume{};

    //to convert v8 string view to string
    v8::Isolate *m_pIsolate{nullptr};// для viewToStr
    v8::Local<v8::Context> m_Context{};//для getJsonProperty

    //logging protocol messages
    bool m_bLog{false};

    //using pointer to limit with forward declaration of smart_ptr
//    std::unique_ptr<
//    NSCommon::smart_ptr<CJSValue>
//    > m_pScriptResult{nullptr};

    //
    CInspectorClient m_Client;




    //
    bool initServer();
    bool connectServer();
    void waitWhileServerReady();

    //
//    void processIncomingMessage(const std::string &message);

    //logging and hints
    void maybeLogOutgoing(const std::string &message) const;
    //
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

    void beforeLaunch();

    std::string getFunctionName(const std::string &json);

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
            , uint16_t port
            //
            , int contextGroupId
            , const std::string &contextName
            , bool log
    );

    //api for inspector client
    void sendData(const v8_inspector::StringView &message);
    bool waitForMessage();
//    void setRetVal(const NSCommon::smart_ptr<CJSValue> &val);
//    void pauseServer();
    void onServerReady();

    //necessary to call before using inspector
    void prepareServer();

    //
    void beforeScript();
    void beforeFunc();

    //running api
//    NSCommon::smart_ptr<CJSValue> runScript(
//            const CScriptExecData &execData
//            );
//    NSCommon::smart_ptr<CJSValue> callFunc(
//            const CFCallData &callData
//            );

    ~CInspectorImpl();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_IMPL_H
