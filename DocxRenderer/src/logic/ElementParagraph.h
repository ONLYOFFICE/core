#pragma once
#include "Common.h"
#include "FontManager.h"

namespace NSDocxRenderer
{
	const double c_dMMToDx	 = 72 * 20 / 25.4;

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

    static std::wstring g_bstr_text_run_Start			= L"<w:r><w:rPr>";
    static std::wstring g_bstr_text_run_PropEnd			= L"</w:rPr>";
    static std::wstring g_bstr_text_run_End				= L"</w:r>";

    static std::wstring g_bstr_text_run_text1			= L"<w:t xml:space=\"preserve\">";
    static std::wstring g_bstr_text_run_text2			= L"</w:t>";

    static std::wstring g_bstr_text_bold_true			= L"<w:b w:val=\"true\"/>";
    static std::wstring g_bstr_text_italic_true			= L"<w:i w:val=\"true\"/>";

    static std::wstring g_string_text_font_size			= L"<w:sz w:val=\"%d\"/><w:szCs w:val=\"%d\"/>";
    static std::wstring g_string_text_font_name			= L"<w:rFonts w:ascii=\"%s\" w:hAnsi=\"%s\" w:cs=\"%s\"/>";

    static std::wstring g_string_text_color				= L"<w:color w:val=\"%06x\"/>";

    static std::wstring g_string_text_paragraph_noframes	= L"<w:pPr><w:spacing w:before=\"%d\" w:line=\"%d\" w:lineRule=\"exact\"/><w:ind w:left=\"%d\"/></w:pPr>";
    static std::wstring g_bstr_text_par_start				= L"<w:p>";
    static std::wstring g_bstr_text_par_end					= L"</w:p>";

    static std::wstring g_string_spacing_character			= L"<w:spacing w:val=\"%d\"/>";
    static std::wstring g_string_spacing_character2			= L"<w:spacing w:val=\"%.3lfpt\"/>";

    static std::wstring g_string_par_props_mode2			= L"<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"%d\" w:y=\"%d\"/></w:pPr>";

    inline void DeleteSpaces(std::wstring& strText)
	{
        std::wstring::size_type nLen   = strText.length();
        std::wstring::size_type nStart = 0;

        while ((nStart < nLen) && (' ' == strText[nStart]))
			++nStart;

		if (nStart == nLen)
		{
            strText = L"";
			return;
		}
		
        std::wstring::size_type nEnd = nLen - 1;
        while ((nEnd > nStart) && (' ' == strText[nEnd]))
			--nEnd;

        strText = strText.substr(nStart, nEnd - nStart + 1);
	}

	class CContText
	{
	public:
		NSStructures::CFont		m_oFont;
		NSStructures::CBrush	m_oBrush;

        std::wstring m_strPickFontName;
		LONG	m_lPickFontStyle;

        std::wstring m_strText;
        std::wstring m_strGidText;

		double m_dX;
		double m_dY;
		double m_dWidth;
		double m_dHeight;

		double m_dWidthWithoutSpaces;
		double m_dLeftWithoutSpaces;

		double m_dPosition;
		double m_dSpaceWidthMM;

		double m_dCalculateWidth;

