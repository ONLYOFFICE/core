#pragma once
#include "Common.h"
#include "FontManager.h"

namespace NSDocxRenderer
{
    const double c_dMMToDx	 = 72 * 20 / 25.4;

    std::map <long, std::wstring> colorHighlight = {
        {8388736, L"black"}, {16711680, L"blue"}, {65280, L"green"},
        {9109504, L"darkBlue"}, {139, L"darkRed"}, {32896, L"darkYellow"},
        {13882323, L"lightGray"}, {11119017, L"darkGray"}, {25600, L"darkGreen"},
        {16711935, L"magenta"}, {255, L"red"}, {16776960, L"cyan"},
        {9145088, L"darkCyan"}, {8388736, L"darkMagenta"}, {65535, L"yellow"}
    };

    // у класса T должен быть метод IsBigger, IsBiggerOrEqual
    template<typename T>
    void SortElements(std::vector<T*>& oArray)
    {
        int nSize = (int)oArray.size();

        // handle 0, 1 and 2 elements
        if (nSize <= 1)
            return;
        if (nSize == 2)
        {
            if (oArray[0]->IsBigger(oArray[1]))
            {
                T* pTemp	= oArray[0];
                oArray[0]	= oArray[1];
                oArray[1]	= pTemp;
            }
            return;
        }

        T* tTemp;

        // arrange elements as tree with greater elements appearing first
        int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
        int nLast = nSize - 1;
        int nHalf = nSize >> 1;
        do
        {
            // save element at start of chain
            tTemp = oArray[nIndex];

            nCurr = nIndex;
            while (nCurr < nHalf)
            {
                nNext = (nCurr << 1) + 1;
                if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                    nNext++;
                if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                    break;

                // promote element in chain
                oArray[nCurr] = oArray[nNext];
                nCurr = nNext;
            }

            // restore element at end of chain
            oArray[nCurr] = tTemp;
        }
        while (nIndex--);

        // sequentially reduce tree size by removing maximum element and rebalancing
        nIndex = nSize;
        while (--nIndex)
        {
            // save element at start of chain
            tTemp = oArray[nIndex];
            oArray[nIndex] = oArray[0];

            nCurr = 0;
            nLast = nIndex - 1;
            nHalf = nIndex >> 1;
            while (nCurr < nHalf)
            {
                nNext = (nCurr << 1) + 1;
                if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                    nNext++;
                if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                    break;

                // promote element in chain
                oArray[nCurr] = oArray[nNext];
                nCurr = nNext;
            }

            // restore element at end of chain
            oArray[nCurr] = tTemp;
        }
    }

    inline void DeleteSpaces(NSStringUtils::CStringUTF32& oText)
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

    class CContText
    {
    public:
        NSStructures::CFont		m_oFont;
        NSStructures::CBrush	m_oBrush;

        std::wstring m_strPickFontName;
        LONG	m_lPickFontStyle;

        NSStringUtils::CStringUTF32 m_oText;
        NSStringUtils::CStringUTF32 m_oGidText;

        double m_dX;
        double m_dY;
        double m_dWidth;
        double m_dHeight;

        double m_dWidthWithoutSpaces;
        double m_dLeftWithoutSpaces;

        double m_dPosition;
        double m_dSpaceWidthMM;

        std::vector <double> m_arWidthText;

    public:
        CContText()
        {
            m_strPickFontName	= L"";
            m_lPickFontStyle	= 0;

            m_dX			= 0;
            m_dY			= 0;
            m_dWidth		= 0;
            m_dHeight		= 0;

            m_dWidthWithoutSpaces	= 0;
            m_dLeftWithoutSpaces	= 0;

            m_dPosition		= 0;
            m_dSpaceWidthMM	= 0;

        }
        ~CContText()
        {
        }

        inline void Clear()
        {
        }

        CContText(const CContText& oSrc)
        {
            *this = oSrc;
        }
        CContText& operator=(const CContText& oSrc)
        {
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

            m_dWidthWithoutSpaces	= oSrc.m_dWidthWithoutSpaces;
            m_dLeftWithoutSpaces	= oSrc.m_dLeftWithoutSpaces;

            m_dPosition = oSrc.m_dPosition;
            m_dSpaceWidthMM = oSrc.m_dSpaceWidthMM;

            return *this;
        }

