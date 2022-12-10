#include "SlidePersistAtom.h"

using namespace PPT;

CRecordSlidePersistAtom::CRecordSlidePersistAtom()
{
}

CRecordSlidePersistAtom::~CRecordSlidePersistAtom()
{
}

void CRecordSlidePersistAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nPsrRef = StreamUtils::ReadDWORD(pStream);

    BYTE Mem = 0;
    Mem = StreamUtils::ReadBYTE(pStream);
    m_bShouldCollapse = ((Mem & 0x02) == 0x02);
    m_bNonOutlineData = ((Mem & 0x04) == 0x04);

    StreamUtils::StreamSkip(3, pStream);

    m_nNumberText = (INT)StreamUtils::ReadLONG(pStream);
    m_nSlideID = StreamUtils::ReadDWORD(pStream);

    StreamUtils::StreamSkip(4, pStream);
}
