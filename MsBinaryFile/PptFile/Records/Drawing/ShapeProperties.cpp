#include "ShapeProperties.h"

using namespace PPT;

CRecordShapeProperties::CRecordShapeProperties()
{

}

CRecordShapeProperties::~CRecordShapeProperties()
{

}

void CRecordShapeProperties::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    m_oProperties.FromStream(pStream, m_oHeader.RecInstance);
//    _UINT32 dwLen = m_oProperties.GetLen();

    // это на всякий случай, может там напридумывают проперти с complex -
    // которые мы не поддерживаем...
    StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
}
