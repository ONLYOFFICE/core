#include "ProgStringTagContainer.h"

using namespace PPT;

CRecordProgStringTagContainer::CRecordProgStringTagContainer()
{

}

CRecordProgStringTagContainer::~CRecordProgStringTagContainer()
{
    RELEASEOBJECT(m_pTagValueAtom)
}

void CRecordProgStringTagContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    SRecordHeader ReadHeader;
    ReadHeader.ReadFromStream(pStream);

    m_oTagNameAtom.ReadFromStream(ReadHeader, pStream);
    if (m_oHeader.RecLen > 8 + ReadHeader.RecLen)
    {
        m_pTagValueAtom = new CRecordTagValueAtom();
        ReadHeader.ReadFromStream(pStream);
        m_pTagValueAtom->ReadFromStream(ReadHeader, pStream);
    }
}
