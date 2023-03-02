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

private:
	void init();
	void startListening();
	void printListeningMessage();
	void waitFrontendMessage();

public:
	// flag that is `true` when server finishes initial connection and script is ready to be executed in V8
	bool isServerReady_ = false;

public:
	WebSocketServer(int port, std::function<void(std::string)> onMessage);
	~WebSocketServer();

	void connect();
	void sendMessage(const std::string &message);
	void waitForFrontendMessageOnPause();
};

#endif // WEBSOCKETSERVER_H
