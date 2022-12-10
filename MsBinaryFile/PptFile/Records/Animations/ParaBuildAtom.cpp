#include "ParaBuildAtom.h"

using namespace PPT;

void CRecordParaBuildAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_nParaBuild					=	StreamUtils::ReadDWORD ( pStream );
    m_nBuildLevel					=	StreamUtils::ReadDWORD ( pStream );

    _UINT32 Value					=	StreamUtils::ReadDWORD ( pStream );

    m_fAnimBackground				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fReverse                      =	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
    m_fUserSetAnimBackground		=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
    m_fAutomatic					=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );

    m_nDelayTime					=	StreamUtils::ReadDWORD ( pStream );
}
