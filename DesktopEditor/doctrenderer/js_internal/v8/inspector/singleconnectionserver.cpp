#include "singleconnectionserver.h"
#include <iostream>

namespace ip = boost::asio::ip;
namespace beast = boost::beast;

std::string NSJSBase::v8_debug::internal::SingleConnectionServer::getData()
{
    if (!m_pWebsocketStream) {
        std::cerr << "no stream" << std::endl;
        return std::string();
    }
    beast::multi_buffer buffer;
    //to check for err
    beast::error_code errCode;
    //read into buffer; blocks here
    m_pWebsocketStream->read(buffer, errCode);
    if (errCode) {
        if (
                errCode == boost::asio::error::operation_aborted
                ) {
            m_bCdtDisconnected = true;
            return std::string();
        }
        reportError(errCode, errCode.message().c_str());
        return std::string();
    }
    //set mode equal to incoming message mode
    m_pWebsocketStream->text(
                //returns true if socket got text message, false on binary
                m_pWebsocketStream->got_text()
                );
    return boost::beast::buffers_to_string(buffer.data());
}

void NSJSBase::v8_debug::internal::SingleConnectionServer::reportError(boost::beast::error_code code, const char *what) const
{
    std::cerr << "error with code " << code << ": " << what << std::endl;
}

NSJSBase::v8_debug::internal::SingleConnectionServer::SingleConnectionServer(uint16_t port, std::string host)
//    : m_ConnectionAcceptor(m_io_context)
//    , m_Socket(m_io_context)

    : m_Endpoint(
          ip::make_address(host)
          , port
          )

    //constructs unopened acceptor
    , m_ConnectionAcceptor(
          m_io_context
          )

{
    //
}

void NSJSBase::v8_debug::internal::SingleConnectionServer::setOnMessageCallback(onMessageCallback callback)
{
    m_fOnMessage = std::move(callback);
}

bool NSJSBase::v8_debug::internal::SingleConnectionServer::waitForConnection()
{
    boost::beast::error_code errCode;
    //make socket on io context
    tcp::socket socket{m_io_context};
    //blocks until connected
    m_ConnectionAcceptor.accept(socket, errCode);
    //check for error
    if (errCode) {
        return false;
    }
    //set up websocket stream
    m_pWebsocketStream = std::make_unique<stream_t>(
                //moving local object to stream
                std::move(socket)
                );
    //accept client handshake
    m_pWebsocketStream->accept(errCode);
    if (errCode) {
        return false;
    }
    return true;
}

bool NSJSBase::v8_debug::internal::SingleConnectionServer::listen()
{
    boost::beast::error_code errCode;
    //open acceptor with endpoint's protocol
    m_ConnectionAcceptor.open(m_Endpoint.protocol(), errCode);
    //check for error
    if (errCode) {
        return false;
    }

    //bind acceptor to endpoint
    m_ConnectionAcceptor.bind(m_Endpoint, errCode);
    //check for error
    if (errCode) {
        return false;
    }

    //listen
    m_ConnectionAcceptor.listen(
                1//max amount of pending connections
                , errCode);
    //check for error
    if (errCode) {
        return false;
    }

    return true;
}

//blocks
void NSJSBase::v8_debug::internal::SingleConnectionServer::run()
{
    while (waitAndProcessMessage()) {
        //
    }
}

void NSJSBase::v8_debug::internal::SingleConnectionServer::sendData(const std::string &data)
{
    if (!m_pWebsocketStream) {
        std::cerr << "no stream" << std::endl;
        return;
    }
    //write data to buffer
    beast::multi_buffer buffer;
    beast::ostream(buffer) << data;

    //text or binary mode as already set
    //
    boost::beast::error_code errCode;
    m_pWebsocketStream->write(buffer.data(), errCode);
    if (errCode) {
        //report error
    }
}

bool NSJSBase::v8_debug::internal::SingleConnectionServer::waitAndProcessMessage()
{
    std::string data = getData();
    if (m_bCdtDisconnected) {
        return false;
    }
    m_fOnMessage(data);
    return true;
}
