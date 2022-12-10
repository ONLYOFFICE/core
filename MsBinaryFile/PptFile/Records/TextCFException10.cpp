#include "TextCFException10.h"

using namespace PPT;

void STextCFException10::ReadFromStream(POLE::Stream *pStream)
{
    m_masks.ReadFromStream(pStream);

    if (m_masks.m_newEATypeface)
        m_newEAFontRef  = new USHORT(StreamUtils::ReadSHORT(pStream));
    if (m_masks.m_csTypeface)
        m_newEAFontRef  = new USHORT(StreamUtils::ReadSHORT(pStream));
    if (m_masks.m_pp11ext)
        m_pp11ext       = new _UINT32(StreamUtils::ReadDWORD(pStream));
}
