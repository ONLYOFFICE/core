#include "HashCode10Atom.h"

using namespace PPT;

void CRecordHashCode10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader	=	oHeader;

    m_nHash = StreamUtils::ReadDWORD(pStream);
}
