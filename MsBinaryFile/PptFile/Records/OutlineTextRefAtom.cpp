#include "OutlineTextRefAtom.h"


void CRecordOutlineTextRefAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nIndex = StreamUtils::ReadDWORD(pStream);
}
