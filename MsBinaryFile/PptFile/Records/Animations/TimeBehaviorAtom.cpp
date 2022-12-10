#include "TimeBehaviorAtom.h"

using namespace PPT;

void CRecordTimeBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    // LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    _UINT32 dwFlags					=	StreamUtils::ReadDWORD ( pStream );

    m_bAdditivePropertyUsed			=	( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
    m_bAttributeNamesPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );

    m_nBehaviorAdditive				=	StreamUtils::ReadDWORD ( pStream );
    m_nBehaviorAccumulate			=	StreamUtils::ReadDWORD ( pStream );
    m_nBehaviorTransform			=	StreamUtils::ReadDWORD ( pStream );

    // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
