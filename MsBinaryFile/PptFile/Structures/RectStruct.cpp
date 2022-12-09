#include "RectStruct.h"


void PPT::RectStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nTop = StreamUtils::ReadLONG(pStream);
    m_nLeft = StreamUtils::ReadLONG(pStream);
    m_nRight = StreamUtils::ReadLONG(pStream);
    m_nBottom = StreamUtils::ReadLONG(pStream);
}
