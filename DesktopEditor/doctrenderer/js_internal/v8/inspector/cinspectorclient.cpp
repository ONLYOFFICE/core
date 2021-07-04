#include "cinspectorclient.h"
#include <iostream>//idk

//hand-made front-end
#include "cchannel.h"

namespace {
    std::unique_ptr<v8_inspector::V8Inspector::Channel> makeChannel(
            v8::Local<v8::Context> context
            , std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> socket) {
        return
                std::unique_ptr<v8_inspector::V8Inspector::Channel>

//                std::make_unique
//                <v8_inspector::V8Inspector::Channel>
                (new
//                 InspectorFrontend(context)
                 CChannel(socket)
                 );
    }
} // namespace anonimous

CInspectorClient::CInspectorClient(v8::Local<v8::Context> context
                                   , const std::string &contextName
                                   , std::shared_ptr<NSNetwork::NSWebSocket::IWebSocket> socket
                                   , std::shared_ptr<CAbstractDebugSocketListener> listener)
    : v8_inspector::V8InspectorClient()

    //v8 data
    , m_pIsolate(context->GetIsolate())
//    , mContext(context)can't assign global with local

    //copy context name
    , m_sContextName(contextName)

    //port to sit on
    , socketListener(listener)
{
    //client instance is being created in this ctor

    //create inspector instance with isolate and client
    m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);
    //passing this out of ctor is sick shit, but official realisation does so

    //create a channel impl
    m_pChannel = makeChannel(context, socket);

    //this is a "state" argument for connecting inspector to channel
    //not sure what does it mean
    v8_inspector::StringView state{};

    //create debugging session through connecting inspector to channel
    m_pSession = m_pInspector->connect(m_iContextGroupId, m_pChannel.get(), state);

    //context name
    v8_inspector::StringView v8ContextName {
        reinterpret_cast<const uint8_t*>(m_sContextName.c_str())
                , m_sContextName.length()
    };

    //create context info for given context
    v8_inspector::V8ContextInfo contextInfo{context
                , m_iContextGroupId
                , v8ContextName};

    //register context objects in the inspector
    m_pInspector->contextCreated(contextInfo);

    //set global context (idk what for)
    mContext.Reset(m_pIsolate, context);
}

void CInspectorClient::runMessageLoopOnPause(int contextGroupId)
{
    //check session
    if (!m_pSession) {
        return;
    }

    //no loops
    if (pause) {
        return;
    }

    //set pause
    pause = true;
    while (pause) {
        if (socketListener->hasMessage()) {
            m_pSession->dispatchProtocolMessage(socketListener->getMessage());
        }
    }

    //in any case
    pause = false;
}

void CInspectorClient::quitMessageLoopOnPause()
{
    pause = false;
}
