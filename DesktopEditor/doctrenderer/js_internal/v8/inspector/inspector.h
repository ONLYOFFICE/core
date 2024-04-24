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

#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "websocket_server.h"
#include "v8_inspector_client.h"

#include <functional>

// To use inspector set V8_USE_INSPECTOR environment variable to any non-zero value
// Unset V8_USE_INSPECTOR or set it to '0' for disabling the inspector


// Inspector code structure based on "V8 Inspector simple examle" from this repository:
// https://github.com/ahmadov/v8_inspector_example

namespace NSJSBase
{

	class CInspector
	{
	private:
		// pointer to V8 isolate in which scripts/functions are executed
		v8::Isolate* m_pIsolate;

		std::unique_ptr<CWebSocketServer> m_pWebsocketServer;
		std::unique_ptr<CV8InspectorClientImpl> m_pIspectorClient;

	private:
		void onMessage(std::string& sMessage);
		void sendMessage(const std::string& sMessage);
		bool waitForFrontendMessage();

	public:
		CInspector(v8::Isolate* pIsolate, int nContextGroupId);

		void startAgent(bool bIsBreakOnStart = true);
	};

}

#endif // INSPECTOR_H