	public:
		CContText()
		{
            m_strText		= L"";
            m_strGidText	= L"";

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

			m_strText	 = oSrc.m_strText;
			m_strGidText = oSrc.m_strGidText;

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

        inline void Write(NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight, bool bIsAddSpace = false)
		{
			oWriter.WriteString(g_bstr_text_run_Start);

			if (m_dWidth != m_dWidthWithoutSpaces)
			{
				DeleteSpaces(m_strText);
				m_dWidth = m_dWidthWithoutSpaces;
			}
			
            if (m_strPickFontName.empty())
			{
				if (m_oFont.Bold)
					oWriter.WriteString(g_bstr_text_bold_true);
				if (m_oFont.Italic)
					oWriter.WriteString(g_bstr_text_italic_true);

				if (bIsAddSpace)
				{
					m_dWidth  += m_dSpaceWidthMM;
                    m_strText += L" ";
				}
			}
			else
			{
				if (0x01 == (0x01 & m_lPickFontStyle))
					oWriter.WriteString(g_bstr_text_bold_true);
				if (0x02 == (0x02 & m_lPickFontStyle))
					oWriter.WriteString(g_bstr_text_italic_true);

				if (bIsAddSpace)
				{
					m_dWidth  += pManagerLight->GetSpaceWidth();
                    m_strText += L" ";
				}

				// нужно перемерять...
				double ___dSize = (double)((LONG)(m_oFont.Size * 2)) / 2;
				pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, FALSE);
				double dWidth = pManagerLight->MeasureStringWidth(m_strText);

				if (fabs(dWidth - m_dWidth) > 2)
				{
                    double dSpacing = (m_dWidth - dWidth) / (m_strText.length() + 1);
					dSpacing *= c_dMMToDx;

                    oWriter.WriteString(StringFormat(g_string_spacing_character, (int)dSpacing));
				}
			}

			LONG lSize = (LONG)(2 * m_oFont.Size);
            oWriter.WriteString(StringFormat(g_string_text_font_size, lSize, lSize));

            const wchar_t* strFontName = m_strPickFontName.empty() ? m_oFont.Name.c_str() : m_strPickFontName.c_str();
            oWriter.WriteString(StringFormat(g_string_text_font_name, strFontName, strFontName, strFontName));

            oWriter.WriteString(StringFormat(g_string_text_color, ConvertColor(m_oBrush.Color1)));

			oWriter.WriteString(g_bstr_text_run_PropEnd);

			oWriter.WriteString(g_bstr_text_run_text1);

            oWriter.WriteEncodeXmlString(m_strText);

			oWriter.WriteString(g_bstr_text_run_text2);

			oWriter.WriteString(g_bstr_text_run_End);
		}

        void WriteTo(double dSpacingMM, NSStringUtils::CStringBuilder& oWriter, CFontManagerLight* pManagerLight)
		{
			oWriter.WriteString(g_bstr_text_run_Start);

			double dSpaceMMSize = m_dSpaceWidthMM;
            if (m_strPickFontName.empty())
			{
				if (m_oFont.Bold)
					oWriter.WriteString(g_bstr_text_bold_true);
				if (m_oFont.Italic)
					oWriter.WriteString(g_bstr_text_italic_true);
			}
			else
			{
				if (0x01 == (0x01 & m_lPickFontStyle))
					oWriter.WriteString(g_bstr_text_bold_true);
				if (0x02 == (0x02 & m_lPickFontStyle))
					oWriter.WriteString(g_bstr_text_italic_true);

				dSpaceMMSize = pManagerLight->GetSpaceWidth();
			}

			LONG lSize = (LONG)(2 * m_oFont.Size);
            oWriter.WriteString(StringFormat(g_string_text_font_size, lSize, lSize));

            const wchar_t* strFontName = m_strPickFontName.empty() ? m_oFont.Name.c_str() : m_strPickFontName.c_str();
            oWriter.WriteString(StringFormat(g_string_text_font_name, strFontName, strFontName, strFontName));

            oWriter.WriteString(StringFormat(g_string_text_color, ConvertColor(m_oBrush.Color1)));

			LONG lSpacing = (LONG)((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
            oWriter.WriteString(StringFormat(g_string_spacing_character, lSpacing));

			oWriter.WriteString(g_bstr_text_run_PropEnd);

			oWriter.WriteString(g_bstr_text_run_text1);

            oWriter.WriteString(L" ");
			oWriter.WriteString(g_bstr_text_run_text2);

			oWriter.WriteString(g_bstr_text_run_End);
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
			oWriter.WriteString(g_bstr_text_par_start);

			switch (m_eTextAssociationType)
			{
			case TextAssociationTypeDefault:
			case TextAssociationTypeLine:
				{
					LONG lX		= (LONG)(m_dLeft * c_dMMToDx);
					LONG lY		= (LONG)(m_dTop * c_dMMToDx);

                    oWriter.WriteString(StringFormat(g_string_par_props_mode2, lX, lY));
					break;
				}
			case TextAssociationTypeBlock:
				{
					LONG lX		= (LONG)(m_dLeft * c_dMMToDx);
					LONG lY		= (LONG)(m_dTop * c_dMMToDx);

                    oWriter.WriteString(StringFormat(g_string_par_props_mode2, lX, lY));
					break;
				}
			case TextAssociationTypeNoFrames:
				{
					LONG lSpaceBefore	= (LONG)(m_dSpaceBefore * c_dMMToDx);
					LONG lHeight		= (LONG)(m_dHeight * c_dMMToDx);
					LONG lLeft			= (LONG)(m_dLeft * c_dMMToDx);
					
                    oWriter.WriteString(StringFormat(g_string_text_paragraph_noframes, lSpaceBefore, lHeight, lLeft));
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

			oWriter.WriteString(g_bstr_text_par_end);
		}
	};
}
