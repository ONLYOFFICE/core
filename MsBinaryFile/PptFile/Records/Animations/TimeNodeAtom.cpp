#include "TimeNodeAtom.h"

using namespace PPT;

void CRecordTimeNodeAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //	LONG lPos = 0;	StreamUtils::StreamPosition(lPos, pStream);

    StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );

    //	0x00000000  Does not restart.
    //	0x00000001  Can restart at any time.
    //	0x00000002  Can restart when the corresponding time node is not active.
    //	0x00000003  Same as 0x00000000.
    m_dwRestart				=	StreamUtils::ReadDWORD ( pStream );
    //	0x00000000 specifies that this time node is a parallel time node,
    //	which allows all of its child nodes to start at the same time.
    m_dwType				=	(TimeNodeTypeEnum)StreamUtils::ReadDWORD ( pStream );

    //	0x00000000  The properties remain at their ending values while the parent time node is still running or holding. After which, the properties reset to  their original values.
    //	0x00000001  The properties reset to their original values after the time node becomes inactive.
    //	0x00000002  The properties remain at their ending values while the parent time node is still running or holding, or until another sibling time node is started under a sequential time node as  specified in the type field. After which, the  properties reset to their original values.
    //	0x00000003  Same as 0x00000000.
    //	0x00000004  Same as 0x00000001.
    m_dwFill				=	StreamUtils::ReadDWORD ( pStream );

    StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );
    StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

    // duration: 0xFFFFFFFF specifies that the duration of the time node is infinite,
    // and that its actual duration is determined by the durations of its child nodes.
    m_nDuration				=	StreamUtils::ReadLONG ( pStream );

    _UINT32 Value				=	StreamUtils::ReadDWORD ( pStream );

    m_fFillProperty			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
    m_fRestartProperty		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

    m_fGroupingTypeProperty	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
    m_fDurationProperty		=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

    // StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
}
