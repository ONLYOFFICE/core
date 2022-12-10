#include "SoundContainer.h"

using namespace PPT;

CRecordSoundContainer::CRecordSoundContainer()
{
}

CRecordSoundContainer::~CRecordSoundContainer()
{
}

void CRecordSoundContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
