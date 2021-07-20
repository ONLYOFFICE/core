#ifndef CLIENT_H
#define CLIENT_H

#include <v8-inspector.h>//v8_inspector stuff
#include <functional>//std::function for callbacks
#include <atomic>//std::atomic for flags
#include "channel.h"//CChannel for debug session
#include "execution_data.h"//data for execution of script and function

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
namespace internal {

//class that is intended to synchronously consume frontend messages on pause
//it also sets up the debugging session
class CInspectorClient : public v8_inspector::V8InspectorClient
{
public:
    using waitMessageCallback = std::function<bool(void)>;
    using setRetValCallback = std::function<void(const NSCommon::smart_ptr<CJSValue>&)>;

private:
    //notable cdt messages
    static constexpr char debugStartMarker[32] = "Runtime.runIfWaitingForDebugger";

    //v8 stuff
    v8::Local<v8::Context> m_Context{};//to register context in inspector
    v8::Isolate *m_pIsolate = nullptr;//to create inspector
    v8::Platform *m_pPlatform = nullptr;//to pump it

    //script execution data
    enum class mode : bool {
        kScriptExec
        , kFuncCall
    };
    mode m_Mode{mode::kScriptExec};//need to execute script before call to any function
    CScriptExecData m_ScriptExecData{};
    CFCallData m_FunctionCallData{};

    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector{nullptr};
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel{nullptr};
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession{nullptr};

    //message loop flag
    std::atomic<bool> m_bPause{false};
    //message loop callback
    waitMessageCallback m_WaitForFrontendMessage{};

    //setting return value of the script to the inspector
    setRetValCallback m_SetRetVal{};

    //log
    bool m_bLog{false};



    //sets up a debugging session
    void setUpDebuggingSession(const std::string &contextName
                               , int contextGroupId
                               , CInspectorChannel::sendDataCallback sendDataCallback);
    //pump platform on pause
    void pumpPlatform();
    //check json for Runtime.runIfWaitingForDebugger method
    //that designates start of debugging session
    bool checkForStartDebugging(const std::string &json) const;
    //log incoming message
    void maybeLogIncoming(const std::string &message) const;
    //schedule pause on next statement
    void pauseOnNextStatement();
    //start debugging itself
    void startDebugging();
    //
    void dispatchProtocolMessage(const std::string &message);

public:
    CInspectorClient(
            //
            v8::Local<v8::Context> context
            //for cdt
            , const std::string &contextName
            //somewhat
            , int contextGroupId
            //to pump it
            , v8::Platform *platform
            //for channel
            , CInspectorChannel::sendDataCallback sendDataFunc
            //to synchronously consume incoming messages
            , waitMessageCallback waitIncomingMessage
            //to set script result to inspector
            , setRetValCallback setRetVal
            //log
            , bool log
            );

    //wait for incoming message
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    //quit waiting
    virtual void quitMessageLoopOnPause() override;


    //dispatch message by session and check it for Runtime.runIfWaitingForDebugger
    void processMessageFromFrontend(const std::string &message);

    //set data for execution of script or function
    void setScriptExecData(const CScriptExecData &data);
    void setFuncCallData(const CFCallData &data);


    ~CInspectorClient();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CLIENT_H
