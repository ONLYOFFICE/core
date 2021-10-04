#include "singleconnectionserver.h"
#include <iostream>//reporting errors

namespace ip = boost::asio::ip;
namespace beast = boost::beast;

std::pair<std::string, beast::error_code>
NSJSBase::v8_debug::internal::CSingleConnectionServer::getData()
{
    //set up buffer
    beast::multi_buffer buffer;
    //to check for err
    beast::error_code errCode;
    //read into buffer; blocks here
    m_pWebsocketStream->read(buffer, errCode);
    //check for error
    if (errCode) {
        if (//cdt disconnection
                errCode == boost::asio::error::operation_aborted
                ) {
            setDisconnected();
            return {std::string(), errCode};
        }
        reportError(errCode, "while reading");
        return {std::string(), errCode};
    }
    //set mode equal to incoming message mode
    m_pWebsocketStream->text(
                //returns true if socket got text message, false on binary
                m_pWebsocketStream->got_text()
                );
    //return value read
    return {boost::beast::buffers_to_string(buffer.data()), errCode};
}

beast::error_code NSJSBase::v8_debug::internal::CSingleConnectionServer::discardData()
{
    beast::flat_buffer buffer;//no need in multi buffer
    beast::error_code errCode;
    m_pWebsocketStream->read(buffer, errCode);
    return errCode;
}

void NSJSBase::v8_debug::internal::CSingleConnectionServer::reportError(
        const beast::error_code &code
        , const char *context) const
{
    beast::error_code ec;
    std::cerr
            << m_Endpoint.address().to_string(ec)
            << ':'
            << m_Endpoint.port()
            << ": "
            << context << ": error with code " << code << ": " << code.message() << std::endl;
}

void NSJSBase::v8_debug::internal::CSingleConnectionServer::setConnected()
{
    m_bCdtConnected = true;
}

void NSJSBase::v8_debug::internal::CSingleConnectionServer::setDisconnected()
{
    m_bCdtConnected = false;
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::checkStream()
{
    if (!m_pWebsocketStream) {
        std::cerr << "no websocket stream when requested one" << std::endl;
        setDisconnected();
        return false;
    }
    return true;
}

NSJSBase::v8_debug::internal::CSingleConnectionServer::CSingleConnectionServer(uint16_t port
                                                                             , std::string host)
    //set up endpoint
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

void NSJSBase::v8_debug::internal::CSingleConnectionServer::setOnMessageCallback(
        onMessageCallback callback)
{
    m_fOnMessage = std::move(callback);
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::waitForConnection()
{
    //to check errors
    boost::beast::error_code errCode;
    //make socket on io context
    tcp::socket socket{m_io_context};
    //blocks until connected
    m_ConnectionAcceptor.accept(socket, errCode);
    //check for error
    if (errCode) {
        reportError(errCode, "while accepting connection");
        return false;
    }
    //set up websocket stream and delete old one if any
    m_pWebsocketStream.reset(new stream_t{
                                 //move local object to stream
                                 std::move(socket)
                             }
                             );
    //accept client handshake
    m_pWebsocketStream->accept(errCode);
    //check error
    if (errCode) {
        reportError(errCode, "while accepting client handshake");
        return false;
    }
    //connection established
    setConnected();
    return true;
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::listen()
{
    //
    boost::beast::error_code errCode;

    //open acceptor with endpoint's protocol
    m_ConnectionAcceptor.open(m_Endpoint.protocol(), errCode);
    //check for error
    if (errCode) {
        reportError(errCode, "while opening acceptor");
        return false;
    }

    //bind acceptor to endpoint
    m_ConnectionAcceptor.bind(m_Endpoint, errCode);
    //check for error
    if (errCode) {
        reportError(errCode, "while binding acceptor to endpoint");
        return false;
    }

    //listen
    m_ConnectionAcceptor.listen(
                1//max amount of pending connections
                , errCode);
    //check for error
    if (errCode) {
        reportError(errCode, "while attempting to listen");
        return false;
    }

    m_bListening = true;
    return true;
}

//blocks
void NSJSBase::v8_debug::internal::CSingleConnectionServer::run(onResumeCallback beforeRun)
{
    //resume stuff
    m_bPaused = false;
    if (beforeRun) {
        beforeRun();
    }

    while (!m_bPaused) {
        if (!waitAndProcessMessage()) {
            return;
        }
    }
}

void NSJSBase::v8_debug::internal::CSingleConnectionServer::sendData(const std::string &data)
{
    //check stream
    if (!checkStream()) {
        return;
    }

    //no need to check for pause

    //write data to buffer
    beast::multi_buffer buffer;
    beast::ostream(buffer) << data;

    //text or binary mode as already set
    boost::beast::error_code errCode;
    m_pWebsocketStream->write(buffer.data(), errCode);
    if (errCode) {
        reportError(errCode, "while sending data");
        return;
    }
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::waitAndProcessMessage()
{
    //check stream
    if (!checkStream()) {
        return false;
    }

    std::pair<std::string, beast::error_code> result = getData();

    if (!connected()) {
        return false;
    }

    if (result.second) {
        return false;
    }

    if (m_fOnMessage){
        m_fOnMessage(result.first);
    }

    //it won't work with check for pause

    return true;
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::connected() const
{
    return m_bCdtConnected;
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::listening() const
{
    return m_bListening;
}

bool NSJSBase::v8_debug::internal::CSingleConnectionServer::shutdown()
{
    //check stream
    if (!checkStream()) {
        return false;
    }

    beast::error_code errCode;
    //send close frame
    m_pWebsocketStream->close(beast::websocket::close_code::normal, errCode);

    if (errCode) {
        reportError(errCode, "while closing");
        return false;
    }

    //discard pending messages until receiving close frame
    while (true) {
        //read and get error_code
        errCode = discardData();

        //close frame is delivered as closed error
        if (errCode == beast::websocket::error::closed) {
            return true;
        }

        //any other errors
        if (errCode) {
            reportError(errCode, "while waiting for close responce at close");
            return false;
        }
    }
}

void NSJSBase::v8_debug::internal::CSingleConnectionServer::pause()
{
    m_bPaused = true;
}

uint16_t NSJSBase::v8_debug::internal::CSingleConnectionServer::port() const
{
    return m_Endpoint.port();
}
