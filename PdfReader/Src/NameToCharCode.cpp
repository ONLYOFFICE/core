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