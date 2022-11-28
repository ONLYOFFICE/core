#include "MasterPersistAtom.h"


void CRecordMasterPersistAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
