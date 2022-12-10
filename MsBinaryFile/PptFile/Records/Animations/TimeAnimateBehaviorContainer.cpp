#include "TimeAnimateBehaviorContainer.h"

using namespace PPT;

void CRecordTimeAnimateBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    SRecordHeader ReadHeader;

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oAnimateBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    ReadHeader.ReadFromStream(pStream) ;
    if (ReadHeader.RecType == RT_TimeAnimationValueList)
        m_oAnimateValueList.ReadFromStream ( ReadHeader, pStream );
    else
        StreamUtils::StreamSkipBack(8, pStream);


    if ( m_oAnimateBehaviorAtom.m_bByPropertyUsed )
    {
        if ( ReadHeader.ReadFromStream(pStream) )
            m_oVarBy.ReadFromStream ( ReadHeader, pStream );
    }

    if ( m_oAnimateBehaviorAtom.m_bFromPropertyUsed )
    {
        if ( ReadHeader.ReadFromStream(pStream) )
            m_oVarFrom.ReadFromStream ( ReadHeader, pStream );
    }

    if ( m_oAnimateBehaviorAtom.m_bToPropertyUsed )
    {
        if ( ReadHeader.ReadFromStream(pStream) )
            m_oVarTo.ReadFromStream ( ReadHeader, pStream );
    }

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oBehavior.ReadFromStream ( ReadHeader, pStream );
}
