#include "SlideListWithText.h"

using namespace PPT;

CRecordSlideListWithText::CRecordSlideListWithText() : m_arSlides()
{
    m_Type = CollectionOfSlides;
}

CRecordSlideListWithText::~CRecordSlideListWithText()
{
}

void CRecordSlideListWithText::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_Type = (Instances)m_oHeader.RecInstance;

    if (m_oHeader.IsContainer())
    {
        // а по-другому и быть не могло...
        _UINT32 lSymbolCount = 0;

        UINT lCurLen = 0;
        SRecordHeader oRec;

        while (lCurLen < m_oHeader.RecLen)
        {
            if (oRec.ReadFromStream(pStream) == FALSE)
            {
                break;
            }

            IRecord* pRecord = CreateByType(oRec);

            if (RT_StyleTextPropAtom == oRec.RecType)
            {
                ((CRecordStyleTextPropAtom*)(pRecord))->m_lCount = lSymbolCount;
            }
            else if (RT_TextSpecialInfoAtom == oRec.RecType)
            {
                ((CRecordTextSpecInfoAtom*)(pRecord))->m_lCount = lSymbolCount;
            }

            pRecord->ReadFromStream(oRec, pStream);
            lCurLen += (8 + oRec.RecLen);

            if (RT_SlidePersistAtom == oRec.RecType)
            {
                m_arSlides.push_back((CRecordSlidePersistAtom*)pRecord);

                std::vector<CTextFullSettings> elm;
                m_arTextPlaceHolders.push_back(elm);
            }
            else
            {
                long nCurrentSlide = (long)m_arSlides.size() - 1;
                if (0 > nCurrentSlide)
                    continue;

                CRecordTextHeaderAtom* pHeader = dynamic_cast<CRecordTextHeaderAtom*>(pRecord);
                if (NULL != pHeader)
                {
                    CTextFullSettings oAttr;
                    m_arTextPlaceHolders[nCurrentSlide].push_back(oAttr);
                    m_arTextPlaceHolders[nCurrentSlide][m_arTextPlaceHolders[nCurrentSlide].size() - 1].m_nTextType = pHeader->m_nTextType;

                    m_arRecords.push_back(pRecord);
                    continue;
                }
                CRecordTextCharsAtom* pChars = dynamic_cast<CRecordTextCharsAtom*>(pRecord);
                CRecordTextBytesAtom* pBytes = dynamic_cast<CRecordTextBytesAtom*>(pRecord);

                long nCurrentTextHeader = (long)m_arTextPlaceHolders[nCurrentSlide].size() - 1;
                if (0 > nCurrentTextHeader)
                    continue;

                // здесь еще настойки кроме текста
                if (NULL != pChars)
                {
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_strText = pChars->m_strText;
                    lSymbolCount = (_UINT32)pChars->m_strText.length();
                }
                else if (NULL != pBytes)
                {
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_strText = pBytes->m_strText;
                    lSymbolCount = (_UINT32)pBytes->m_strText.length();
                }

                if (RT_StyleTextPropAtom == oRec.RecType)
                {
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextStyleProp =
                            dynamic_cast<CRecordStyleTextPropAtom*>(pRecord);
                }
                if (RT_TextSpecialInfoAtom == oRec.RecType)
                {
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextSpecInfo =
                            dynamic_cast<CRecordTextSpecInfoAtom*>(pRecord);
                }
                if (RT_TextRulerAtom == oRec.RecType)
                {
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextRuler =
                            dynamic_cast<CRecordTextRulerAtom*>(pRecord);
                }
                if (RT_TextInteractiveInfoAtom == oRec.RecType)
                {
                    CRecordTextInteractiveInfoAtom* pTxRanges = dynamic_cast<CRecordTextInteractiveInfoAtom*>(pRecord);
                    PPT::CTextRange oRange;
                    oRange.m_lStart	= pTxRanges->m_lStart;
                    oRange.m_lEnd	= pTxRanges->m_lEnd;
                    m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_arRanges.push_back(oRange);
                }
            }

            m_arRecords.push_back(pRecord);
        }
    }
}
