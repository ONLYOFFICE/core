/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include <map>

#include "../Reader/Records.h"

class CPersistDirectoryEntry
{
public:
	DWORD m_nPersistID;			// PersistOffsetID[index] = m_nPersistID + index
	DWORD m_nPersistCount;

	std::vector<DWORD> m_arPersistOffsets;

public:

	CPersistDirectoryEntry() : m_arPersistOffsets()
	{
		m_nPersistID = 0;
		m_nPersistCount = 0;
	}

	DWORD FromStream(POLE::Stream* pStream)
	{
		DWORD nFlag = StreamUtils::ReadDWORD(pStream);
		m_nPersistID = (nFlag & 0x000FFFFF);			// 20 bit
		m_nPersistCount = (nFlag & 0xFFF00000) >> 20;	// 12 bit

		m_arPersistOffsets.clear();

		for (DWORD index = 0; index < m_nPersistCount; ++index)
		{
			DWORD Mem = StreamUtils::ReadDWORD(pStream);
			m_arPersistOffsets.push_back(Mem);
		}

		return 4 * (m_nPersistCount + 1);
	}
};

class CRecordPersistDirectoryAtom : public CUnknownRecord
{
	std::vector<CPersistDirectoryEntry> m_arEntries;

public:
	
	CRecordPersistDirectoryAtom() : m_arEntries()
	{
	}

	~CRecordPersistDirectoryAtom()
	{
		m_arEntries.clear();
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		
		DWORD nCountRead = 0;
		DWORD nCountEnries = 0;
		while (nCountRead < m_oHeader.RecLen)
		{
			CPersistDirectoryEntry elm;
			m_arEntries.push_back(elm);

			nCountRead += m_arEntries[nCountEnries].FromStream(pStream);
			++nCountEnries;
		}
	}

	void ToMap(std::map<DWORD, DWORD>* pMap)
	{
		pMap->clear();

		for (size_t nEntry = 0; nEntry < m_arEntries.size(); ++nEntry)
		{
			DWORD nPID = m_arEntries[nEntry].m_nPersistID;

			for (size_t nIndex = 0; nIndex < m_arEntries[nEntry].m_nPersistCount; ++nIndex)
			{
				DWORD nOffset = m_arEntries[nEntry].m_arPersistOffsets[nIndex];
				pMap->insert(std::pair<DWORD, DWORD>(nPID, nOffset));
				++nPID;
			}
		}
	}
};