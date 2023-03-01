#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <functional>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <thread>
#include <vector>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocketServer
{
private:
	int port_;
	// boost stuff
	tcp::endpoint endpoint_;
	net::io_context ioc_{1};
	tcp::acceptor acceptor_;
	std::unique_ptr<websocket::stream<tcp::socket>> ws_;
	// callbacks
	std::function<void(std::string)> onMessage_;
	std::function<bool(void)> isScriptRunning_;

private:
	void startListening();
	void printListeningMessage();
	void waitFrontendMessage();

public:
	WebSocketServer(int port, std::function<void(std::string)> onMessage, std::function<bool(void)> isScriptRunning);

	void run();
	void sendMessage(const std::string &message);
	void waitForFrontendMessageOnPause();
};

#endif // WEBSOCKETSERVER_H
