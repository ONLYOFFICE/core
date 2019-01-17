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
#include <string.h>
#include "MemoryUtils.h"
#include "NameToCharCode.h"

namespace PdfReader
{
	struct NameToCharCodeEntry
	{
		char    *sName;
		CharCode nCode;
	};

	//------------------------------------------------------------------------

	NameToCharCode::NameToCharCode()
	{
		m_nSize = 31;
		m_nLen = 0;
		m_pTable = (NameToCharCodeEntry *)MemUtilsMallocArray(m_nSize, sizeof(NameToCharCodeEntry));

		for (int nIndex = 0; nIndex < m_nSize; ++nIndex)
		{
			m_pTable[nIndex].sName = NULL;
		}
	}

	NameToCharCode::~NameToCharCode()
	{
		for (int nIndex = 0; nIndex < m_nSize; ++nIndex)
		{
			if (m_pTable[nIndex].sName)
			{
				MemUtilsFree(m_pTable[nIndex].sName);
			}
		}
		MemUtilsFree(m_pTable);
	}

	void NameToCharCode::Add(char *sName, CharCode nCode)
	{
		// Увеличиваем таблицу, в случае необходимости
		if (m_nLen >= m_nSize / 2)
		{
			int nOldSize = m_nSize;
			NameToCharCodeEntry *pOldTable = m_pTable;
			m_nSize = 2 * m_nSize + 1;
			m_pTable = (NameToCharCodeEntry *)MemUtilsMallocArray(m_nSize, sizeof(NameToCharCodeEntry));
			for (int nIndex = 0; nIndex < m_nSize; ++nIndex)
			{
				m_pTable[nIndex].sName = NULL;
			}
			for (int nIndex = 0; nIndex < nOldSize; ++nIndex)
			{
				if (pOldTable[nIndex].sName)
				{
					int nHash = Hash(pOldTable[nIndex].sName);
					while (m_pTable[nHash].sName)
					{
						if (++nHash == m_nSize)
						{
							nHash = 0;
						}
					}
					m_pTable[nHash] = pOldTable[nIndex];
				}
			}
			MemUtilsFree(pOldTable);
		}

		// Добавляем новое имя
		int nHash = Hash(sName);
		while (m_pTable[nHash].sName && strcmp(m_pTable[nHash].sName, sName))
		{
			if (++nHash == m_nSize)
			{
				nHash = 0;
			}
		}
		if (!m_pTable[nHash].sName)
		{
			m_pTable[nHash].sName = CopyString(sName);
		}
		m_pTable[nHash].nCode = nCode;

		++m_nLen;
	}

	CharCode NameToCharCode::Lookup(char *sName)
	{
		int nHash = Hash(sName);
		while (m_pTable[nHash].sName)
		{
			if (!strcmp(m_pTable[nHash].sName, sName))
			{
				return m_pTable[nHash].nCode;
			}
			if (++nHash == m_nSize)
			{
				nHash = 0;
			}
		}
		return 0;
	}

	int NameToCharCode::Hash(char *sName)
	{
		char *pCur;
		unsigned int unHash = 0;
		for (pCur = sName; *pCur; ++pCur)
		{
			unHash = 17 * unHash + (int)(*pCur & 0xff);
		}
		return (int)(unHash % m_nSize);
	}
}