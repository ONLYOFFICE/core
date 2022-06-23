#include "ElementParagraph.h"
#include "src\resources\ColorTable.h"
#include "src\resources\Constants.h"
#include "src\resources\SingletonTemplate.h"
#include "src\resources\utils.h"

namespace NSDocxRenderer
{
    CParagraph::CParagraph(const TextAssociationType& eType):
        CBaseItem(etParagraph), m_arLines()
    {
        m_eTextConversionType	 = tctUnknown;
        m_bIsNeedFirstLineIndent = false;
        m_bIsAroundTextWrapping  = true; //по умолчанию в word
        m_bIsShadingPresent      = false;
        m_lColorOfShadingFill    = 0xFFFFFF; //белый
        m_eTextAlignmentType     = tatUnknown;

        m_dLeft		= 0.0;
        m_dRight    = 0.0;
        m_dTop		= 0.0;
        m_dWidth	= 0.0;
        m_dHeight	= 0.0;
        m_dFirstLine= 0.0;

        m_dSpaceBefore = 0.0;
        m_dSpaceAfter  = 0.0;
        m_dBaselinePos = 0.0;

        m_pManagerLight = NULL;
        m_eTextAssociationType = eType;

        m_nNumLines  = 0;
    }

    CParagraph::CParagraph(const CParagraph& oSrc):
        CBaseItem(etParagraph)
    {
        *this = oSrc;
    }

    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Clear()
    {
        for (size_t i = 0; i < m_arLines.size(); ++i)
        {
            RELEASEOBJECT(m_arLines[i]);
        }
        m_arLines.clear();

        m_pManagerLight = NULL;
    }

    CParagraph& CParagraph::operator=(const CParagraph& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        CBaseItem::operator=(oSrc);

        m_eTextConversionType	 = oSrc.m_eTextConversionType;
        m_bIsNeedFirstLineIndent = oSrc.m_bIsNeedFirstLineIndent;
        m_bIsAroundTextWrapping  = oSrc.m_bIsAroundTextWrapping;
        m_bIsShadingPresent      = oSrc.m_bIsShadingPresent;
        m_lColorOfShadingFill    = oSrc.m_lColorOfShadingFill;
        m_eTextAlignmentType     = oSrc.m_eTextAlignmentType;

        m_dLeft		= oSrc.m_dLeft;
        m_dRight    = oSrc.m_dRight;
        m_dTop		= oSrc.m_dTop;
        m_dWidth	= oSrc.m_dWidth;
        m_dHeight	= oSrc.m_dHeight;
        m_dFirstLine= oSrc.m_dFirstLine;

        m_dSpaceBefore	= oSrc.m_dSpaceBefore;
        m_dSpaceAfter   = oSrc.m_dSpaceAfter;
        m_dBaselinePos  = oSrc.m_dBaselinePos;

        m_eTextAssociationType		= oSrc.m_eTextAssociationType;

        Clear();
        size_t nCount = oSrc.m_arLines.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            m_arLines.push_back(new CTextLine(*oSrc.m_arLines[i]));
        }

        m_pManagerLight = oSrc.m_pManagerLight;

        m_nNumLines = oSrc.m_nNumLines;
        return *this;
    }

    void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter)
    {
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

        size_t nCount = m_arLines.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CTextLine* pTextLine = m_arLines[i];
            if (m_eTextAssociationType != tatPlainParagraph)
            {
                pTextLine->SortConts();
            }
            pTextLine->ToXml(oWriter, m_pManagerLight);
        }

        oWriter.WriteString(L"</w:p>");
    }
}
