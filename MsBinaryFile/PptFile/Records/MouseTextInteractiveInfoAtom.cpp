#include "MouseTextInteractiveInfoAtom.h"

using namespace PPT;

void CRecordMouseTextInteractiveInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
