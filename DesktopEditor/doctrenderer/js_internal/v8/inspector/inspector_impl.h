#ifndef INSPECTOR_IMPL_H
#define INSPECTOR_IMPL_H

#include <stdint.h>//uintxx_t
#include <v8.h>//v8 stuff
#include "client.h"//inspector is what holds client
#include "inspectorinfo.h"//info for constructing inspector

namespace NSJSBase {
namespace v8_debug {
namespace internal {

//server forward declaration
class SingleConnectionServer;

class CInspectorImpl
{
    //pointer to keep boost stuff in .cpp file
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
            //
            v8::Local<v8::Context> context
            //for cdt
            , int contextGroupId
            , const std::string &contextName
            //
            , v8::Platform *platform
            );

    void maybeLogOutgoing(const std::string &message) const;

    //
    void printChromeLaunchHint(std::ostream &out
                               , uint16_t port);

    //running stuff
    bool checkServer() const;
    void waitAndRunServer();
    NSCommon::smart_ptr<CJSValue> getReturnValue();

public:
    CInspectorImpl(const CInspectorImpl&) = delete;
    CInspectorImpl(CInspectorImpl&&) = delete;
    CInspectorImpl& operator=(const CInspectorImpl&) = delete;
    CInspectorImpl& operator=(CInspectorImpl&&) = delete;
    CInspectorImpl(
            //stript executor
            v8::Local<v8::Context> context
            //platform to pump
            , v8::Platform *platform
            //
            , CInspectorInfo info
    );

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
