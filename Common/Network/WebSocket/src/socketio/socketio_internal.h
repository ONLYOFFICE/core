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
