#include "ShapeFlags10Atom.h"

using namespace PPT;

void CRecordShapeFlags10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
