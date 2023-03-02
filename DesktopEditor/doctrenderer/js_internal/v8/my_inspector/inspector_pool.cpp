#include "inspector_pool.h"

#include "../v8_base.h"

Inspector& CInspectorPool::addInspector(v8::Isolate* isolate)
{
	std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
				std::piecewise_construct
				, std::forward_as_tuple(isolate)
				, std::forward_as_tuple(
					isolate
					, getPort()
					, getContextGroupId()
				)
			);
	Inspector& inspector = result.first->second;
	return inspector;
}

uint16_t CInspectorPool::getPort()
{
	static uint16_t initialPort{8080};
	return initialPort++;
}

int CInspectorPool::getContextGroupId()
{
	static int initialId{1};
	return initialId++;
}

Inspector&
CInspectorPool::getInspector(v8::Isolate* isolate)
{
	storage_t::iterator iter = m_Inspectors.find(isolate);
	if (iter == m_Inspectors.end())
	{
		return addInspector(isolate);
	}
	return iter->second;
}

void CInspectorPool::disposeInspector(v8::Isolate* isolate)
{
	storage_t::iterator iter = m_Inspectors.find(isolate);
	if (m_Inspectors.end() != iter)
	{
		m_Inspectors.erase(iter);
	}
}

CInspectorPool&
CInspectorPool::get()
{
	static CInspectorPool pool;
	return pool;
}
