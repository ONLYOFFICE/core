#include "GridSpacing10Atom.h"

using namespace PPT;

void CRecordGridSpacing10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
