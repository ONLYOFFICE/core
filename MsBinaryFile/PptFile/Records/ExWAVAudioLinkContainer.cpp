#include "ExWAVAudioLinkContainer.h"

using namespace PPT;

void CRecordWAVAudioLinkContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
