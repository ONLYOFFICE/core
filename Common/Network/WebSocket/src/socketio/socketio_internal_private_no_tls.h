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

///////////////////////////////////////////////////////////
/// auto generated file. please use generator (make.py) ///
///////////////////////////////////////////////////////////

#pragma once
#include "socketio_internal.h"
#include <iostream>
#include "../../../../3dParty/socketio/socket.io-client-cpp/src_no_tls/internal/sio_packet.h"
#include "../../../../3dParty/socketio/socket.io-client-cpp/src_no_tls/sio_client.h"
#include <memory>
#include "../../../../../DesktopEditor/graphics/BaseThread.h"

namespace NSNetwork
{
    namespace NSWebSocket
    {
        class CIOWebSocket_private_no_tls : public CIOWebSocket_private
        {
        public:
            std::shared_ptr<sio_no_tls::client> m_socket;
            // problem closing socket while it's not yet connected
            bool m_connecting_in_process;
            bool m_closing_in_progress;

        public:
            CIOWebSocket_private_no_tls(CIOWebSocket* base) : CIOWebSocket_private(base)
            {
                m_connecting_in_process = false;
                m_closing_in_progress = false;
                m_base = base;
            }
            ~CIOWebSocket_private_no_tls()
            {
                close();
            }

        public:
            void event_onConnected()
            {
                CTemporaryCS oCS(&m_oCS_Events);
                m_connecting_in_process = false;

                if (!m_closing_in_progress)
                    m_base->listener->onOpen();
            }
            void event_onClose(sio_no_tls::client::close_reason const& reason)
            {
                CTemporaryCS oCS(&m_oCS_Events);
                m_connecting_in_process = false;

                if (!m_closing_in_progress)
                    m_base->listener->onClose(0, "");
            }
            void event_onFail()
            {
                CTemporaryCS oCS(&m_oCS_Events);
                m_connecting_in_process = false;

                if (!m_closing_in_progress)
                    m_base->listener->onError("");
            }

        public:
            virtual void open(const std::map<std::string, std::string>& query) override
            {
                m_socket = std::make_shared<sio_no_tls::client>();
                m_socket->set_open_listener (std::bind(&CIOWebSocket_private_no_tls::event_onConnected, this));
                m_socket->set_close_listener(std::bind(&CIOWebSocket_private_no_tls::event_onClose, this, std::placeholders::_1));
                m_socket->set_fail_listener (std::bind(&CIOWebSocket_private_no_tls::event_onFail, this));

                sio_no_tls::message::ptr objAuth = sio_no_tls::object_message::create();
                //std::string sAuth;

                std::map<std::string, std::string> queryDst = query;
                std::map<std::string, std::string>::iterator iterAuth = queryDst.find("token");
                if (iterAuth != queryDst.end())
                {
                    objAuth->get_map()["token"] = sio_no_tls::string_message::create(iterAuth->second);
                    //sAuth = "{\"token\":\"" + iterAuth->second + "\"}";
                    queryDst.erase(iterAuth);
                }

                //webSocket.connect(url, queryDst, sio_no_tls::string_message::create(sAuth));
                m_connecting_in_process = true;
                m_socket->connect(m_base->url, queryDst, objAuth);

                m_socket->socket()->on("message", [&](sio_no_tls::event& event){
                    CTemporaryCS oCS(&m_oCS_Events);
                    if (m_closing_in_progress)
                        return;

                    const sio_no_tls::message::ptr& message = event.get_message();
                    if (!message)
                        return;

                    // TODO: for now only text and json commands
                    switch (message->get_flag())
                    {
                    case sio_no_tls::message::flag_null:
                    {
                        m_base->listener->onMessage("null");
                        break;
                    }
                    case sio_no_tls::message::flag_integer:
                    case sio_no_tls::message::flag_double:
                    case sio_no_tls::message::flag_boolean:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio_no_tls::message::flag_binary:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio_no_tls::message::flag_array:
                    {
                        m_base->listener->onMessage("");
                        break;
                    }
                    case sio_no_tls::message::flag_object:
                    {
                        sio_no_tls::packet_manager manager;

                        std::stringstream ss;
                        sio_no_tls::packet packet("/", message);
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
                    case sio_no_tls::message::flag_string:
                    {
                        m_base->listener->onMessage(message->get_string());
                        break;
                    }
                    default:
                        break;
                    }
                });
            }

            virtual void send(const std::string& message_str) override
            {
                //CTemporaryCS (&m_internal->m_oCS);

                // if json - then need object
                if (0 == message_str.find("{") ||
                    0 == message_str.find("["))
                {
                    sio_no_tls::packet_manager manager;
                    sio_no_tls::message::ptr message;
                    manager.set_decode_callback([&](sio_no_tls::packet const& p)
                    {
                        message = p.get_message();
                    });

                    // Magic message type / ID
                    manager.put_payload("42" + message_str);
                    manager.reset();

                    m_socket->socket()->emit("message", message);
                }
                else
                {
                    m_socket->socket()->emit("message", sio_no_tls::string_message::create(message_str));
                }
            }

            virtual void close() override
            {
                CTemporaryCS oCS(&m_oCS);

                m_oCS_Events.Enter();
                m_closing_in_progress = true;
                m_oCS_Events.Leave();

                // https://github.com/socketio/socket.io-client-cpp/issues/254
                while (m_connecting_in_process)
                    NSThreads::Sleep(50);

                m_socket.reset();
            }

            virtual void setReconnectInfo(const int& attemtCount, const int& delay, const int& delayMax) override
            {
                m_socket->set_reconnect_attempts(attemtCount);
                m_socket->set_reconnect_delay(delay);
                m_socket->set_reconnect_delay_max(delayMax);
            }
        };
    }
}
