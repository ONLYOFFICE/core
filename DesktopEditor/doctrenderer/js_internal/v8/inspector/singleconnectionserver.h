#ifndef SINGLECONNECTIONSERVER_H
#define SINGLECONNECTIONSERVER_H

//boost stuff
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>

#include <functional>//std::function
#include <atomic>//std::atomic for thread-safe flags

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CSingleConnectionServer
{
public:
    using onMessageCallback = std::function<void(const std::string &message)>;
    using onResumeCallback = std::function<void()>;//not used, for any case

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
    //using pointer to be able to create stream on connection, not in ctor
    std::unique_ptr<stream_t> m_pWebsocketStream{nullptr};

    //message handler
    onMessageCallback m_fOnMessage{};

    //flags
    std::atomic<bool> m_bCdtConnected{false};//используется для выхода из цикла после закрытия chrome dev tools
    std::atomic<bool> m_bListening{false};//слушает ли сервак входящие соединения
    std::atomic<bool> m_bPaused{false};//пауза



    //private api
    //read data(blocks)
    std::pair<std::string, boost::beast::error_code> getData();
    boost::beast::error_code discardData();
    //report error
    void reportError(const boost::beast::error_code &code, const char *context) const;
    //
    void setConnected();
    void setDisconnected();
    //
    bool checkStream();

public:
    CSingleConnectionServer(uint16_t port = 8080, std::string host = "127.0.0.1");

    void setOnMessageCallback(onMessageCallback callback);

    bool waitForConnection();
    bool listen();
    void run(onResumeCallback beforeRun = onResumeCallback{});
    void sendData(const std::string &data);
    bool waitAndProcessMessage();
    bool connected() const;
    bool listening() const;
    bool shutdown();
    void pause();
    uint16_t port() const;
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // SINGLECONNECTIONSERVER_H
