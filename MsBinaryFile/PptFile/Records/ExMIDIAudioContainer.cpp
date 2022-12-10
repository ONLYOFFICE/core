#include "ExMIDIAudioContainer.h"

using namespace PPT;

void CRecordExMIDIAudioContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
