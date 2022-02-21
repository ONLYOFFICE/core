#pragma once
#include "Common.h"
#include "FontManager.h"
#include "ElementShape.h"

namespace NSDocxRenderer
{
    const double c_dMMToDx	 = 72 * 20 / 25.4;

    enum class ModeFontOptions {STRIKEOUT, UNDERLINE, HIGHLIGHT, SHADING};

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

        double m_dCalculateWidth;
        std::vector <double> m_arWidthText;

        long m_lShadingColor;

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

            m_dCalculateWidth = 0;
            m_lShadingColor = -1;
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

            m_dCalculateWidth = oSrc.m_dCalculateWidth;

            m_arWidthText = oSrc.m_arWidthText;

            m_lShadingColor = oSrc.m_lShadingColor;

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

        void CheckingBoldFont(CShape* pShape)
        {
            if (!(m_oFont.Bold || 0x01 == (0x01 & m_lPickFontStyle)) )
                return;

            switch (pShape->m_TypeLine)
            {
                case NSStructures::LineProperties::THICK:
                    pShape->m_TypeLine = NSStructures::LineProperties::SINGLE;
                    break;
                case NSStructures::LineProperties::DOTTEDHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::DOTTED;
                    break;
                case NSStructures::LineProperties::DASHEDHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASH;
                    break;
                case NSStructures::LineProperties::DASHLONGHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::DASHLONG;
                    break;
                case NSStructures::LineProperties::DASHDOTHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::DOTDASH;
                    break;
                case NSStructures::LineProperties::DASHDOTDOTHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::DOTDOTDASH;
                    break;
                case NSStructures::LineProperties::WAVYHEAVY:
                    pShape->m_TypeLine = NSStructures::LineProperties::WAVE;
                    break;
                default:
                    break;
            }
        }

        std::vector<CContText*> BreakCont(CShape* pShape, ModeFontOptions oMode)
        {
            double dLeftLineCoord = pShape->m_dLeft;
            double dRightLineCoord = pShape->m_dLeft+pShape->m_dWidth;

            std::vector<CContText*> arReturnConts;
            size_t nCountChars = m_arWidthText.size();
            bool bPrevMark{};
            size_t nLastIdx{};

            for (size_t i = 0; i < nCountChars; ++i)
            {
                double dCoordChar{};
                if (0 == i)
                    dCoordChar = m_dX + m_arWidthText[i]/2;
                else
                    dCoordChar = m_dX + m_arWidthText[i-1] + (m_arWidthText[i] - m_arWidthText[i-1])/2;


                if (dCoordChar >= dLeftLineCoord && dCoordChar <= dRightLineCoord)
                {
                    if (0 == i)
                        bPrevMark = true;

                    if (bPrevMark != true)
                    {
                        arReturnConts.push_back(AddNewCont(nLastIdx, i, oMode, bPrevMark, pShape)) ;
                        nLastIdx = i;
                    }

                    bPrevMark = true;
                }
                else
                {
                    if (0 == i)
                        bPrevMark = false;

                    if (bPrevMark != false)
                    {
                        arReturnConts.push_back(AddNewCont(nLastIdx, i, oMode, bPrevMark, pShape)) ;
                        nLastIdx = i;
                    }

                    bPrevMark = false;
                }
            }

            arReturnConts.push_back(AddNewCont(nLastIdx, nCountChars, oMode, bPrevMark, pShape));
            return arReturnConts;
        }

        CContText* AddNewCont(const size_t &nLastIdx, const size_t &nCurrentIdx, ModeFontOptions oMode, const bool &bMark, CShape* pShape)
        {
            CContText* pCont = new CContText(*this);
            if (true == bMark)
                pCont->MarkFontProperties(pShape, oMode);

            if (0 == nLastIdx)
            {
                pCont->m_dX = m_dX ;
                pCont->m_dWidth = m_arWidthText[nCurrentIdx-1];
            }
            else
            {
                pCont->m_dX = m_dX + m_arWidthText[nLastIdx-1];
                pCont->m_dWidth = m_arWidthText[nCurrentIdx-1] - m_arWidthText[nLastIdx-1];
            }
            pCont->m_dLeftWithoutSpaces = pCont->m_dX;
            pCont->m_dWidthWithoutSpaces = pCont->m_dWidth;

            pCont->ChangeParam(nLastIdx, nCurrentIdx-1);

            return pCont;
        }

        void ChangeParam(const size_t &nFrom, const size_t &nBefore )
        {
            std::vector<uint32_t> arNewVec;
            for (size_t i = nFrom; i <= nBefore; ++i)
                arNewVec.push_back(m_oText[i]);

            m_oText = arNewVec;

            std::vector <double> arNewWidth;
            for (size_t i = nFrom; i <= nBefore; ++i)
            {
                if (0 == nFrom)
                    arNewWidth.push_back(m_arWidthText[i]);
                else
                    arNewWidth.push_back(m_arWidthText[i] - m_arWidthText[nFrom-1]);
            }

            m_arWidthText = arNewWidth;

        }

