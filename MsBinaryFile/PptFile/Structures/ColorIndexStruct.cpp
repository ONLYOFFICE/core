#include "ColorIndexStruct.h"


PPT::ColorIndexStruct::ColorIndexStruct()
{
    clear();
}

void PPT::ColorIndexStruct::clear()
{
    m_red = m_green = m_blue = 0;
    m_index = 0;
}

void PPT::ColorIndexStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_red       = StreamUtils::ReadBYTE(pStream);
    m_green     = StreamUtils::ReadBYTE(pStream);
    m_blue      = StreamUtils::ReadBYTE(pStream);
    m_index     = StreamUtils::ReadBYTE(pStream);
}
