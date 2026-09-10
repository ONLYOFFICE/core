/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "../../include/localipc.h"

#include <thread>
#include <atomic>
#include <vector>
#include <cstdint>

// asio is header-only; it just needs ASIO_STANDALONE + the include path (the
// latter is provided by the libsocketio branch in websocket.pri). Guarded so
// there is no clash with the branch's own -DASIO_STANDALONE.
#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif
#include <asio.hpp>

namespace NSNetwork
{
	namespace NSLocalIPC
	{
		static const size_t kMaxFrame = 64u * 1024u * 1024u; // 64 MB guard

		static void PackLen(uint32_t nLen, unsigned char aOut[4])
		{
			aOut[0] = (unsigned char)( nLen        & 0xFF);
			aOut[1] = (unsigned char)((nLen >> 8)  & 0xFF);
			aOut[2] = (unsigned char)((nLen >> 16) & 0xFF);
			aOut[3] = (unsigned char)((nLen >> 24) & 0xFF);
		}
		static uint32_t UnpackLen(const unsigned char aIn[4])
		{
			return (uint32_t)aIn[0] | ((uint32_t)aIn[1] << 8) |
			       ((uint32_t)aIn[2] << 16) | ((uint32_t)aIn[3] << 24);
		}

		static bool WriteFrame(asio::ip::tcp::socket& oSocket, const std::string& sData)
		{
			unsigned char aLen[4];
			PackLen((uint32_t)sData.size(), aLen);
			asio::error_code ec;
			asio::write(oSocket, asio::buffer(aLen, 4), ec);
			if (ec) return false;
			asio::write(oSocket, asio::buffer(sData.data(), sData.size()), ec);
			return !ec;
		}

		static bool ReadFrame(asio::ip::tcp::socket& oSocket, std::string& sData)
		{
			unsigned char aLen[4];
			asio::error_code ec;
			asio::read(oSocket, asio::buffer(aLen, 4), ec);
			if (ec) return false;
			uint32_t nLen = UnpackLen(aLen);
			if (nLen > kMaxFrame) return false;
			sData.resize(nLen);
			if (nLen > 0)
				asio::read(oSocket, asio::buffer(&sData[0], nLen), ec);
			return !ec;
		}

		// ---- server ----------------------------------------------------------

		class CLocalIpcServer : public IServer
		{
		public:
			CLocalIpcServer(std::shared_ptr<IRequestHandler> pHandler)
				: m_pHandler(pHandler), m_oAcceptor(m_oContext), m_nPort(0), m_bRunning(false)
			{
			}
			virtual ~CLocalIpcServer() { stop(); }

			virtual bool start(unsigned short nPort)
			{
				try
				{
					asio::ip::tcp::endpoint oEndpoint(asio::ip::make_address("127.0.0.1"), nPort);
					m_oAcceptor.open(oEndpoint.protocol());
					m_oAcceptor.set_option(asio::socket_base::reuse_address(true));
					m_oAcceptor.bind(oEndpoint);
					m_oAcceptor.listen();
					m_nPort = m_oAcceptor.local_endpoint().port();
				}
				catch (...)
				{
					return false;
				}

				m_bRunning = true;
				DoAccept();
				m_oThread = std::thread([this]() { m_oContext.run(); });
				return true;
			}

			virtual unsigned short port() const { return m_nPort; }

			virtual void stop()
			{
				if (!m_bRunning)
					return;
				m_bRunning = false;
				asio::error_code ec;
				m_oAcceptor.close(ec);
				m_oContext.stop();
				if (m_oThread.joinable())
					m_oThread.join();
			}

		private:
			void DoAccept()
			{
				auto pSocket = std::make_shared<asio::ip::tcp::socket>(m_oContext);
				m_oAcceptor.async_accept(*pSocket, [this, pSocket](const asio::error_code& ec)
				{
					if (!ec)
					{
						// Handle this connection on its own thread (blocking IO);
						// the handler may block while marshaling to another thread.
						std::thread([this, pSocket]() { HandleConnection(pSocket); }).detach();
					}
					if (m_bRunning)
						DoAccept();
				});
			}

			void HandleConnection(std::shared_ptr<asio::ip::tcp::socket> pSocket)
			{
				std::string sRequest;
				while (ReadFrame(*pSocket, sRequest))
				{
					std::string sResponse = m_pHandler ? m_pHandler->onRequest(sRequest) : std::string("{}");
					if (!WriteFrame(*pSocket, sResponse))
						break;
				}
				asio::error_code ec;
				pSocket->shutdown(asio::ip::tcp::socket::shutdown_both, ec);
				pSocket->close(ec);
			}

		private:
			std::shared_ptr<IRequestHandler> m_pHandler;
			asio::io_context m_oContext;
			asio::ip::tcp::acceptor m_oAcceptor;
			std::thread m_oThread;
			unsigned short m_nPort;
			std::atomic<bool> m_bRunning;
		};

		// ---- client ----------------------------------------------------------

		class CLocalIpcClient : public IClient
		{
		public:
			CLocalIpcClient() : m_oSocket(m_oContext), m_bConnected(false) {}
			virtual ~CLocalIpcClient() { close(); }

			virtual bool connect(unsigned short nPort, int /*nTimeoutMs*/)
			{
				try
				{
					asio::ip::tcp::endpoint oEndpoint(asio::ip::make_address("127.0.0.1"), nPort);
					asio::error_code ec;
					m_oSocket.connect(oEndpoint, ec);
					if (ec)
						return false;
					m_bConnected = true;
					return true;
				}
				catch (...)
				{
					return false;
				}
			}

			virtual bool request(const std::string& sRequest, std::string& sResponse)
			{
				if (!m_bConnected)
					return false;
				if (!WriteFrame(m_oSocket, sRequest))
					return false;
				return ReadFrame(m_oSocket, sResponse);
			}

			virtual void close()
			{
				if (!m_bConnected)
					return;
				asio::error_code ec;
				m_oSocket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
				m_oSocket.close(ec);
				m_bConnected = false;
			}

		private:
			asio::io_context m_oContext;
			asio::ip::tcp::socket m_oSocket;
			bool m_bConnected;
		};

		// ---- factories -------------------------------------------------------

		std::shared_ptr<IServer> createLocalIpcServer(std::shared_ptr<IRequestHandler> pHandler)
		{
			return std::make_shared<CLocalIpcServer>(pHandler);
		}
		std::shared_ptr<IClient> createLocalIpcClient()
		{
			return std::make_shared<CLocalIpcClient>();
		}
	}
}
