#include "TextSpecInfoAtom.h"

using namespace PPT;

CRecordTextSpecInfoAtom::CRecordTextSpecInfoAtom()
{
    m_lCount = 0;
    m_lOffsetInStream = 0;
}

CRecordTextSpecInfoAtom::~CRecordTextSpecInfoAtom()
{
    m_lCount = 0;
}

void CRecordTextSpecInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

    _UINT32 lMemCount = 0;
    while (true)
    {
        PPT::CTextSIRun elm;
        m_arrSIs.push_back(elm);

        NSStreamReader::Read(pStream, m_arrSIs.back());
        lMemCount += m_arrSIs.back().lCount;

        long sz = (long)(pStream->tell() - m_lOffsetInStream);

        if (sz >= (long)m_oHeader.RecLen)
            break;
    }

    // на всякий случай...
    // здесь когда текст сначала другой (т.е. например - placeholder в мастере) -
    // то у нас неправильно выставился m_lCount... на число m_lCount пилюем .. берем структур si скока прописано
    StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
}

void CRecordTextSpecInfoAtom::ApplyProperties(CTextAttributesEx *pText)
{
    if (m_arrSIs.size() < 1) return;

    int pos_text = 0, pos_si = 0;
    size_t ind = 0;

    for (size_t i = 0; i < pText->m_arParagraphs.size(); i++)
    {
        if (ind >= m_arrSIs.size()) break;

        for (size_t j = 0 ; j < pText->m_arParagraphs[i].m_arSpans.size(); j++)
        {
            if (pos_text + pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() > pos_si + m_arrSIs[ind].lCount )
            {
                pos_si += m_arrSIs[ind].lCount;
                ind++;
            }
            if (ind >= m_arrSIs.size()) break;
            if (m_arrSIs[ind].bLang)
                pText->m_arParagraphs[i].m_arSpans[j].m_oRun.Language = m_arrSIs[ind].Lang;
            pos_text += pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() ;
        }

    }
}
