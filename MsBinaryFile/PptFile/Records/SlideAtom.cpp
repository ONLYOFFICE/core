#include "SlideAtom.h"

using namespace PPT;

CRecordSlideAtom::CRecordSlideAtom()
{
    m_oLayout.m_hash = 0;
}

CRecordSlideAtom::~CRecordSlideAtom()
{
}

void CRecordSlideAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_oLayout.m_nGeom = (INT)StreamUtils::ReadLONG(pStream);
    for (int nIndex = 0; nIndex < 8; nIndex++)
    {
        m_oLayout.m_pPlaceHolderID[nIndex] = StreamUtils::ReadBYTE(pStream);
        m_oLayout.m_hash = (m_oLayout.m_hash << 4) + m_oLayout.m_pPlaceHolderID[nIndex];
    }

    m_oLayout.m_hash += (((_UINT64)m_oLayout.m_nGeom) << 32) ;

    m_nMasterIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
    m_nNotesIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

    USHORT nFlag = StreamUtils::ReadWORD(pStream);

    m_bMasterObjects	= ((nFlag & 0x01) == 0x01);
    m_bMasterScheme		= ((nFlag & 0x02) == 0x02);
    m_bMasterBackground = ((nFlag & 0x04) == 0x04);

    StreamUtils::StreamSkip(2, pStream);
}
