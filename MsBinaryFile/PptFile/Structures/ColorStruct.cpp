#include "ColorStruct.h"


PPT::ColorStruct::ColorStruct()
{
    clear();
}

void PPT::ColorStruct::clear()
{
    m_red = m_green = m_blue = 0;
}

void PPT::ColorStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_red       = StreamUtils::ReadBYTE(pStream);
    m_green     = StreamUtils::ReadBYTE(pStream);
    m_blue      = StreamUtils::ReadBYTE(pStream);
    StreamUtils::StreamSkip(1, pStream);
}
