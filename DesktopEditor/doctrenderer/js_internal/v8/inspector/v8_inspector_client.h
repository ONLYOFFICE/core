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
