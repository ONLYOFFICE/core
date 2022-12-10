#include "VisualShapeAtom.h"

using namespace PPT;

void CRecordVisualShapeAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    m_Type				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
    m_RefType			=	(ElementTypeEnum) StreamUtils::ReadDWORD ( pStream );
    m_nObjectIdRef                  =	StreamUtils::ReadDWORD ( pStream );
    m_nData1			=	StreamUtils::ReadDWORD ( pStream );
    m_nData2			=	StreamUtils::ReadDWORD ( pStream );
}
