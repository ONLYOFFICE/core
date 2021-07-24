//хедер с бустом должен быть выше всех
#include "singleconnectionserver.h"//CSingleConnectionServer
#include "serverholder.h"
#include <iostream>//std::cout

void NSJSBase::v8_debug::internal::CServerHolder::printLaunchHint(uint16_t port) const
{
    std::cout << "chrome "
         << "--remote-debugging-port=9222 "
         << "http://localhost:9222/devtools/inspector.html?ws=localhost:"
         << port
         << std::endl;
}

NSJSBase::v8_debug::internal::CServerHolder::CServerHolder(
        uint16_t port
          )
    : m_pServer{std::make_unique<CSingleConnectionServer>(port)}
{
    if (!m_pServer->listen()) {
        std::cerr << "server can't listen to connection" << std::endl;
        return;
    }
    printLaunchHint(port);
    //blocks until connected
    if (!m_pServer->waitForConnection()) {
        std::cerr << "server can't connect" << std::endl;
        return;
    }
}

NSJSBase::v8_debug::internal::CServerHolder::CUseData NSJSBase::v8_debug::internal::CServerHolder::getServer()
{
    return {
        m_pServer.get()
        , m_bInUse
        , m_bServerReady
    };
}

bool NSJSBase::v8_debug::internal::CServerHolder::free() const
{
    return !m_bInUse;
}

NSJSBase::v8_debug::internal::CServerHolder::shared_flag_t &NSJSBase::v8_debug::internal::CServerHolder::getServerReadyFlag()
{
    return m_bServerReady;
}

bool NSJSBase::v8_debug::internal::CServerHolder::isServerReady()
{
    return m_bServerReady;
}

NSJSBase::v8_debug::internal::CServerHolder::~CServerHolder()
{
    if (m_pServer->listening()) {
        m_pServer->shutdown();
    }
}
