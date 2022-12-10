#include "ExObjListContainer.h"

using namespace PPT;

void CRecordExObjListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
