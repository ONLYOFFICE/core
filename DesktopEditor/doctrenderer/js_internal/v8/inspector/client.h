#ifndef CLIENT_H
#define CLIENT_H

#include <v8-inspector.h>
#include <functional>
#include <atomic>
#include "channel.h"

//forward declarations
namespace NSJSBase {
    class CJSContextPrivate;
    class CJSTryCatch;
    class CJSValue;
}
namespace NSCommon {
    template <typename T>
    class smart_ptr;
}


namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CInspectorClient : public v8_inspector::V8InspectorClient
{
public:
    using waitMessageCallback = std::function<bool(void)>;
    using setScriptRetValCallback = std::function<void(const NSCommon::smart_ptr<CJSValue>&)>;

private:
    const std::string debugStartMarker = "Runtime.runIfWaitingForDebugger";

    //v8 stuff
    v8::Local<v8::Context> m_Context{};//to register context in inspector
    v8::Isolate *m_pIsolate = nullptr;//to create inspector
    v8::Platform *m_pPlatform = nullptr;//to pump it

    //script execution data
    std::string m_sScriptStr{};
    NSCommon::smart_ptr<CJSTryCatch> &m_pException;
    std::wstring m_sScriptPath{};

    //script executor
    CJSContextPrivate *m_pContextPrivate = nullptr;

    //context data
    std::string m_sContextName{};
    const int m_iContextGroupId = 1;

    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector = nullptr;
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel = nullptr;
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession = nullptr;

    //message loop flag
    std::atomic<bool> m_bPause{false};
    //message loop callback
    waitMessageCallback m_WaitForFrontendMessage{};

    //setting return value of the script to the inspector
    setScriptRetValCallback m_SetRetVal{};




    //sets up a debugging session
    void setUpDebuggingSession(CInspectorChannel::sendDataCallback sendDataCallback);
    //
    void pumpPlatform();
    //
    bool checkForStartDebugging(const std::string &json);

public:
//    CInspectorClient(
//            v8::Local<v8::Context> context//for some stuff
//            , const std::string &contextName//why not
//            , v8::Platform *platform
//            , v8::Local<v8::Script> script
//            , CInspectorChannel::sendDataCallback sendDataFunc//for channel
//            , waitMessageCallback waitIncomingMessage//to synchronously consume incoming messages
//            , setScriptRetValCallback setScriptRetVal//to set script result to inspector
//            );
    CInspectorClient(
            //script executor
            CJSContextPrivate *pContextPrivate
            //for cdt
            , const std::string &contextName
            //script
            , const std::string &scriptStr
            //exception catcher
            , NSCommon::smart_ptr<CJSTryCatch> &pException
            //script origin
            , const std::wstring &scriptPath
            , CInspectorChannel::sendDataCallback sendDataFunc//for channel
            , waitMessageCallback waitIncomingMessage//to synchronously consume incoming messages
            , setScriptRetValCallback setScriptRetVal//to set script result to inspector
            );

    //wait for incoming message
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    //quit waiting
    virtual void quitMessageLoopOnPause() override;

    //start debugging
    void startDebugging();
    //
    void processMessageFromFrontend(const std::string &message);


    ~CInspectorClient();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CLIENT_H
