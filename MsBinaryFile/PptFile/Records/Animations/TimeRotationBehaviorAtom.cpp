#include "TimeRotationBehaviorAtom.h"

using namespace PPT;

void CRecordTimeRotationBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    _UINT32 src						=	StreamUtils::ReadDWORD ( pStream );

    m_fByPropertyUsed					=	( 0x01 == ( 0x01 & ((BYTE)src) ) );
    m_fFromPropertyUsed                 =	( 0x02 == ( 0x02 & ((BYTE)src) ) );
    m_fToPropertyUsed					=	( 0x04 == ( 0x04 & ((BYTE)src) ) );
    m_fDirectionPropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)src) ) );

    m_By								=	StreamUtils::ReadFLOAT ( pStream );
    m_From                             =	StreamUtils::ReadFLOAT ( pStream );
    m_To								=	StreamUtils::ReadFLOAT ( pStream );

    m_nRotationDirection				=	StreamUtils::ReadDWORD ( pStream );
}
