#include "FontCollection10Container.h"

using namespace PPT;

CRecordFontCollection10Container::~CRecordFontCollection10Container()
{
    for (auto pEl : m_rgFontCollectionEntry)
    {
        RELEASEOBJECT(pEl)
    }
}

void CRecordFontCollection10Container::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader			=	oHeader;
    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    _UINT32 lCurLen		=	0;

    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen += 8 + ReadHeader.RecLen;

        auto pRec = new FontCollectionEntry;
        pRec->ReadFromStream(pStream);
        m_rgFontCollectionEntry.push_back(pRec);
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
