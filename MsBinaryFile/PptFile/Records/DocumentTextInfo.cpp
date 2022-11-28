#include "DocumentTextInfo.h"


void CRecordDocumentTextInfo::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}

void CRecordFontCollection::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
