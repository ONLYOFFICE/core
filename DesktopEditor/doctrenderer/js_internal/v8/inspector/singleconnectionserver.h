#ifndef SINGLECONNECTIONSERVER_H
#define SINGLECONNECTIONSERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <functional>
#include <atomic>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class SingleConnectionServer
{
public:
    using onMessageCallback = std::function<void(const std::string &message)>;

private:
    //tcp is a class
    using tcp = boost::asio::ip::tcp;
    //
    using stream_t = boost::beast::websocket::stream<
    tcp::socket
    >;

    //boost stuff
    //required for all io operations
    boost::asio::io_context m_io_context{
                                    1//use only one thread
                                      };
    //tcp endpoint
    boost::asio::ip::tcp::endpoint m_Endpoint;
    //connection acceptor
    boost::asio::ip::tcp::acceptor m_ConnectionAcceptor;
    //using pointer to create stream on connection, not in ctor
    std::unique_ptr<stream_t> m_pWebsocketStream{nullptr};



    //message handler
    onMessageCallback m_fOnMessage;

    //flags
    std::atomic<bool> m_bCdtDisconnected{true};
    std::atomic<bool> m_bListening{false};



    //private api
    //read data(blocks)
    std::string getData();
    //report error
    void reportError(boost::beast::error_code code, const char *what) const;

public:
    SingleConnectionServer(uint16_t port = 8080, std::string host = "127.0.0.1");
    void setOnMessageCallback(onMessageCallback callback);
    bool waitForConnection();
    bool listen();
    void run();//start and wait
    void sendData(const std::string &data);
    bool waitAndProcessMessage();
    bool connected() const;
    bool listening() const;
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // SINGLECONNECTIONSERVER_H
