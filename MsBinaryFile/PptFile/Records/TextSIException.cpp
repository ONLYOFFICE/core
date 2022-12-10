#include "TextSIException.h"

using namespace PPT;

SSmartTags::SSmartTags(){}

SSmartTags::SSmartTags(const SSmartTags &other)
{
    m_count = other.m_count;
    m_rgSmartTagIndex = other.m_rgSmartTagIndex;
}

void SSmartTags::ReadFromStream(POLE::Stream *pStream)
{
    m_count = StreamUtils::ReadDWORD(pStream);
    for (_UINT32 i = 0; i < m_count; i++)
    {
        _UINT32 SmartTagIndex = StreamUtils::ReadDWORD(pStream);
        m_rgSmartTagIndex.push_back(SmartTagIndex);
    }
}

STextSIException::STextSIException()
{

}

STextSIException::~STextSIException()
{

}

void STextSIException::ReadFromStream(POLE::Stream *pStream)
{
    _UINT16 flags = StreamUtils::ReadWORD(pStream);
    StreamUtils::StreamSkip(2, pStream);

    m_spell     = 0x1 & flags;
    m_lang      = 0x2 & flags;
    m_altLang   = 0x4 & flags;


    m_fPp10ext  = 0x20 & flags;
    m_fBidi     = 0x40 & flags;


    m_smartTag  = 0x200 & flags;

    if (m_spell)
        m_spellInfo = StreamUtils::ReadWORD(pStream);
    if (m_lang)
        m_altLang   = StreamUtils::ReadWORD(pStream);
    if (m_altLang)
        m_altLid    = StreamUtils::ReadWORD(pStream);
    if (m_fBidi)
        m_bidi      = StreamUtils::ReadWORD(pStream);

    if (m_fPp10ext)
    {
        m_pp10runid     = 0x0F & StreamUtils::ReadBYTE(pStream);
        StreamUtils::StreamSkip(2, pStream);
        m_grammarError  = 0x80 & StreamUtils::ReadBYTE(pStream);
    }

    if (m_smartTag)
    {
        StreamUtils::StreamSkip(StreamUtils::ReadDWORD(pStream) * 4, pStream);
        //            m_smartTags = new SSmartTags;
        //            m_smartTags->ReadFromStream(pStream);
    }
}
