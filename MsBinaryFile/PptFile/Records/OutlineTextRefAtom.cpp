#include "OutlineTextRefAtom.h"

using namespace PPT;

void CRecordOutlineTextRefAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nIndex = StreamUtils::ReadDWORD(pStream);
}
