#include "VisualPageAtom.h"


void CRecordVisualPageAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_eType				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
}
