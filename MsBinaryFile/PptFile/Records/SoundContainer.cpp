#include "SoundContainer.h"


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
