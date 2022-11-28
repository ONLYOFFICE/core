#include "ShapeFlagsAtom.h"


void CRecordShapeFlagsAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
