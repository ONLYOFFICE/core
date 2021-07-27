#ifndef CLIENT_H
#define CLIENT_H

#include <v8-inspector.h>//v8_inspector stuff
#include <functional>//std::function for callbacks
#include <atomic>//std::atomic for flags
#include "channel.h"//CChannel for debug session
//#include "execution_data.h"//data for execution of script and function


namespace NSJSBase {
namespace v8_debug {
namespace internal {

//inspector holds client, but instead of using callbacks client directly accesses inspector
//it's safe due to inspector and client living in the same thread
//and inspector managing lifetime of client, but not vice versa
class CInspectorImpl;

//class that is intended to synchronously consume frontend messages on pause
//it also sets up the debugging session
class CInspectorClient : public v8_inspector::V8InspectorClient
{
public:
    //notable cdt messages
    static constexpr char readyMessage[32] = "Runtime.runIfWaitingForDebugger";
    static constexpr char scriptResumeFlag_1[25] = "Debugger.getScriptSource";
    static constexpr char funcResumeFlag_1[16] = "Debugger.paused";

    static constexpr char funcResumeFlag_2[22] = "Runtime.getProperties";
    static constexpr char funcResumeFlag_3[35] = "Overlay.setPausedInDebuggerMessage";

private:
    //v8 stuff
    v8::Local<v8::Context> m_CurrentContext{};//to register context in inspector
    v8::Isolate *m_pIsolate = nullptr;//to create inspector
    v8::Platform *m_pPlatform = nullptr;//to pump it

    //state
//    enum class mode : bool {
//        kScriptExec
//        , kFuncCall
//    };
//    enum class debugState : int {
//        kServerNotReady
//        , kDebugPaused
//        , kSessionResumed
//    };
//    mode m_Mode{mode::kScriptExec};//need to execute script before call to any function
//    debugState m_State{debugState::kServerNotReady};//
    enum class state : char {
        kServerUnready
        , kScript
        , kFunc
    } m_State{state::kServerUnready};

    bool autoResume = true;

    //execution data
//    CScriptExecData m_ScriptExecData{};
//    CFCallData m_FunctionCallData{};

    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector{nullptr};
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel{nullptr};
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession{nullptr};

    //message loop flag
    std::atomic<bool> m_bPause{false};

    //to interact with superior class which holds all the stuff, including client
    CInspectorImpl *m_pInspectingWrapper{nullptr};

    //log
    bool m_bLog{false};

    const int m_iContextGroupId;


    //sets up a debugging session
    void setUpDebuggingSession(const std::string &contextName
//                               , CInspectorChannel::sendDataCallback sendDataCallback
                               );
    //pump platform on pause
    void pumpPlatform();
    //log incoming message
    void maybeLogIncoming(const std::string &message) const;
    //



    //
    void dispatchProtocolMessage(const std::string &message);


    //
    void checkFrontendMessage(const std::string &message);

    //
    void onServerReady();
    void onResume();


public:
    CInspectorClient() = delete;
    CInspectorClient(const CInspectorClient&) = delete;
    CInspectorClient(CInspectorClient&&) = delete;
    CInspectorClient& operator=(const CInspectorClient&) = delete;
    CInspectorClient& operator=(CInspectorClient&&) = delete;

    CInspectorClient(
            v8::Local<v8::Context> context,
            //for cdt
            const std::string &contextName
            //somewhat
            , int contextGroupId
            //to pump it
            , v8::Platform *platform
            //to interact with inspector, which holds client
            , CInspectorImpl *inspector
            //
            , bool log
            );

    //wait for incoming message
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    //quit waiting
    virtual void quitMessageLoopOnPause() override;

    //dispatch message by session
    void processFrontendMessage(const std::string &message);

    //schedule pause on next statement
    void pauseOnNextStatement();
    void resumeDebuggingSession();

    //
    void setFunc();
    void setScript();

    void setAutoResume(bool how);

    ~CInspectorClient();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CLIENT_H
