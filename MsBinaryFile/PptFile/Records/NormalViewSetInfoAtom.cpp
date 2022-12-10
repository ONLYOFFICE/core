#include "NormalViewSetInfoAtom.h"

using namespace PPT;

void CRecordNormalViewSetInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
