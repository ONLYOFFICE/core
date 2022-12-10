#include "StyleTextProp9Atom.h"

using namespace PPT;

CRecordStyleTextProp9Atom::~CRecordStyleTextProp9Atom()
{
}

void CRecordStyleTextProp9Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lCurPos; StreamUtils::StreamPosition(lCurPos, pStream);
    LONG lEndPos = lCurPos + m_oHeader.RecLen;

    while(lCurPos < lEndPos)
    {
        SStyleTextProp9 rec;
        rec.ReadFromStream(pStream);
        m_rgStyleTextProp9.push_back(rec);

        StreamUtils::StreamPosition(lCurPos, pStream);
    }
}

void SStyleTextProp9::ReadFromStream(POLE::Stream *pStream)
{
    m_pf9.ReadFromStream(pStream);
    m_cf9.ReadFromStream(pStream);
    m_si.ReadFromStream(pStream);
}
