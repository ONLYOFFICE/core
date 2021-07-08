#include "singleconnectionserver.h"
#include <iostream>

namespace ip = boost::asio::ip;
namespace beast = boost::beast;

std::string SingleConnectionServer::getData()
{
    if (!m_pWebsocketStream) {
        std::cerr << "no stream" << std::endl;
        return std::string();
    }
    beast::multi_buffer buffer;
    //check err
    beast::error_code errCode;
    //read into buffer; blocks here
    m_pWebsocketStream->read(buffer, errCode);
    if (errCode) {
        reportError(errCode, "err on reading");
        return std::string();
    }
    //set mode equal to incoming message mode
    m_pWebsocketStream->text(
                //returns true if socket got text message, false on binary
                m_pWebsocketStream->got_text()
                );
    return boost::beast::buffers_to_string(buffer.data());
}

void SingleConnectionServer::reportError(boost::beast::error_code code, const char *what) const
{
    std::cerr << "error with code " << code << ": " << what << std::endl;
}

SingleConnectionServer::SingleConnectionServer(uint16_t port, std::string host)
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

void SingleConnectionServer::setOnMessageCallback(onMessageCallback callback)
{
    m_fOnMessage = std::move(callback);
}

bool SingleConnectionServer::waitForConnection()
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

bool SingleConnectionServer::listen()
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
void SingleConnectionServer::run()
{
    while (true) {
        waitAndProcessMessage();
    }
}

void SingleConnectionServer::sendData(const std::string &data)
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

void SingleConnectionServer::waitAndProcessMessage()
{
    m_fOnMessage(getData());
}
