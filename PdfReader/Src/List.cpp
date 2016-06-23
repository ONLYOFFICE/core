#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "List.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CList
	//------------------------------------------------------------------------

	CList::CList()
	{
		m_nItemSize = 8;
		m_ppData = (void **)MemUtilsMallocArray(m_nItemSize, sizeof(void*));
		m_nCount = 0;
		m_nIncreament = 0;
	}

	CList::CList(int nSize)
	{
		m_nItemSize = nSize;
		m_ppData = (void **)MemUtilsMallocArray(m_nItemSize, sizeof(void*));
		m_nCount = 0;
		m_nIncreament = 0;
	}

	CList::~CList()
	{
		MemUtilsFree(m_ppData);
	}

	void CList::Append(void *pItem)
	{
		if (m_nCount >= m_nItemSize)
			Expand();
		m_ppData[m_nCount++] = pItem;
	}

	void CList::Append(CList *pList)
	{
		while (m_nCount + pList->m_nCount > m_nItemSize)
			Expand();
		for (int nIndex = 0; nIndex < pList->m_nCount; ++nIndex)
			m_ppData[m_nCount++] = pList->m_ppData[nIndex];
	}

	void CList::Insert(int nIndex, void *pItem)
	{
		if (0 > nIndex || nIndex > m_nCount)
			return;
		if (m_nCount >= m_nItemSize)
			Expand();
		if (nIndex < m_nCount)
			memmove(m_ppData + nIndex + 1, m_ppData + nIndex, (m_nCount - nIndex) * sizeof(void *));
		m_ppData[nIndex] = pItem;
		++m_nCount;
	}

	void *CList::Delete(int nIndex)
	{
		void *pItem = m_ppData[nIndex];
		if (nIndex < m_nCount - 1)
			memmove(m_ppData + nIndex, m_ppData + nIndex + 1, (m_nCount - nIndex - 1) * sizeof(void *));
		--m_nCount;
		if (m_nItemSize - m_nCount >= ((m_nIncreament > 0) ? m_nIncreament : m_nItemSize / 2))
			Shrink();
		return pItem;
	}

	void CList::Sort(int(*CompareFunc)(const void *pItem1, const void *pItem2))
	{
		qsort(m_ppData, m_nCount, sizeof(void *), CompareFunc);
	}

	void CList::Expand()
	{
		m_nItemSize += (m_nIncreament > 0) ? m_nIncreament : m_nItemSize;
		m_ppData = (void **)MemUtilsReallocArray(m_ppData, m_nItemSize, sizeof(void*));
	}

	void CList::Shrink()
	{
		m_nItemSize -= (m_nIncreament > 0) ? m_nIncreament : m_nItemSize / 2;
		m_ppData = (void **)MemUtilsReallocArray(m_ppData, m_nItemSize, sizeof(void*));
	}
}