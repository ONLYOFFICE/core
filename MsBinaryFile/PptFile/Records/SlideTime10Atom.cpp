#include "SlideTime10Atom.h"

using namespace PPT;

void CRecordSlideTime10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oFileTime.dwLowDateTime = StreamUtils::ReadDWORD(pStream);
    m_oFileTime.dwHighDateTime = StreamUtils::ReadDWORD(pStream);
}
