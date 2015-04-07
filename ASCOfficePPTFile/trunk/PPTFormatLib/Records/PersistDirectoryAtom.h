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
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE);

		return oWriter.GetXmlString();
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