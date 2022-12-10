#include "DocumentRecords.h"
#include "RecordsIncluder.h"

using namespace PPT;

CRecordDocument::CRecordDocument()
{

}

CRecordDocument::~CRecordDocument()
{
    m_arMasterPersists.clear();
    m_arNotePersists.clear();
    m_arSlidePersists.clear();
}

void CRecordDocument::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_arMasterPersists.clear();
    m_arNotePersists.clear();
    m_arSlidePersists.clear();

    m_arRecords.clear();

    CRecordsContainer::ReadFromStream(oHeader, pStream);

    std::vector<CRecordSlideListWithText*> oArraySlideWithText;
    this->GetRecordsByType(&oArraySlideWithText, true, false);

    for (size_t nIndexList = 0; nIndexList < oArraySlideWithText.size(); ++nIndexList)
    {
        CRecordSlideListWithText* pAtom = oArraySlideWithText[nIndexList];
        std::vector<SSlidePersist>* pArray = NULL;

        switch (pAtom->m_Type)
        {
            case CRecordSlideListWithText::CollectionOfMasterSlides:
            {
                pArray = &m_arMasterPersists;
            }break;
            case CRecordSlideListWithText::CollectionOfNotesSlides:
            {
                pArray = &m_arNotePersists;
            }break;
            case CRecordSlideListWithText::CollectionOfSlides:
            {
                pArray = &m_arSlidePersists;
            }break;
            default:
            {
                // этого не может быть...
                continue;
            }
        };

        size_t nCountItems = pAtom->m_arSlides.size();
        for (size_t index = 0; index < nCountItems; ++index)
        {
            SSlidePersist oPersist;
            oPersist.m_nPsrRef = pAtom->m_arSlides[index]->m_nPsrRef;
            oPersist.m_nSlideID = pAtom->m_arSlides[index]->m_nSlideID;

//            oPersist.m_arTextAttrs.insert(oPersist.m_arTextAttrs.end(), pAtom->m_arTextPlaceHolders[index].begin(),pAtom->m_arTextPlaceHolders[index].end());
            for (auto& textPlaceHolder : pAtom->m_arTextPlaceHolders[index])
                            oPersist.m_arTextAttrs.push_back(&textPlaceHolder);

            pArray->push_back(oPersist);
        }
    }
}
