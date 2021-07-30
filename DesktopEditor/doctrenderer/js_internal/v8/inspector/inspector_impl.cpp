#include "inspector_impl.h"
#include <iostream>//std::cout
#include "singlethreadutils.h"//string conversion

bool NSJSBase::v8_debug::internal::CInspectorImpl::initServer()
{
    //set on message callback
    auto messageCallback = [this](const std::string &message) {
        if (!message.empty()) {
            m_Client.processFrontendMessage(message);
        }
    };
    m_Server.setOnMessageCallback(messageCallback);

    //listen
    return m_Server.listen();
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::connectServer()
{
    printChromeLaunchHint(std::cout, m_Server.port());
    return m_Server.waitForConnection();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::waitWhileServerReady()
{
    m_Server.run();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::maybeLogOutgoing(
        const std::string &message) const
{
    if (!m_bLog) {
        return;
    }
    logOutgoingMessage(std::cout, message);
}

void NSJSBase::v8_debug::internal::CInspectorImpl::printChromeLaunchHint(
        std::ostream &out
        , uint16_t port)
{
    out << "chrome "
         << "--remote-debugging-port=9222 "
         << "http://localhost:9222/devtools/inspector.html?ws=localhost:"
         << port
         << std::endl;
}

void NSJSBase::v8_debug::internal::CInspectorImpl::sendData(const v8_inspector::StringView &message)
{
    //не отправляем пустые сообщения
    if (message.length() == 0) {
        return;
    }
    std::string str = internal::viewToStr(
                m_pIsolate
                , message);
    //
    maybeLogOutgoing(str);
    //
    this->m_Server.sendData(str);
}

bool NSJSBase::v8_debug::internal::CInspectorImpl::waitForMessage()
{
    return m_Server.waitAndProcessMessage();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::onServerReady()
{
    //когда сервак готов, ставим его на паузу и идём дальше по плюсовому коду
    m_Server.pause();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::prepareServer()
{
    //ждём, когда юзер законнектится
    if (!connectServer()) {
        return;
    }
    //прокидываем первые неинтересные сообщения
    waitWhileServerReady();
}

void NSJSBase::v8_debug::internal::CInspectorImpl::beforeLaunch()
{
    //перед запуском кода ставим паузу в его начале
    m_Client.pauseOnNextStatement();
}

NSJSBase::v8_debug::internal::CInspectorImpl::CInspectorImpl(
        //
        v8::Local<v8::Context> context
        //platform to pump
        , v8::Platform *platform
        //
        , uint16_t port
        , int contextGroupId
        , const std::string &contextName
        , bool log
        )
    : m_Server{port}
    , m_pIsolate{context->GetIsolate()}
    , m_bLog{log}
    , m_Client{context, contextName, contextGroupId, platform, this, log}
{
    //аттачим acceptor к эндпойнту прямо в конструкторе
    if (!initServer()) {
        std::cerr << "server can't listen to incoming connections" << std::endl;
        return;
    }
}

NSJSBase::v8_debug::internal::CInspectorImpl::~CInspectorImpl() {
    if (m_Server.listening() && m_Server.connected()) {
        m_Server.shutdown();
    }
}
