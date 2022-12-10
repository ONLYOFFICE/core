#include "VisualShapeGeneralAtom.h"

using namespace PPT;

void CRecordVisualShapeGeneralAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    m_eType             = (TimeVisualElementEnum)StreamUtils::ReadDWORD(pStream);
    m_eRefType          = (ElementTypeEnum)StreamUtils::ReadDWORD(pStream);

    m_nShapeIdRef       = StreamUtils::ReadDWORD(pStream);
    m_nData1            = StreamUtils::ReadDWORD(pStream);
    m_nData2            = StreamUtils::ReadDWORD(pStream);
}
