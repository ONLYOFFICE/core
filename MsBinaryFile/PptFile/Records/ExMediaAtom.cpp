#include "ExMediaAtom.h"

using namespace PPT;

void CRecordExMediaAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nExObjID = StreamUtils::ReadDWORD(pStream);

    USHORT nFlag = StreamUtils::ReadWORD(pStream);

    m_bLoop = ((nFlag & 0x01) == 0x01);
    m_bRewind = ((nFlag & 0x02) == 0x02);
    m_bNarration = ((nFlag & 0x04) == 0x04);

    StreamUtils::StreamSkip(2, pStream);
}
