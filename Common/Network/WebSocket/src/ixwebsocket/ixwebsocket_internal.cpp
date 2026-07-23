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

#include "ixwebsocket_internal.h"

namespace NSNetwork
{
    namespace NSWebSocket
    {
        void CIXWebSocket::open(const std::map<std::string, std::string>& query)
        {
            ix::SocketTLSOptions tls;
            tls.caFile = "NONE";
            webSocket.setTLSOptions(tls);
            webSocket.setUrl(url);
            std::function<void(const ix::WebSocketMessagePtr&)> f = std::bind(&CIXWebSocket::receive, this, std::placeholders::_1);
            webSocket.setOnMessageCallback(f);
            webSocket.start();
        }

        void CIXWebSocket::receive(const ix::WebSocketMessagePtr& msg)
        {
            if (msg->type == ix::WebSocketMessageType::Message)
            {
                CWebWorkerBase::listener->onMessage(msg->str);
            }
            else if (msg->type == ix::WebSocketMessageType::Open)
            {
                CWebWorkerBase::listener->onOpen();
            }
            else if (msg->type == ix::WebSocketMessageType::Error)
            {
                CWebWorkerBase::listener->onError(msg->errorInfo.reason);
            }
            else if (msg->type == ix::WebSocketMessageType::Close)
            {
                CWebWorkerBase::listener->onClose(msg->closeInfo.code, msg->closeInfo.reason);
            }
        }

        void CIXWebSocket::send(const std::string& message)
        {
            webSocket.send(message);
        }

        void CIXWebSocket::close()
        {
            webSocket.stop();
        }

        CIXWebSocket::~CIXWebSocket() = default;
    }
}
