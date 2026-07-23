/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
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
        // and it couldn't be otherwise...
        _UINT32 lSymbolCount = 0;

        UINT lCurLen = 0;
        SRecordHeader oRec;

        while (lCurLen < m_oHeader.RecLen)
        {
            if (oRec.ReadFromStream(pStream) == FALSE)
            {
                break;
            }

            IRecord* pRecord = CreateByType(oRec, m_pCommonInfo);

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

                // here are more settings besides text
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
