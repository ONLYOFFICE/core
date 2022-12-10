#include "SlideViewInfoAtom.h"

using namespace PPT;

CRecordSlideViewInfoAtom::CRecordSlideViewInfoAtom()
{
}

CRecordSlideViewInfoAtom::~CRecordSlideViewInfoAtom()
{
}

void CRecordSlideViewInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_bShowGuides = StreamUtils::ReadBYTE(pStream);
    m_bSnapToGrid = StreamUtils::ReadBYTE(pStream);
    m_bSnapToShape = StreamUtils::ReadBYTE(pStream);
}
