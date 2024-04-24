/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "websocket_server.h"

#include <iostream>

namespace NSJSBase
{
    static int getPort()
    {
        static int nInitialPort{8080};
        return nInitialPort++;
    }

    CWebSocketServer::CWebSocketServer(std::function<void(std::string&)> fOnMessage)
        : m_nPort(getPort())
		, m_oEndpoint(net::ip::make_address("127.0.0.1"), m_nPort)
		, m_oAcceptor(m_oIoContext)
		, m_fOnMessage(std::move(fOnMessage))
	{
		init();
	}

	CWebSocketServer::~CWebSocketServer()
	{
		m_oWs->close(beast::websocket::close_code::normal);
		m_oAcceptor.close();
	}

	void CWebSocketServer::init()
	{
		bool bIsInit = false;
		while (!bIsInit)
		{
			try
			{
				m_oAcceptor.open(m_oEndpoint.protocol());
				m_oAcceptor.set_option(tcp::acceptor::reuse_address(true));
				// here may be thrown an exception if port already in use by other process
				m_oAcceptor.bind(m_oEndpoint);
				m_oAcceptor.listen(1);
				// if no exception was thrown at this point the server is successfully initialized
				bIsInit = true;
			}
			catch (const beast::system_error& se)
			{
				// if port already in use by other process try to bind to next port number
				if (se.code() == boost::asio::error::access_denied)
				{
					boost::system::error_code ec;
					m_oAcceptor.close(ec);
					if (ec)
					{
						std::cerr << "Error on trying to get new port: " << ec.message() << std::endl;
						break;
					}
					m_nPort = getPort();
					m_oEndpoint.port(m_nPort);
				}
				else
				{
					std::cerr << "Error on init the server: " << se.code().message() << std::endl;
					break;
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "System error: " << e.what() << std::endl;
				break;
			}
		}
	}

	void CWebSocketServer::connect()
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

	void CWebSocketServer::sendMessage(const std::string& sMessage)
	{
		try
		{
			boost::beast::multi_buffer oBuffer;
			boost::beast::ostream(oBuffer) << sMessage;

			m_oWs->text(m_oWs->got_text());
			m_oWs->write(oBuffer.data());
		}
		catch (const beast::system_error& se)
		{
			if (se.code() != websocket::error::closed)
				std::cerr << "Error on message send: " << se.code().message() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "System error: " << e.what() << std::endl;
		}
	}

	void CWebSocketServer::startListening()
	{
		try
		{
			m_oWs->accept();
			while (!isServerReady_)
			{
				waitFrontendMessage();
			}
		}
		catch (const beast::system_error& se)
		{
			if (se.code() != websocket::error::closed)
				std::cerr << "Error on listening: " << se.code().message() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << "System error: " << e.what() << std::endl;
		}
	}

	void CWebSocketServer::printListeningMessage()
	{
		std::cout << "WebSocket based Inspector Agent started" << std::endl;
		std::cout << "Open the following link in your Chrome/Chromium browser: devtools://devtools/bundled/inspector.html?ws=127.0.0.1:" <<  m_nPort << std::endl;
	}

	void CWebSocketServer::waitForFrontendMessageOnPause()
	{
		waitFrontendMessage();
	}

	void CWebSocketServer::waitFrontendMessage()
	{
		beast::flat_buffer oBuffer;
		m_oWs->read(oBuffer);
		std::string message = boost::beast::buffers_to_string(oBuffer.data());
		m_fOnMessage(message);
	}

}
