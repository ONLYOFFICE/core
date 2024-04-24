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

#ifndef V8_INSPECTOR_CLIENT_IMPL_H
#define V8_INSPECTOR_CLIENT_IMPL_H

#include <v8.h>
#include <v8-inspector.h>

#include "v8_inspector_channel.h"

namespace NSJSBase
{

	class CV8InspectorClientImpl final: public v8_inspector::V8InspectorClient
	{
	private:
		v8::Platform* m_pPlatform;
		// V8 Inspector stuff
		std::unique_ptr<v8_inspector::V8Inspector> m_pInspector;
		std::unique_ptr<v8_inspector::V8InspectorSession> m_pSession;
		std::unique_ptr<CV8InspectorChannelImpl> m_pChannel;
		// V8 isolate
		v8::Isolate* m_pIsolate;
		// context group id
		const int m_nContextGroupId;
		// callbacks
		std::function<bool(void)> m_fOnWaitFrontendMessageOnPause;
		// server message loop terminate flag
		bool m_bTerminated = false;
		// flag for preventing server from falling into two loops
		bool m_bRunNestedLoop = false;

	private:
		v8::Local<v8::Context> ensureDefaultContextInGroup(int nContextGroupId) override;

	public:
		CV8InspectorClientImpl(
			v8::Platform* pPlatform,
			v8::Isolate* pIsolate,
			int nContextGroupId,
			const std::function<void(std::string)>& fOnResponse,
			const std::function<bool(void)>& fOnWaitFrontendMessageOnPause);
		// send message to V8 internals
		void dispatchProtocolMessage(const v8_inspector::StringView& oMessage);
		// overriden interface methods
		void runMessageLoopOnPause(int nContextGroupId) override;
		void quitMessageLoopOnPause() override;
		// pause on next statement
		void schedulePauseOnNextStatement(const v8_inspector::StringView& oReason);
		// wait for message from frontend
		void waitFrontendMessageOnPause();

	};

}

#endif // V8_INSPECTOR_CLIENT_IMPL_H
