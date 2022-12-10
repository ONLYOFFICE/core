#include "StyleTextPropAtom.h"

using namespace PPT;

CRecordStyleTextPropAtom::CRecordStyleTextPropAtom()
{
    m_lCount = 0;
    m_lOffsetInStream = 0;
}

CRecordStyleTextPropAtom::~CRecordStyleTextPropAtom()
{
    m_lCount = 0;
}

void CRecordStyleTextPropAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

    _UINT32 lMemCount = 0;
    _UINT32 lCountItems = 0;

    if (0 == m_lCount)
    {
        StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
        return;
    }

    while (lMemCount < m_lCount + 1)
    {
        CTextPFRunRecord elm;
        m_arrPFs.push_back(elm);
        m_arrPFs[lCountItems].LoadFromStream(pStream);
        lMemCount += m_arrPFs[lCountItems].m_lCount;

        ++lCountItems;
    }

    lMemCount = 0;
    lCountItems = 0;
    while (lMemCount < m_lCount + 1)
    {
        CTextCFRunRecord elm;
        m_arrCFs.push_back(elm);

        m_arrCFs[lCountItems].LoadFromStream(pStream);
        lMemCount += m_arrCFs[lCountItems].m_lCount;

        ++lCountItems;
    }

    // на всякий случай...
    // здесь когда текст сначала другой (т.е. например - placeholder в мастере) -
    // то у нас неправильно выставился m_lCount...
    StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
}
