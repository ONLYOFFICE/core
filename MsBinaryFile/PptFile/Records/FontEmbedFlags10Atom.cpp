#include "FontEmbedFlags10Atom.h"

using namespace PPT;

void CRecordFontEmbedFlags10Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 flags = StreamUtils::ReadDWORD(pStream);
    m_fSubset                   = 0x1 & flags;
    m_fSubsetOptionConfirmed    = 0x2 & flags;
}
