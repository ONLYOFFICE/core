#include "DocInfoListContainer.h"

using namespace PPT;

void DocInfoListSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    switch (oHeader.RecType)
    {
    case RT_ProgTags:
    {
        m_record.reset(new CRecordDocProgTagsContainer);
        m_record->ReadFromStream(oHeader, pStream);
        break;
    }
    case RT_VbaInfo:
    {
        m_record.reset(new CRecordVBAInfoContainer);
        m_record->ReadFromStream(oHeader, pStream);
        break;
    }
//        case RT_OutlineViewInfo:
//        {

//        }
    default:
        StreamUtils::StreamSkip(oHeader.RecLen, pStream);
        break;
    }
}

CRecordDocInfoListContainer::CRecordDocInfoListContainer()
{

}

CRecordDocInfoListContainer::~CRecordDocInfoListContainer()
{
    for (auto child : m_rgChildRec)
    {
        RELEASEOBJECT(child)
    }
}

void CRecordDocInfoListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos; StreamUtils::StreamPosition(lPos, pStream);
    LONG lCurLen = 0;
    SRecordHeader ReadHeader;

    while (lCurLen < (LONG)m_oHeader.RecLen)
    {

        if (!ReadHeader.ReadFromStream(pStream))
        {
            break;
        }


        lCurLen += 8 + ReadHeader.RecLen;

        auto pRec = new DocInfoListSubContainerOrAtom;
        pRec->ReadFromStream(ReadHeader, pStream);
        m_rgChildRec.push_back(pRec);
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

IRecord *CRecordDocInfoListContainer::getDocBinaryTagExtension(const std::wstring &extVersion)
{
    for (auto* pChild : m_rgChildRec)
    {
        if(pChild == nullptr || pChild->m_record.IsInit() == false)
            continue;

        auto* pDocProgTagsCont = dynamic_cast<CRecordDocProgTagsContainer*>(pChild->m_record.GetPointer());
        if (pDocProgTagsCont == nullptr)
            continue;

        return pDocProgTagsCont->getDocBinaryTagExtension(extVersion);
    }
    return nullptr;
}

CRecordVBAInfoAtom *CRecordDocInfoListContainer::getVBAInfoAtom() const
{
    for (const auto* pChild : m_rgChildRec)
    {
        if (!pChild || !pChild->m_record.IsInit())
            continue;
        IRecord* pRecord = pChild->m_record.GetPointer();
        auto* pVBA = dynamic_cast<CRecordVBAInfoContainer*>(pRecord);
        if (pVBA)
            return static_cast<CRecordVBAInfoAtom*>(pVBA->m_arRecords[0]);
    }

    return nullptr;
}
