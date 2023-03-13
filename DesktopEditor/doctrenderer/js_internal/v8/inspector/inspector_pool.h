#ifndef INSPECTOR_POOL_H
#define INSPECTOR_POOL_H

#include "inspector.h"

#include <unordered_map>

namespace NSJSBase
{

	class CInspectorPool
	{
		using storage_t = std::unordered_map<v8::Isolate*, CInspector>;

		storage_t m_Inspectors;

		// add new inspector
		CInspector& addInspector(v8::Isolate* pIsolate);

		// group id for V8 internals
		static int getContextGroupId();

		// private due to singleton pattern
		CInspectorPool() = default;
		~CInspectorPool() = default;

	public:
		CInspectorPool(const CInspectorPool&) = delete;
		CInspectorPool(CInspectorPool&&) = delete;
		CInspectorPool& operator=(const CInspectorPool&) = delete;
		CInspectorPool& operator=(CInspectorPool&&) = delete;

		// get instance
		static CInspectorPool& get();
		// get inspector by isolate
		CInspector& getInspector(v8::Isolate* pIsolate);
		// remove inspector from isolate
		void disposeInspector(v8::Isolate* pIsolate);
	};

}

#endif // INSPECTOR_POOL_H
