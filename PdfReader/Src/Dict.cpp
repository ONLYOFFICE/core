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
#include <stddef.h>
#include <string.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "XRef.h"
#include "Dict.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Dict
	//------------------------------------------------------------------------

	Dict::Dict(XRef *pXref)
	{
		m_pXref = pXref;
		m_arrEntries = NULL;
		m_nEntriesSize = m_nEntriesCount = 0;
		m_nRef = 1;
	}

	Dict::~Dict()
	{
		for (int nIndex = 0; nIndex < m_nEntriesCount; ++nIndex)
		{
			MemUtilsFree(m_arrEntries[nIndex].sKey);
			m_arrEntries[nIndex].oValue.Free();
		}
		MemUtilsFree(m_arrEntries);
	}

	void Dict::AddItem(char *sKey, Object *pValue)
	{
		if (m_nEntriesCount == m_nEntriesSize)
		{
			if (0 == m_nEntriesCount)
			{
				m_nEntriesSize = 8;
			}
			else
			{
				m_nEntriesSize *= 2;
			}
			m_arrEntries = (DictEntry *)MemUtilsReallocArray(m_arrEntries, m_nEntriesSize, sizeof(DictEntry));
		}
		m_arrEntries[m_nEntriesCount].sKey = sKey;
		m_arrEntries[m_nEntriesCount].oValue = *pValue;
		++m_nEntriesCount;
	}

	inline DictEntry *Dict::Find(char *sKey)
	{
		for (int nIndex = 0; nIndex < m_nEntriesCount; ++nIndex)
		{
			if (!strcmp(sKey, m_arrEntries[nIndex].sKey))
				return &m_arrEntries[nIndex];
		}
		return NULL;
	}

	bool Dict::CheckType(char *sType)
	{
		DictEntry *pEntry = NULL;

		return (pEntry = Find("Type")) && pEntry->oValue.IsName(sType);
	}

	Object *Dict::Search(char *sKey, Object *pObj)
	{
		DictEntry *pEntry = NULL;

		return (pEntry = Find(sKey)) ? pEntry->oValue.Fetch(m_pXref, pObj) : pObj->InitNull();
	}

	Object *Dict::SearchAndCopy(char *sKey, Object *pObj)
	{
		DictEntry *pEntry = NULL;

		return (pEntry = Find(sKey)) ? pEntry->oValue.Copy(pObj) : pObj->InitNull();
	}

	char *Dict::GetKey(int nIndex)
	{
		return m_arrEntries[nIndex].sKey;
	}

	Object *Dict::GetValue(int nIndex, Object *pObj)
	{
		return m_arrEntries[nIndex].oValue.Fetch(m_pXref, pObj);
	}

	Object *Dict::GetValueCopy(int nIndex, Object *pObj)
	{
		return m_arrEntries[nIndex].oValue.Copy(pObj);
	}
}