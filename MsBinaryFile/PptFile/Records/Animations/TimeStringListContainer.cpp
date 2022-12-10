#include "TimeStringListContainer.h"

using namespace PPT;

void CRecordTimeStringListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;

    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);

    UINT lCurLen		=	0;
    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if (ReadHeader.ReadFromStream(pStream) == false)
            break;

        CRecordTimeVariantString Element;
        Element.ReadFromStream  ( ReadHeader, pStream );
        lCurLen += 8 + ReadHeader.RecLen;


        m_arrRgChildRec.push_back ( Element );
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
