#include "ElementParagraph.h"
#include "src\resources\ColorTable.h"
#include "src\resources\SingletonTemplate.h"
#include "src\resources\utils.h"

namespace NSDocxRenderer
{
    CParagraph::CParagraph(const TextAssociationType& eType):
        CBaseItem(ElemType::etParagraph), m_eTextAssociationType(eType)
    {
    }

    CParagraph::CParagraph(const CParagraph& oSrc):
        CBaseItem(ElemType::etParagraph)
    {
        *this = oSrc;
    }

    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Clear()
    {
        for (auto pLine : m_arLines)
        {
            pLine->Clear();
        }
        m_arLines.clear();
    }

    CParagraph& CParagraph::operator=(const CParagraph& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        Clear();

        CBaseItem::operator=(oSrc);

        m_eTextConversionType	 = oSrc.m_eTextConversionType;
        m_bIsNeedFirstLineIndent = oSrc.m_bIsNeedFirstLineIndent;
        m_bIsAroundTextWrapping  = oSrc.m_bIsAroundTextWrapping;
        m_bIsShadingPresent      = oSrc.m_bIsShadingPresent;
        m_lColorOfShadingFill    = oSrc.m_lColorOfShadingFill;
        m_eTextAlignmentType     = oSrc.m_eTextAlignmentType;

        m_dRight     = oSrc.m_dRight;
        m_dFirstLine = oSrc.m_dFirstLine;

        m_dSpaceBefore	= oSrc.m_dSpaceBefore;
        m_dSpaceAfter   = oSrc.m_dSpaceAfter;
        m_dBaselinePos  = oSrc.m_dBaselinePos;

        m_eTextAssociationType		= oSrc.m_eTextAssociationType;

        for (auto pLine : oSrc.m_arLines)
        {
            m_arLines.push_back(new CTextLine(*pLine));
        }

        m_nNumLines = oSrc.m_nNumLines;
        return *this;
    }

    void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
        if (m_bIsNotNecessaryToUse)
        {
            return;
        }

        //todo использовать паттерн builder
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

            oWriter.WriteString(L" w:line=\"");
            oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToDx));
            oWriter.WriteString(L"\" w:lineRule=\"exact\""); // exact - точный размер строки

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

        for(auto pLine : m_arLines)
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

        for(auto pLine : m_arLines)
        {
            if (pLine->m_pDominantShape)
            {
                for (auto pCont : pLine->m_arConts)
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
        if (m_nNumLines < 2)
        {
            return;
        }

        CTextLine* pLine = m_arLines.front();

        for(size_t i = 1; i < m_arLines.size(); i++)
        {
            if (pLine->m_arConts.back()->m_bIsNeedSpace)
            {
                pLine->m_arConts.back()->m_oText += L" ";
                pLine->m_arConts.back()->m_bIsNeedSpace = false;
            }

            auto pNext = m_arLines[i];

            for (auto pCont : pNext->m_arConts)
            {
                if (pLine->m_arConts.back()->IsEqual(pCont))
                {
                    pLine->m_arConts.back()->m_oText += pCont->m_oText;
                }
                else
                {
                    pLine->m_arConts.push_back(new CContText(*pCont));
                }
                pCont->m_bIsNotNecessaryToUse = true;
            }

            pNext->m_bIsNotNecessaryToUse = true;
        }
    }
}
