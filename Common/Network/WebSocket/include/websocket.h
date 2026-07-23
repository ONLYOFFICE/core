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

#ifndef _WEB_SOCKET_H_
#define _WEB_SOCKET_H_

#include "../../../kernel_config.h"
#include <string>
#include <memory>
#include <map>

namespace NSNetwork
{
    namespace NSWebSocket
    {
        class IWebSocket
        {
        public:
            virtual void open(const std::map<std::string, std::string>& query) = 0;
            virtual void send(const std::string& message) = 0;
            virtual void close() = 0;
            virtual void setUrl(const std::string& url) = 0;
            virtual bool setReconnectInfo(const int& attemtCount, const int& delay, const int& delayMax) { return false; }
            virtual ~IWebSocket() {}
        };

        class IListener
        {
        public:
            virtual void onMessage(const std::string& message) = 0;
            virtual void onOpen() = 0;
            virtual void onError(const std::string& error) = 0;
            virtual void onClose(int code, const std::string& reason) = 0;
            virtual ~IListener() {}
        };

        // type : { "ixwebsocket" | "socketRocket" | "socketio" }
        KERNEL_DECL std::shared_ptr<IWebSocket> createWebsocket(const std::string& type, std::shared_ptr<IListener> listener, const std::string& url = "");
    }
}

#endif /* _WEB_SOCKET_H_ */
