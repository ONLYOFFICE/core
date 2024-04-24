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

#include "v8_inspector_client.h"

#include <libplatform/libplatform.h>

#include "utils.h"

namespace NSJSBase
{

	CV8InspectorClientImpl::CV8InspectorClientImpl(
		v8::Platform* pPlatform,
		v8::Isolate* pIsolate,
		int nContextGroupId,
		const std::function<void(std::string)>& fOnResponse,
		const std::function<bool(void)>& fOnWaitFrontendMessageOnPause)
		: m_pPlatform(pPlatform)
		, m_pIsolate(pIsolate)
		, m_nContextGroupId(nContextGroupId)
		, m_fOnWaitFrontendMessageOnPause(std::move(fOnWaitFrontendMessageOnPause))
	{
		v8::Local<v8::Context> context = m_pIsolate->GetCurrentContext();
		// initialize all V8 inspector stuff
		m_pChannel.reset(new CV8InspectorChannelImpl(m_pIsolate, fOnResponse));
		m_pInspector = v8_inspector::V8Inspector::create(m_pIsolate, this);
		m_pSession = m_pInspector->connect(m_nContextGroupId, m_pChannel.get(), v8_inspector::StringView());
		context->SetAlignedPointerInEmbedderData(1, this);

		v8_inspector::StringView oContextName = convertToStringView("inspector" + std::to_string(nContextGroupId));
		m_pInspector->contextCreated(v8_inspector::V8ContextInfo(context, m_nContextGroupId, oContextName));
	}

	void CV8InspectorClientImpl::dispatchProtocolMessage(const v8_inspector::StringView& oMessage)
	{
		m_pSession->dispatchProtocolMessage(oMessage);
	}

	void CV8InspectorClientImpl::runMessageLoopOnPause(int nContextGroupId)
	{
		if (m_bRunNestedLoop)
		{
			return;
		}
		m_bTerminated = false;
		m_bRunNestedLoop = true;
		while (!m_bTerminated && m_fOnWaitFrontendMessageOnPause())
		{
			while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) {}
		}
		m_bTerminated = true;
		m_bRunNestedLoop = false;
	}

	void CV8InspectorClientImpl::quitMessageLoopOnPause()
	{
		m_bTerminated = true;
	}

	v8::Local<v8::Context> CV8InspectorClientImpl::ensureDefaultContextInGroup(int nContextGroupId)
	{
		return m_pIsolate->GetCurrentContext();
	}

	void CV8InspectorClientImpl::schedulePauseOnNextStatement(const v8_inspector::StringView& oReason)
	{
		m_pSession->schedulePauseOnNextStatement(oReason, {});
	}

	void CV8InspectorClientImpl::waitFrontendMessageOnPause()
	{
		m_bTerminated = false;
	}

}
