#include "TimeEffectBehaviorAtom.h"

using namespace PPT;

void CRecordTimeEffectBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    // LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

    _UINT32 dwFlags				=	StreamUtils::ReadDWORD ( pStream );

    m_bTransitionPropertyUsed	= ( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
    m_bTypePropertyUsed			= ( 0x02 == ( 0x02 & ((BYTE)dwFlags) ) );
    m_bProgressPropertyUsed		= ( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );
    m_bRuntimeContextObsolete	= ( 0x08 == ( 0x08 & ((BYTE)dwFlags) ) );

    m_nEffectTransition			=	StreamUtils::ReadDWORD ( pStream );

    // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
