#include "TimeColorBehaviorAtom.h"

using namespace PPT;

void CRecordTimeColorBehaviorAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    _UINT32 flag			=	StreamUtils::ReadDWORD ( pStream );

    m_fByPropertyUsed			=	( 0x01 == ( 0x01 & ((BYTE)flag) ) );
    m_fFromPropertyUsed         =	( 0x02 == ( 0x02 & ((BYTE)flag) ) );
    m_fToPropertyUsed			=	( 0x04 == ( 0x04 & ((BYTE)flag) ) );
    m_fColorSpacePropertyUsed	=	( 0x08 == ( 0x08 & ((BYTE)flag) ) );
    m_fDirectionPropertyUsed	=	( 0x10 == ( 0x10 & ((BYTE)flag) ) );

    pStream->read ((unsigned char*) &m_sColorBy     , sizeof ( TimeAnimateColorBy ) );
    pStream->read ((unsigned char*) &m_sColorFrom	, sizeof ( TimeAnimateColor ) );
    pStream->read ((unsigned char*) &m_sColorTo     , sizeof ( TimeAnimateColor ) );
}
