/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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