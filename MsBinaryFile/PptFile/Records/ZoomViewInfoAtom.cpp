#include "ZoomViewInfoAtom.h"

using namespace PPT;

CRecordZoomViewInfoAtom::CRecordZoomViewInfoAtom()
{
}

CRecordZoomViewInfoAtom::~CRecordZoomViewInfoAtom()
{
}

void CRecordZoomViewInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oCurScale.ReadFromStream(pStream);

    StreamUtils::StreamSkip(24, pStream);

    m_oOrigin.ReadFromStream(pStream);

    m_fUseVarScale  = StreamUtils::ReadBYTE(pStream);
    m_fDraftMode    = StreamUtils::ReadBYTE(pStream);

    StreamUtils::StreamSkip(2, pStream);
}
