#include "NormalViewSetInfoAtom.h"

void CRecordNormalViewSetInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
