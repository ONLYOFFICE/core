#ifndef CLIENT_H
#define CLIENT_H

#include <v8-inspector.h>//v8_inspector stuff
#include "channel.h"//CChannel for debug session

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
    //notable cdt messages
    static constexpr char serverReadyMessage[32] {"Runtime.runIfWaitingForDebugger"};

    static constexpr char scriptResumeMessage[25] {"Debugger.getScriptSource"};

    static constexpr char funcResumeMessageEarly[35] {"Overlay.setPausedInDebuggerMessage"};
    static constexpr char funcResumeMessageLate[22] {"Runtime.getProperties"};

    static constexpr char activateBreakpointsMethod[30] {"Debugger.setBreakpointsActive"};
    static constexpr char setBreakpointMethod[23] {"Debugger.setBreakpoint"};
    static constexpr char removeBreakpointMethod[26] {"Debugger.removeBreakpoint"};
    static constexpr char getPossibleBreakpointsMethod[32] {"Debugger.getPossibleBreakpoints"};

    //v8 stuff
    v8::Local<v8::Context> m_Context{};//to register context in inspector
    v8::Isolate *m_pIsolate{nullptr};//to create inspector
    v8::Platform *m_pPlatform{nullptr};//to pump platform on pause

    //debug state
    bool m_bServerReady{false};
    bool m_bMySessionPause{false};

    //debug session data
    std::unique_ptr<v8_inspector::V8Inspector> m_pInspector{nullptr};
    std::unique_ptr<v8_inspector::V8Inspector::Channel> m_pChannel{nullptr};
    std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession{nullptr};

    //сообщения об изменении брейкпойнтов могут приходить в любом порядке
    //поэтому вместо конечного автомата просто флаги
    bool m_bBreakpointAdded{false};
    bool m_bBreakpointAlterMethodCame{false};
    bool m_bResumeOnBreakpointCame{false};

    //message loop flag
    bool m_bPause{false};

    //to interact with superior class which holds all the stuff, including client
    CInspectorImpl *m_pInspectingWrapper{nullptr};

    //log
    bool m_bLog{false};

    //idk what is intended for
    const int m_iContextGroupId;


    //sets up a debugging session
    void setUpDebuggingSession(const std::string &contextName);

    //pumps platform on pause
    void pumpPlatform();

    //logs incoming message
    void maybeLogIncoming(const std::string &message) const;

    //client's duty to send protocol messages to v8 internals
    void dispatchProtocolMessage(const std::string &message);

    //check for pause flags
    void checkFrontendMessage(const std::string &message);

    //
    void resumeDebuggingSession();

    //
    bool isBreakpointAlterMethod(const std::string &method);
    bool isActiveTrue(const std::string &message);

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

    //api for v8 internals
    virtual void runMessageLoopOnPause(int contextGroupId) override;
    virtual void quitMessageLoopOnPause() override;

    //dispatch and check message
    void processFrontendMessage(const std::string &message);

    //api for inspector: schedule pause on next statement
    void pauseOnNextStatement();

    ~CInspectorClient();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CLIENT_H
