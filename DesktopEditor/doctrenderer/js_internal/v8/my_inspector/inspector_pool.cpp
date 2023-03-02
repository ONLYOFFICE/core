#include "inspector_pool.h"

Inspector& CInspectorPool::addInspector(NSJSBase::CJSContext* context)
{
	std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
				std::piecewise_construct
				, std::forward_as_tuple(context)
				, std::forward_as_tuple(
					context
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
CInspectorPool::getInspector(NSJSBase::CJSContext* context)
{
	storage_t::iterator iter = m_Inspectors.find(context);
	if (iter == m_Inspectors.end())
	{
		return addInspector(context);
	}
	return iter->second;
}

void CInspectorPool::disposeInspector(NSJSBase::CJSContext* context)
{
	storage_t::iterator iter = m_Inspectors.find(context);
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
