#include "TimeAnimateBehaviorAtom.h"

using namespace PPT;

void CRecordTimeAnimateBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_nCalcMode						=	StreamUtils::ReadDWORD ( pStream );

    _UINT32 Value					=	StreamUtils::ReadDWORD ( pStream );

    m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
    m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
    m_bCalcModePropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
    m_bAnimationValuesPropertyUsed	=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );
    m_bValueTypePropertyUsed		=	( 0x20 == ( 0x20 & ((BYTE)Value) ) );

    m_ValueType						=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
}
