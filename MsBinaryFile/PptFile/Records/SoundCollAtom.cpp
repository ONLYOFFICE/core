#include "SoundCollAtom.h"

using namespace PPT;

CRecordSoundCollAtom::CRecordSoundCollAtom()
{
}

CRecordSoundCollAtom::~CRecordSoundCollAtom()
{
}

void CRecordSoundCollAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nObjectIdSeed = StreamUtils::ReadDWORD(pStream);
}
