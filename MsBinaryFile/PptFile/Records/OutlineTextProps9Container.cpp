#include "OutlineTextProps9Container.h"

using namespace PPT;

void CRecordOutlineTextProps9Entry::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordOutlineTextPropsHeaderExAtom::ReadFromStream(oHeader, pStream);

    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    if (ReadHeader.bBadHeader)
    {
        oHeader.bBadHeader = true; // GZoabli_PhD.ppt
        return;
    }

    m_styleTextProp9Atom.ReadFromStream(ReadHeader, pStream);
}

CRecordOutlineTextProps9Container::~CRecordOutlineTextProps9Container()
{
    for (auto pEl : m_rgOutlineTextProps9Entry)
    {
        RELEASEOBJECT(pEl)
    }
}

void CRecordOutlineTextProps9Container::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    LONG lPos = 0;
    StreamUtils::StreamPosition(lPos, pStream);

    UINT lCurLen = 0;

    SRecordHeader ReadHeader;

    while (lCurLen < m_oHeader.RecLen)
    {
        if (ReadHeader.ReadFromStream(pStream) == false || ReadHeader.bBadHeader)
            break;

        lCurLen += 8 + ReadHeader.RecLen;

        auto pRec = new CRecordOutlineTextProps9Entry;
        pRec->ReadFromStream(ReadHeader, pStream);

        if (ReadHeader.bBadHeader)
            break;

        m_rgOutlineTextProps9Entry.push_back(pRec);
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
