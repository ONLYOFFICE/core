#include "BlipStoreEntry.h"

using namespace PPT;

CRecordBlipStoreEntry::CRecordBlipStoreEntry()
{

}

CRecordBlipStoreEntry::~CRecordBlipStoreEntry()
{

}

void CRecordBlipStoreEntry::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_btWin32 = (ODRAW::eBlipType)StreamUtils::ReadBYTE(pStream);
    m_btMacOS = (ODRAW::eBlipType)StreamUtils::ReadBYTE(pStream);

    pStream->read(m_pRgbUid, 16);

    m_nTag = StreamUtils::ReadWORD(pStream);

    m_nSize = StreamUtils::ReadDWORD(pStream);
    m_nCountRef = StreamUtils::ReadDWORD(pStream);
    m_nFoDelay = StreamUtils::ReadDWORD(pStream);

    m_eUsage = (ODRAW::eBlipUsage)StreamUtils::ReadBYTE(pStream);

    m_nLenName = StreamUtils::ReadBYTE(pStream);

    m_nUnused2 = StreamUtils::ReadBYTE(pStream);
    m_nUnused3 = StreamUtils::ReadBYTE(pStream);
}
