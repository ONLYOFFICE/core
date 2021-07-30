#ifndef INSPECTOR_IMPL_H
#define INSPECTOR_IMPL_H

//здесь лежат бустовые хедеры, он должен быть включён выше всех
#include "singleconnectionserver.h"//CSingleConnectionServer
#include "client.h"//inspector is what holds client

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CInspectorImpl
{
    //server
    CSingleConnectionServer m_Server;

    //to convert v8 string view to string
    v8::Isolate *m_pIsolate{nullptr};// для viewToStr

    //logging protocol messages
    bool m_bLog{false};

    //сам клиент
    CInspectorClient m_Client;



    //сервер
    bool initServer();
    bool connectServer();
    void waitWhileServerReady();

    //logging and hints
    void maybeLogOutgoing(const std::string &message) const;
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

public:
    //explicitly delete all the stuff
    CInspectorImpl(const CInspectorImpl&) = delete;
    CInspectorImpl(CInspectorImpl&&) = delete;
    CInspectorImpl& operator=(const CInspectorImpl&) = delete;
    CInspectorImpl& operator=(CInspectorImpl&&) = delete;

    //only allowed ctor
    CInspectorImpl(
            //context for client
            v8::Local<v8::Context> context
            //platform to pump
            , v8::Platform *platform
            //port to connect to
            , uint16_t port
            //some stuff for cdt and v8 internals
            , int contextGroupId
            , const std::string &contextName
            //
            , bool log
    );

    //api for inspector client
    void sendData(const v8_inspector::StringView &message);
    bool waitForMessage();
    void onServerReady();

    //necessary to call before using inspector
    void prepareServer();

    //before debugged code
    void beforeLaunch();

    ~CInspectorImpl();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // INSPECTOR_IMPL_H
