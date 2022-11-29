#include "ColorIndexStruct.h"


PPT_FORMAT::ColorIndexStruct::ColorIndexStruct()
{
    clear();
}

void PPT_FORMAT::ColorIndexStruct::clear()
{
    m_red = m_green = m_blue = 0;
    m_index = 0;
}

void PPT_FORMAT::ColorIndexStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_red       = StreamUtils::ReadBYTE(pStream);
    m_green     = StreamUtils::ReadBYTE(pStream);
    m_blue      = StreamUtils::ReadBYTE(pStream);
    m_index     = StreamUtils::ReadBYTE(pStream);
}
