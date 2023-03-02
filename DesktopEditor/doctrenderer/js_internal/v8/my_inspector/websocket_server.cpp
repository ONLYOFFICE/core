#include "websocket_server.h"

WebSocketServer::WebSocketServer(int port, std::function<void(std::string)> onMessage, std::function<bool(void)> isScriptRunning)
	: port_(port)
	, endpoint_(net::ip::make_address("127.0.0.1"), port_)
	, acceptor_(ioc_)
	, onMessage_(std::move(onMessage))
	, isScriptRunning_(std::move(isScriptRunning))
{
	init();
}

WebSocketServer::~WebSocketServer()
{
	// close the connection
	ws_->close(beast::websocket::close_code::normal);
	acceptor_.close();
}

void WebSocketServer::init()
{
	try
	{
		acceptor_.open(endpoint_.protocol());
		acceptor_.set_option(tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint_);
		acceptor_.listen(1);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error on init the server: " << e.what() << std::endl;
	}
}

void WebSocketServer::run()
{
	try
	{
		printListeningMessage();

		tcp::socket socket(ioc_);
		acceptor_.accept(socket);
		ws_.reset(new websocket::stream<tcp::socket>(std::move(socket)));
		startListening();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error on running the server: " << e.what() << std::endl;
	}
}

void WebSocketServer::sendMessage(const std::string &message)
{
	try
	{
		boost::beast::multi_buffer b;
		boost::beast::ostream(b) << message;

		ws_->text(ws_->got_text());
		ws_->write(b.data());
	}
	catch(beast::system_error const& se)
	{
		if (se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	}
	catch(std::exception const& e)
	{
		std::cerr << "System error: " << e.what() << std::endl;
	}
}

void WebSocketServer::startListening()
{
	try
	{
		ws_->accept();
		while (isScriptRunning_())
		{
			waitFrontendMessage();
		}
	}
	catch(beast::system_error const& se)
	{
		if (se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code() << std::endl;
	}
	catch(std::exception const& e)
	{
		std::cerr << "System error: " << e.what() << std::endl;
	}
}

void WebSocketServer::printListeningMessage()
{
	std::cout << "WebSocket based Inspector Agent started" << std::endl;
	std::cout << "Open the following link in your Chrome/Chromium browser: devtools://devtools/bundled/inspector.html?ws=127.0.0.1:" <<  port_ << std::endl;
}

void WebSocketServer::waitForFrontendMessageOnPause()
{
	waitFrontendMessage();
}

void WebSocketServer::waitFrontendMessage()
{
	beast::flat_buffer buffer;
	ws_->read(buffer);
	std::string message = boost::beast::buffers_to_string(buffer.data());
	onMessage_(std::move(message));
}
