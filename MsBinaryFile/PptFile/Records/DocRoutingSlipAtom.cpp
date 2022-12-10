#include "DocRoutingSlipAtom.h"

using namespace PPT;

void CRecordDocRoutingSlipAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
