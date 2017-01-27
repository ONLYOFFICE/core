#pragma once
#include "Common.h"

#include "FontManager.h"

namespace NSDocxRenderer
{
	const double c_dMMToDx	 = 72 * 20 / 25.4;

	// у класса T должен быть метод IsBigger, IsBiggerOrEqual
	template<typename T> 
    void SortElements(CArray<T*>& oArray)
	{
		int nSize = (int)oArray.GetCount();

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

	static _bstr_t g_bstr_text_run_Start				= L"<w:r><w:rPr>";
	static _bstr_t g_bstr_text_run_PropEnd				= L"</w:rPr>";
	static _bstr_t g_bstr_text_run_End					= L"</w:r>";

	static _bstr_t g_bstr_text_run_text1				= L"<w:t xml:space=\"preserve\">";
	static _bstr_t g_bstr_text_run_text2				= L"</w:t>";

	static _bstr_t g_bstr_text_bold_true				= L"<w:b w:val=\"true\"/>";
	static _bstr_t g_bstr_text_italic_true				= L"<w:i w:val=\"true\"/>";

	static CString g_string_text_font_size				= _T("<w:sz w:val=\"%d\"/><w:szCs w:val=\"%d\"/>");
	static CString g_string_text_font_name				= _T("<w:rFonts w:ascii=\"%s\" w:hAnsi=\"%s\" w:cs=\"%s\"/>");

	static CString g_string_text_color					= _T("<w:color w:val=\"%06x\"/>");

	static CString g_string_text_paragraph_noframes		= _T("<w:pPr><w:spacing w:before=\"%d\" w:line=\"%d\" w:lineRule=\"exact\"/><w:ind w:left=\"%d\"/></w:pPr>");
	static _bstr_t g_bstr_text_par_start				= L"<w:p>";
	static _bstr_t g_bstr_text_par_end					= L"</w:p>";

	static CString g_string_spacing_character			= _T("<w:spacing w:val=\"%d\"/>");
	static CString g_string_spacing_character2			= _T("<w:spacing w:val=\"%.3lfpt\"/>");

	static CString g_string_par_props_mode2				= _T("<w:pPr><w:framePr w:hAnchor=\"page\" w:vAnchor=\"page\" w:x=\"%d\" w:y=\"%d\"/></w:pPr>");

    inline void DeleteSpaces(CString& strText)
	{
		int nLen   = strText.GetLength();
		int nStart = 0;

		while ((nStart < nLen) && (TCHAR(' ') == strText[nStart]))
			++nStart;

		if (nStart == nLen)
		{
			strText = _T("");
			return;
		}
		
		int nEnd = nLen - 1;
		while ((nEnd > nStart) && (TCHAR(' ') == strText[nEnd]))
			--nEnd;

		strText = strText.Mid(nStart, nEnd - nStart + 1);
	}

	class CContText
	{
	public:
		NSStructures::CFont		m_oFont;
		NSStructures::CBrush	m_oBrush;

		CString m_strPickFontName;
		LONG	m_lPickFontStyle;

		CString m_strText;
		CString m_strGidText;

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
			m_strText		= _T("");
			m_strGidText	= _T("");

			m_strPickFontName	= _T("");
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

		AVSINLINE void Clear()
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

		AVSINLINE bool IsBigger(const CContText* oSrc)
		{
			return (m_dX > oSrc->m_dX) ? true : false;
		}
		AVSINLINE bool IsBiggerOrEqual(const CContText* oSrc)
		{
			return (m_dX >= oSrc->m_dX) ? true : false;
		}

		AVSINLINE void Write(NSDocxRenderer::CStringWriter& oWriter, CFontManagerLight* pManagerLight, bool bIsAddSpace = false)
		{
			oWriter.WriteString(g_bstr_text_run_Start);

			if (m_dWidth != m_dWidthWithoutSpaces)
			{
				DeleteSpaces(m_strText);
				m_dWidth = m_dWidthWithoutSpaces;
			}
			
			if (_T("") == m_strPickFontName)
			{
				if (m_oFont.Bold)
					oWriter.WriteString(g_bstr_text_bold_true);
				if (m_oFont.Italic)
					oWriter.WriteString(g_bstr_text_italic_true);

				if (bIsAddSpace)
				{
					m_dWidth  += m_dSpaceWidthMM;
					m_strText += _T(" ");
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
					m_strText += _T(" ");
				}

				// нужно перемерять...
				double ___dSize = (double)((LONG)(m_oFont.Size * 2)) / 2;
				pManagerLight->LoadFont(m_strPickFontName, m_lPickFontStyle, ___dSize, FALSE);
				double dWidth = pManagerLight->MeasureStringWidth(m_strText);

				if (fabs(dWidth - m_dWidth) > 2)
				{
					double dSpacing = (m_dWidth - dWidth) / (m_strText.GetLength() + 1);
					dSpacing *= c_dMMToDx;

					CString strSpacing = _T("");
					strSpacing.Format(g_string_spacing_character, (LONG)dSpacing);
					oWriter.WriteString(strSpacing);
				}
			}

			CString strSize = _T("");
			LONG lSize = (LONG)(2 * m_oFont.Size);
			strSize.Format(g_string_text_font_size, lSize, lSize);
			oWriter.WriteString(strSize);

			CString strName = _T("");

			if (_T("") == m_strPickFontName)
				strName.Format(g_string_text_font_name, m_oFont.Name, m_oFont.Name, m_oFont.Name);
			else
				strName.Format(g_string_text_font_name, m_strPickFontName, m_strPickFontName, m_strPickFontName);

			oWriter.WriteString(strName);

			CString strColor = _T("");
			strColor.Format(g_string_text_color, ConvertColor(m_oBrush.Color1));
			oWriter.WriteString(strColor);

			oWriter.WriteString(g_bstr_text_run_PropEnd);

			oWriter.WriteString(g_bstr_text_run_text1);

			CString strText = m_strText;
			CorrectString(strText);

			oWriter.WriteString(strText);
			oWriter.WriteString(g_bstr_text_run_text2);

			oWriter.WriteString(g_bstr_text_run_End);
		}

		AVSINLINE void WriteTo(double dSpacingMM, NSDocxRenderer::CStringWriter& oWriter, CFontManagerLight* pManagerLight)
		{
			oWriter.WriteString(g_bstr_text_run_Start);

			double dSpaceMMSize = m_dSpaceWidthMM;
			if (_T("") == m_strPickFontName)
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

			CString strSize = _T("");
			LONG lSize = (LONG)(2 * m_oFont.Size);
			strSize.Format(g_string_text_font_size, lSize, lSize);
			oWriter.WriteString(strSize);

			CString strName = _T("");

			if (_T("") == m_strPickFontName)
				strName.Format(g_string_text_font_name, m_oFont.Name, m_oFont.Name, m_oFont.Name);
			else
				strName.Format(g_string_text_font_name, m_strPickFontName, m_strPickFontName, m_strPickFontName);

			oWriter.WriteString(strName);

			CString strColor = _T("");
			strColor.Format(g_string_text_color, ConvertColor(m_oBrush.Color1));
			oWriter.WriteString(strColor);

			LONG lSpacing = (LONG)((dSpacingMM - dSpaceMMSize) * c_dMMToDx);
			CString strSpacing = _T("");
			strSpacing.Format(g_string_spacing_character, lSpacing);
			oWriter.WriteString(strSpacing);

			oWriter.WriteString(g_bstr_text_run_PropEnd);

			oWriter.WriteString(g_bstr_text_run_text1);

			CString strText = _T(" ");
			oWriter.WriteString(strText);
			oWriter.WriteString(g_bstr_text_run_text2);

			oWriter.WriteString(g_bstr_text_run_End);
		}
	};
	
	class CTextLine
	{
	public:
		CAtlArray<CContText*> m_arConts;

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
		AVSINLINE void Clear()
		{
			size_t nCount = m_arConts.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CContText* pText = m_arConts[i];
				RELEASEOBJECT(pText);
			}
			m_arConts.RemoveAll();
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
			size_t nCount = oSrc.m_arConts.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arConts.Add(new CContText(*oSrc.m_arConts[i]));
			}

			m_dBaselinePos	= oSrc.m_dBaselinePos;
			m_dX			= oSrc.m_dX;
			m_dY			= oSrc.m_dY;
			m_dWidth		= oSrc.m_dWidth;
			m_dHeight		= oSrc.m_dHeight;
		}

