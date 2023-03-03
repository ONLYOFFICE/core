#include "inspector_pool.h"

#include "../v8_base.h"

Inspector& CInspectorPool::addInspector(v8::Isolate* pIsolate)
{
	std::pair<storage_t::iterator, bool> result = m_Inspectors.emplace(
				std::piecewise_construct
				, std::forward_as_tuple(pIsolate)
				, std::forward_as_tuple(
					pIsolate
					, getPort()
					, getContextGroupId()
				)
			);
	Inspector& oInspector = result.first->second;
	return oInspector;
}

uint16_t CInspectorPool::getPort()
{
	static uint16_t nInitialPort{8080};
	return nInitialPort++;
}

int CInspectorPool::getContextGroupId()
{
	static int nInitialId{1};
	return nInitialId++;
}

Inspector&
CInspectorPool::getInspector(v8::Isolate* pIsolate)
{
	storage_t::iterator iter = m_Inspectors.find(pIsolate);
	if (iter == m_Inspectors.end())
	{
		return addInspector(pIsolate);
	}
	return iter->second;
}

void CInspectorPool::disposeInspector(v8::Isolate* pIsolate)
{
	storage_t::iterator iter = m_Inspectors.find(pIsolate);
	if (m_Inspectors.end() != iter)
	{
		m_Inspectors.erase(iter);
	}
}

CInspectorPool&
CInspectorPool::get()
{
	static CInspectorPool oPool;
	return oPool;
}
