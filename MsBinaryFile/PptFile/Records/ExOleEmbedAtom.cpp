#include "ExOleEmbedAtom.h"

using namespace PPT;

void CRecordExOleEmbedAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nColorFollow = StreamUtils::ReadLONG(pStream);
    m_nCantLockServer = StreamUtils::ReadBYTE(pStream);
    m_nNoSizeToServer = StreamUtils::ReadBYTE(pStream);
    m_nIsTable = StreamUtils::ReadBYTE(pStream);
    StreamUtils::StreamSkip(1, pStream);
}
