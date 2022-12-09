#include "ScalingStruct.h"


void PPT::ScalingStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_x.ReadFromStream(pStream);
    m_y.ReadFromStream(pStream);
}
