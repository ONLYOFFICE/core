#include "TimeAnimationValueAtom.h"


void CRecordTimeAnimationValueAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_nTime				=	StreamUtils::ReadDWORD ( pStream );
}
