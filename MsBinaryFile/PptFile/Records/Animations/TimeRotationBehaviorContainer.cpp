#include "TimeRotationBehaviorContainer.h"

using namespace PPT;

void CRecordTimeRotationBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    SRecordHeader ReadHeader;

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oRotationBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oBehavior.ReadFromStream ( ReadHeader, pStream );
}
