#include "ExObjListAtom.h"


void CRecordExObjListAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nObjectIdSeed = StreamUtils::ReadDWORD(pStream);
}
