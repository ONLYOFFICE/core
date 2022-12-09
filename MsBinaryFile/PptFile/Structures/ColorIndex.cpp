#include "ColorIndex.h"

void PPT::ColorIndex::clear()
{
    m_red = m_green = m_blue = 0;
    m_index = 0xFF;         // Color is undefined.
}

void PPT::ColorIndex::ReadFromStream(POLE::Stream *pStream)
{
    m_red       = StreamUtils::ReadBYTE(pStream);
    m_green     = StreamUtils::ReadBYTE(pStream);
    m_blue      = StreamUtils::ReadBYTE(pStream);
    m_index     = StreamUtils::ReadBYTE(pStream);
}
