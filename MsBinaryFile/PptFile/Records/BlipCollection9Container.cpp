#include "BlipCollection9Container.h"

using namespace PPT;

void CRecordBlipCollection9Container::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
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

        std::shared_ptr<CRecordBlipEntityAtom> pRec(new CRecordBlipEntityAtom);
        pRec->ReadFromStream(ReadHeader, pStream);
        m_rgBlipEntityAtom.push_back(pRec);
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
