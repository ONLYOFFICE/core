#include "KinsokuLeadingAtom.h"

using namespace PPT;

void CRecordKinsokuLeadingAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_arKinsokuLeading = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen - sizeof (oHeader));
}
