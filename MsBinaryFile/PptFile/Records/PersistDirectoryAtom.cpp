#include "PersistDirectoryAtom.h"

using namespace PPT;

_UINT32 CPersistDirectoryEntry::FromStream(POLE::Stream *pStream)
{
    _UINT32 nFlag = StreamUtils::ReadDWORD(pStream);
    m_nPersistID = (nFlag & 0x000FFFFF);			// 20 bit
    m_nPersistCount = (nFlag & 0xFFF00000) >> 20;	// 12 bit

    m_arPersistOffsets.clear();

    for (_UINT32 index = 0; index < m_nPersistCount; ++index)
    {
        _UINT32 Mem = StreamUtils::ReadDWORD(pStream);
        m_arPersistOffsets.push_back(Mem);
    }

    return 4 * (m_nPersistCount + 1);
}

void CRecordPersistDirectoryAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 nCountRead = 0;
    _UINT32 nCountEnries = 0;
    while (nCountRead < m_oHeader.RecLen)
    {
        CPersistDirectoryEntry elm;
        m_arEntries.push_back(elm);

        nCountRead += m_arEntries[nCountEnries].FromStream(pStream);
        ++nCountEnries;
    }
}

void CRecordPersistDirectoryAtom::ToMap(std::map<_UINT32, _UINT32> *pMap)
{
    pMap->clear();

    for (size_t nEntry = 0; nEntry < m_arEntries.size(); ++nEntry)
    {
        _UINT32 nPID = m_arEntries[nEntry].m_nPersistID;

        for (size_t nIndex = 0; nIndex < m_arEntries[nEntry].m_nPersistCount; ++nIndex)
        {
            _UINT32 nOffset = m_arEntries[nEntry].m_arPersistOffsets[nIndex];
            pMap->insert(std::pair<_UINT32, _UINT32>(nPID, nOffset));
            ++nPID;
        }
    }
}
