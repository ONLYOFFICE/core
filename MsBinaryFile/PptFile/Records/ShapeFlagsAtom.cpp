#include "ShapeFlagsAtom.h"

using namespace PPT;

void CRecordShapeFlagsAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
