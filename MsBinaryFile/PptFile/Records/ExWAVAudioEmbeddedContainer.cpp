#include "ExWAVAudioEmbeddedContainer.h"

using namespace PPT;

void CRecordWAVAudioEmbeddedContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader oExHeader;
    oExHeader.ReadFromStream(pStream);

    m_oMedia.ReadFromStream(oExHeader, pStream);

    StreamUtils::StreamSkip(8, pStream);

    m_nSoundID	= StreamUtils::ReadDWORD(pStream);
    m_nDuration	= StreamUtils::ReadLONG(pStream);
}
