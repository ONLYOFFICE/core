#include "TmsfTimeStruct.h"


void PPT::TmsfTimeStruct::ReadFromStream(POLE::Stream *pStream)
{
    m_nTrack    = StreamUtils::ReadBYTE(pStream);
    m_nMinute   = StreamUtils::ReadBYTE(pStream);
    m_nSecond   = StreamUtils::ReadBYTE(pStream);
    m_nFrame    = StreamUtils::ReadBYTE(pStream);
}
