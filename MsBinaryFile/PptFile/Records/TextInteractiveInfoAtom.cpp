#include "TextInteractiveInfoAtom.h"

using namespace PPT;

CRecordTextInteractiveInfoAtom::CRecordTextInteractiveInfoAtom()
{
    m_lStart	= 0;
    m_lEnd		= 0;
}

CRecordTextInteractiveInfoAtom::~CRecordTextInteractiveInfoAtom()
{
}

void CRecordTextInteractiveInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_lStart	= StreamUtils::ReadLONG(pStream);
    m_lEnd		= StreamUtils::ReadLONG(pStream);
}
