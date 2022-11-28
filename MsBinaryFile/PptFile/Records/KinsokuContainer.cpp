#include "KinsokuContainer.h"


void CRecordKinsokuContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    CUnknownRecord::ReadFromStream(m_oHeader, pStream);
}
