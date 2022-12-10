#include "ParaBuildLevel.h"

using namespace PPT;

void CRecordParaBuildLevel::ReadFromStream(POLE::Stream *pStream)
{
    SRecordHeader oHeader;

    if (oHeader.ReadFromStream(pStream))
    {
        m_oLevelInfoAtom.ReadFromStream ( oHeader, pStream );
        m_nRecordLen += oHeader.RecLen + 8;
    }

    if (oHeader.ReadFromStream(pStream))
    {
        m_oTimeNode.ReadFromStream ( oHeader, pStream );
        m_nRecordLen += oHeader.RecLen + 8;
    }
}
