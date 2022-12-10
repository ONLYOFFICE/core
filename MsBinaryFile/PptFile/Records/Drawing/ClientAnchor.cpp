#include "ClientAnchor.h"

using namespace PPT;

CRecordClientAnchor::CRecordClientAnchor()
{

}

CRecordClientAnchor::~CRecordClientAnchor()
{

}

void CRecordClientAnchor::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    if (m_oHeader.RecLen == 0x00000008) //SSmallRectAtom
    {
        m_oBounds.Top		= StreamUtils::ReadSHORT(pStream);
        m_oBounds.Left		= StreamUtils::ReadSHORT(pStream);
        m_oBounds.Right		= StreamUtils::ReadSHORT(pStream);
        m_oBounds.Bottom	= StreamUtils::ReadSHORT(pStream);
    }
    if (m_oHeader.RecLen == 0x00000010) //SRectAtom
    {
        m_oBounds.Top		= StreamUtils::ReadLONG(pStream);
        m_oBounds.Left		= StreamUtils::ReadLONG(pStream);
        m_oBounds.Right		= StreamUtils::ReadLONG(pStream);
        m_oBounds.Bottom	= StreamUtils::ReadLONG(pStream);
    }
}
