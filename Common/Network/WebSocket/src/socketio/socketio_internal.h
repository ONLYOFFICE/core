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

#ifndef _IO_WEB_SOCKET_H_
#define _IO_WEB_SOCKET_H_

#include "../websocketbase.h"
#include "../../../../../DesktopEditor/graphics/TemporaryCS.h"

namespace NSNetwork
{
    namespace NSWebSocket
    {
        class CIOWebSocket;
        class CIOWebSocket_private
        {
        protected:
            CIOWebSocket* m_base;
            NSCriticalSection::CRITICAL_SECTION m_oCS;
            NSCriticalSection::CRITICAL_SECTION m_oCS_Events;
        public:
            CIOWebSocket_private(CIOWebSocket* base)
            {
                m_base = base;
                m_oCS.InitializeCriticalSection();
                m_oCS_Events.InitializeCriticalSection();
            }
            virtual ~CIOWebSocket_private()
            {
                m_oCS_Events.DeleteCriticalSection();
                m_oCS.DeleteCriticalSection();
            }

            virtual void open(const std::map<std::string, std::string>& query) = 0;
            virtual void send(const std::string& message) = 0;
            virtual void close() = 0;
            virtual void setReconnectInfo(const int& attemtCount, const int& delay, const int& delayMax) = 0;
        };

        class CIOWebSocket: public CWebWorkerBase
        {
        private:
            CIOWebSocket_private* m_internal;
           
        public:
            CIOWebSocket(const std::string& url, std::shared_ptr<IListener> listener);
            virtual ~CIOWebSocket();

        public:
            virtual void open(const std::map<std::string, std::string>& query) override;
            virtual void send(const std::string& message) override;
            virtual void close() override;
            virtual bool setReconnectInfo(const int& attemtCount, const int& delay, const int& delayMax);

            friend class CIOWebSocket_private;
            friend class CIOWebSocket_private_tls;
            friend class CIOWebSocket_private_no_tls;
        };
    }
}

#endif /* _IO_WEB_SOCKET_H_ */
