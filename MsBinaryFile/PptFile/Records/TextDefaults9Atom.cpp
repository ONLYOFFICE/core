#include "TextDefaults9Atom.h"

using namespace PPT;

void CRecordTextDefaults9Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_cf9.ReadFromStream(pStream);
    m_pf9.ReadFromStream(pStream);
}
