#include "ColorSchemeAtom.h"

using namespace PPT;
using namespace ODRAW;

void CRecordColorSchemeAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    NSStreamReader::Read(pStream, m_oBackgroundColor);
    NSStreamReader::Read(pStream, m_oTextAndLinesColor);
    NSStreamReader::Read(pStream, m_oShadowsColor);
    NSStreamReader::Read(pStream, m_oTitleTextColor);
    NSStreamReader::Read(pStream, m_oFillsColor);
    NSStreamReader::Read(pStream, m_oAccentColor);
    NSStreamReader::Read(pStream, m_oAccentAndHyperlinkColor);
    NSStreamReader::Read(pStream, m_oAccentAndFollowingHyperlinkColor);
}

void CRecordColorSchemeAtom::ToArray(std::vector<SColorAtom> *pArray)
{
    if (NULL != pArray)
    {
        pArray->push_back(m_oBackgroundColor);
        pArray->push_back(m_oTextAndLinesColor);
        pArray->push_back(m_oShadowsColor);
        pArray->push_back(m_oTitleTextColor);
        pArray->push_back(m_oFillsColor);
        pArray->push_back(m_oAccentColor);
        pArray->push_back(m_oAccentAndHyperlinkColor);
        pArray->push_back(m_oAccentAndFollowingHyperlinkColor);
    }
}

void CRecordColorSchemeAtom::ToArray(std::vector<CColor> *pArray)
{
    CColor oColor;
    if (NULL != pArray)
    {
        m_oBackgroundColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oTextAndLinesColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oShadowsColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oTitleTextColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oFillsColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentAndHyperlinkColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentAndFollowingHyperlinkColor.ToColor(&oColor);
        pArray->push_back(oColor);
    }
}
