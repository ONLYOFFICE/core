#include "SoundCollectionContainer.h"

using namespace PPT;

CRecordSoundCollectionContainer::CRecordSoundCollectionContainer()
{
}

CRecordSoundCollectionContainer::~CRecordSoundCollectionContainer()
{
}

void CRecordSoundCollectionContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}