		AVSINLINE void AddCont(CContText* pCont, double dBaselineOffset)
		{
			if (0 == m_arConts.GetCount())
				m_dBaselineOffset = dBaselineOffset;

			if ( ( pCont->m_dX > 0 ) && ( ( m_dX == 0 ) || ( pCont->m_dX < m_dX ) ) )
				m_dX = pCont->m_dX;

			if (m_dHeight < pCont->m_dHeight)
				m_dHeight = pCont->m_dHeight;

			m_arConts.Add(pCont);
		}

		AVSINLINE bool IsBigger(const CTextLine* oSrc)
		{
			return (m_dBaselinePos > oSrc->m_dBaselinePos) ? true : false;
		}
		AVSINLINE bool IsBiggerOrEqual(const CTextLine* oSrc)
		{
			return (m_dBaselinePos >= oSrc->m_dBaselinePos) ? true : false;
		}

		AVSINLINE void SortConts()
		{
			// сортировка непрерывных слов по m_dX
			SortElements(m_arConts);
		}

		void Merge(CTextLine* pTextLine)
		{
			size_t nCount = pTextLine->m_arConts.GetCount();
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
					m_arConts.Add(pTextLine->m_arConts[i]);
				}
			}			
		}

		void ToXml(NSDocxRenderer::CStringWriter& oWriter, CFontManagerLight* pManagerLight)
		{
			size_t nCountConts = m_arConts.GetCount();

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

		CAtlArray<CTextLine*> m_arLines;

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

		AVSINLINE void Clear()
		{
			size_t nCount = m_arLines.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CTextLine* pText = m_arLines[i];
				RELEASEOBJECT(pText);
			}
			m_arLines.RemoveAll();

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
			size_t nCount = oSrc.m_arLines.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arLines.Add(new CTextLine(*oSrc.m_arLines[i]));
			}

			m_pManagerLight = oSrc.m_pManagerLight;

			return *this;
		}

		virtual void ToXml(NSDocxRenderer::CStringWriter& oWriter)
		{
			oWriter.WriteString(g_bstr_text_par_start);

			switch (m_eTextAssociationType)
			{
			case TextAssociationTypeDefault:
			case TextAssociationTypeLine:
				{
					LONG lX		= (LONG)(m_dLeft * c_dMMToDx);
					LONG lY		= (LONG)(m_dTop * c_dMMToDx);

					CString strTextProps = _T("");
					strTextProps.Format(g_string_par_props_mode2, lX, lY);
					oWriter.WriteString(strTextProps);
					break;
				}
			case TextAssociationTypeBlock:
				{
					LONG lX		= (LONG)(m_dLeft * c_dMMToDx);
					LONG lY		= (LONG)(m_dTop * c_dMMToDx);

					CString strTextProps = _T("");
					strTextProps.Format(g_string_par_props_mode2, lX, lY);
					oWriter.WriteString(strTextProps);
					break;
				}
			case TextAssociationTypeNoFrames:
				{
					LONG lSpaceBefore	= (LONG)(m_dSpaceBefore * c_dMMToDx);
					LONG lHeight		= (LONG)(m_dHeight * c_dMMToDx);
					LONG lLeft			= (LONG)(m_dLeft * c_dMMToDx);
					
					CString strParProperties = _T("");
					strParProperties.Format(g_string_text_paragraph_noframes, lSpaceBefore, lHeight, lLeft);

					oWriter.WriteString(strParProperties);
					break;
				}
			default:
				break;
			}

			size_t nCount = m_arLines.GetCount();
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
