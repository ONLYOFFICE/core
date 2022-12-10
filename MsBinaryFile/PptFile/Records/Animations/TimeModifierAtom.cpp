#include "TimeModifierAtom.h"

using namespace PPT;

void CRecordTimeModifierAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader	=	oHeader;

    m_nType		=	StreamUtils::ReadDWORD(pStream);
    m_Value		=	StreamUtils::ReadFLOAT(pStream);
}
