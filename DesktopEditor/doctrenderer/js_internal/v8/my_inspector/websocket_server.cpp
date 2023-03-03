#include "websocket_server.h"

#include <iostream>

WebSocketServer::WebSocketServer(int nPort, std::function<void(std::string&)> fOnMessage)
	: m_nPort(nPort)
	, m_oEndpoint(net::ip::make_address("127.0.0.1"), m_nPort)
	, m_oAcceptor(m_oIoContext)
	, m_fOnMessage(std::move(fOnMessage))
{
	init();
}

WebSocketServer::~WebSocketServer()
{
	m_oWs->close(beast::websocket::close_code::normal);
	m_oAcceptor.close();
}

void WebSocketServer::init()
{
	try
	{
		m_oAcceptor.open(m_oEndpoint.protocol());
		m_oAcceptor.set_option(tcp::acceptor::reuse_address(true));
		m_oAcceptor.bind(m_oEndpoint);
		m_oAcceptor.listen(1);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error on init the server: " << e.what() << std::endl;
	}
}

void WebSocketServer::connect()
{
	try
	{
		printListeningMessage();

		tcp::socket oSocket(m_oIoContext);
		m_oAcceptor.accept(oSocket);
		m_oWs.reset(new websocket::stream<tcp::socket>(std::move(oSocket)));

		startListening();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error on connecting to server: " << e.what() << std::endl;
	}
}

void WebSocketServer::sendMessage(const std::string& sMessage)
{
	try
	{
		boost::beast::multi_buffer oBuffer;
		boost::beast::ostream(oBuffer) << sMessage;

		m_oWs->text(m_oWs->got_text());
		m_oWs->write(oBuffer.data());
	}
	catch(beast::system_error const& se)
	{
		if (se.code() != websocket::error::closed)
			std::cerr << "Error on message send: " << se.code().message() << std::endl;
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
		m_oWs->accept();
		while (!isServerReady_)
		{
			waitFrontendMessage();
		}
	}
	catch(beast::system_error const& se)
	{
		if (se.code() != websocket::error::closed)
			std::cerr << "Error on listening: " << se.code().message() << std::endl;
	}
	catch(std::exception const& e)
	{
		std::cerr << "System error: " << e.what() << std::endl;
	}
}

void WebSocketServer::printListeningMessage()
{
	std::cout << "WebSocket based Inspector Agent started" << std::endl;
	std::cout << "Open the following link in your Chrome/Chromium browser: devtools://devtools/bundled/inspector.html?ws=127.0.0.1:" <<  m_nPort << std::endl;
}

void WebSocketServer::waitForFrontendMessageOnPause()
{
	waitFrontendMessage();
}

void WebSocketServer::waitFrontendMessage()
{
	beast::flat_buffer oBuffer;
	m_oWs->read(oBuffer);
	std::string message = boost::beast::buffers_to_string(oBuffer.data());
	m_fOnMessage(message);
}
