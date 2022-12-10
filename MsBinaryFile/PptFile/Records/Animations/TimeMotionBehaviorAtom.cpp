#include "TimeMotionBehaviorAtom.h"

using namespace PPT;

void CRecordTimeMotionBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

    m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
    m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
    m_bOriginPropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
    m_bPathPropertyUsed				=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

    m_bEditRotationPropertyUsed		=	( 0x40 == ( 0x40 & ((BYTE)Value) ) );
    m_bPointsTypesPropertyUsed		=	( 0x80 == ( 0x80 & ((BYTE)Value) ) );

    m_nXBY							=	StreamUtils::ReadFLOAT ( pStream );
    m_nYBY							=	StreamUtils::ReadFLOAT ( pStream );
    m_nXFROM						=	StreamUtils::ReadFLOAT ( pStream );
    m_nYFROM						=	StreamUtils::ReadFLOAT ( pStream );
    m_nXTO							=	StreamUtils::ReadFLOAT ( pStream );
    m_nYTO							=	StreamUtils::ReadFLOAT ( pStream );
    m_nBehaviorOrigin				=	StreamUtils::ReadDWORD ( pStream );
}
