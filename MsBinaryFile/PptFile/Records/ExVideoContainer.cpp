#include "ExVideoContainer.h"

using namespace PPT;

void CRecordExVideoContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
