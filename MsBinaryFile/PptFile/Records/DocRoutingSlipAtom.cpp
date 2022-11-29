#include "DocRoutingSlipAtom.h"


void CRecordDocRoutingSlipAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
