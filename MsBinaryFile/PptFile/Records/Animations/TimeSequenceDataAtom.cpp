#include "TimeSequenceDataAtom.h"

using namespace PPT;

void CRecordTimeSequenceDataAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    // LONG lPos = 0;		StreamUtils::StreamPosition ( lPos, pStream );

    m_nConcurrency					=	StreamUtils::ReadDWORD ( pStream );
    m_nNextAction					=	StreamUtils::ReadDWORD ( pStream );
    m_nPreviousAction				=	StreamUtils::ReadDWORD ( pStream );

    StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

    _UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

    m_fConcurrencyPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fNextActionPropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
    m_fPreviousActionPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

    // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
