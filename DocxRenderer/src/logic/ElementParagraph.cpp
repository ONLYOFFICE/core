#include "ElementParagraph.h"
#include "src\resources\ColorTable.h"
#include "src\resources\SingletonTemplate.h"

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

        m_dSpaceWidthMM	= 0;
        m_dBaselineOffset = 0;

        m_bIsNeedSpaceAtTheEnd = false;
        m_bIsHighlightPresent = false;
        m_lHighlightColor = 0; //черный

        m_eUnderlineType = utUnknown;
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

        m_strPickFontName	= oSrc.m_strPickFontName;
        m_lPickFontStyle	= oSrc.m_lPickFontStyle;

        m_oText	 = oSrc.m_oText;

        m_dX		= oSrc.m_dX;
        m_dY		= oSrc.m_dY;
        m_dWidth	= oSrc.m_dWidth;
        m_dHeight	= oSrc.m_dHeight;
        m_dLastX    = oSrc.m_dLastX;

        m_dWidthWithoutSpaces	= oSrc.m_dWidthWithoutSpaces;
        m_dLeftWithoutSpaces	= oSrc.m_dLeftWithoutSpaces;

        m_dSpaceWidthMM = oSrc.m_dSpaceWidthMM;
        m_dBaselineOffset = oSrc.m_dBaselineOffset;

        m_bIsNeedSpaceAtTheEnd = oSrc.m_bIsNeedSpaceAtTheEnd;
        m_bIsHighlightPresent = oSrc.m_bIsHighlightPresent;
        m_lHighlightColor = oSrc.m_lHighlightColor;

        m_eUnderlineType = oSrc.m_eUnderlineType;

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
        oWriter.WriteString(L"<w:r>");
        oWriter.WriteString(L"<w:rPr>");

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
            double ___dSize = (double)(static_cast<LONG>(m_oFont.Size * 2)) / 2;
            pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, false);
            double dWidth = pManagerLight->MeasureStringWidth(m_oText.ToStdWString());

            if (fabs(dWidth - m_dWidth) > 2)
            {
                double dSpacing = (m_dWidth - dWidth) / (m_oText.length() + 1);
                dSpacing *= c_dMMToDx;

                oWriter.WriteString(L"<w:spacing w:val=\"");
                oWriter.AddInt(static_cast<int>(dSpacing));
                oWriter.WriteString(L"\"/>");
            }
        }

        int lSize = static_cast<int>(2 * m_oFont.Size);
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

        if (m_oBrush.Color1 != 0)
        {
            oWriter.WriteString(L"<w:color w:val=\"");
            oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
            oWriter.WriteString(L"\"/>");
        }

        if (m_oFont.Strikeout == TRUE)
        {
            oWriter.WriteString(L"<w:strike/>");
        }

        if (m_oFont.Underline == TRUE)
        {
            switch (m_eUnderlineType)
            {
            case utThinLine:
                oWriter.WriteString(L"<w:u w:val=\"single\"/>");
                break;
            case utThickLine:
                oWriter.WriteString(L"<w:u w:val=\"thick\"/>");
                break;
            case utDoubleThinLine:
                oWriter.WriteString(L"<w:u w:val=\"double\"/>");
                break;
            default:
                break;
            }
        }

        if (m_bIsHighlightPresent)
        {
            oWriter.WriteString(L"<w:highlight w:val=\"");
            ColorTable& colorTable = SingletonInstance<ColorTable>();
            oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
            oWriter.WriteString(L"\"/>");
        }

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
        oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_oBrush.Color1));
        oWriter.WriteString(L"\"/>");

        LONG lSpacing = static_cast<LONG>((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
        oWriter.WriteString(L"<w:spacing w:val=\"");
        oWriter.AddInt(static_cast<int>(lSpacing));
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

        m_eAlignmentType = atatUnknown;
    }
    void CTextLine::Clear()
    {
        for (size_t i = 0; i < m_arConts.size(); i++)
        {
            RELEASEOBJECT(m_arConts[i]);
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

        for (size_t i = 0; i < oSrc.m_arConts.size(); i++)
        {
            m_arConts.push_back(new CContText(*oSrc.m_arConts[i]));
        }

        m_dBaselinePos	= oSrc.m_dBaselinePos;
        m_dBaselineOffset = oSrc.m_dBaselineOffset;

        m_dX			= oSrc.m_dX;
        m_dY			= oSrc.m_dY;
        m_dWidth		= oSrc.m_dWidth;
        m_dHeight		= oSrc.m_dHeight;

        m_eAlignmentType = oSrc.m_eAlignmentType;

        return *this;
    }

    void CTextLine::AddCont(CContText* pCont)
    {
        //if (0 == m_arConts.size())
            m_dBaselineOffset = fabs(m_dBaselineOffset) > fabs(pCont->m_dBaselineOffset) ? m_dBaselineOffset : pCont->m_dBaselineOffset;

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

            for (size_t i = 0; i < nCount; ++i)
            {
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
                !pFirst->m_oFont.IsEqual(&pCurrent->m_oFont) ||
                pFirst->m_eUnderlineType != pCurrent->m_eUnderlineType ||
                pFirst->m_bIsHighlightPresent != pCurrent->m_bIsHighlightPresent ||
                pFirst->m_lHighlightColor != pCurrent->m_lHighlightColor ||
                fabs(dDelta) > c_dTHE_STRING_X_PRECISION_MM)
            {
                if (i < nCountConts - 1)
                {
                    //переходим на
                    pFirst = m_arConts[i];
                }
                continue;
            }

            // продолжаем слово
            pFirst->m_oText += pCurrent->m_oText;
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
        m_dWidth = m_arConts[0]->m_dWidthWithoutSpaces;

        for (size_t i = 1; i < m_arConts.size(); ++i)
        {
            m_dWidth += m_arConts[i]->m_dLeftWithoutSpaces - (m_arConts[i-1]->m_dLeftWithoutSpaces + m_arConts[i-1]->m_dWidthWithoutSpaces);;
            m_dWidth += m_arConts[i]->m_dWidthWithoutSpaces;
        }
    }

    void CTextLine::AddSpaceToEnd()
    {
        if (m_arConts.empty())
        {
            return;
        }

        CContText* pCurrent = m_arConts.back();

        if (pCurrent->m_oText[pCurrent->m_oText.length()-1] != uint32_t(' '))
        {
            pCurrent->AddSpaceToEnd();
            m_dWidth += pCurrent->m_dSpaceWidthMM;
        }
    }

    void CTextLine::DetermineAssumedTextAlignmentType(double dWidthOfPage)
    {
        //рассматриваем строки, которые короче трети ширины страницы
        double maxTextLineWidth = dWidthOfPage/3; //нужна какая-нибудь отправная точка...
        double delta = 2 * c_dCENTER_POSITION_ERROR_MM; //координата m_dWidth/2 +- c_dCENTER_POSITION_ERROR_MM

        if (fabs(dWidthOfPage/2 - m_dX - m_dWidth/2) <= delta && //если середины линий по x одинаковы
                 m_dWidth < maxTextLineWidth )
        {
            m_eAlignmentType = atatByCenter;
        }
        else if ((m_dX + m_dWidth/2) > (dWidthOfPage/2 + c_dCENTER_POSITION_ERROR_MM) && //середина строки правее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByRightEdge;
        }
        else if ((m_dX + m_dWidth/2) < (dWidthOfPage/2 - c_dCENTER_POSITION_ERROR_MM) && //середина строки левее центра страницы
                 m_dWidth < maxTextLineWidth)
        {
            m_eAlignmentType = atatByLeftEdge;
        }
        else if (fabs(dWidthOfPage/2 - m_dX - m_dWidth/2) <= delta &&
                 m_dWidth > maxTextLineWidth + maxTextLineWidth/2 )
        {
            m_eAlignmentType = atatByWidth;
        }
        else
        {
            m_eAlignmentType = atatUnknown;
        }
    }

    bool CTextLine::AreAlignmentsAppropriate(const CTextLine* oSrc)
    {
        if ((m_eAlignmentType == oSrc->m_eAlignmentType && m_eAlignmentType!= atatByLeftEdge) ||
            (m_eAlignmentType == atatByWidth && oSrc->m_eAlignmentType == atatByLeftEdge) ||
            (m_eAlignmentType == atatByWidth && oSrc->m_eAlignmentType == atatUnknown) ||
            (m_eAlignmentType == atatUnknown && oSrc->m_eAlignmentType == atatByWidth))
        {
            return true;
        }
        return false;
    }

    bool CTextLine::AreLinesCrossing(const CTextLine* oSrc)
    {
        double dCurrentTop = m_dBaselinePos - m_dHeight - m_dBaselineOffset;
        double dNextTop = oSrc->m_dBaselinePos - oSrc->m_dHeight - oSrc->m_dBaselineOffset;

        if ((oSrc->m_dBaselinePos < m_dBaselinePos && dCurrentTop < oSrc->m_dBaselinePos) ||
            (oSrc->m_dBaselinePos > m_dBaselinePos && dNextTop < m_dBaselinePos))
        {
            return true;
        }
        return false;
    }

    double CTextLine::CalculateBeforeSpacing(const double* pPreviousStringOffset)
    {
        return m_dBaselinePos - *pPreviousStringOffset - m_dHeight - m_dBaselineOffset;
    }

    double CTextLine::CalculateStringOffset()
    {
        return m_dBaselinePos - m_dBaselineOffset;
    }

    double CTextLine::CalculateRightBorder(const double* pPageWidth)
    {
        return *pPageWidth - (m_dX + m_dWidth + m_arConts.back()->m_dSpaceWidthMM); //добавляем ширину пробела;
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

    CParagraph::CParagraph(const TextAssociationType& eType):
        CBaseItem(etParagraph), m_arLines()
    {
        m_eTextConversionType	 = tctUnknown;
        m_bIsNeedFirstLineIndent = false;
        m_bIsAroundTextWrapping  = true; //по умолчанию в word
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
            if (m_eTextAssociationType == tatPlainParagraph && m_dRight > 0)
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
