#include "TimeSetBehaviorContainer.h"

using namespace PPT;

void CRecordTimeSetBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    //UINT res = 0;
    SRecordHeader ReadHeader;
    if ( ReadHeader.ReadFromStream(pStream) )
        m_oSetBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

    //if ( m_oSetBehaviorAtom.m_bToPropertyUsed )
    //{
    if ( ReadHeader.ReadFromStream(pStream) )
        m_oVarTo.ReadFromStream ( ReadHeader, pStream );
    //}

    if ( ReadHeader.ReadFromStream(pStream) )
        m_oBehavior.ReadFromStream ( ReadHeader, pStream );

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
