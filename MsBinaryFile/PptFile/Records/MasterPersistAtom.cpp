#include "MasterPersistAtom.h"

using namespace PPT;

void CRecordMasterPersistAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
