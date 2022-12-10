#include "BuildAtom.h"

using namespace PPT;

void CRecordBuildAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_nBuildType	=	StreamUtils::ReadDWORD ( pStream );
    m_nBuildId		=	StreamUtils::ReadDWORD ( pStream );
    m_nShapeIdRef	=	StreamUtils::ReadDWORD ( pStream );

    _UINT32 Value		=	StreamUtils::ReadDWORD ( pStream );

    m_fExpanded		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fUIExpanded	=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
}
