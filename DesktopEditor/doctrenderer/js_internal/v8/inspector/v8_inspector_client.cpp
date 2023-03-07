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
