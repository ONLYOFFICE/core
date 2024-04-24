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

#include "inspector.h"

#include "../v8_base.h"
#include "utils.h"

namespace NSJSBase
{

	CInspector::CInspector(v8::Isolate* pIsolate, int nContextGroupId)
		: m_pIsolate(pIsolate)
	{
		m_pWebsocketServer.reset(
			new CWebSocketServer(
				std::bind(&CInspector::onMessage, this, std::placeholders::_1)
			)
		);
		m_pIspectorClient.reset(
			new CV8InspectorClientImpl(
				CV8Worker::getInitializer().getPlatform(),
				m_pIsolate,
				nContextGroupId,
				std::bind(&CInspector::sendMessage, this, std::placeholders::_1),
				std::bind(&CInspector::waitForFrontendMessage, this)
			)
		);
		m_pWebsocketServer->connect();
	}

	void CInspector::onMessage(std::string& sMessage)
	{
		v8::Local<v8::Object> oJsonObject = parseJson(m_pIsolate->GetCurrentContext(), sMessage);

		if (!oJsonObject.IsEmpty())
		{
			std::string sMethod = getPropertyFromJson(m_pIsolate, oJsonObject, "method");
			if (sMethod == "Runtime.runIfWaitingForDebugger")
			{
				m_pWebsocketServer->isServerReady_ = true;
			}
#ifdef V8_VERSION_89_PLUS

#else
			// In older version of V8 there is an eror "Either url or urlRegex must be specified." on trying to set a breakpoint in CDT.
			// So we have to specify it explicitly before "lineNumber" param
			else if (sMethod == "Debugger.setBreakpointByUrl" && sMessage.find("\"url") == sMessage.npos)
			{
				int pos = sMessage.find("\"lineNumber\"");
				sMessage.insert(pos, "\"url\":\"\",");
			}
#endif
		}

		v8_inspector::StringView oProtocolMessage = convertToStringView(sMessage);
		m_pIspectorClient->dispatchProtocolMessage(oProtocolMessage);
	}

	void CInspector::sendMessage(const std::string& sMessage)
	{
		m_pWebsocketServer->sendMessage(sMessage);
	}

	void CInspector::startAgent(bool bIsBreakOnStart)
	{
		if (bIsBreakOnStart)
		{
			m_pIspectorClient->schedulePauseOnNextStatement(convertToStringView("debugging"));
		}
	}

	bool CInspector::waitForFrontendMessage()
	{
		m_pWebsocketServer->waitForFrontendMessageOnPause();
		return true;
	}

}
