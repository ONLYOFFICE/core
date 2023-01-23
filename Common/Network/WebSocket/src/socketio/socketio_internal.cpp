/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "socketio_internal.h"
#include <iostream>
#include "../../../../3dParty/socketio/socket.io-client-cpp/src/internal/sio_packet.h"
#include "../../../../3dParty/socketio/socket.io-client-cpp/src/sio_client.h"
#include "../../../../../DesktopEditor/graphics/TemporaryCS.h"

namespace NSNetwork
{
    namespace NSWebSocket
    {
        class CIOWebSocket_private
        {
        public:
            sio::client m_socket;
            NSCriticalSection::CRITICAL_SECTION m_oCS;
            CIOWebSocket* m_base;

        public:
            CIOWebSocket_private(CIOWebSocket* base)
            {
                m_base = base;
                m_oCS.InitializeCriticalSection();
            }
            ~CIOWebSocket_private()
            {
                m_socket.close();
                m_oCS.DeleteCriticalSection();
            }

        public:
            void event_onConnected()
            {
                m_base->listener->onOpen();
            }
            void event_onClose(sio::client::close_reason const& reason)
            {
                m_base->listener->onClose(0, "");
            }
            void event_onFail()
            {
                m_base->listener->onError("");
            }

        public:
            void open(const std::map<std::string, std::string>& query)
            {
                m_socket.set_open_listener (std::bind(&CIOWebSocket_private::event_onConnected, this));
                m_socket.set_close_listener(std::bind(&CIOWebSocket_private::event_onClose, this, std::placeholders::_1));
                m_socket.set_fail_listener (std::bind(&CIOWebSocket_private::event_onFail, this));

                sio::message::ptr objAuth = sio::object_message::create();
                //std::string sAuth;

                std::map<std::string, std::string> queryDst = query;
                std::map<std::string, std::string>::iterator iterAuth = queryDst.find("token");
                if (iterAuth != queryDst.end())
                {
                    objAuth->get_map()["token"] = sio::string_message::create(iterAuth->second);
                    //sAuth = "{\"token\":\"" + iterAuth->second + "\"}";
                    queryDst.erase(iterAuth);
                }

                //webSocket.connect(url, queryDst, sio::string_message::create(sAuth));
                m_socket.connect(m_base->url, queryDst, objAuth);

                m_socket.socket()->on("message", [&](sio::event& event){
                    CTemporaryCS oCS(&m_oCS);
                    const sio::message::ptr& message = event.get_message();
                    if (!message)
                        return;

                    // TODO: пока только текстовые и json команды
                    switch (message->get_flag())
                    {
                    case sio::message::flag_null:
                    {
                        m_base->listener->onMessage("null");
                        break;
                    }
                    case sio::message::flag_integer:
                    case sio::message::flag_double:
                    case sio::message::flag_boolean:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio::message::flag_binary:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio::message::flag_array:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio::message::flag_object:
                    {
                        sio::packet_manager manager;

                        std::stringstream ss;
                        sio::packet packet("/", message);
                        manager.encode( packet, [&](bool isBinary, std::shared_ptr<const std::string> const& json)
                        {
                            ss << *json;
                        });
                        manager.reset();

                        std::string result = ss.str();

                        std::size_t indexList = result.find('[');
                        std::size_t indexObject = result.find('{');
                        std::size_t indexString = result.find('"');

                        std::size_t index = indexList;
                        if (indexObject != std::string::npos && indexObject < index)
                            index = indexObject;
                        if (indexString != std::string::npos && indexString < index)
                            index = indexString;

                        if (index != std::string::npos)
                            result = result.substr(index);
                        else
                            result = "";

                        m_base->listener->onMessage(result);
                        break;
                    }
                    case sio::message::flag_string:
                    {
                        m_base->listener->onMessage(message->get_string());
                        break;
                    }
                    default:
                        break;
                    }
                });
            }
        };

        CIOWebSocket::CIOWebSocket(const std::string& url, std::shared_ptr<IListener> listener): CWebWorkerBase(url, listener)
        {
            m_internal = new CIOWebSocket_private(this);
        }

        void CIOWebSocket::open(const std::map<std::string, std::string>& query)
        {
            m_internal->open(query);
        }

        void CIOWebSocket::send(const std::string& message_str)
        {
            //CTemporaryCS (&m_internal->m_oCS);

            // если json -то надо объект
            if (0 == message_str.find("{") ||
                0 == message_str.find("["))
            {
                sio::packet_manager manager;
                sio::message::ptr message;
                manager.set_decode_callback([&](sio::packet const& p)
                {
                    message = p.get_message();
                });

                // Magic message type / ID
                manager.put_payload("42" + message_str);
                manager.reset();

                m_internal->m_socket.socket()->emit("message", message);
            }
            else
            {
                m_internal->m_socket.socket()->emit("message", sio::string_message::create(message_str));
            }
        }

        void CIOWebSocket::close()
        {
            //CTemporaryCS (&m_internal->m_oCS);
            m_internal->m_socket.socket()->off_all();
            m_internal->m_socket.close();
        }

        CIOWebSocket::~CIOWebSocket()
        {
            delete m_internal;
        }
    }
}
