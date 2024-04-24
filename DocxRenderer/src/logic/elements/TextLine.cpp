/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "TextLine.h"
#include "../../resources/Constants.h"
#include "../../resources/SortElements.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
    CTextLine::CTextLine() : CBaseItem(ElemType::etTextLine)
    {
    }

    void CTextLine::Clear()
    {
        m_arConts.clear();
    }

    CTextLine::~CTextLine()
    {
        Clear();
    }

    void CTextLine::AddCont(CContText *pCont)
    {
        m_dBaselinePos = std::max(m_dBaselinePos, pCont->m_dBaselinePos);

        if ( ( pCont->m_dLeft > 0 ) && ( ( m_dLeft == 0 ) || ( pCont->m_dLeft < m_dLeft ) ) )
            m_dLeft = pCont->m_dLeft;

        if (m_dHeight < pCont->m_dHeight)
            m_dHeight = pCont->m_dHeight;

        if (m_dTop > pCont->m_dTop || m_dTop == 0.0)
            m_dTop = pCont->m_dTop;

        if (pCont->m_pCont && m_eVertAlignType == eVertAlignType::vatUnknown)
        {
            m_eVertAlignType = pCont->m_eVertAlignType;
        }

        m_arConts.push_back(pCont);
    }

    bool CTextLine::IsBigger(const CBaseItem* oSrc)
    {
        return (m_dBaselinePos > dynamic_cast<const CTextLine*>(oSrc)->m_dBaselinePos) ? true : false;
    }

    bool CTextLine::IsBiggerOrEqual(const CBaseItem* oSrc)
    {
        return (m_dBaselinePos >= dynamic_cast<const CTextLine*>(oSrc)->m_dBaselinePos) ? true : false;
    }

    void CTextLine::SortConts()
    {
        // сортировка непрерывных слов по m_dX
        SortElements(m_arConts);
    }

    void CTextLine::Merge(CTextLine* pLine)
    {
        size_t nCount = pLine->m_arConts.size();
        if (0 != nCount)
        {
            if (pLine->m_dLeft < m_dLeft)
            {
                m_dLeft = pLine->m_dLeft;
            }
            if (pLine->m_dBaselinePos < m_dBaselinePos)
            {
                m_dHeight = (m_dBaselinePos - pLine->m_dBaselinePos + pLine->m_dHeight);
            }
            else
            {
                m_dHeight = (pLine->m_dBaselinePos - m_dBaselinePos + m_dHeight);
            }

            for (const auto &pCont : pLine->m_arConts)
            {
                m_arConts.push_back(new CContText(*pCont));
            }

            SortConts();
            CalculateWidth();
        }
    }

    void CTextLine::MergeConts()
    {
        if (m_arConts.empty())
            return;

        auto pFirst = m_arConts.front();

        for (size_t i = 1; i < m_arConts.size(); ++i)
        {
            auto pCurrent = m_arConts[i];

            if (pCurrent->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            bool bIsEqual = pFirst->IsEqual(pCurrent) && pFirst->m_eVertAlignType == pCurrent->m_eVertAlignType;
            bool bIsBigDelta = fabs(pFirst->m_dRight - pCurrent->m_dLeft) > pFirst->CalculateThinSpace();
            bool bIsVeryBigDelta = fabs(pFirst->m_dRight - pCurrent->m_dLeft) > pFirst->CalculateWideSpace();

            if (bIsVeryBigDelta)
            {
                pFirst->m_bSpaceIsNotNeeded = false;
                pFirst = pCurrent;

            }
            else if (bIsEqual)
            {
                if (bIsBigDelta)
                {
                    pFirst->m_oText += L" ";
                    pFirst->m_dWidth += pFirst->m_dSpaceWidthMM;
                }

                pFirst->m_oText += pCurrent->m_oText;
                pFirst->m_dWidth += pCurrent->m_dWidth;
                pFirst->m_dRight = pCurrent->m_dRight;

                pFirst->m_bSpaceIsNotNeeded = true;
                pCurrent->m_bIsNotNecessaryToUse = true;
            }
            else
            {
                if (bIsBigDelta)
                {
                    if (!IsSpaceUtf32(pFirst->m_oText[pFirst->m_oText.length()-1]) &&
                        !IsSpaceUtf32(pCurrent->m_oText[0]))
                    {
                        if (pFirst->GetNumberOfFeatures() <= pCurrent->GetNumberOfFeatures())
                        {
                            pFirst->m_oText += L" ";
                            pFirst->m_dWidth += pFirst->m_dSpaceWidthMM;
                        }
                        else
                        {
                            NSStringUtils::CStringUTF32 oNewText = L" ";
                            oNewText += pCurrent->m_oText;
                            pCurrent->m_oText = oNewText;
                            pCurrent->m_dWidth += pCurrent->m_dSpaceWidthMM;
                        }
                    }

                    pFirst->m_bSpaceIsNotNeeded = true;
                }
                else
                {
                    pFirst->m_bSpaceIsNotNeeded = false;
                }
                pFirst = pCurrent;
            }
        }
    }

    void CTextLine::CalculateWidth()
    {
        if (m_arConts.empty())
        {
            return;
        }

        m_dWidth = m_arConts[0]->m_dWidth;

        for (size_t i = 1; i < m_arConts.size(); ++i)
        {
            m_dWidth += m_arConts[i]->m_dLeft - (m_arConts[i-1]->m_dLeft + m_arConts[i-1]->m_dWidth);
            m_dWidth += m_arConts[i]->m_dWidth;
        }
        m_dRight = m_dLeft + m_dWidth;
    }

    void CTextLine::DetermineAssumedTextAlignmentType(double dWidthOfPage)
    {
        //рассматриваем строки, которые короче трети ширины страницы
        double maxTextLineWidth = dWidthOfPage/3; //нужна какая-нибудь отправная точка...
        double delta = 2 * c_dCENTER_POSITION_ERROR_MM; //координата m_dWidth/2 +- c_dCENTER_POSITION_ERROR_MM

        if (fabs(dWidthOfPage/2 - m_dLeft - m_dWidth/2) <= delta && //если середины линий по x одинаковы
                m_dWidth < maxTextLineWidth )
        {
            m_eAlignmentType = atatByCenter;
        }
        else if ((m_dLeft + m_dWidth/2) > (dWidthOfPage/2 + c_dCENTER_POSITION_ERROR_MM) && //середина строки правее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByRightEdge;
        }
        else if ((m_dLeft + m_dWidth/2) < (dWidthOfPage/2 - c_dCENTER_POSITION_ERROR_MM) && //середина строки левее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByLeftEdge;
        }
        else if (fabs(dWidthOfPage/2 - m_dLeft - m_dWidth/2) <= delta &&
                 m_dWidth > maxTextLineWidth + maxTextLineWidth/2 )
        {
            m_eAlignmentType = atatByWidth;
        }
        else
        {
            m_eAlignmentType = atatUnknown;
        }
    }

    bool CTextLine::AreAlignmentsAppropriate(const CTextLine *pLine)
    {
        if ((m_eAlignmentType == pLine->m_eAlignmentType && m_eAlignmentType!= atatByLeftEdge) ||
            (m_eAlignmentType == atatByWidth && pLine->m_eAlignmentType == atatByLeftEdge) ||
            (m_eAlignmentType == atatByWidth && pLine->m_eAlignmentType == atatUnknown) ||
            (m_eAlignmentType == atatUnknown && pLine->m_eAlignmentType == atatByWidth))
        {
            return true;
        }
        return false;
    }

    void CTextLine::SetVertAlignType(const eVertAlignType& oType)
    {
        for (const auto &pCont : m_arConts)
        {
            pCont->m_eVertAlignType = oType;
        }
    }

    double CTextLine::CalculateBeforeSpacing(double dPreviousStringBaseline)
    {
        return m_dBaselinePos - dPreviousStringBaseline - m_dHeight;
    }

    double CTextLine::CalculateRightBorder(const double& dPageWidth)
    {
        return dPageWidth - (m_dLeft + m_dWidth);
    }

    bool CTextLine::IsForceBlock()
    {
        // линия отсортирована, так что сравниваем только соседние conts
        size_t nCount = m_arConts.size();
        if (nCount <= 1)
            return false;

        for (size_t i = 0; i < nCount; i++)
        {
            for (size_t j = i + 1; j < nCount; j++)
            {
                if (m_arConts[i]->GetIntersect(m_arConts[j]) > 10)
                    return true;
            }
        }
        return false;
    }

    void CTextLine::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        size_t nCountConts = m_arConts.size();

        if (0 == nCountConts)
            return;

        auto pPrev = m_arConts[0];
        double dDelta = 0;

        for (size_t i = 1; i < nCountConts; ++i)
        {
            auto pCurrent = m_arConts[i];

            if (pCurrent->m_bIsNotNecessaryToUse)
            {
                continue;
            }

            dDelta = pCurrent->m_dLeft - pPrev->m_dRight;

            if (dDelta < pPrev->CalculateWideSpace() ||
                pPrev->m_bSpaceIsNotNeeded)
            {
                // просто текст на тексте или сменились настройки (font/brush)
                pPrev->ToXml(oWriter);
                pPrev = pCurrent;
            }
            else
            {
                // расстояние слишком большое. нужно сделать большой пробел
                pPrev->ToXml(oWriter);
                pPrev->AddWideSpaceToXml(dDelta, oWriter, pPrev->IsEqual(pCurrent));
                pPrev = pCurrent;
            }
        }

        pPrev->ToXml(oWriter);
    }
}
