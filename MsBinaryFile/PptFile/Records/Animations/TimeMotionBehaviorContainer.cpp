#include "TimeMotionBehaviorContainer.h"

using namespace PPT;

CRecordTimeMotionBehaviorContainer::~CRecordTimeMotionBehaviorContainer()
{
    RELEASEOBJECT(m_pVarPath)
}

void CRecordTimeMotionBehaviorContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    //UINT res = 0;

    SRecordHeader header;
    if ( header.ReadFromStream(pStream) )
    {
        m_oMotionBehaviorAtom.ReadFromStream ( header, pStream );
    }

    UINT lCurLen	=	m_oMotionBehaviorAtom.m_oHeader.RecLen + 8;
    while ( lCurLen < m_oHeader.RecLen )
    {
        header.ReadFromStream(pStream);

        lCurLen += header.RecLen + 8;

        switch (header.RecType) {
        case RT_TimeVariant:
        {
            if (header.RecLen > 5 ) {
                m_pVarPath = new CRecordTimeVariantString();
                m_pVarPath->ReadFromStream(header, pStream);
            }else if (header.RecLen == 5){
                StreamUtils::StreamSkip(5, pStream);
            } else
                StreamUtils::StreamSkip(header.RecLen, pStream);
            break;
        }

        case RT_TimeBehaviorContainer:
        {
            m_oTimeBehavior.ReadFromStream(header, pStream);
            break;
        }
        default:
            break;
        }
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
