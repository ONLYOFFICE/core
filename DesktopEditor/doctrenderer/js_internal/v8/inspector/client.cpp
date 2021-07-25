#include "client.h"
#include "channel.h"//client holds channel
#include "singlethreadutils.h"//converting strings
#include <libplatform/libplatform.h>//v8::Platform
#include "../../v8/v8_base.h"//runScript and callFunc impls
#include "inspector_impl.h"//to interact with inspector

namespace {
    //use them in loop
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
    //pause before current script on debugging launch
    pauseOnNextStatement();
    std::cout << "TO EXECUTE\n";

    execute();

    m_pInspectingWrapper->pauseServer();
}

void NSJSBase::v8_debug::internal::CInspectorClient::processFrontendMessage(
        const std::string &message
        )
{
    //send message to V8 internals
    dispatchProtocolMessage(message);
    //
    checkFrontendMessage(message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessage(const std::string &message)
{
    switch (m_State) {
    case debugState::kDebugPaused: return checkFrontendMessageOnDebugPaused(message);
    case debugState::kServerNotReady: return checkFrontendMessageBeforeReady(message);
    case debugState::kSessionResumed: return;//do nothing
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessageOnScript(const std::string &message)
{
    if (getMethod(m_Context, message) ==
            scriptResumeFlag_1
            ) {
        resumeDebuggingSession();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessageOnFunc(const std::string &message)
{
    if (getMethod(m_Context, message) ==
            funcResumeFlag_1
//            funcResumeFlag_2
            ) {
        resumeDebuggingSession();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessageBeforeReady(const std::string &message)
{
    if (checkForReadyMessage(message)) {
        m_pInspectingWrapper->onServerReady();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::checkFrontendMessageOnDebugPaused(const std::string &message)
{
    //
    switch (m_Mode) {
    case mode::kFuncCall : return checkFrontendMessageOnFunc(message);
    case mode::kScriptExec: return checkFrontendMessageOnScript(message);
    }
}

bool NSJSBase::v8_debug::internal::CInspectorClient::checkForReadyMessage(
        const std::string &json) const
{
    return getMethod(m_Context, json) == readyMessage;
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

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient() {
    //tmp
    m_pSession.release();
}

void NSJSBase::v8_debug::internal::CInspectorClient::maybeLogIncoming(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }
    //log incoming message
    logCdtMessage(std::cout, message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::pauseOnNextStatement()
{
    m_State = debugState::kDebugPaused;
    m_pSession->schedulePauseOnNextStatement(strToView("debugCommand"), {});
}

void NSJSBase::v8_debug::internal::CInspectorClient::resumeDebuggingSession()
{
    m_State = debugState::kSessionResumed;
    m_pSession->resume();
}

void NSJSBase::v8_debug::internal::CInspectorClient::dispatchProtocolMessage(
        const std::string &message)
{
    maybeLogIncoming(message);
    v8_inspector::StringView v8message = strToView(message);
    m_pSession->dispatchProtocolMessage(v8message);
}

void NSJSBase::v8_debug::internal::CInspectorClient::execute()
{
    switch (m_Mode) {
    case mode::kFuncCall: return callFunc();
    case mode::kScriptExec: return runScript();
    }
}

void NSJSBase::v8_debug::internal::CInspectorClient::runScript()
{
    JSSmart<CJSValue> result = runScriptImpl(
                            m_Context
                            , m_ScriptExecData.scriptSource
                            , *m_ScriptExecData.pException
                            , m_ScriptExecData.scriptPath
                            );
    m_ScriptExecData.dispose();
    m_pInspectingWrapper->setRetVal(result);
}

void NSJSBase::v8_debug::internal::CInspectorClient::callFunc()
{
    JSSmart<CJSValue> result = callFuncImpl(
                            m_FunctionCallData.value
                            , m_Context
                            , m_FunctionCallData.name
                            , m_FunctionCallData.argc
                            , m_FunctionCallData.argv
                            );
    m_FunctionCallData.dispose();
    m_pInspectingWrapper->setRetVal(result);
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(//
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
        //
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
                          , [inspector](const v8_inspector::StringView &message) {
        inspector->sendData(message);
    });
}
