#include "ParaBuildContainer.h"

using namespace PPT;

CRecordParaBuildContainer::CRecordParaBuildContainer()
{

}

CRecordParaBuildContainer::~CRecordParaBuildContainer()
{

}

void CRecordParaBuildContainer::ReadFromStream(SRecordHeader &header, POLE::Stream *pStream)
{
    LONG lPos(0); StreamUtils::StreamPosition(lPos, pStream);
    CRecordBuildListSubContainer::ReadFromStream(header, pStream);

    UINT lCurLen = buildAtom.m_oHeader.RecLen + 8 + 24; // BuildAtom - 24


    SRecordHeader paraBuildAtomHeader;
    if (paraBuildAtomHeader.ReadFromStream(pStream))
    {
        m_oParaBuildAtom.ReadFromStream ( paraBuildAtomHeader, pStream );
        lCurLen += paraBuildAtomHeader.RecLen + 8;
    }

    while (lCurLen < m_oHeader.RecLen )
    {
        CRecordParaBuildLevel buildLevel;
        buildLevel.ReadFromStream(pStream);

        rgParaBuildLevel.push_back(buildLevel);

        lCurLen += buildLevel.getRecordLen();
    }

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
