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
#include <list>
#include <mutex>
#include <memory>
#include <chrono>
#include <cstdint>

#ifndef _WIN32
#include <poll.h>
#include <cerrno>
#endif

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

		// How often a connection waiting for the next request checks for stop():
		// a blocking read cannot be interrupted portably from another thread.
		static const int kStopCheckMs = 200;

		// Once a request has started arriving, the whole of it must arrive within
		// this time, and the whole response must be taken by the peer within it
		// too. A peer that stalls in the middle of a frame is dropped instead of
		// holding its connection thread - and stop() - forever.
		static const int kFrameTimeoutMs = 30000;

		static std::chrono::steady_clock::time_point FrameDeadline()
		{
			return std::chrono::steady_clock::now() + std::chrono::milliseconds(kFrameTimeoutMs);
		}

		// asio 1.11 has no timed synchronous wait: poll the native socket.
		// > 0 - ready (for a read: data, EOF or error - the read tells),
		// 0 - timeout, < 0 - failure.
		static int WaitReady(asio::ip::tcp::socket& oSocket, bool bWrite, int nTimeoutMs)
		{
#ifdef _WIN32
			SOCKET hSocket = oSocket.native_handle();
			fd_set oSet;
			FD_ZERO(&oSet);
			FD_SET(hSocket, &oSet);
			timeval oTimeout;
			oTimeout.tv_sec = nTimeoutMs / 1000;
			oTimeout.tv_usec = (nTimeoutMs % 1000) * 1000;
			return bWrite ? ::select(0, NULL, &oSet, NULL, &oTimeout)
			              : ::select(0, &oSet, NULL, NULL, &oTimeout);
#else
			pollfd oPoll;
			oPoll.fd = oSocket.native_handle();
			oPoll.events = bWrite ? POLLOUT : POLLIN;
			oPoll.revents = 0;
			int nResult = ::poll(&oPoll, 1, nTimeoutMs);
			if (nResult < 0 && EINTR == errno)
				return 0;
			return nResult;
#endif
		}

		// Server side only: moves exactly nSize bytes over a NON-BLOCKING socket.
		// Whatever the socket can take or give right now is always moved - a
		// response that fits the socket buffer still reaches the peer while the
		// server stops. Only WAITING for the peer is bounded: by oDeadline, and by
		// bRunning in kStopCheckMs slices. False when the peer closes or fails, or
		// a wait is given up.
		static bool TransferExact(asio::ip::tcp::socket& oSocket, bool bWrite, char* pData, size_t nSize,
		                          const std::atomic<bool>& bRunning,
		                          const std::chrono::steady_clock::time_point& oDeadline)
		{
			size_t nDone = 0;
			while (nDone < nSize)
			{
				asio::error_code ec;
				size_t nMoved = bWrite ? oSocket.write_some(asio::buffer(pData + nDone, nSize - nDone), ec)
				                       : oSocket.read_some(asio::buffer(pData + nDone, nSize - nDone), ec);
				if (!ec)
				{
					nDone += nMoved;
					continue;
				}
				if (ec != asio::error::would_block && ec != asio::error::try_again)
					return false; // eof included: the peer closed

				// the peer is not there yet: wait for it, but not past stop() or the deadline
				while (true)
				{
					if (!bRunning || std::chrono::steady_clock::now() >= oDeadline)
						return false;
					int nReady = WaitReady(oSocket, bWrite, kStopCheckMs);
					if (nReady < 0)
						return false;
					if (nReady > 0)
						break;
				}
			}
			return true;
		}

		static bool ReadFrameUntil(asio::ip::tcp::socket& oSocket, std::string& sData, const std::atomic<bool>& bRunning,
		                           const std::chrono::steady_clock::time_point& oDeadline)
		{
			unsigned char aLen[4];
			if (!TransferExact(oSocket, false, (char*)aLen, 4, bRunning, oDeadline))
				return false;
			uint32_t nLen = UnpackLen(aLen);
			if (nLen > kMaxFrame) return false;
			sData.resize(nLen);
			return 0 == nLen || TransferExact(oSocket, false, &sData[0], nLen, bRunning, oDeadline);
		}

		static bool WriteFrameUntil(asio::ip::tcp::socket& oSocket, const std::string& sData, const std::atomic<bool>& bRunning,
		                            const std::chrono::steady_clock::time_point& oDeadline)
		{
			unsigned char aLen[4];
			PackLen((uint32_t)sData.size(), aLen);
			if (!TransferExact(oSocket, true, (char*)aLen, 4, bRunning, oDeadline))
				return false;
			return sData.empty() || TransferExact(oSocket, true, const_cast<char*>(sData.data()), sData.size(), bRunning, oDeadline);
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

				// Nothing may run in this object or in the handler after stop(): the
				// owner can be a plugin library that is unloaded right after it. An
				// idle connection notices m_bRunning within kStopCheckMs, and so does
				// one waiting for the rest of a frame; a busy one once its request is
				// answered.
				std::list<std::shared_ptr<TConnection> > arConnections;
				{
					std::lock_guard<std::mutex> oLock(m_oConnectionsMutex);
					arConnections.swap(m_arConnections);
				}
				for (std::list<std::shared_ptr<TConnection> >::iterator it = arConnections.begin(); it != arConnections.end(); ++it)
				{
					if ((*it)->oThread.joinable())
						(*it)->oThread.join();
				}
			}

		private:
			struct TConnection
			{
				std::thread oThread;
				std::atomic<bool> bFinished;
				TConnection() : bFinished(false) {}
			};

			void DoAccept()
			{
				auto pSocket = std::make_shared<asio::ip::tcp::socket>(m_oContext);
				m_oAcceptor.async_accept(*pSocket, [this, pSocket](const asio::error_code& ec)
				{
					if (!ec && m_bRunning)
					{
						// Handle this connection on its own thread (synchronous IO);
						// the handler may block while marshaling to another thread.
						std::lock_guard<std::mutex> oLock(m_oConnectionsMutex);
						JoinFinished();

						std::shared_ptr<TConnection> pConnection = std::make_shared<TConnection>();
						TConnection* pState = pConnection.get(); // owned by m_arConnections until joined
						pConnection->oThread = std::thread([this, pSocket, pState]()
						{
							HandleConnection(pSocket);
							pState->bFinished = true;
						});
						m_arConnections.push_back(pConnection);
					}
					if (m_bRunning)
						DoAccept();
				});
			}

			// m_oConnectionsMutex is held. Closed connections are joined (instantly),
			// so their threads do not pile up while the server runs.
			void JoinFinished()
			{
				for (std::list<std::shared_ptr<TConnection> >::iterator it = m_arConnections.begin(); it != m_arConnections.end(); )
				{
					if ((*it)->bFinished)
					{
						if ((*it)->oThread.joinable())
							(*it)->oThread.join();
						it = m_arConnections.erase(it);
					}
					else
						++it;
				}
			}

			void HandleConnection(std::shared_ptr<asio::ip::tcp::socket> pSocket)
			{
				// Non-blocking, so that no read or write of a frame can outlast its
				// deadline or stop() (see TransferExact).
				asio::error_code ecMode;
				pSocket->non_blocking(true, ecMode);
				if (!ecMode)
				{
					std::string sRequest;
					while (m_bRunning)
					{
						int nReady = WaitReady(*pSocket, false, kStopCheckMs);
						if (0 == nReady)
							continue;
						if (nReady < 0 || !ReadFrameUntil(*pSocket, sRequest, m_bRunning, FrameDeadline()))
							break;
						std::string sResponse = m_pHandler ? m_pHandler->onRequest(sRequest) : std::string("{}");
						if (!WriteFrameUntil(*pSocket, sResponse, m_bRunning, FrameDeadline()))
							break;
					}
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

			std::mutex m_oConnectionsMutex;
			std::list<std::shared_ptr<TConnection> > m_arConnections;
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
