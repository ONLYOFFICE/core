#include "client.h"
#include "channel.h"//client holds channel
#include "singlethreadutils.h"//converting strings
#include <libplatform/libplatform.h>//v8::Platform
#include "../../v8/v8_base.h"//runScript and callFunc impls
#include "inspector_impl.h"//to interact with inspector

namespace {
    //use them in loop
    class FalseSetter {
        std::atomic<bool> &m_bool;
    public:
        FalseSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = false;}
        ~FalseSetter() {m_bool = true;}
    };
    class TrueSetter {
        std::atomic<bool> &m_bool;
    public:
        TrueSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = true;}
        ~TrueSetter() {m_bool = false;}
    };
}

void NSJSBase::v8_debug::internal::CInspectorClient::setUpDebuggingSession(
        const std::string &contextName
        , int contextGroupId
        , CInspectorChannel::sendDataCallback sendDataCallback)
{
    //client instance is this

    //channel
    m_pChannel = std::make_unique<CInspectorChannel>(
                std::move(sendDataCallback)
                );

    //inspector
    m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);

    //state for session
    v8_inspector::StringView state{};

    //session
    m_pSession = m_pInspector->connect(contextGroupId, m_pChannel.get(), state);

    //context name as string view
    v8_inspector::StringView viewContextName = strToView(contextName);

    //context info
    v8_inspector::V8ContextInfo info{
                m_Context
                , contextGroupId
                , viewContextName
    };

    //register context objects in inspector
    m_pInspector->contextCreated(info);
}

void NSJSBase::v8_debug::internal::CInspectorClient::pumpPlatform()
{
    while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) {
        //just pump until pumped
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::runMessageLoopOnPause(
        int contextGroupId
        ) {
    if (m_bPause) {
        return;
    }

    TrueSetter f(m_bPause);

    while (m_bPause) {
        //while not paused by cdt or cdt disconnected
        if (
                !m_pInspectingWrapper->waitForMessage()
                ) {
            break;
        }
        //
        pumpPlatform();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::quitMessageLoopOnPause() {
    m_bPause = false;
}


void NSJSBase::v8_debug::internal::CInspectorClient::startDebugging()
{
//    std::string s1 = "function function_1() { return 1 + 2; }";
//    std::string s2 = "function function_2() { return 100 + function_1(); }";
//    v8::Script::Compile(m_Context, tov8str(m_pIsolate, s1)).ToLocalChecked()->Run(m_Context);
//    v8::Script::Compile(m_Context, tov8str(m_pIsolate, s2)).ToLocalChecked()->Run(m_Context);
    //pause before current script on debugging launch
    pauseOnNextStatement();

    //prepare result
    JSSmart<CJSValue> result;

    //tmp
    struct kkk {
        JSSmart<CJSValue> &r;
        CInspectorImpl *p;
        kkk(JSSmart<CJSValue> &kk, CInspectorImpl *pp) : r(kk), p(pp) {}
        ~kkk() {
//            p->setRetVal(r);
        }
    } iiiiiiiiiiiiiii(result, m_pInspectingWrapper);

    switch (m_Mode) {
    case mode::kScriptExec: {
        //run script
        m_pInspectingWrapper->setRetVal(
        NSJSBase::runScriptImpl(
                    m_Context
                    , m_ScriptExecData.scriptSource
                    , *m_ScriptExecData.pException
                    , m_ScriptExecData.scriptPath
                        )
                    );
        //dispose script data
        m_ScriptExecData.dispose();
        break;
    }
    case mode::kFuncCall: {
        //call function
        m_pInspectingWrapper->setRetVal(
                    NSJSBase::callFuncImpl(
                    m_FunctionCallData.value
                    , m_Context
                    , m_FunctionCallData.name
                    , m_FunctionCallData.argc
                    , m_FunctionCallData.argv
                    )
                    );
        //dispose function data
        m_FunctionCallData.dispose();
        break;
    }
    }

    //save result on inspector
//    m_pInspectingWrapper->setRetVal(result);
}

void NSJSBase::v8_debug::internal::CInspectorClient::processMessageFromFrontend(
        const std::string &message)
{
    //send message to V8 internals
    dispatchProtocolMessage(message);
    //check message for Runtime.runIfWaitingForDebugger
    if (checkForStartDebugging(message)) {
        startDebugging();
    }
}

bool NSJSBase::v8_debug::internal::CInspectorClient::checkForStartDebugging(
        const std::string &json) const
{
    return getMethod(m_Context, json) == debugStartMarker;
}

void NSJSBase::v8_debug::internal::CInspectorClient::setScriptExecData(
        const CScriptExecData &data)
{
    m_Mode = mode::kScriptExec;
    m_ScriptExecData = data;
}

void NSJSBase::v8_debug::internal::CInspectorClient::setFuncCallData(
        const CFCallData &data)
{
    m_Mode = mode::kFuncCall;
    m_FunctionCallData = data;
}

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient() = default;

void NSJSBase::v8_debug::internal::CInspectorClient::maybeLogIncoming(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }

    //log incoming message
    logCdtMessage(std::clog, message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::pauseOnNextStatement()
{
    m_pSession->schedulePauseOnNextStatement(strToView("debugCommand"), {});
}

void NSJSBase::v8_debug::internal::CInspectorClient::dispatchProtocolMessage(
        const std::string &message)
{
    maybeLogIncoming(message);
    v8_inspector::StringView v8message = strToView(message);
    m_pSession->dispatchProtocolMessage(v8message);
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(
        //
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
        )

    //v8 stuff
    : m_Context(context)
    , m_pIsolate(m_Context->GetIsolate())
    , m_pPlatform(platform)

    //callbacks
    , m_pInspectingWrapper{inspector}

    //logging
    , m_bLog(log)
{
    setUpDebuggingSession(contextName
                          , contextGroupId
                          , [inspector, this](const v8_inspector::StringView &message) {
        inspector->sendData(message);
        std::string msg = viewToStr(m_pIsolate, message);
        if (msg.find("DOM.getDocument") != std::string::npos) {
            std::cout << "kkk\n";
            m_pSession->dispatchProtocolMessage(
                        strToView(R"({"id":1,"method":"Debugger.resume","params":{"terminateOnResume":false}})")
                        );
        }
        return;
        if (getMethod(m_Context, message) ==
//                "Debugger.paused"
                "DOM.getDocument"
                ) {
            std::cout << "kkk\n";
//            m_pSession->resume();

        }
    });
}
