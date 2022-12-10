#include "TimeConditionContainer.h"

using namespace PPT;

void CRecordTimeConditionContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    SRecordHeader header;

    if ( header.ReadFromStream(pStream) )
    {
        m_oTimeConditionAtom.ReadFromStream ( header, pStream );

        if ( TL_TOT_VisualElement == m_oTimeConditionAtom.m_TriggerObject )
        {
            if ( header.ReadFromStream(pStream) )
            {
                m_oVisualElement.ReadFromStream ( header, pStream );
            }
        }
    }

    StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
