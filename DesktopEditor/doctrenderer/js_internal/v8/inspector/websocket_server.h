#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <functional>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

namespace NSJSBase
{

	class CWebSocketServer
	{
	private:
		int m_nPort;
		// boost stuff
		tcp::endpoint m_oEndpoint;
		net::io_context m_oIoContext{1};
		tcp::acceptor m_oAcceptor;
		std::unique_ptr<websocket::stream<tcp::socket>> m_oWs;
		// callbacks
		std::function<void(std::string&)> m_fOnMessage;

	private:
		void init();
		void startListening();
		void printListeningMessage();
		void waitFrontendMessage();

	public:
		// flag that is `true` when server finishes initial connection and script is ready to be executed in V8
		bool isServerReady_ = false;

	public:
		CWebSocketServer(std::function<void(std::string&)> fOnMessage);
		~CWebSocketServer();

		void connect();
		void sendMessage(const std::string& sMessage);
		void waitForFrontendMessageOnPause();
	};

}

#endif // WEBSOCKET_SERVER_H
