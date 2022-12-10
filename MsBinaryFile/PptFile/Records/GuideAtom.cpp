#include "GuideAtom.h"

using namespace PPT;

void CRecordGuideAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nType = StreamUtils::ReadDWORD(pStream);
    m_nPos = StreamUtils::ReadDWORD(pStream);
}
