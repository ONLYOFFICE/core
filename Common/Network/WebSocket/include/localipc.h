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

#ifndef _CORE_NETWORK_LOCALIPC_H_
#define _CORE_NETWORK_LOCALIPC_H_

#include "../../../kernel_config.h"
#include <string>
#include <memory>

namespace NSNetwork
{
	// Local loopback (127.0.0.1) request-response IPC over TCP, framed as
	// [uint32 little-endian length][JSON payload]. Reuses the standalone asio
	// already vendored for socketio. Available only in the libsocketio build
	// branch (see websocket.pri, USE_LOCAL_IPC).
	namespace NSLocalIPC
	{
		// Handles one request and returns the response body. Called from a
		// connection worker thread - implementations must be thread-safe.
		class IRequestHandler
		{
		public:
			virtual std::string onRequest(const std::string& sRequest) = 0;
			virtual ~IRequestHandler() {}
		};

		class IServer
		{
		public:
			// Binds 127.0.0.1:nPort (nPort == 0 -> OS picks a free port).
			virtual bool start(unsigned short nPort) = 0;
			// Actual bound port (useful when started with 0).
			virtual unsigned short port() const = 0;
			virtual void stop() = 0;
			virtual ~IServer() {}
		};

		class IClient
		{
		public:
			virtual bool connect(unsigned short nPort, int nTimeoutMs = 5000) = 0;
			// Synchronous round-trip. Returns false on connection/IO error.
			virtual bool request(const std::string& sRequest, std::string& sResponse) = 0;
			virtual void close() = 0;
			virtual ~IClient() {}
		};

		KERNEL_DECL std::shared_ptr<IServer> createLocalIpcServer(std::shared_ptr<IRequestHandler> pHandler);
		KERNEL_DECL std::shared_ptr<IClient> createLocalIpcClient();
	}
}

#endif // _CORE_NETWORK_LOCALIPC_H_
