#include "TimeColorBehaviorContainer.h"

using namespace PPT;

void CRecordTimeColorBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    SRecordHeader ReadHeader;
    if (ReadHeader.ReadFromStream(pStream) )
        m_oColorBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    if (ReadHeader.ReadFromStream(pStream) )
        m_oBehavior.ReadFromStream (ReadHeader, pStream );
}
