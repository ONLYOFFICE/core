#include "ExObjListAtom.h"

using namespace PPT;

void CRecordExObjListAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nObjectIdSeed = StreamUtils::ReadDWORD(pStream);
}
