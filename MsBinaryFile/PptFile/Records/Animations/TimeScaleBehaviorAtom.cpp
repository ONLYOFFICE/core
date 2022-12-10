#include "TimeScaleBehaviorAtom.h"

using namespace PPT;

void CRecordTimeScaleBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    _UINT32 src						=	StreamUtils::ReadDWORD ( pStream );

    m_fByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)src) ) );
    m_fFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)src) ) );
    m_fToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)src) ) );
    m_fZoomContentsUsed				=	( 0x08 == ( 0x08 & ((BYTE)src) ) );

    m_XBy							=	StreamUtils::ReadFLOAT ( pStream );
    m_YBy							=	StreamUtils::ReadFLOAT ( pStream );
    m_XFrom							=	StreamUtils::ReadFLOAT ( pStream );
    m_YFrom							=	StreamUtils::ReadFLOAT ( pStream );
    m_XTo							=	StreamUtils::ReadFLOAT ( pStream );
    m_YTo							=	StreamUtils::ReadFLOAT ( pStream );

    src								=	StreamUtils::ReadDWORD ( pStream );
    m_fZoomContents					=	( 0x01 == ( 0x01 & ((BYTE)src) ) );
}
