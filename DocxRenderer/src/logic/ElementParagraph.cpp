#include "ElementParagraph.h"

namespace NSDocxRenderer
{
    void RemoveSpacesAtBothEnds(NSStringUtils::CStringUTF32& oText)
    {
        size_t nLen = oText.length();
        size_t nStart = 0;

        while ((nStart < nLen) && (' ' == oText[nStart]))
            ++nStart;

        if (nStart == nLen)
        {
            oText = L"";
            return;
        }

        size_t nEnd = nLen - 1;
        while ((nEnd > nStart) && (' ' == oText[nEnd]))
            --nEnd;

        oText = oText.substr(nStart, nEnd - nStart + 1);
    }

    CContText::CContText()
    {
        m_strPickFontName	= L"";
        m_lPickFontStyle	= 0;

        m_dX			= 0;
        m_dY			= 0;
        m_dWidth		= 0;
        m_dHeight		= 0;
        m_dLastX        = 0;

        m_dWidthWithoutSpaces	= 0;
        m_dLeftWithoutSpaces	= 0;

        m_dPosition		= 0;
        m_dSpaceWidthMM	= 0;

        m_dCalculateWidth = 0;
        m_dSpaceByText = 0;

        m_bIsNeedSpaceAtTheEnd = false;
    }

    void CContText::Clear()
    {
    }

    CContText::CContText(const CContText& oSrc)
    {
        *this = oSrc;
    }
    CContText& CContText::operator=(const CContText& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        m_oFont		= oSrc.m_oFont;
        m_oBrush	= oSrc.m_oBrush;

        m_oText	 = oSrc.m_oText;
        m_oGidText = oSrc.m_oGidText;

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;

        m_dX		= oSrc.m_dX;
        m_dY		= oSrc.m_dY;
        m_dWidth	= oSrc.m_dWidth;
        m_dHeight	= oSrc.m_dHeight;

        m_dLastX    = oSrc.m_dLastX;

        m_dWidthWithoutSpaces	= oSrc.m_dWidthWithoutSpaces;
        m_dLeftWithoutSpaces	= oSrc.m_dLeftWithoutSpaces;

        m_dPosition = oSrc.m_dPosition;
        m_dSpaceWidthMM = oSrc.m_dSpaceWidthMM;

        m_dCalculateWidth = oSrc.m_dCalculateWidth;
        m_dSpaceByText = oSrc.m_dSpaceByText;

        m_bIsNeedSpaceAtTheEnd = oSrc.m_bIsNeedSpaceAtTheEnd;

        return *this;
    }

    bool CContText::IsBigger(const CContText* oSrc)
    {
        return (m_dX > oSrc->m_dX) ? true : false;
    }
    bool CContText::IsBiggerOrEqual(const CContText* oSrc)
    {
        return (m_dX >= oSrc->m_dX) ? true : false;
    }

    double CContText::GetIntersect(const CContText* oSrc) const
    {
        double d1 = std::max(m_dX, oSrc->m_dX);
        double d2 = std::min(m_dX + m_dWidth, oSrc->m_dX + oSrc->m_dWidth);

        if (d2 > d1)
            return d2 - d1;
        return 0;
    }

