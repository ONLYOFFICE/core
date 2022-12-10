#include "KinsokuContainer.h"

using namespace PPT;

void CRecordKinsokuContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    CUnknownRecord::ReadFromStream(m_oHeader, pStream);
}
