#include "EndDocument.h"

using namespace PPT;

void CRecordEndDocument::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
