#include "PointStruct.h"


void PPT::PointStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_x = StreamUtils::ReadLONG(pStream);
    m_y = StreamUtils::ReadLONG(pStream);
}
