#include "TimeIterateDataAtom.h"

using namespace PPT;

void CRecordTimeIterateDataAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_nIterateInterval						=	StreamUtils::ReadDWORD ( pStream );
    m_nIterateType							=	StreamUtils::ReadDWORD ( pStream );
    m_nIterateDirection                     =	StreamUtils::ReadDWORD ( pStream );
    m_nIterateIntervalType					=	StreamUtils::ReadDWORD ( pStream );

    _UINT32 Value							=	StreamUtils::ReadDWORD ( pStream );

    m_fIterateDirectionPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fIterateTypePropertyUsed			=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
    m_fIterateIntervalPropertyUsed		=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
    m_fIterateIntervalTypePropertyUsed	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
}
