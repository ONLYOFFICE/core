#include "RatioStruct.h"


void PPT_FORMAT::RatioStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nNumber	= StreamUtils::ReadLONG(pStream);
    m_nDenom	= StreamUtils::ReadLONG(pStream);
}
