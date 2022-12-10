#include "TimeSetBehaviorAtom.h"

using namespace PPT;

void CRecordTimeSetBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    _UINT32 Value					=	StreamUtils::ReadDWORD ( pStream );

    m_bToPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_bValueTypePropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

    m_eValueType					=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
}
