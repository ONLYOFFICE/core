#include "BuildAtom.h"

using namespace PPT;

void CRecordBuildAtom::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
    m_oHeader = oHeader;

    m_BuildType         = (BuildTypeEnum)StreamUtils::ReadDWORD(pStream);
    m_BuildId           = StreamUtils::ReadDWORD(pStream);
    m_ShapeIdRef        = StreamUtils::ReadDWORD(pStream);

    m_fExpanded         = StreamUtils::ReadBYTE(pStream);
    m_fUIExpanded       = StreamUtils::ReadBYTE(pStream);

    StreamUtils::StreamSkip(2, pStream);
}
