#include "TimeEffectBehaviorContainer.h"

using namespace PPT;

void CRecordTimeEffectBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    SRecordHeader header;
    if ( header.ReadFromStream(pStream) )
        m_effectBehaviorAtom.ReadFromStream ( header, pStream );

    if ( m_effectBehaviorAtom.m_bTypePropertyUsed )
    {
        if ( header.ReadFromStream(pStream) )
            m_oVarType.ReadFromStream ( header, pStream );
    }

    if ( m_effectBehaviorAtom.m_bProgressPropertyUsed )
    {
        if ( header.ReadFromStream(pStream) )
            m_oVarProgress.ReadFromStream ( header, pStream );
    }

    if ( m_effectBehaviorAtom.m_bRuntimeContextObsolete )
    {
        if ( header.ReadFromStream(pStream) )
            m_oVarRuntimeContext.ReadFromStream ( header, pStream );
    }

    if ( header.ReadFromStream(pStream) )
    {
        if (header.RecType == 0xF12A)
        {
            m_oBehavior.ReadFromStream (header, pStream);
        }
        else
        {
            StreamUtils::StreamSkip (header.RecLen, pStream);
            if ( header.ReadFromStream(pStream) )
            {
                m_oBehavior.ReadFromStream ( header, pStream );
            }
        }
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
