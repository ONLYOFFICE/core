#include "RatioStruct.h"


void PPT::RatioStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nNumber	= StreamUtils::ReadLONG(pStream);
    m_nDenom	= StreamUtils::ReadLONG(pStream);
}
