#ifndef INSPECTORPOOL_H
#define INSPECTORPOOL_H

#include "inspector.h"
#include <unordered_map>

class CInspectorPool
{
	using storage_t = std::unordered_map<NSJSBase::CJSContext*, Inspector>;

	storage_t m_Inspectors;

	// add new inspector
	Inspector& addInspector(NSJSBase::CJSContext* context);

	// port for server
	static uint16_t getPort();
	// group id for V8 internals
	static int getContextGroupId();

	// private due to singletone pattern
	CInspectorPool() = default;
	~CInspectorPool() = default;

public:
	CInspectorPool(const CInspectorPool&) = delete;
	CInspectorPool(CInspectorPool&&) = delete;
	CInspectorPool& operator=(const CInspectorPool&) = delete;
	CInspectorPool& operator=(CInspectorPool&&) = delete;

	// get instance
	static CInspectorPool& get();
	// get inspector by JSContext
	Inspector& getInspector(NSJSBase::CJSContext* context);
	// remove inspector from JSContext
	void disposeInspector(NSJSBase::CJSContext* context);
};

#endif // INSPECTORPOOL_H
