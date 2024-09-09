#include "ContText.h"

#include "../../resources/ColorTable.h"
#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	bool IsTextOnlySpaces(const NSStringUtils::CStringUTF32& oText)
	{
		bool only_spaces = true;
		for (size_t j = 0; j < oText.length(); ++j)
			if (!IsSpaceUtf32(oText.at(j)))
			{
				only_spaces = false;
				break;
			}
		return only_spaces;
	}

	CSelectedSizes::CSelectedSizes(const CSelectedSizes& oSelectedSizes)
	{
		*this = oSelectedSizes;
	}
	CSelectedSizes& CSelectedSizes::operator=(const CSelectedSizes& oSelectedSizes)
	{
		dWidth = oSelectedSizes.dWidth;
		dHeight = oSelectedSizes.dHeight;
		return *this;
	}

	CContText::CContText(const CContText& rCont)
	{
		*this = rCont;
	}

	CContText::~CContText()
	{
		Clear();
	}

	void CContText::Clear()
	{
		m_pFontStyle = nullptr;
	}

	CContText& CContText::operator= (const CContText& rCont)
	{
		if (this == &rCont)
			return *this;

		CBaseItem::operator=(rCont);

		m_pFontStyle = rCont.m_pFontStyle;

		m_bIsStrikeoutPresent = rCont.m_bIsStrikeoutPresent;
		m_bIsDoubleStrikeout = rCont.m_bIsDoubleStrikeout;
		m_bIsHighlightPresent = rCont.m_bIsHighlightPresent;
		m_lHighlightColor = rCont.m_lHighlightColor;
		m_bIsUnderlinePresent = rCont.m_bIsUnderlinePresent;
		m_eUnderlineType = rCont.m_eUnderlineType;
		m_lUnderlineColor = rCont.m_lUnderlineColor;
		m_bIsShadowPresent = rCont.m_bIsShadowPresent;
		m_bIsOutlinePresent = rCont.m_bIsOutlinePresent;
		m_bIsEmbossPresent = rCont.m_bIsEmbossPresent;
		m_bIsEngravePresent = rCont.m_bIsEngravePresent;

		m_oText = rCont.m_oText;

		m_oSelectedSizes = rCont.m_oSelectedSizes;
		m_dSpaceWidthMM = rCont.m_dSpaceWidthMM;
		m_eVertAlignType = rCont.m_eVertAlignType;

		m_pManager = rCont.m_pManager;

		m_pShape = rCont.m_pShape;
		m_pCont = rCont.m_pCont;

		m_iNumDuplicates = rCont.m_iNumDuplicates;

		m_dTopWithAscent = rCont.m_dTopWithAscent;
		m_dBotWithDescent = rCont.m_dBotWithDescent;

		m_oSelectedFont = rCont.m_oSelectedFont;
		m_bPossibleSplit = rCont.m_bPossibleSplit;

		m_arSymWidths.clear();
		m_arSymWidths.resize(rCont.m_arSymWidths.size());
		for (size_t i = 0; i < rCont.m_arSymWidths.size(); ++i)
			m_arSymWidths[i] = rCont.m_arSymWidths[i];

		return *this;
	}

	void CContText::CalcSelected()
	{
		if (!m_pFontStyle->wsFontName.empty() && !m_oText.empty())
		{
			// нужно перемерять...
			if (m_oSelectedFont.Path.empty())
				m_pManager->LoadFontByName(m_oSelectedFont);
			else
				m_pManager->LoadFontByFile(m_oSelectedFont);

			double dBoxX;
			double dBoxY;
			double dBoxWidth;
			double dBoxHeight;

			m_pManager->SetStringGid(0);
			m_pManager->MeasureString(m_oText.ToStdWString(), 0, 0, dBoxX, dBoxY, dBoxWidth, dBoxHeight, CFontManager::mtPosition);

			m_oSelectedSizes.dWidth = dBoxWidth;
			m_oSelectedSizes.dHeight = dBoxHeight;
		}
	}


	size_t CContText::GetLength() const noexcept
	{
		return m_oText.length();
	}

	std::shared_ptr<CContText> CContText::Split(size_t index)
	{
		const size_t len = GetLength();
		if (index >= len - 1)
			return nullptr;

		auto lefts = GetSymLefts();

		auto cont = std::make_shared<CContText>(*this);
		cont->m_oText = m_oText.substr(index + 1, (len - (index + 1)));
		cont->m_dLeft = lefts[index + 1];
		cont->m_dWidth = cont->m_dRight - cont->m_dLeft;

		cont->m_arSymWidths.clear();
		for (size_t i = index + 1; i < len; ++i)
			cont->m_arSymWidths.push_back(m_arSymWidths[i]);

		m_oText = m_oText.substr(0, index + 1);
		m_dRight = cont->m_dLeft;
		m_dWidth = m_dRight - m_dLeft;
		m_arSymWidths.resize(index + 1);
		m_bPossibleSplit = false;

		return cont;
	}
	std::shared_ptr<CContText> CContText::Split(double dLeft)
	{
		if (dLeft < m_dLeft)
			return nullptr;

		auto lefts = GetSymLefts();
		auto it = std::lower_bound(lefts.begin(), lefts.end(), dLeft);

		if (it == lefts.end())
			return nullptr;

		size_t index = std::distance(lefts.begin(), it);
		if (index == 0)
			return nullptr;

		index--;

		// if a little overlapped the next one - take the previous one
		if (abs(lefts[index] - dLeft) < c_dTHE_STRING_X_PRECISION_MM)
			index--;

		if (index == 0)
			return nullptr;

		return Split(index);
	}

	eVerticalCrossingType CContText::GetVerticalCrossingType(const CContText* pCont) const noexcept
	{
		const double& this_top = m_dTopWithAscent;
		const double& this_bot = m_dBotWithDescent;

		const double& other_top = pCont->m_dTopWithAscent;
		const double& other_bot = pCont->m_dBotWithDescent;

		if (this_top > other_top && this_bot < other_bot)
			return eVerticalCrossingType::vctCurrentInsideNext;

		else if (this_top < other_top && this_bot > other_bot)
			return  eVerticalCrossingType::vctCurrentOutsideNext;

		else if (this_top < other_top && this_bot < other_bot &&
				 (this_bot >= other_top || fabs(this_bot - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
			return  eVerticalCrossingType::vctCurrentAboveNext;

		else if (this_top > other_top && this_bot > other_bot &&
				 (this_top <= other_bot || fabs(this_top - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
			return  eVerticalCrossingType::vctCurrentBelowNext;

		else if (this_top == other_top && this_bot == other_bot &&
				 m_dLeft == pCont->m_dLeft && m_dRight == pCont->m_dRight)
			return  eVerticalCrossingType::vctDublicate;

		else if (fabs(this_top - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
				 fabs(this_bot - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctTopAndBottomBordersMatch;

		else if (fabs(this_top - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctTopBorderMatch;

		else if (fabs(this_bot - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctBottomBorderMatch;

		else if (this_bot < other_top)
			return  eVerticalCrossingType::vctNoCrossingCurrentAboveNext;

		else if (this_top > other_bot)
			return  eVerticalCrossingType::vctNoCrossingCurrentBelowNext;

		else
			return  eVerticalCrossingType::vctUnknown;
	}

	void CContText::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:r>");
		oWriter.WriteString(L"<w:rPr>");
		oWriter.WriteString(L"<w:noProof/>");

		if (!m_bWriteStyleRaw)
		{
			oWriter.WriteString(L"<w:rStyle w:val=\"");
			oWriter.WriteString(m_pFontStyle->wsFontStyleId);
			oWriter.WriteString(L"\"/>");
		}

		LONG lCalculatedSpacing = 0;

		if (!m_oText.empty())
		{
			double dSpacing = (m_dWidth - m_oSelectedSizes.dWidth) / (m_oText.length());
			dSpacing *= c_dMMToDx;

			//mm to points * 20
			lCalculatedSpacing = static_cast<LONG>(dSpacing);
		}

		// принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
		lCalculatedSpacing -= 1;

		if (lCalculatedSpacing != 0)
		{
			oWriter.WriteString(L"<w:spacing w:val=\"");
			oWriter.AddInt(lCalculatedSpacing);
			oWriter.WriteString(L"\"/>");
		}

		if (m_bIsEmbossPresent)
			oWriter.WriteString(L"<w:emboss/>");
		else if(m_bIsEngravePresent)
			oWriter.WriteString(L"<w:imprint/>");
		else
		{
			if (m_bIsOutlinePresent)
				oWriter.WriteString(L"<w:outline/>");
			if (m_bIsShadowPresent)
				oWriter.WriteString(L"<w:shadow/>");
		}

		if (m_bIsStrikeoutPresent)
		{
			if (m_bIsDoubleStrikeout)
				oWriter.WriteString(L"<w:dstrike/>");
			else
				oWriter.WriteString(L"<w:strike/>");
		}

		if (m_bIsUnderlinePresent)
		{
			oWriter.WriteString(L"<w:u w:val=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToString(m_eUnderlineType));
			if (m_lUnderlineColor != m_pFontStyle->oBrush.Color1)
			{
				oWriter.WriteString(L" w:color=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
				oWriter.WriteString(L"\"");
			}
			oWriter.WriteString(L"/>");
		}

		if (m_bIsHighlightPresent)
		{
			//note В <w:style это не работает
			ColorTable& colorTable = SingletonInstance<ColorTable>();
			if (colorTable.IsStandardColor(m_lHighlightColor))
			{
				oWriter.WriteString(L"<w:highlight w:val=\"");
				oWriter.WriteString(colorTable.ConverColorToString(ConvertColorBGRToRGB(m_lHighlightColor)));
			}
			else
			{
				oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
			}
			oWriter.WriteString(L"\"/>");
		}

		if (m_eVertAlignType == eVertAlignType::vatSubscript || m_eVertAlignType == eVertAlignType::vatSuperscript)
		{
			int lSize = static_cast<int>(3.0 * m_pFontStyle->dFontSize);
			oWriter.WriteString(L"<w:sz w:val=\"");
			oWriter.AddInt(lSize);
			oWriter.WriteString(L"\"/><w:szCs w:val=\"");
			oWriter.AddInt(lSize);
			oWriter.WriteString(L"\"/>");
		}
		else if (m_bWriteStyleRaw)
		{
			int lSize = static_cast<int>(2.0 * m_pFontStyle->dFontSize);
			oWriter.WriteString(L"<w:sz w:val=\"");
			oWriter.AddInt(lSize);
			oWriter.WriteString(L"\"/><w:szCs w:val=\"");
			oWriter.AddInt(lSize);
			oWriter.WriteString(L"\"/>");
		}

		if (m_bWriteStyleRaw)
		{
			oWriter.WriteString(L"<w:rFonts w:ascii=\"");
			oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
			oWriter.WriteString(L"\" w:hAnsi=\"");
			oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
			oWriter.WriteString(L"\" w:cs=\"");
			oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
			oWriter.WriteString(L"\" w:hint=\"default\"/>");

			if (m_pFontStyle->bBold)
			{
				oWriter.WriteString(L"<w:b/>");
				oWriter.WriteString(L"<w:bCs/>");
			}
			if (m_pFontStyle->bItalic)
			{
				oWriter.WriteString(L"<w:i/>");
				oWriter.WriteString(L"<w:iCs/>");
			}
			if (ConvertColorBGRToRGB(m_pFontStyle->oBrush.Color1) != c_iBlackColor2)
			{
				oWriter.WriteString(L"<w:color w:val=\"");
				oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_pFontStyle->oBrush.Color1));
				oWriter.WriteString(L"\"/>");
			}
		}

		if (m_eVertAlignType == eVertAlignType::vatSubscript)
			oWriter.WriteString(L"<w:vertAlign w:val=\"subscript\"/>");
		else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
			oWriter.WriteString(L"<w:vertAlign w:val=\"superscript\"/>");

		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"<w:t xml:space=\"preserve\">");
		oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
		oWriter.WriteString(L"</w:t>");
		if (m_bIsAddBrEnd) oWriter.WriteString(L"<w:br/>");
		oWriter.WriteString(L"</w:r>");
	}

	void CContText::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<a:r>");
		oWriter.WriteString(L"<a:rPr noProof=\"1\"");

		LONG lCalculatedSpacing = 0;
		if (!m_oText.empty())
		{
			double dSpacing = (m_dWidth - m_oSelectedSizes.dWidth) / (m_oText.length());
			dSpacing *= c_dMMToPt * 100;
			lCalculatedSpacing = static_cast<LONG>(dSpacing);
		}

		// принудительно уменьшаем spacing чтобы текстовые линии не выходили за правую границу
		lCalculatedSpacing -= 15;

		oWriter.WriteString(L" spc=\"");
		oWriter.AddInt(lCalculatedSpacing);
		oWriter.WriteString(L"\"");

		if (m_bIsOutlinePresent)
			oWriter.WriteString(L" ln=\"1\"");
		// if (m_bIsShadowPresent)

		if (m_bIsStrikeoutPresent)
		{
			if (m_bIsDoubleStrikeout)
				oWriter.WriteString(L" strike=\"dblStrike\"");
			else
				oWriter.WriteString(L" strike=\"sngStrike\"");
		}

		if (m_bIsUnderlinePresent)
		{
			oWriter.WriteString(L" u=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToStringPptx(m_eUnderlineType));
		}

		UINT lSize = 0;
		if (m_eVertAlignType == eVertAlignType::vatSubscript || m_eVertAlignType == eVertAlignType::vatSuperscript)
			lSize = static_cast<int>(1.5 * m_pFontStyle->dFontSize) * 100;
		else if (m_bWriteStyleRaw)
			lSize = static_cast<int>(m_pFontStyle->dFontSize) * 100;

		oWriter.WriteString(L" sz=\"");
		oWriter.AddUInt(lSize);
		oWriter.WriteString(L"\"");

		if (m_pFontStyle->bBold)
			oWriter.WriteString(L" b=\"1\"");
		if (m_pFontStyle->bItalic)
			oWriter.WriteString(L" i=\"1\"");

		if (m_eVertAlignType == eVertAlignType::vatSubscript)
			oWriter.WriteString(L" baseline=\"-20000\">");
		else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
			oWriter.WriteString(L" baseline=\"30000\">");

		oWriter.WriteString(L">");

		oWriter.WriteString(L"<a:sym typeface=\"");
		oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<a:cs typeface=\"");
		oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<a:latin typeface=\"");
		oWriter.WriteEncodeXmlString(m_pFontStyle->wsFontName);
		oWriter.WriteString(L"\"/>");

		if (m_bIsUnderlinePresent && m_lUnderlineColor != m_pFontStyle->oBrush.Color1)
		{
			oWriter.WriteString(L"<a:uFill>");
			oWriter.WriteString(L"<a:solidFill>");
			oWriter.WriteString(L"<a:srgbClr val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lUnderlineColor));
			oWriter.WriteString(L"\"/>");
			oWriter.WriteString(L"</a:solidFill>");
			oWriter.WriteString(L"</a:uFill>");
		}

		if (m_bIsHighlightPresent)
		{
			oWriter.WriteString(L"<a:highlight>");
			oWriter.WriteString(L"<a:srgbClr val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lHighlightColor));
			oWriter.WriteString(L"\"/>");
			oWriter.WriteString(L"</a:highlight>");
		}

		if (ConvertColorBGRToRGB(m_pFontStyle->oBrush.Color1) != c_iBlackColor)
		{
			oWriter.WriteString(L"<a:solidFill>");
			oWriter.WriteString(L"<a:srgbClr val=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_pFontStyle->oBrush.Color1));
			oWriter.WriteString(L"\"/>");
			oWriter.WriteString(L"</a:solidFill>");
		}


		oWriter.WriteString(L"</a:rPr>");
		oWriter.WriteString(L"<a:t>");
		oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
		oWriter.WriteString(L"</a:t>");
		if (m_bIsAddBrEnd) oWriter.WriteString(L"<a:br/>");

		// meta info for pdf-editor
		oWriter.WriteString(L"<metaorigin:pos");
		oWriter.WriteString(L" x=\"");
		oWriter.AddDouble(m_dLeft, 4);
		oWriter.WriteString(L"\" y=\"");
		oWriter.AddDouble(m_dBaselinePos, 4);
		oWriter.WriteString(L"\" widths=\"");
		for (auto& w : m_arSymWidths)
		{
			oWriter.AddDouble(w, 4);
			oWriter.WriteString(L",");
		}
		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"</a:r>");
	}

	bool CContText::IsEqual(const CContText *pCont) const noexcept
	{
		bool bIf1 = m_pFontStyle->wsFontStyleId == pCont->m_pFontStyle->wsFontStyleId;
		bool bIf2 = m_bIsStrikeoutPresent == pCont->m_bIsStrikeoutPresent;
		bool bIf3 = m_bIsDoubleStrikeout == pCont->m_bIsDoubleStrikeout;
		bool bIf4 = m_bIsHighlightPresent == pCont->m_bIsHighlightPresent;
		bool bIf5 = m_lHighlightColor == pCont->m_lHighlightColor;
		bool bIf6 = m_bIsUnderlinePresent == pCont->m_bIsUnderlinePresent;
		bool bIf7 = m_eUnderlineType == pCont->m_eUnderlineType;
		bool bIf8 = m_lUnderlineColor == pCont->m_lUnderlineColor;
		bool bIf9 = m_bIsShadowPresent == pCont->m_bIsShadowPresent;
		bool bIf10 = m_bIsOutlinePresent == pCont->m_bIsOutlinePresent;
		bool bIf11 = m_bIsEmbossPresent == pCont->m_bIsEmbossPresent;
		bool bIf12 = m_bIsEngravePresent == pCont->m_bIsEngravePresent;
		bool bIf13 = m_eVertAlignType == pCont->m_eVertAlignType;
		bool bIf14 = m_eVertAlignType == eVertAlignType::vatUnknown && pCont->m_eVertAlignType == eVertAlignType::vatBase;
		bool bIf15 = m_eVertAlignType == eVertAlignType::vatBase && pCont->m_eVertAlignType == eVertAlignType::vatUnknown;

		return (bIf1 && bIf2 && bIf3 && bIf4 && bIf5 && bIf6 && bIf7 &&
				bIf8 && bIf9 && bIf10 && bIf11 && bIf12 && (bIf13 || bIf14 || bIf15));
	}

	UINT CContText::GetNumberOfFeatures() const noexcept
	{
		UINT ret = 0;
		if (m_pFontStyle->bBold) ret++;
		if (m_pFontStyle->bItalic) ret++;
		if (m_bIsStrikeoutPresent) ret++;
		if (m_bIsDoubleStrikeout) ret++;
		if (m_bIsHighlightPresent) ret++;
		if (m_bIsUnderlinePresent) ret++;
		if (m_eVertAlignType != eVertAlignType::vatUnknown) ret++;
		return ret;
	}

	bool CContText::IsDuplicate(CContText* pCont, eVerticalCrossingType eVType) const noexcept
	{
		if (eVType == eVerticalCrossingType::vctDublicate && m_oText == pCont->m_oText)
			return true;
		return false;
	}

	bool CContText::IsOnlySpaces() const
	{
		return IsTextOnlySpaces(m_oText);
	}

	void CContText::AddTextBack(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths)
	{
		bool is_space_twice = m_oText.at(m_oText.length() - 1) == c_SPACE_SYM &&
							  oText.at(0) == c_SPACE_SYM;

		for (size_t i = 0; i < arSymWidths.size(); ++i)
		{
			auto& w = arSymWidths[i];
			if (i == 0 && is_space_twice)
			{
				m_arSymWidths.back() = m_arSymWidths.back() + arSymWidths[i];
				m_dWidth += arSymWidths[i];
				continue;
			}
			m_arSymWidths.push_back(w);
			m_dWidth += w;
			m_oText += oText.at(i);
		}
		m_dRight = m_dLeft + m_dWidth;
	}
	void CContText::AddTextFront(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths)
	{
		m_oText = oText + m_oText;

		auto ar_sym_w = m_arSymWidths;
		m_arSymWidths = arSymWidths;

		for (auto& w : ar_sym_w)
			m_arSymWidths.push_back(w);
	}
	void CContText::SetText(const NSStringUtils::CStringUTF32& oText, const std::vector<double>& arSymWidths)
	{
		m_oText = oText;
		m_arSymWidths.clear();
		m_dWidth = 0;
		for (auto& w : arSymWidths)
		{
			m_arSymWidths.push_back(w);
			m_dWidth += w;
		}
		m_dRight = m_dLeft + m_dWidth;
	}

	void CContText::AddSymBack(uint32_t cSym, double dWidth)
	{
		bool is_space_twice = m_oText.at(m_oText.length() - 1) == c_SPACE_SYM && cSym == c_SPACE_SYM;

		if (is_space_twice)
			m_arSymWidths.back() += dWidth;
		else
		{
			m_arSymWidths.push_back(dWidth);
			m_oText += cSym;
		}
		m_dWidth += dWidth;
		m_dRight = m_dLeft + m_dWidth;

	}
	void CContText::AddSymFront(uint32_t cSym, double dWidth)
	{
		NSStringUtils::CStringUTF32 text;
		text += cSym;
		text += m_oText;
		m_oText = text;
		m_arSymWidths.insert(m_arSymWidths.begin(), dWidth);
	}
	void CContText::SetSym(uint32_t cSym, double dWidth)
	{
		m_oText = L"";
		m_oText += cSym;
		m_arSymWidths.clear();
		m_arSymWidths.push_back(dWidth);
		m_dWidth = dWidth;
		m_dRight = m_dLeft + m_dWidth;
	}
	void CContText::RemoveLastSym()
	{
		m_oText = m_oText.substr(0, m_oText.length() - 1);
		m_dWidth -= m_arSymWidths[m_arSymWidths.size() - 1];
		m_dRight = m_dLeft + m_dWidth;
		m_arSymWidths.resize(m_arSymWidths.size() - 1);
	}

	const NSStringUtils::CStringUTF32& CContText::GetText() const noexcept
	{
		return m_oText;
	}
	const std::vector<double>& CContText::GetSymWidths() const noexcept
	{
		return m_arSymWidths;
	}
	const std::vector<double> CContText::GetSymLefts() const noexcept
	{
		std::vector<double> lefts;
		double left = m_dLeft;
		for (auto& w : m_arSymWidths)
		{
			lefts.push_back(left);
			left += w;
		}
		return lefts;
	}

	bool CContText::CheckFontEffects
		(std::shared_ptr<CContText>& pFirstCont,
		 std::shared_ptr<CContText>& pSecondCont,
		 eVerticalCrossingType eVType,
		 eHorizontalCrossingType eHType)
	{
		//Условие пересечения по вертикали
		bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext; //текущий cont выше
		bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext; //текущий cont ниже

		//Условие пересечения по горизонтали
		bool bIf3 = eHType == eHorizontalCrossingType::hctCurrentLeftOfNext; //текущий cont левее
		bool bIf4 = eHType == eHorizontalCrossingType::hctCurrentRightOfNext; //текущий cont правее

		//Размеры шрифта и текст должны бать одинаковыми
		bool bIf5 = pFirstCont->m_pFontStyle->dFontSize == pSecondCont->m_pFontStyle->dFontSize;
		bool bIf6 = pFirstCont->m_oText == pSecondCont->m_oText;

		//Цвет тени должен быть серым
		bool bIf7 = pFirstCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;
		bool bIf8 = pSecondCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor;

		bool bIf9 = pFirstCont->m_pFontStyle->oBrush.Color1 == c_iBlackColor;
		bool bIf10 = pSecondCont->m_pFontStyle->oBrush.Color1 == c_iBlackColor;

		bool bIf11 = pFirstCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor2;
		bool bIf12 = pSecondCont->m_pFontStyle->oBrush.Color1 == c_iGreyColor2;

		//note Каждый символ с Emboss или Engrave разбиваются на 3 символа с разными цветами
		//note Логика подобрана для конкретного примера - возможно нужно будет ее обобщить.
		//todo существует проблема неправильного определением FontEffects с физически пересекаемыми строчками - файл generaltest.pdf p.14
		if (bIf5 && bIf6)
		{
			if (bIf12 && pFirstCont->m_bIsEmbossPresent)
				if (bIf1 && bIf3)
				{
					pFirstCont->m_bIsEmbossPresent = true;
					pSecondCont = nullptr;
					return true;
				}

			if (bIf10 && pFirstCont->m_bIsEngravePresent)
				if (bIf1 && bIf3)
				{
					pFirstCont->m_bIsEngravePresent = true;
					pSecondCont = nullptr;
					return true;
				}

			//Shadow
			if (bIf1 && bIf3 && bIf8)
			{
				pFirstCont->m_bIsShadowPresent = true;
				pSecondCont = nullptr;
				return true;
			}
			else if (bIf2 && bIf4 && bIf7)
			{
				pSecondCont->m_bIsShadowPresent = true;
				pFirstCont = nullptr;
				return true;
			}

			//Emboss
			//Первый проход
			//c_iBlackColor -> c_iBlackColor -> c_iGreyColor2
			else if (bIf1 && bIf3 && bIf9)
			{
				pSecondCont->m_bIsEmbossPresent = true;
				pFirstCont = nullptr;
				return true;
			}
			//Engrave
			else if (bIf1 && bIf3 && bIf11)
			{
				pSecondCont->m_bIsEngravePresent = true;
				pFirstCont = nullptr;
				return true;
			}
		}
		return false;
	}

	bool CContText::CheckVertAlignTypeBetweenConts
		(std::shared_ptr<CContText> pFirstCont,
		 std::shared_ptr<CContText> pSecondCont,
		 eVerticalCrossingType eVType,
		 eHorizontalCrossingType eHType)
	{

		bool bIf1 = eVType == eVerticalCrossingType::vctCurrentAboveNext ||
					eVType == eVerticalCrossingType::vctCurrentInsideNext;

		bool bIf2 = eVType == eVerticalCrossingType::vctCurrentBelowNext;

		bool bIf3 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
					 eHType == eHorizontalCrossingType::hctCurrentLeftOfNext) &&
					fabs(pFirstCont->m_dRight - pSecondCont->m_dLeft) < c_dTHE_STRING_X_PRECISION_MM * 3;

		bool bIf4 = (eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext ||
					 eHType == eHorizontalCrossingType::hctCurrentRightOfNext) &&
					fabs(pFirstCont->m_dLeft - pSecondCont->m_dRight) < c_dTHE_STRING_X_PRECISION_MM * 3;

		//Размеры шрифта должны бать разными
		bool bIf5 = pFirstCont->m_pFontStyle->dFontSize * 0.7 > pSecondCont->m_pFontStyle->dFontSize;
		bool bIf6 = pFirstCont->m_pFontStyle->dFontSize <  pSecondCont->m_pFontStyle->dFontSize * 0.7;

		if (bIf3 || bIf4)
		{
			if (bIf1 && bIf5)
			{
				pSecondCont->m_eVertAlignType = eVertAlignType::vatSubscript;
				pSecondCont->m_pCont = pFirstCont;
				pFirstCont->m_eVertAlignType = eVertAlignType::vatBase;
				pFirstCont->m_pCont = pSecondCont;
				pFirstCont->m_bPossibleSplit = false;
				return true;
			}
			else if (bIf2 && bIf5)
			{
				pSecondCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
				pSecondCont->m_pCont = pFirstCont;
				pFirstCont->m_eVertAlignType = eVertAlignType::vatBase;
				pFirstCont->m_pCont = pSecondCont;
				pFirstCont->m_bPossibleSplit = false;
				return true;
			}
			else if (bIf1 && bIf6)
			{
				pFirstCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
				pFirstCont->m_pCont = pSecondCont;
				pSecondCont->m_eVertAlignType = eVertAlignType::vatBase;
				pSecondCont->m_pCont = pFirstCont;
				pSecondCont->m_bPossibleSplit = false;
				return true;
			}
			else if (bIf2 && bIf6)
			{
				pFirstCont->m_eVertAlignType = eVertAlignType::vatSubscript;
				pFirstCont->m_pCont = pSecondCont;
				pSecondCont->m_eVertAlignType = eVertAlignType::vatBase;
				pSecondCont->m_pCont = pFirstCont;
				pSecondCont->m_bPossibleSplit = false;
				return true;
			}
		}
		return false;
	}

	double CContText::CalculateSpace() const noexcept
	{
		return m_dSpaceWidthMM;
	}
}
