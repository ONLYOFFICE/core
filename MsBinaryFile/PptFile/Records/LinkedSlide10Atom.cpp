#include "LinkedSlide10Atom.h"

using namespace PPT;

void CRecordLinkedSlide10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nLinkedSlideIdRef = StreamUtils::ReadLONG(pStream);
    m_cLinkedShapes     = StreamUtils::ReadLONG(pStream);
}