    void CContText::Write(NSStringUtils::CStringBuilder& oWriter,
                          CFontManagerLight* pManagerLight,
                          bool bIsAddSpace)
    {
        oWriter.WriteString(L"<w:r><w:rPr>");

        if (m_dWidth != m_dWidthWithoutSpaces)
        {
            RemoveSpacesAtBothEnds(m_oText);
            m_dWidth = m_dWidthWithoutSpaces;
        }

        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (m_oFont.Italic)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            if (bIsAddSpace || m_bIsNeedSpaceAtTheEnd)
            {
                m_dWidth += m_dSpaceWidthMM;
                m_oText += L" ";
            }
        }
        else
        {
            if (0x01 == (0x01 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (0x02 == (0x02 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            if (bIsAddSpace || m_bIsNeedSpaceAtTheEnd)
            {
                m_dWidth  += pManagerLight->GetSpaceWidth();
                m_oText += L" ";
            }

            // нужно перемерять...
            double ___dSize = (double)((LONG)(m_oFont.Size * 2)) / 2;
            pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, false);
            double dWidth = pManagerLight->MeasureStringWidth(m_oText.ToStdWString());

            if (fabs(dWidth - m_dWidth) > 2)
            {
                double dSpacing = (m_dWidth - dWidth) / (m_oText.length() + 1);
                dSpacing *= c_dMMToDx;

                oWriter.WriteString(L"<w:spacing w:val=\"");
                oWriter.AddInt((int)dSpacing);
                oWriter.WriteString(L"\"/>");
            }
        }

        int lSize = (int)(2 * m_oFont.Size);
        oWriter.WriteString(L"<w:sz w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/><w:szCs w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/>");

        std::wstring& strFontName = m_strPickFontName.empty() ? m_oFont.Name : m_strPickFontName;
        oWriter.WriteString(L"<w:rFonts w:ascii=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hAnsi=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:cs=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<w:color w:val=\"");
        oWriter.WriteHexInt3(ConvertColor(m_oBrush.Color1));
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"</w:rPr>");

        oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
        oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
        oWriter.WriteString(L"</w:t>");

        oWriter.WriteString(L"</w:r>");
    }

    void CContText::WriteTo(double dSpacingMM,
                            NSStringUtils::CStringBuilder& oWriter,
                            CFontManagerLight* pManagerLight)
    {
        oWriter.WriteString(L"<w:r><w:rPr>");

        double dSpaceMMSize = m_dSpaceWidthMM;
        if (m_strPickFontName.empty())
        {
            if (m_oFont.Bold)
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (m_oFont.Italic)
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");
        }
        else
        {
            if (0x01 == (0x01 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:b w:val=\"true\"/>");
            if (0x02 == (0x02 & m_lPickFontStyle))
                oWriter.WriteString(L"<w:i w:val=\"true\"/>");

            dSpaceMMSize = pManagerLight->GetSpaceWidth();
        }

        int lSize = (int)(2 * m_oFont.Size);
        oWriter.WriteString(L"<w:sz w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/><w:szCs w:val=\"");
        oWriter.AddInt(lSize);
        oWriter.WriteString(L"\"/>");

        std::wstring& strFontName = m_strPickFontName.empty() ? m_oFont.Name : m_strPickFontName;
        oWriter.WriteString(L"<w:rFonts w:ascii=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:hAnsi=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\" w:cs=\"");
        oWriter.WriteEncodeXmlString(strFontName);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<w:color w:val=\"");
        oWriter.WriteHexInt3(ConvertColor(m_oBrush.Color1));
        oWriter.WriteString(L"\"/>");

        LONG lSpacing = (LONG)((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
        oWriter.WriteString(L"<w:spacing w:val=\"");
        oWriter.AddInt((int)lSpacing);
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"</w:rPr>");

        oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
        oWriter.WriteString(L" ");
        oWriter.WriteString(L"</w:t>");

        oWriter.WriteString(L"</w:r>");
    }

    void CContText::AddSpaceToEnd()
    {
        m_bIsNeedSpaceAtTheEnd = true;
        m_dWidth += m_dSpaceWidthMM;
    }

    CTextLine::CTextLine() : m_arConts()
    {
        m_dBaselinePos	= 0;
        m_dBaselineOffset = 0;

        m_dX			= 0;
        m_dY			= 0;
        m_dWidth		= 0;
        m_dHeight		= 0;
    }
    void CTextLine::Clear()
    {
        for (std::vector<CContText*>::iterator iter = m_arConts.begin();
             iter != m_arConts.end(); iter++)
        {
            CContText* pText = *iter;
            RELEASEOBJECT(pText);
        }
        m_arConts.clear();
    }

    CTextLine::~CTextLine()
    {
        Clear();
    }

    CTextLine::CTextLine(const CTextLine& oSrc)
    {
        *this = oSrc;
    }
    CTextLine& CTextLine::operator=(const CTextLine& oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        Clear();
        for (std::vector<CContText*>::const_iterator iter = oSrc.m_arConts.begin(); iter != oSrc.m_arConts.end(); iter++)
        {
            m_arConts.push_back(new CContText(*(*iter)));
        }

        m_dBaselinePos	= oSrc.m_dBaselinePos;
        m_dBaselineOffset = oSrc.m_dBaselineOffset;

        m_dX			= oSrc.m_dX;
        m_dY			= oSrc.m_dY;
        m_dWidth		= oSrc.m_dWidth;
        m_dHeight		= oSrc.m_dHeight;

        return *this;
    }

    void CTextLine::AddCont(CContText* pCont, double dBaselineOffset)
    {
        if (0 == m_arConts.size())
            m_dBaselineOffset = dBaselineOffset;

        if ( ( pCont->m_dX > 0 ) && ( ( m_dX == 0 ) || ( pCont->m_dX < m_dX ) ) )
            m_dX = pCont->m_dX;

        if (m_dHeight < pCont->m_dHeight)
            m_dHeight = pCont->m_dHeight;

        m_arConts.push_back(pCont);
    }

    bool CTextLine::IsBigger(const CTextLine* oSrc)
    {
        return (m_dBaselinePos > oSrc->m_dBaselinePos) ? true : false;
    }

    bool CTextLine::IsBiggerOrEqual(const CTextLine* oSrc)
    {
        return (m_dBaselinePos >= oSrc->m_dBaselinePos) ? true : false;
    }

    void CTextLine::SortConts()
    {
        // сортировка непрерывных слов по m_dX
        SortElements(m_arConts);
    }

    void CTextLine::Merge(CTextLine* pTextLine)
    {
        size_t nCount = pTextLine->m_arConts.size();
        if (0 != nCount)
        {
            if (pTextLine->m_dX < m_dX)
            {
                m_dX = pTextLine->m_dX;
            }
            if (pTextLine->m_dBaselinePos < m_dBaselinePos)
            {
                m_dHeight = (m_dBaselinePos - pTextLine->m_dBaselinePos + pTextLine->m_dHeight);
            }
            else
            {
                m_dHeight = (pTextLine->m_dBaselinePos - m_dBaselinePos + m_dHeight);
            }

            double dSubPosition = m_dBaselinePos - pTextLine->m_dBaselinePos;

            for (size_t i = 0; i < nCount; ++i)
            {
                pTextLine->m_arConts[i]->m_dPosition = dSubPosition;
                m_arConts.push_back(pTextLine->m_arConts[i]);
            }
        }
    }

    void CTextLine::Analyze()
    {
        size_t nCountConts = m_arConts.size();

        if (0 == nCountConts)
            return;

        CContText* pFirst = m_arConts[0];

        for (size_t i = 1; i < nCountConts; ++i)
        {
            CContText* pCurrent = m_arConts[i];

            double dFirstRight = pFirst->m_dX + pFirst->m_dWidth;
            double dCurrLeft = pCurrent->m_dX;
            double dDelta = dFirstRight - dCurrLeft;

            if (pFirst->m_strPickFontName != pCurrent->m_strPickFontName ||
                pFirst->m_oFont.Bold != pCurrent->m_oFont.Bold ||
                pFirst->m_oFont.Italic != pCurrent->m_oFont.Italic ||
                fabs(dDelta) > c_dTHE_STRING_X_PRECISION_MM)
            {
                // вообще надо бы все объединить. но пока этот метод на соединение "первых"
                //break;
                if (i < nCountConts - 1)
                {
                    //переходим на
                    pFirst = m_arConts[i];
                }
                continue;
            }

            // продолжаем слово
            pFirst->m_oText += pCurrent->m_oText;
            //pFirst->m_dWidth = (dLeft + pCurrent->m_dWidth - pFirst->m_dX);
            pFirst->m_dWidth += pCurrent->m_dWidth + fabs(dDelta);

            pFirst->m_dWidthWithoutSpaces < 0.0001 ?
                        pFirst->m_dLeftWithoutSpaces = pCurrent->m_dLeftWithoutSpaces :
                    pFirst->m_dWidthWithoutSpaces = pCurrent->m_dLeftWithoutSpaces +
                    pCurrent->m_dWidthWithoutSpaces - pFirst->m_dLeftWithoutSpaces;

            m_arConts.erase(m_arConts.begin() + i);
            --i;
            --nCountConts;
        }
    }

    void CTextLine::CalculateWidth()
    {
        m_dWidth = 1; //todo исправить этот хак - по идее должно == 0.

        m_dWidth += m_arConts[0]->m_dWidthWithoutSpaces;

        for (size_t i = 1; i < m_arConts.size(); ++i)
        {
            m_dWidth += m_arConts[i]->m_dLeftWithoutSpaces - (m_arConts[i-1]->m_dLeftWithoutSpaces + m_arConts[i-1]->m_dWidthWithoutSpaces);;
            m_dWidth += m_arConts[i]->m_dWidthWithoutSpaces;
        }
    }

    void CTextLine::AddSpaceToEnd()
    {
        CContText* pCurrent = m_arConts.back();

        if (pCurrent->m_oText[pCurrent->m_oText.length()-1] != uint32_t(' '))
        {
            pCurrent->AddSpaceToEnd();
            this->m_dWidth += pCurrent->m_dSpaceWidthMM;
        }
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

    void CTextLine::ToXml(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight)
    {
        size_t nCountConts = m_arConts.size();

        if (0 == nCountConts)
            return;

        CContText* pPrev = m_arConts[0];
        double dDelta = 0;

        for (size_t i = 1; i < nCountConts; ++i)
        {
            CContText* pCurrent = m_arConts[i];

            if (0 == pCurrent->m_dWidthWithoutSpaces)
                continue;

            dDelta = pCurrent->m_dLeftWithoutSpaces - (pPrev->m_dLeftWithoutSpaces + pPrev->m_dWidthWithoutSpaces);

            if (dDelta < c_dTHE_STRING_X_PRECISION_MM)
            {
                // просто текст на тексте или сменились настройки (font/brush)
                pPrev->Write(oWriter, pManagerLight);
                pPrev  = pCurrent;
            }
            //else if (dDelta < 2 * pPrev->m_dSpaceWidthMM)
            //{
            //	// сменились настройки, но пробел все-таки вставить нужно
            //	pPrev->Write(oWriter, pManagerLight, true);
            //	pPrev = pCurrent;
            //}
            else
            {
                // расстояние слишком большое. нужно сделать большой пробел
                pPrev->Write(oWriter, pManagerLight);
                pPrev->WriteTo(dDelta, oWriter, pManagerLight);
                pPrev = pCurrent;
            }
        }

        pPrev->Write(oWriter, pManagerLight);
    }

    CParagraph::CParagraph(const TextAssociationType& eType) : m_arLines()
    {
        m_eType = etParagraph;

        m_eTextConversionType	 = UnknownConversionType;
        m_bIsNeedFirstLineIndent = false;
        m_bIsAroundTextWrapping  = true; //по умолчанию в word
        m_eTextAlignmentType     = TextAlignmentType_Unknown;

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
    CParagraph::CParagraph(const CParagraph& oSrc)
    {
        *this = oSrc;
    }
    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Clear()
    {
        size_t nCount = m_arLines.size();
        for (size_t i = 0; i < nCount; ++i)
        {
            CTextLine* pText = m_arLines[i];
            RELEASEOBJECT(pText);
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

        m_eType = etParagraph;

        m_eTextConversionType	 = oSrc.m_eTextConversionType;
        m_bIsNeedFirstLineIndent = oSrc.m_bIsNeedFirstLineIndent;
        m_bIsAroundTextWrapping  = oSrc.m_bIsAroundTextWrapping;
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
        case TextToFrame:
        {
            oWriter.WriteString(L"<w:framePr");

            if (m_eTextAssociationType == TextAssociationTypePlainParagraph)
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
            oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
            oWriter.WriteString(L"\"");

            oWriter.WriteString(L" w:y=\"");
            oWriter.AddInt((int)(m_dTop * c_dMMToDx));
            oWriter.WriteString(L"\"");

            oWriter.WriteString(L"/>"); //конец w:framePr
            break;
        }
        case TextToShape:
        case TextToParagraph:
        {
            oWriter.WriteString(L"<w:spacing");
            if (m_eTextConversionType == TextToParagraph)
            {
                oWriter.WriteString(L" w:before=\"");
                oWriter.AddInt((int)(m_dSpaceBefore * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }

            if (m_eTextConversionType == TextToShape)
            {
                oWriter.WriteString(L" w:after=\"");
                oWriter.AddInt((int)(m_dSpaceAfter * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            //if (m_eTextConversionType == TextToParagraph)
            {
                oWriter.WriteString(L" w:line=\"");
                oWriter.AddInt((int)(m_dHeight * c_dMMToDx));
                oWriter.WriteString(L"\" w:lineRule=\"exact\""); // exact - точный размер строки
            }
            oWriter.WriteString(L"/>"); //конец w:spacing

            oWriter.WriteString(L"<w:ind");
            if (m_dLeft > 0)
            {
                oWriter.WriteString(L" w:left=\"");
                oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            if (m_eTextAssociationType == TextAssociationTypePlainParagraph && m_dRight > 0)
            {
                oWriter.WriteString(L" w:right=\"");
                oWriter.AddInt((int)(m_dRight * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            if (m_bIsNeedFirstLineIndent)
            {
                oWriter.WriteString(L" w:firstLine=\"");
                oWriter.AddInt((int)(m_dFirstLine * c_dMMToDx));
                oWriter.WriteString(L"\"");
            }
            oWriter.WriteString(L"/>"); //конец w:ind


            if (m_eTextAssociationType == TextAssociationTypePlainParagraph)
            {
                switch (m_eTextAlignmentType)
                {
                case TextAlignmentType_ByCenter:
                    oWriter.WriteString(L"<w:jc w:val=\"center\"/>");
                    break;
                case TextAlignmentType_ByRightEdge:
                    oWriter.WriteString(L"<w:jc w:val=\"end\"/>");
                    break;
                case TextAlignmentType_ByWidth:
                    oWriter.WriteString(L"<w:jc w:val=\"both\"/>");
                    break;
                case TextAlignmentType_ByLeftEdge:
                    oWriter.WriteString(L"<w:jc w:val=\"begin\"/>");
                    break;
                default: //по умолчанию выравнивание по левому краю - можно ничего не добавлять
                    break;
                }
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
            if (m_eTextAssociationType != TextAssociationTypePlainParagraph)
            {
                pTextLine->SortConts();
            }
            pTextLine->ToXml(oWriter, m_pManagerLight);
        }

        oWriter.WriteString(L"</w:p>");
    }
}
