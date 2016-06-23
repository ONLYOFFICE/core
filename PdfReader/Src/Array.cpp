#include <stdlib.h>
#include <stddef.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "Array.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Array
	//------------------------------------------------------------------------

	Array::Array(XRef *pXRef)
	{
		m_pXRef = pXRef;
		m_arrItems = NULL;
		m_nItemSize = m_nCount = 0;
		m_nRef = 1;
	}

	Array::~Array()
	{
		for (int nIndex = 0; nIndex < m_nCount; ++nIndex)
			m_arrItems[nIndex].Free();
		MemUtilsFree(m_arrItems);
	}

	void Array::Add(Object *pItem)
	{
		if (m_nCount == m_nItemSize)
		{
			if (m_nCount == 0)
			{
				m_nItemSize = 8;
			}
			else
			{
				m_nItemSize *= 2;
			}
			m_arrItems = (Object *)MemUtilsReallocArray(m_arrItems, m_nItemSize, sizeof(Object));
		}
		m_arrItems[m_nCount] = *pItem;
		++m_nCount;
	}

	Object *Array::Get(int nIndex, Object *pObject)
	{
		if (nIndex < 0 || nIndex >= m_nCount)
		{
			return pObject->InitNull();
		}
		return m_arrItems[nIndex].Fetch(m_pXRef, pObject);
	}

	Object *Array::GetCopy(int nIndex, Object *pObject)
	{
		if (nIndex < 0 || nIndex >= m_nCount)
		{
			return pObject->InitNull();
		}
		return m_arrItems[nIndex].Copy(pObject);
	}
}