        inline bool IsBigger(const CContText* oSrc)
        {
            return (m_dX > oSrc->m_dX) ? true : false;
        }
        inline bool IsBiggerOrEqual(const CContText* oSrc)
        {
            return (m_dX >= oSrc->m_dX) ? true : false;
        }

        void ChangeText(size_t nFrom, size_t nBefore )
        {
            std::vector<uint32_t> arNewVec;
            for (size_t i = nFrom; i <= nBefore; ++i)
            {
                uint32_t text = this->m_oText[i];
                arNewVec.push_back(text);
            }
            m_oText = arNewVec;

        }

        void MarkFontProperties(int i)
        {
            switch (i) {
            case 0:
                m_oFont.Strikeout = 1;
                break;
            case 1:
                m_oFont.Underline = 1;
                break;
            default:
                m_oFont.BackgroundColor = i;
                break;
            }
        }

        inline void Write(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight, bool bIsAddSpace = false)
        {
            oWriter.WriteString(L"<w:r><w:rPr>");

            if (m_dWidth != m_dWidthWithoutSpaces)
            {
                DeleteSpaces(m_oText);
                m_dWidth = m_dWidthWithoutSpaces;
            }

            if (m_strPickFontName.empty())
            {
                if (m_oFont.Bold)
                    oWriter.WriteString(L"<w:b w:val=\"true\"/>");
                if (m_oFont.Italic)
                    oWriter.WriteString(L"<w:i w:val=\"true\"/>");

                if (m_oFont.Underline)
                    oWriter.WriteString(L"<w:u w:val=\"single\"/>");
                if (m_oFont.Strikeout)
                    oWriter.WriteString(L"<w:strike/>");
                if (m_oFont.BackgroundColor > 0)
                {
                    oWriter.WriteString(L"<w:highlight w:val=\"");
                    oWriter.WriteString(colorHighlight[m_oFont.BackgroundColor]);
                    oWriter.WriteString(L"\"/>");
                }
                if (bIsAddSpace)
                {
                    m_dWidth  += m_dSpaceWidthMM;
                    m_oText += L" ";
                }
            }
            else
            {
                if (0x01 == (0x01 & m_lPickFontStyle))
                    oWriter.WriteString(L"<w:b w:val=\"true\"/>");
                if (0x02 == (0x02 & m_lPickFontStyle))
                    oWriter.WriteString(L"<w:i w:val=\"true\"/>");
                if (m_oFont.Underline)
                    oWriter.WriteString(L"<w:u w:val=\"single\"/>");
                if (m_oFont.Strikeout)
                    oWriter.WriteString(L"<w:strike/>");
                if (m_oFont.BackgroundColor > 0)
                {
                    oWriter.WriteString(L"<w:highlight w:val=\"");
                    oWriter.WriteString(colorHighlight[m_oFont.BackgroundColor]);
                    oWriter.WriteString(L"\"/>");
                }
                if (bIsAddSpace)
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

        void WriteTo(double dSpacingMM, NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight)
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
    };

    class CTextLine
    {
    public:
        std::vector<CContText*> m_arConts;

        double m_dBaselinePos;
        double m_dBaselineOffset;

        double m_dX;
        double m_dY;
        double m_dWidth;
        double m_dHeight;

    public:
        CTextLine() : m_arConts()
        {
            m_dBaselinePos	= 0;

            m_dX			= 0;
            m_dY			= 0;
            m_dWidth		= 0;
            m_dHeight		= 0;
        }
        void Clear()
        {
            for (std::vector<CContText*>::iterator iter = m_arConts.begin(); iter != m_arConts.end(); iter++)
            {
                CContText* pText = *iter;
                RELEASEOBJECT(pText);
            }
            m_arConts.clear();
        }

        ~CTextLine()
        {
            Clear();
        }

        CTextLine(const CTextLine& oSrc)
        {
            *this = oSrc;
        }
        CTextLine& operator=(const CTextLine& oSrc)
        {
            Clear();
            for (std::vector<CContText*>::const_iterator iter = oSrc.m_arConts.begin(); iter != oSrc.m_arConts.end(); iter++)
            {
                m_arConts.push_back(new CContText(*(*iter)));
            }

            m_dBaselinePos	= oSrc.m_dBaselinePos;
            m_dX			= oSrc.m_dX;
            m_dY			= oSrc.m_dY;
            m_dWidth		= oSrc.m_dWidth;
            m_dHeight		= oSrc.m_dHeight;
        }

        inline void AddCont(CContText* pCont, double dBaselineOffset)
        {
            if (0 == m_arConts.size())
                m_dBaselineOffset = dBaselineOffset;

            if ( ( pCont->m_dX > 0 ) && ( ( m_dX == 0 ) || ( pCont->m_dX < m_dX ) ) )
                m_dX = pCont->m_dX;

            if (m_dHeight < pCont->m_dHeight)
                m_dHeight = pCont->m_dHeight;

            m_arConts.push_back(pCont);
        }

        inline bool IsBigger(const CTextLine* oSrc)
        {
            return (m_dBaselinePos > oSrc->m_dBaselinePos) ? true : false;
        }
        inline bool IsBiggerOrEqual(const CTextLine* oSrc)
        {
            return (m_dBaselinePos >= oSrc->m_dBaselinePos) ? true : false;
        }

        inline void SortConts()
        {
            // сортировка непрерывных слов по m_dX
            SortElements(m_arConts);
        }

        void Merge(CTextLine* pTextLine)
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

        std::vector<CContText*> BreakCont(size_t idx, double dLeftLineCoord, double dRightLineCoord, int bMode)
        {
            std::vector<CContText*> arReturnConts;

            std::vector <bool> arTagget;
            size_t nCountChars = m_arConts[idx]->m_arWidthText.size();
            for (size_t i = 0; i < nCountChars; ++i)
            {
                double dCoordChar = m_arConts[idx]->m_dX + m_arConts[idx]->m_arWidthText[i];
                if (dCoordChar >= dLeftLineCoord && dCoordChar <= dRightLineCoord)
                {
                    arTagget.push_back(true);
                }
                else
                {
                    arTagget.push_back(false);
                }
            }

            bool bPrevTag = arTagget[0];
            size_t nBlockStart = 0;
            for (size_t j = 1; j < arTagget.size(); ++j)
            {
                if (bPrevTag != arTagget[j])
                {
                    CContText* pTempCont = m_arConts[idx];
                    pTempCont->ChangeText(nBlockStart, j-1);

                    if (arTagget[j-1] == true)
                        pTempCont->MarkFontProperties(bMode);

                    arReturnConts.push_back(pTempCont);
                    nBlockStart = j;
                }
                bPrevTag = arTagget[j];
            }
            if (!nBlockStart)
                arReturnConts.push_back(m_arConts[idx]);

            return arReturnConts;
        }

        void SearchInclusions(double dLeftLineCoord, double dRightLineCoord, int bMode)
        {
            std::vector<CContText*> arTempConts;

            size_t nCountConts = m_arConts.size();
            for (size_t i = 0; i < nCountConts; ++i)
            {
                double dLeftCont = m_arConts[i]->m_dX;
                double dRightCont = dLeftCont + m_arConts[i]->m_dWidth;

                if (dRightLineCoord < dRightCont)
                {
                    if (dRightLineCoord > dLeftCont)
                    {
                        //разбиваем
                        std::vector<CContText*> arBreakCont = BreakCont(i, dLeftLineCoord, dRightLineCoord, bMode);
                        for (size_t j = 0; j < arBreakCont.size(); ++j)
                        {
                            arTempConts.push_back(arBreakCont[j]);
                        }
                    }
                    else
                    {
                        //ничего не делаем
                        arTempConts.push_back(m_arConts[i]);
                    }
                }
                else
                {
                    if (dLeftLineCoord > dLeftCont)
                    {
                        //разбиваем
                        std::vector<CContText*> arBreakCont = BreakCont(i, dLeftLineCoord, dRightLineCoord, bMode);
                        for (size_t j = 0; j < arBreakCont.size(); ++j)
                        {
                            arTempConts.push_back(arBreakCont[j]);
                        }

                    }
                    else
                    {
                        //помечаем
                        m_arConts[i]->MarkFontProperties(bMode);
                        arTempConts.push_back(m_arConts[i]);
                    }
                }
            }
            this->m_arConts = arTempConts;
        }

        void ToXml(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight)
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

                if (dDelta < 0.5)
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
    };

    class CParagraph : public CBaseItem
    {
    public:
        // text frame properties
        bool		m_bIsTextFrameProperties;

        // geometry paragraph
        double		m_dLeft;
        double		m_dTop;
        double		m_dWidth;
        double		m_dHeight;

        CFontManagerLight* m_pManagerLight;

        double		m_dSpaceBefore;
        TextAssociationType m_eTextAssociationType;

        std::vector<CTextLine*> m_arLines;

    public:
        CParagraph(const TextAssociationType& eType) : m_arLines()
        {
            m_eType = etParagraph;

            m_bIsTextFrameProperties	= false;

            m_dLeft		= 0.0;
            m_dTop		= 0.0;
            m_dWidth	= 0.0;
            m_dHeight	= 0.0;

            m_dSpaceBefore = 0.0;

            m_pManagerLight = NULL;
            m_eTextAssociationType = eType;
        }
        CParagraph(const CParagraph& oSrc)
        {
            *this = oSrc;
        }
        ~CParagraph()
        {
            Clear();
        }

        void Clear()
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

        CParagraph& operator=(const CParagraph& oSrc)
        {
            m_eType = etParagraph;

            m_bIsTextFrameProperties	= oSrc.m_bIsTextFrameProperties;

            m_dLeft		= oSrc.m_dLeft;
            m_dTop		= oSrc.m_dTop;
            m_dWidth	= oSrc.m_dWidth;
            m_dHeight	= oSrc.m_dHeight;

            m_dSpaceBefore	= oSrc.m_dSpaceBefore;

            m_eTextAssociationType		= oSrc.m_eTextAssociationType;

            Clear();
            size_t nCount = oSrc.m_arLines.size();
            for (size_t i = 0; i < nCount; ++i)
            {
                m_arLines.push_back(new CTextLine(*oSrc.m_arLines[i]));
            }

            m_pManagerLight = oSrc.m_pManagerLight;

            return *this;
        }

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter)
        {
            oWriter.WriteString(L"<w:p>");

            switch (m_eTextAssociationType)
            {
            case TextAssociationTypeDefault:
            case TextAssociationTypeLine:
                {
                    oWriter.WriteString(L"<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\" w:y=\"");
                    oWriter.AddInt((int)(m_dTop * c_dMMToDx));
                    oWriter.WriteString(L"\"/></w:pPr>");
                    break;
                }
            case TextAssociationTypeBlock:
                {
                    oWriter.WriteString(L"<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\" w:y=\"");
                    oWriter.AddInt((int)(m_dTop * c_dMMToDx));
                    oWriter.WriteString(L"\"/></w:pPr>");
                    break;
                }
            case TextAssociationTypeNoFrames:
                {
                    oWriter.WriteString(L"<w:pPr><w:spacing w:before=\"");
                    oWriter.AddInt((int)(m_dSpaceBefore * c_dMMToDx));
                    oWriter.WriteString(L"\" w:line=\"");
                    oWriter.AddInt((int)(m_dHeight * c_dMMToDx));
                    oWriter.WriteString(L"\" w:lineRule=\"exact\"/><w:ind w:left=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\"/></w:pPr>");
                    break;
                }
            default:
                break;
            }

            size_t nCount = m_arLines.size();
            for (size_t i = 0; i < nCount; ++i)
            {
                CTextLine* pTextLine = m_arLines[i];
                pTextLine->SortConts();
                pTextLine->ToXml(oWriter, m_pManagerLight);
            }

            oWriter.WriteString(L"</w:p>");
        }
    };
}
