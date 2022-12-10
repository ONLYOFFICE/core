#include "GroupShape.h"

using namespace PPT;

CRecordGroupShape::CRecordGroupShape()
{

}

CRecordGroupShape::~CRecordGroupShape()
{

}

void CRecordGroupShape::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oBounds.left		= StreamUtils::ReadLONG(pStream);
    m_oBounds.top		= StreamUtils::ReadLONG(pStream);
    m_oBounds.right		= StreamUtils::ReadLONG(pStream);
    m_oBounds.bottom	= StreamUtils::ReadLONG(pStream);
}