        void MarkFontProperties(CShape* pShape, ModeFontOptions oMode)
        {
            switch (oMode) {
                case ModeFontOptions::STRIKEOUT:
                    m_oFont.Strikeout = 1;
                    m_oFont.StrikeoutType = pShape->m_TypeLine;
                    break;
                case ModeFontOptions::UNDERLINE:
                    m_oFont.Underline = 1;
                    m_oFont.UnderlineType = pShape->m_TypeLine;
                    break;
                case ModeFontOptions::HIGHLIGHT:
                    if (m_lShadingColor >= 0)
                        m_lShadingColor = -1;

                    m_oFont.HighlightColor = pShape->m_oBrush.Color1;
                    break;
                case ModeFontOptions::SHADING:
                    if (m_oFont.HighlightColor >= 0)
                        m_oFont.HighlightColor = -1;

                    m_lShadingColor = pShape->m_oBrush.Color1;
                    break;
            }
        }

        std::wstring GetHighlightColor(const int &key)
        {
            std::map <int, std::wstring> colorHighlight = {
                {0, L"black"}, {16711680, L"blue"}, {65280, L"green"},
                {9109504, L"darkBlue"}, {139, L"darkRed"}, {32896, L"darkYellow"},
                {13882323, L"lightGray"}, {11119017, L"darkGray"}, {25600, L"darkGreen"},
                {16711935, L"magenta"}, {255, L"red"}, {16776960, L"cyan"},
                {9145088, L"darkCyan"}, {8388736, L"darkMagenta"}, {65535, L"yellow"}
            };

            auto it = colorHighlight.find(key);
            if ( it == colorHighlight.end() )
               return L"default";
            else
               return it->second;
        }

