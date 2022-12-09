#include "SmallRectStruct.h"


void PPT::SmallRectStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nTop = StreamUtils::ReadSHORT(pStream);
    m_nLeft = StreamUtils::ReadSHORT(pStream);
    m_nRight = StreamUtils::ReadSHORT(pStream);
    m_nBottom = StreamUtils::ReadSHORT(pStream);
}
