#include "client.h"
#include "channel.h"//client holds channel
#include "singlethreadutils.h"//converting strings
#include <libplatform/libplatform.h>//v8::Platform
#include "../../v8/v8_base.h"//CJSContextPrivate

namespace {
    //
    class FalseSetter {
        std::atomic<bool> &m_bool;
    public:
        FalseSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = false;}
        ~FalseSetter() {m_bool = true;}
    };
    //
    class TrueSetter {
        std::atomic<bool> &m_bool;
    public:
        TrueSetter(std::atomic<bool> &b) : m_bool(b) {m_bool = true;}
        ~TrueSetter() {m_bool = false;}
    };
}

void NSJSBase::v8_debug::internal::CInspectorClient::setUpDebuggingSession(
        CInspectorChannel::sendDataCallback sendDataCallback)
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
    m_pSession = m_pInspector->connect(m_iContextGroupId, m_pChannel.get(), state);

    //context name as string view
    v8_inspector::StringView viewContextName = strToView(m_sContextName);

    //context info
    v8_inspector::V8ContextInfo info{
        m_Context
                , m_iContextGroupId
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

void NSJSBase::v8_debug::internal::CInspectorClient::runMessageLoopOnPause(int contextGroupId) {
    if (m_bPause) {
        return;
    }

    TrueSetter f(m_bPause);

    while (m_bPause) {
        //while not paused by cdt or cdt disconnected
        if (!m_WaitForFrontendMessage()) {
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
    //set pause at the start of script
    m_pSession->schedulePauseOnNextStatement(strToView("debug"),{});
    //contextPrivate implements script execution
    JSSmart<CJSValue> scriptResult =
            m_pContextPrivate->runScriptImpl(m_sScriptStr
                                             , m_pException
                                             , m_sScriptPath);
    //save result
    m_SetRetVal(scriptResult);
}

void NSJSBase::v8_debug::internal::CInspectorClient::processMessageFromFrontend(
        const std::string &message)
{
    //send message to V8 internals
    m_pSession->dispatchProtocolMessage(strToView(message));
    //check message for Runtime.runIfWaitingForDebugger
    if (checkForStartDebugging(message)) {
        startDebugging();
    }
}

bool NSJSBase::v8_debug::internal::CInspectorClient::checkForStartDebugging(
        const std::string &json)
{
    return getMethod(m_Context, json) == debugStartMarker;
}

NSJSBase::v8_debug::internal::CInspectorClient::CInspectorClient(
        CJSContextPrivate *pContextPrivate
        , const std::string &contextName
        , const std::string &scriptStr
        , NSCommon::smart_ptr<CJSTryCatch> &pException
        , const std::wstring &scriptPath
        , CInspectorChannel::sendDataCallback sendDataFunc
        , waitMessageCallback waitIncomingMessage
        , setScriptRetValCallback setScriptRetVal)

    //v8 stuff
    : m_Context(pContextPrivate->m_context)
    , m_pIsolate(m_Context->GetIsolate())
    , m_pPlatform(pContextPrivate->getPlatform())

    //script exec data
    , m_sScriptStr(scriptStr)
    , m_pException(pException)
    , m_sScriptPath(scriptPath)

    //executor itself
    , m_pContextPrivate(pContextPrivate)

    //context name
    , m_sContextName(contextName)

    //callbacks
    , m_WaitForFrontendMessage(std::move(waitIncomingMessage))
    , m_SetRetVal(std::move(setScriptRetVal))
{
    setUpDebuggingSession(std::move(sendDataFunc));
}

NSJSBase::v8_debug::internal::CInspectorClient::~CInspectorClient()
{
    //
}
