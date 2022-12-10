#include "ViewInfoAtom.h"

using namespace PPT;

CRecordViewInfoAtom::CRecordViewInfoAtom()
{
}

CRecordViewInfoAtom::~CRecordViewInfoAtom()
{
}

void CRecordViewInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oCurScale.FromStream(pStream);
    m_oPrevScale.FromStream(pStream);
    NSStreamReader::Read(pStream, m_oViewSize);
    NSStreamReader::Read(pStream, m_oOrigin);

    m_bZoomToFit = StreamUtils::ReadBYTE(pStream);
    m_bDraftMode = StreamUtils::ReadBYTE(pStream);

    // 2 байта зарезервированы...
    StreamUtils::StreamSkip(2, pStream);
}
