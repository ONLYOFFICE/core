#include "PFMasks.h"

using namespace PPT;

static inline bool getBit(_UINT32 data, _UINT32 mask)
{
    return mask == (mask & data);
}


void PFMasks::ReadFromStream(POLE::Stream *pStream)
{
    _UINT32 data = StreamUtils::ReadDWORD(pStream);

    m_hasBullet         = getBit(data, 0x0001);
    m_bulletHasFont     = getBit(data, 0x0002);
    m_bulletHasColor    = getBit(data, 0x0004);
    m_bulletHasSize     = getBit(data, 0x0008);
    m_bulletFont        = getBit(data, 0x0010);
    m_bulletColor       = getBit(data, 0x0020);
    m_bulletSize        = getBit(data, 0x0040);
    m_bulletChar        = getBit(data, 0x0080);
    m_leftMargin        = getBit(data, 0x0100);
    // unused
    m_indent            = getBit(data, 0x000400);
    m_align             = getBit(data, 0x000800);
    m_lineSpacing       = getBit(data, 0x001000);
    m_spaceBefore       = getBit(data, 0x002000);
    m_spaceAfter        = getBit(data, 0x004000);
    m_defaultTabSize    = getBit(data, 0x008000);
    m_fontAlign         = getBit(data, 0x010000);
    m_charWrap          = getBit(data, 0x020000);
    m_wordWrap          = getBit(data, 0x040000);
    m_overflow          = getBit(data, 0x080000);
    m_tabStops          = getBit(data, 0x100000);
    m_textDirection     = getBit(data, 0x200000);
    // reserved1
    m_bulletBlip        = getBit(data, 0x0800000);
    m_bulletScheme      = getBit(data, 0x1000000);
    m_bulletHasScheme   = getBit(data, 0x2000000);
}
