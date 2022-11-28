#include "TextCFException9.h"


void STextCFException9::ReadFromStream(POLE::Stream *pStream)
{
    m_masks.ReadFromStream(pStream);

    if (m_masks.m_pp10ext)
        m_pp10runid = 0xF & StreamUtils::ReadDWORD(pStream);
}
