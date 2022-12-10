#include "NoZoomViewInfoAtom.h"

using namespace PPT;

void CRecordNoZoomViewInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
