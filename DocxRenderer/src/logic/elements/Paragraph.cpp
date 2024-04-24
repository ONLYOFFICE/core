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

#include "Paragraph.h"
#include "src/resources/ColorTable.h"
#include "src/resources/utils.h"

namespace NSDocxRenderer
{
    CParagraph::CParagraph(const TextAssociationType& eType):
        CBaseItem(ElemType::etParagraph), m_eTextAssociationType(eType)
    {
    }

    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Clear()
    {
        m_arLines.clear();
    }

    void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        oWriter.WriteString(L"<w:p>");
        oWriter.WriteString(L"<w:pPr>");

        switch (m_eTextConversionType)
        {
        case tctTextToFrame:
        {
            oWriter.WriteString(L"<w:framePr");

            if (m_eTextAssociationType == tatPlainParagraph)
            {
                if (m_bIsAroundTextWrapping)
                {
                    oWriter.WriteString(L" w:wrap=\"around\"");
                }
                else
                {
                    oWriter.WriteString(L" w:wrap=\"notBeside\"");
                }
            }

            oWriter.WriteString(L" w:hAnchor=\"page\"");
            oWriter.WriteString(L" w:vAnchor=\"page\"");

            oWriter.WriteString(L" w:x=\"");
            oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToDx));
            oWriter.WriteString(L"\"");

            oWriter.WriteString(L" w:y=\"");
            oWriter.AddInt(static_cast<int>(m_dTop * c_dMMToDx));
            oWriter.WriteString(L"\"");

            oWriter.WriteString(L"/>"); //конец w:framePr
            break;
        }
        case tctTextToShape:
        case tctTextToParagraph:
        {
            oWriter.WriteString(L"<w:spacing");
            if (m_eTextConversionType == tctTextToParagraph)
            {
                oWriter.WriteString(L" w:before=\"");
                oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }

            if (m_eTextConversionType == tctTextToShape)
            {
                oWriter.WriteString(L" w:after=\"");
                oWriter.AddInt(static_cast<int>(m_dSpaceAfter * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }

            if (m_dHeight > 0)
            {
                oWriter.WriteString(L" w:line=\"");
                oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToDx));
                oWriter.WriteString(L"\" w:lineRule=\"exact\""); // exact - точный размер строки
            }

            oWriter.WriteString(L"/>"); //конец w:spacing

            oWriter.WriteString(L"<w:ind");
            if (m_dLeft > 0)
            {
                oWriter.WriteString(L" w:left=\"");
                oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            if (m_dRight > 0)
            { 
                oWriter.WriteString(L" w:right=\"");
                oWriter.AddInt(static_cast<int>(m_dRight * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            if (m_bIsNeedFirstLineIndent)
            {
                oWriter.WriteString(L" w:firstLine=\"");
                oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>"); //конец w:ind


            if (m_eTextAssociationType == tatPlainParagraph)
            {
                switch (m_eTextAlignmentType)
                {
                case tatByCenter:
                    oWriter.WriteString(L"<w:jc w:val=\"center\"/>");
                    break;
                case tatByRightEdge:
                    oWriter.WriteString(L"<w:jc w:val=\"end\"/>");
                    break;
                case tatByWidth:
                    oWriter.WriteString(L"<w:jc w:val=\"both\"/>");
                    break;
                case tatByLeftEdge:
                    oWriter.WriteString(L"<w:jc w:val=\"begin\"/>");
                    break;
                case tatUnknown:
                default: //по умолчанию выравнивание по левому краю - можно ничего не добавлять
                    break;
                }
            }

            if (m_bIsShadingPresent)
            {
                oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
                oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lColorOfShadingFill));
                oWriter.WriteString(L"\"/>");
            }
            break;
        }
        default:
            break;
        }

        oWriter.WriteString(L"</w:pPr>");

        for(const auto &pLine : m_arLines)
        {
            pLine->ToXml(oWriter);
        }

        oWriter.WriteString(L"</w:p>");
    }

    void CParagraph::RemoveHighlightColor()
    {
        if (!m_bIsShadingPresent)
        {
            return;
        }

        for(const auto &pLine : m_arLines)
        {
            if (pLine->m_pDominantShape)
            {
                for (const auto &pCont : pLine->m_arConts)
                {
                    if (m_lColorOfShadingFill == pCont->m_lHighlightColor)
                    {
                        pCont->m_bIsHighlightPresent = false;
                    }
                }
            }
        }
    }

    void CParagraph::MergeLines()
    {
        auto pLine = m_arLines.front();

        for(size_t i = 1; i < m_arLines.size(); i++)
        {
            auto pLastCont = pLine->m_arConts.back();
            size_t iNumConts = pLine->m_arConts.size() - 1;

            while (pLastCont->m_bIsNotNecessaryToUse)
            {
                pLastCont = pLine->m_arConts[--iNumConts];
            }

            //Добавляем пробел в конец каждой строки
            pLastCont->m_oText += L" ";
            pLastCont->m_bSpaceIsNotNeeded = true;
            pLastCont->m_dWidth += pLine->m_arConts.back()->m_dSpaceWidthMM;

            auto pNext = m_arLines[i];

            auto pCont = pNext->m_arConts.front();

            if (pLastCont->IsEqual(pCont) &&
                pLastCont->m_eVertAlignType == pCont->m_eVertAlignType)
            {
                pLastCont->m_oText += pCont->m_oText;
                pLastCont->m_dWidth += pCont->m_dWidth;
                pLastCont->m_dRight = pCont->m_dRight;

                pLastCont->m_bSpaceIsNotNeeded = false;

                pCont->m_bIsNotNecessaryToUse = true;
            }

            for (const auto &pCont : pNext->m_arConts)
            {
                if (!pCont->m_bIsNotNecessaryToUse)
                {
                    pLine->m_arConts.push_back(pCont);
                }
            }

            pNext->m_bIsNotNecessaryToUse = true;
        }
    }
}
