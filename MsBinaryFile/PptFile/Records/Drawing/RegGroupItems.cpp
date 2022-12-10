#include "RegGroupItems.h"

using namespace PPT;


CRecordRegGroupItems::CRecordRegGroupItems()
{
}

CRecordRegGroupItems::~CRecordRegGroupItems()
{
}

void CRecordRegGroupItems::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_arItemsNew.clear();
    m_arItemsOld.clear();

    LONG lCount = (LONG)(oHeader.RecLen / 4);

    for (LONG i = 0; i < lCount; ++i)
    {
        WORD w1 = StreamUtils::ReadWORD(pStream);
        WORD w2 = StreamUtils::ReadWORD(pStream);

        m_arItemsNew.push_back(w1);
        m_arItemsOld.push_back(w2);
    }
}
