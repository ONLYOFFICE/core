#include "MouseTextInteractiveInfoAtom.h"


void CRecordMouseTextInteractiveInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