        std::wstring GetTypeUnderline(NSStructures::LineProperties type)
        {
            std::vector <std::wstring> typesUnderline = {L"none"         , L"single"         , L"double",
                                                         L"thick"        , L"dotted"         , L"dottedHeavy",
                                                         L"dash"         , L"dashedHeavy"    , L"dashLong",
                                                         L"dashLongHeavy", L"dotDash"        , L"dashDotHeavy",
                                                         L"dotDotDash"   , L"dashDotDotHeavy", L"wave",
                                                         L"wavyHeavy"    , L"wavyDouble"};

            int idx = static_cast<int>(type);
            if (idx >= 0 && idx < typesUnderline.size())
                return typesUnderline[idx];
            else
                return typesUnderline[0];
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
                {
                    oWriter.WriteString(L"<w:u w:val=\"");
                    oWriter.WriteString(GetTypeUnderline(m_oFont.UnderlineType));
                    oWriter.WriteString(L"\"/>");
                }
                if (m_oFont.Strikeout)
                {
                    if (m_oFont.StrikeoutType == NSStructures::LineProperties::DOUBLE)
                    {
                        oWriter.WriteString(L"<w:dstrike w:val=\"true\" />");
                    }
                    else
                    {
                        oWriter.WriteString(L"<w:strike w:val=\"true\" />");
                    }
                }

                if (m_oFont.HighlightColor >= 0)
                {
                    oWriter.WriteString(L"<w:highlight w:val=\"");
                    oWriter.WriteString(GetHighlightColor(m_oFont.HighlightColor));
                    oWriter.WriteString(L"\"/>");
                }

                if (m_lShadingColor >= 0)
                {
                    oWriter.WriteString(L"<w:shd w:fill=\"");
                    oWriter.WriteHexInt3(ConvertColor(m_lShadingColor));
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
                {
                    oWriter.WriteString(L"<w:u w:val=\"");
                    oWriter.WriteString(GetTypeUnderline(m_oFont.UnderlineType));
                    oWriter.WriteString(L"\"/>");
                }
                if (m_oFont.Strikeout)
                {
                    if (m_oFont.StrikeoutType == NSStructures::LineProperties::DOUBLE)
                    {
                        oWriter.WriteString(L"<w:dstrike w:val=\"true\" />");
                    }
                    else
                    {
                        oWriter.WriteString(L"<w:strike w:val=\"true\" />");
                    }
                }

                if (m_oFont.HighlightColor >= 0)
                {
                    oWriter.WriteString(L"<w:highlight w:val=\"");
                    oWriter.WriteString(GetHighlightColor(m_oFont.HighlightColor));
                    oWriter.WriteString(L"\"/>");
                }

                if (m_lShadingColor >= 0)
                {
                    oWriter.WriteString(L"<w:shd w:fill=\"");
                    oWriter.WriteHexInt3(ConvertColor(m_lShadingColor));
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

        double m_dWidthFirstWord;
        bool   m_bIsFirstWord;

    public:
        CTextLine() : m_arConts()
        {
            m_dBaselinePos	= 0;

            m_dX			= 0;
            m_dY			= 0;
            m_dWidth		= 0;
            m_dHeight		= 0;

            m_dWidthFirstWord = 0;
            m_bIsFirstWord    = false;
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

            m_dWidthFirstWord = oSrc.m_dWidthFirstWord;
            m_bIsFirstWord    = oSrc.m_bIsFirstWord;
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

        bool IsMatchColor(const int &color)
        {
            size_t nCountConts = m_arConts.size();
            for (size_t i = 0; i < nCountConts; ++i)
            {
                if (color == m_arConts[i]->m_oBrush.Color1)
                    return true;
            }
            return false;
        }

        void SearchInclusions(CShape* pShape, ModeFontOptions oMode)
        {
            double dLeftLineCoord = pShape->m_dLeft;
            double dRightLineCoord = pShape->m_dLeft+pShape->m_dWidth;

            std::vector<CContText*> arTempConts;

            size_t nCountConts = m_arConts.size();
            for (size_t i = 0; i < nCountConts; ++i)
            {
                double dLeftCont = m_arConts[i]->m_dX;
                double dRightCont = dLeftCont + m_arConts[i]->m_dWidth;

                if (dLeftLineCoord > dRightCont || dLeftCont > dRightLineCoord){
                    m_arConts[i]->CheckingBoldFont(pShape);
                    arTempConts.push_back(m_arConts[i]);
                    continue;
                }

                if (dRightLineCoord < dRightCont)
                {
                    if (dRightLineCoord > dLeftCont)
                    {
                        m_arConts[i]->CheckingBoldFont(pShape);
                        std::vector<CContText*> arBreakCont = m_arConts[i]->BreakCont(pShape, oMode);
                        for (size_t j = 0; j < arBreakCont.size(); ++j)
                            arTempConts.push_back(arBreakCont[j]);
                    }
                    else
                    {
                        arTempConts.push_back(m_arConts[i]);
                    }
                }
                else
                {
                    if (dLeftLineCoord > dLeftCont)
                    {
                        m_arConts[i]->CheckingBoldFont(pShape);
                        std::vector<CContText*> arBreakCont = m_arConts[i]->BreakCont(pShape, oMode);
                        for (size_t j = 0; j < arBreakCont.size(); ++j)
                            arTempConts.push_back(arBreakCont[j]);
                    }
                    else
                    {
                        m_arConts[i]->CheckingBoldFont(pShape);
                        m_arConts[i]->MarkFontProperties(pShape, oMode);
                        arTempConts.push_back(m_arConts[i]);
                    }
                }
            }
            m_arConts = arTempConts;
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

        void CalculatingLineWidth()
        {
            size_t countConts = m_arConts.size();
            for (size_t i = 0; i < countConts; ++i)
            {
                m_dWidth += m_arConts[i]->m_dWidth + m_arConts[i]->m_dSpaceWidthMM;
            }
        }

        double CalculatingLineHeight(double dBeforeSpacing)
        {
            double dTempHeight = 1;
            if (abs(m_dHeight) > 0.001)
            {
                dTempHeight = m_dHeight;
                if (dBeforeSpacing < 0)
                    dTempHeight += dBeforeSpacing;
            }
            return dTempHeight;
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
        double      m_dSpaceRight;
        TextAssociationType m_eTextAssociationType;

        std::vector<CTextLine*> m_arLines;

        std::wstring m_strAvailable;

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
            m_dSpaceRight  = 0.0;

            m_pManagerLight = NULL;
            m_eTextAssociationType = eType;

            m_strAvailable = L"left";
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

            //m_strAvailable  = oSrc.m_strAvailable; m_strAlignment

            return *this;
        }

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter)
        {
            oWriter.WriteString(L"<w:p>");

            switch (m_eTextAssociationType)
            {
            case TextAssociationTypeBlockChar:
                {
                    oWriter.WriteString(L"<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\" w:y=\"");
                    oWriter.AddInt((int)(m_dTop * c_dMMToDx));
                    oWriter.WriteString(L"\"/></w:pPr>");
                    break;
                }
            case TextAssociationTypeBlockLine:
                {
                    oWriter.WriteString(L"<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\" w:y=\"");
                    oWriter.AddInt((int)(m_dTop * c_dMMToDx));
                    oWriter.WriteString(L"\"/></w:pPr>");
                    break;
                }
            case TextAssociationTypePlainLine:
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
            case TextAssociationTypePlainParagraph:
                {
                    oWriter.WriteString(L"<w:pPr><w:spacing w:before=\"");
                    oWriter.AddInt((int)(m_dSpaceBefore * c_dMMToDx));
                    oWriter.WriteString(L"\" w:line=\"");
                    oWriter.AddInt((int)(m_dHeight * c_dMMToDx));
                    oWriter.WriteString(L"\" w:lineRule=\"exact\"/><w:ind w:right=\"");
                    oWriter.AddInt((int)(m_dSpaceRight * c_dMMToDx));
                    oWriter.WriteString(L"\"  w:left=\"");
                    oWriter.AddInt((int)(m_dLeft * c_dMMToDx));
                    oWriter.WriteString(L"\"/> <w:jc w:val=\"");
                    oWriter.WriteString(this->m_strAvailable);
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
                oWriter.WriteString(L"<w:r><w:t xml:space=\"preserve\"> </w:t></w:r>");
            }

            oWriter.WriteString(L"</w:p>");
        }
    };
}
