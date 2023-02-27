#include "websocket_server.h"

WebSocketServer::WebSocketServer(int port, std::function<void(std::string)> onMessage)
{
	port_ = port;
	onMessage_ = std::move(onMessage);
}

void WebSocketServer::run() {
	try
	{
		auto const address = net::ip::make_address("127.0.0.1");
		net::io_context ioc{1};
		tcp::acceptor acceptor{ioc, {address, static_cast<unsigned short>(port_)}};
		printListeningMessage();

		tcp::socket socket{ioc};
		acceptor.accept(socket);
		ws_ = std::unique_ptr<websocket::stream<tcp::socket>>(new websocket::stream<tcp::socket>(std::move(socket)));
		startListening();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void WebSocketServer::sendMessage(const std::string &message)
{
	try {
		boost::beast::multi_buffer b;
		boost::beast::ostream(b) << message;

		ws_->text(ws_->got_text());
		ws_->write(b.data());
	} catch(beast::system_error const& se) {
		if (se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	} catch(std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void WebSocketServer::startListening()
{
	try {
		ws_->accept();
		while (true) {
			waitFrontendMessage();
		}
	} catch(beast::system_error const& se) {
		if(se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	} catch(std::exception const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void WebSocketServer::printListeningMessage() {
	std::cout << "WebSocket based Inspector Agent started" << std::endl;
	std::cout << "Open the following link in your Chrome/Chromium browser: devtools://devtools/bundled/inspector.html?ws=127.0.0.1:" <<  port_ << std::endl;
}

void WebSocketServer::waitForFrontendMessageOnPause() {
	waitFrontendMessage();
}

void WebSocketServer::waitFrontendMessage()
{
	beast::flat_buffer buffer;
	ws_->read(buffer);
	std::string message = boost::beast::buffers_to_string(buffer.data());
	onMessage_(std::move(message));
}
