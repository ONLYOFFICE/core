#include "ContText.h"

#include "../../resources/ColorTable.h"
#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
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
		m_bPossibleHorSplit = rCont.m_bPossibleHorSplit;
		m_bWriteStyleRaw = rCont.m_bWriteStyleRaw;

		m_nOriginFontFaceIndex = rCont.m_nOriginFontFaceIndex;
		m_wsOriginFontName = rCont.m_nOriginFontFaceIndex;

		m_arSymWidths.clear();
		m_arSymWidths.resize(rCont.m_arSymWidths.size());
		for (size_t i = 0; i < rCont.m_arSymWidths.size(); ++i)
			m_arSymWidths[i] = rCont.m_arSymWidths[i];

		m_arGids.clear();
		m_arGids.resize(rCont.m_arGids.size());
		for (size_t i = 0; i < rCont.m_arGids.size(); ++i)
			m_arGids[i] = rCont.m_arGids[i];

		m_arOriginLefts.clear();
		m_arOriginLefts.resize(rCont.m_arOriginLefts.size());
		for (size_t i = 0; i < rCont.m_arOriginLefts.size(); ++i)
			m_arOriginLefts[i] = rCont.m_arOriginLefts[i];

		m_bFontSubstitution = rCont.m_bFontSubstitution;

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

			if (m_oText.ToStdWString() == L" ")
				dBoxWidth = m_pManager->GetSpaceWidthMM();
			else
			{
				m_pManager->SetStringGid(0);
				m_pManager->MeasureString(m_oText.ToStdWString(), 0, 0, dBoxX, dBoxY, dBoxWidth, dBoxHeight, CFontManager::mtPosition);
			}

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
		cont->m_arOriginLefts.clear();
		cont->m_arGids.clear();
		for (size_t i = index + 1; i < len; ++i)
		{
			cont->m_arSymWidths.push_back(m_arSymWidths[i]);
			cont->m_arOriginLefts.push_back(m_arOriginLefts[i]);

			if (!m_arGids.empty() && cont->m_bCollectMetaInfo)
				cont->m_arGids.push_back(m_arGids[i]);
		}

		m_oText = m_oText.substr(0, index + 1);
		m_dRight = cont->m_dLeft;
		m_dWidth = m_dRight - m_dLeft;

		m_arSymWidths.resize(index + 1);
		m_arOriginLefts.resize(index + 1);

		if (!m_arGids.empty())
			m_arGids.resize(index + 1);

		m_bPossibleHorSplit = false;

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

		else if (this_top == other_top && this_bot == other_bot)
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

		if (m_bIsRtl)
			oWriter.WriteString(L"<w:rtl/> ");

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
			oWriter.WriteString(L"\" w:eastAsia=\"");
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

		if (m_bIsRtl)
			oWriter.WriteString(L"<w:rtl/> ");

		oWriter.WriteString(L"</a:rPr>");
		oWriter.WriteString(L"<a:t>");
		oWriter.WriteEncodeXmlString(m_oText.ToStdWString());
		oWriter.WriteString(L"</a:t>");
		if (m_bIsAddBrEnd) oWriter.WriteString(L"<a:br/>");

		std::wstring origin_gids{};
		for (auto& gid : m_arGids)
			origin_gids += std::to_wstring(gid) + L";";

		// Origin width string
		std::wstring origin_lefts{};
		for (auto& l : m_arOriginLefts)
			origin_lefts += std::to_wstring(static_cast<int>(l * c_dMMToEMU)) + L";";

		// meta info for pdf-editor
		oWriter.WriteString(L"<metaorigin:pos");
		oWriter.WriteString(L" x=\"");
		oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToEMU));
		oWriter.WriteString(L"\" y=\"");
		oWriter.AddInt(static_cast<int>(m_dBot * c_dMMToEMU));
		oWriter.WriteString(L"\" />");

		oWriter.WriteString(L"<metaorigin:font ");
		oWriter.WriteString(L" name=\"");
		oWriter.WriteString(m_wsOriginFontName);
		oWriter.WriteString(L"\" faceindex=\"");
		oWriter.AddInt(m_nOriginFontFaceIndex);
		oWriter.WriteString(L"\" />");

		oWriter.WriteString(L"<metaorigin:syminfo");
		oWriter.WriteString(L" lefts=\"");
		oWriter.WriteString(origin_lefts);
		oWriter.WriteString(L"\" gids=\"");
		oWriter.WriteString(origin_gids);
		oWriter.WriteString(L"\" />");

		oWriter.WriteString(L"<metaorigin:fontsubstitution");
		oWriter.WriteString(L" value=\"");
		oWriter.AddInt(m_bFontSubstitution);
		oWriter.WriteString(L"\" />");

		oWriter.WriteString(L"</a:r>");
	}
	void CContText::ToBin(NSWasm::CData& oWriter) const
	{
		int lCalculatedSpacing = 0;
		if (!m_oText.empty())
		{
			double dSpacing = (m_dWidth - m_oSelectedSizes.dWidth) / (m_oText.length());
			dSpacing *= c_dMMToPt * 100;
			lCalculatedSpacing = static_cast<LONG>(dSpacing);
		}
		lCalculatedSpacing -= 15;

		const BYTE kPARRUN_TYPE_RUN = 1;
		oWriter.StartRecord(kPARRUN_TYPE_RUN);

		oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
		oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(m_oText.ToStdWString());
		oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

		// Meta-info
		oWriter.StartRecord(111);
		oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
		oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(m_wsOriginFontName); // Origin font name
		oWriter.WriteBYTE(1); oWriter.AddInt(m_nOriginFontFaceIndex); // Origin face index
		oWriter.WriteBYTE(2); oWriter.AddSInt(static_cast<int>(m_dLeft * c_dMMToEMU)); // Origin left
		oWriter.WriteBYTE(3); oWriter.AddSInt(static_cast<int>(m_dBot * c_dMMToEMU)); // Origin bot

		std::wstring origin_gids{};
		for (auto& gid : m_arGids)
			origin_gids += std::to_wstring(gid) + L";";

		oWriter.WriteBYTE(4); oWriter.WriteStringUtf16(origin_gids); // Origin gids string

		// Origin width string
		std::wstring origin_lefts{};
		for (auto& l : m_arOriginLefts)
			origin_lefts += std::to_wstring(static_cast<int>(l * c_dMMToEMU)) + L";";

		oWriter.WriteBYTE(5); oWriter.WriteStringUtf16(origin_lefts); // Origin lefts
		oWriter.WriteBYTE(6); oWriter.WriteBool(m_bFontSubstitution); // Font Substitution (just pass from pdf)

		oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
		oWriter.EndRecord();

		// WriteRecord WriteRunProperties
		[&oWriter, this, lCalculatedSpacing] () {
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);

			oWriter.WriteBYTE(1); oWriter.WriteBool(m_pFontStyle->bBold);
			oWriter.WriteBYTE(7); oWriter.WriteBool(m_pFontStyle->bItalic);

			BYTE strike = 1;
			if (m_bIsDoubleStrikeout) strike = 0;
			else if (m_bIsStrikeoutPresent) strike = 2;
			oWriter.WriteBYTE(16); oWriter.WriteBYTE(strike);
			oWriter.WriteBYTE(15); oWriter.AddSInt(lCalculatedSpacing);
			oWriter.WriteBYTE(18); oWriter.WriteBYTE(m_bIsUnderlinePresent ? 13 : 12);
			unsigned int font_size = static_cast<unsigned int>(m_pFontStyle->dFontSize) * 100;
			const unsigned int min_font_size = 100;
			oWriter.WriteBYTE(17); oWriter.AddInt(std::max(font_size, std::max(font_size, min_font_size)));

			oWriter.WriteBYTE(2);
			if (m_eVertAlignType == eVertAlignType::vatSubscript)
				oWriter.AddSInt(-25000);
			else if (m_eVertAlignType == eVertAlignType::vatSuperscript)
				oWriter.AddSInt(30000);
			else
				oWriter.AddInt(0);

			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

			// WriteTextFontTypeface
			auto WriteTextFontTypeface = [this, &oWriter] (const std::wstring& wsTypeface) {
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(3); oWriter.WriteStringUtf16(wsTypeface);
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			};

			// WriteRecord WriteTextFontTypeface
			oWriter.StartRecord(3); WriteTextFontTypeface(m_pFontStyle->wsFontName); oWriter.EndRecord();
			oWriter.StartRecord(4); WriteTextFontTypeface(m_pFontStyle->wsFontName); oWriter.EndRecord();
			oWriter.StartRecord(5); WriteTextFontTypeface(m_pFontStyle->wsFontName); oWriter.EndRecord();

			// WriteUniColor
			auto WriteUniColor = [&oWriter] (long color, long alpha) {
				BYTE r = reinterpret_cast<BYTE*>(&color)[0];
				BYTE g = reinterpret_cast<BYTE*>(&color)[1];
				BYTE b = reinterpret_cast<BYTE*>(&color)[2];

				oWriter.StartRecord(1); // COLOR_TYPE_SRGB
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(0); oWriter.WriteBYTE(r);
				oWriter.WriteBYTE(1); oWriter.WriteBYTE(g);
				oWriter.WriteBYTE(2); oWriter.WriteBYTE(b);
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				// WriteMods (alpha)
				oWriter.StartRecord(0);
				oWriter.AddInt(1);
				oWriter.StartRecord(1);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(L"alpha");
				oWriter.WriteBYTE(1); oWriter.AddInt(alpha * 100000 / 255);
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.EndRecord();
				oWriter.EndRecord();
				oWriter.EndRecord();
			};

			auto WriteUniFill = [&oWriter, this, &WriteUniColor] () {
				oWriter.StartRecord(3); // FILL_TYPE_SOLID
				oWriter.StartRecord(0);
				WriteUniColor(m_pFontStyle->oBrush.Color1, m_pFontStyle->oBrush.Alpha1);
				oWriter.EndRecord();
				oWriter.EndRecord();
			};

			// WriteRecord WriteUniFill
			oWriter.StartRecord(1);
			WriteUniFill();
			oWriter.EndRecord();

			// WriteRecord WriteHighlightColor
			if (m_bIsHighlightPresent)
			{
				oWriter.StartRecord(12);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.StartRecord(0);
				WriteUniColor(m_lHighlightColor, 255);
				oWriter.EndRecord();
				oWriter.EndRecord();
			}
			oWriter.EndRecord();
		}();
		oWriter.EndRecord();
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

	bool CContText::IsDuplicate(CContText* pCont, eVerticalCrossingType eVType, eHorizontalCrossingType eHType) const noexcept
	{
		return m_oText == pCont->m_oText &&
		        eVType == eVerticalCrossingType::vctDublicate &&
		        (eHType == eHorizontalCrossingType::hctDublicate ||
		         eHType == eHorizontalCrossingType::hctCurrentLeftOfNext ||
		         eHType == eHorizontalCrossingType::hctCurrentRightOfNext ||
		         eHType == eHorizontalCrossingType::hctLeftAndRightBordersMatch);
	}

	bool CContText::IsOnlySpaces() const
	{
		bool only_spaces = true;
		for (size_t j = 0; j < m_oText.length(); ++j)
			if (!CContText::IsUnicodeSpace(m_oText.at(j)))
			{
				only_spaces = false;
				break;
			}
		return only_spaces;
	}
	bool CContText::IsDiacritical() const noexcept
	{
		const auto& text = GetText();
		return text.length() == 1 && CContText::IsUnicodeDiacriticalMark(text.at(0));
	}

	void CContText::AddTextBack(const NSStringUtils::CStringUTF32& oText,
	                             const std::vector<double>& arSymWidths,
	                             const std::vector<unsigned int>& arGids,
	                             const std::vector<double>& arOriginLefts)
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
			m_arOriginLefts.push_back(arOriginLefts[i]);

			if (!arGids.empty() && m_bCollectMetaInfo)
				m_arGids.push_back(arGids[i]);
		}
		m_dRight = m_dLeft + m_dWidth;
	}
	void CContText::AddTextFront(const NSStringUtils::CStringUTF32& oText,
	                             const std::vector<double>& arSymWidths,
	                             const std::vector<unsigned int>& arGids,
	                             const std::vector<double>& arOriginLefts)
	{
		m_oText = oText + m_oText;

		double addtitional_width = 0;
		for (auto& w : arSymWidths)
			addtitional_width += w;

		auto ar_sym_w = m_arSymWidths;
		m_arSymWidths = arSymWidths;
		for (auto& w : ar_sym_w)
			m_arSymWidths.push_back(w);

		m_dWidth += addtitional_width;
		m_dLeft = m_dRight - m_dWidth;

		if (!arGids.empty() && m_bCollectMetaInfo)
		{
			auto ar_gids = m_arGids;
			m_arGids = arGids;
			for (auto& gid : ar_gids)
				m_arGids.push_back(gid);
		}

		auto ar_lefts = m_arOriginLefts;
		m_arOriginLefts = arOriginLefts;
		for (auto& left : ar_lefts)
			m_arOriginLefts.push_back(left);
	}
	void CContText::SetText(const NSStringUtils::CStringUTF32& oText,
	                        const std::vector<double>& arSymWidths,
	                        std::vector<unsigned int>&& arGids,
	                        std::vector<double>&& arOriginLefts)
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
		m_arOriginLefts = std::move(arOriginLefts);

		if (m_bCollectMetaInfo)
			m_arGids = std::move(arGids);
	}

	void CContText::AddSymBack(uint32_t cSym, double dWidth, double dLeft, unsigned int nGid)
	{
		bool is_space_twice = m_oText.at(m_oText.length() - 1) == c_SPACE_SYM && cSym == c_SPACE_SYM;

		if (is_space_twice)
		{
			m_arSymWidths.back() += dWidth;
		}
		else
		{
			m_arSymWidths.push_back(dWidth);
			m_oText += cSym;
			m_arOriginLefts.push_back(dLeft);

			if (m_bCollectMetaInfo)
			{
				m_arGids.push_back(nGid);
			}
		}
		m_dWidth += dWidth;
		m_dRight = m_dLeft + m_dWidth;
	}
	void CContText::AddSymFront(uint32_t cSym, double dWidth, double dLeft, unsigned int nGid)
	{
		NSStringUtils::CStringUTF32 text;
		text += cSym;
		text += m_oText;
		m_oText = text;

		m_dLeft -= dWidth;
		m_dWidth = m_dRight - m_dLeft;

		m_arSymWidths.insert(m_arSymWidths.begin(), dWidth);
		m_arOriginLefts.insert(m_arOriginLefts.begin(), dLeft);
		if (m_bCollectMetaInfo)
		{
			m_arGids.insert(m_arGids.begin(), nGid);
		}
	}
	void CContText::SetSym(uint32_t cSym, double dWidth, double dLeft, unsigned int nGid)
	{
		m_oText = L"";
		m_oText += cSym;
		m_arSymWidths.clear();
		m_arSymWidths.push_back(dWidth);
		m_dWidth = dWidth;
		m_dRight = m_dLeft + m_dWidth;

		m_arSymWidths.clear();
		m_arSymWidths.push_back(dWidth);

		m_arOriginLefts.clear();
		m_arOriginLefts.push_back(dLeft);

		if (m_bCollectMetaInfo)
		{
			m_arGids.clear();
			m_arGids.push_back(nGid);
		}
	}
	void CContText::RemoveLastSym()
	{
		m_oText = m_oText.substr(0, m_oText.length() - 1);
		m_dWidth -= m_arSymWidths[m_arSymWidths.size() - 1];
		m_dRight = m_dLeft + m_dWidth;
		m_arSymWidths.resize(m_arSymWidths.size() - 1);
		m_arOriginLefts.resize(m_arOriginLefts.size() - 1);

		if (!m_arGids.empty() && m_bCollectMetaInfo)
			m_arGids.resize(m_arGids.size() - 1);
	}
	uint32_t CContText::GetLastSym() const
	{
		return m_oText.at(m_oText.length() - 1);
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
		return m_arOriginLefts;
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
        bool bIf5 = pFirstCont->m_pFontStyle->dFontSize * 0.8 > pSecondCont->m_pFontStyle->dFontSize;
        bool bIf6 = pFirstCont->m_pFontStyle->dFontSize <  pSecondCont->m_pFontStyle->dFontSize * 0.8;

		if (bIf3 || bIf4)
		{
			if (bIf1 && bIf5)
			{
				pSecondCont->m_eVertAlignType = eVertAlignType::vatSubscript;
				pSecondCont->m_pCont = pFirstCont;
				pFirstCont->m_eVertAlignType = eVertAlignType::vatBase;
				pFirstCont->m_pCont = pSecondCont;
				pFirstCont->m_bPossibleHorSplit = false;
				return true;
			}
			else if (bIf2 && bIf5)
			{
				pSecondCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
				pSecondCont->m_pCont = pFirstCont;
				pFirstCont->m_eVertAlignType = eVertAlignType::vatBase;
				pFirstCont->m_pCont = pSecondCont;
				pFirstCont->m_bPossibleHorSplit = false;
				return true;
			}
			else if (bIf1 && bIf6)
			{
				pFirstCont->m_eVertAlignType = eVertAlignType::vatSuperscript;
				pFirstCont->m_pCont = pSecondCont;
				pSecondCont->m_eVertAlignType = eVertAlignType::vatBase;
				pSecondCont->m_pCont = pFirstCont;
				pSecondCont->m_bPossibleHorSplit = false;
				return true;
			}
			else if (bIf2 && bIf6)
			{
				pFirstCont->m_eVertAlignType = eVertAlignType::vatSubscript;
				pFirstCont->m_pCont = pSecondCont;
				pSecondCont->m_eVertAlignType = eVertAlignType::vatBase;
				pSecondCont->m_pCont = pFirstCont;
				pSecondCont->m_bPossibleHorSplit = false;
				return true;
			}
		}
		return false;
	}
	bool CContText::IsUnicodeRtl(uint32_t cSym)
	{
		bool is_herbew_arabic = (cSym >= 0x0590 && cSym <= 0x08FF);

		// alphabetic presentation forms
		bool is_apf = (cSym >= 0xFB00 && cSym <= 0xFDFF);
		bool is_apf_b = (cSym >= 0xFE70 && cSym <= 0xFEFF);

		bool is_other = (cSym >= 0x10800 && cSym <= 0x108AF);

		// more https://www.unicode.org/Public/UNIDATA/extracted/DerivedBidiClass.txt
		return is_herbew_arabic || is_apf || is_apf_b || is_other;
	}
	bool CContText::IsUnicodeBullet(uint32_t cSym)
	{
		// more symbols of delims
		bool is_bullet =
		        (cSym == 0x2022) || (cSym == 0x2023) || (cSym == 0x2043) || (cSym == 0x204C) ||
		        (cSym == 0x204D) || (cSym == 0x2219) || (cSym == 0x25CB) || (cSym == 0x25CF) ||
		        (cSym == 0x25D8) || (cSym == 0x25E6) || (cSym == 0x2619) || (cSym == 0x2765) ||
		        (cSym == 0x2767) || (cSym == 0x29BE) || (cSym == 0x29BF) || (cSym == 0x25C9);

		bool is_another =
		        (cSym == 0xB7) || (cSym == 0xA7) || (cSym == 0xF076) || (cSym == 0x2013) ||
		        (cSym == 0x2713) || (cSym == 0x2714) || (cSym == 0x2756) || (cSym == 0x25C6) ||
		        (cSym == 0x25C7) || (cSym == 0x25C8);

		return is_bullet || is_another;
	}
	bool CContText::IsUnicodeEnumEnd(uint32_t cSym)
	{
		return cSym == 0x29 || cSym == 0x2e;
	}
	bool CContText::IsUnicodeNumber(uint32_t cSym)
	{
		return cSym >= 0x30 && cSym <= 0x39;
	}
	bool CContText::IsUnicodeSpace(uint32_t cSym)
	{
		return (0x20 == cSym || 0xA0 == cSym || 0x2003 == cSym);
	}

	bool CContText::IsUnicodeSymbol(uint32_t cSym )
	{
		bool is_unicode =
		        (( 0x0009 == cSym) || (0x000A == cSym ) || (0x000D == cSym ) ||
		         (( 0x0020 <= cSym) && (0xD7FF >= cSym )) || ((0xE000 <= cSym) && (cSym <= 0xFFFD )) ||
		         (( 0x10000 <= cSym) && cSym));

		return is_unicode;
	}
	bool CContText::IsUnicodeDiacriticalMark(uint32_t cSym)
	{
		return 0x0300 <= cSym && 0x036F >= cSym;
	}

	double CContText::CalculateSpace() const noexcept
	{
		return m_dSpaceWidthMM;
	}

	CContTextBuilder::CContTextBuilder(CFontStyleManager* pFontStyleManager, CFontSelector* pFontSelector) :
	    m_pFontStyleManager(pFontStyleManager), m_pFontSelector(pFontSelector)
	{}

	std::vector<CContTextBuilder::cont_ptr_t> CContTextBuilder::GetConts()
	{
		return std::move(m_arConts);
		m_pCurrCont = nullptr;
	}
	std::vector<CContTextBuilder::cont_ptr_t> CContTextBuilder::GetDiacs()
	{
		return std::move(m_arDiacs);
		m_pCurrCont = nullptr;
	}

	void CContTextBuilder::AddUnicode(
	        double dTop,
	        double dBot,
	        double dLeft,
	        double dRight,
	        const NSStructures::CFont& oFont,
	        const NSStructures::CBrush& oBrush,
	        CFontManager* pFontManager,
	        const NSStringUtils::CStringUTF32& oText,
	        const PUINT pGids,
	        bool bForcedBold,
	        bool bUseDefaultFont,
	        bool bWriteStyleRaw,
	        bool bCollectMetaInfo,
	        bool bFontSubstitution)
	{
		double dWidth = dRight - dLeft;
		double dHeight = dBot - dTop;

		std::vector<unsigned int> gids;
		if (bCollectMetaInfo)
		{
			for (size_t i = 0; i < oText.length(); ++i)
				if (pGids)
					gids.push_back(pGids[i]);
				else
					gids.push_back(0);
		}

		std::vector<double> origin_lefts;
		double curr_origin_left = dLeft;
		for (size_t i = 0; i < oText.length(); ++i)
		{
			origin_lefts.push_back(curr_origin_left);
			curr_origin_left += dWidth / oText.length();
		}

		// if new text is close to current cont
		if (m_pCurrCont != nullptr &&
		        fabs(m_pCurrCont->m_dBot - dBot) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
		        m_oPrevFont.IsEqual2(&oFont) &&
		        m_oPrevBrush.IsEqual(&oBrush) && !(
		            oText.length() == 1 && CContText::IsUnicodeDiacriticalMark(oText.at(0))) &&
		        bFontSubstitution == m_pCurrCont->m_bFontSubstitution)
		{

			double avg_width = dWidth / oText.length();
			for (size_t i = 0; i < oText.length(); ++i)
				if (oText.at(i) == c_SPACE_SYM)
					m_pCurrCont->m_pFontStyle->UpdateAvgSpaceWidth(avg_width);

			double avg_space_width = m_pCurrCont->m_pFontStyle->GetAvgSpaceWidth();
			double space_width =
			        avg_space_width != 0.0 ?
			            avg_space_width * c_dAVERAGE_SPACE_WIDTH_COEF :
			            m_pCurrCont->CalculateSpace() * c_dSPACE_WIDTH_COEF;

			bool is_added = false;

			// some_text+more_text
			if (fabs(m_pCurrCont->m_dRight - dLeft) < space_width && dRight > m_pCurrCont->m_dRight)
			{
				double left_avg_width = (dRight - m_pCurrCont->m_dRight) / oText.length();
				std::vector<double> ar_widths;
				for (size_t i = 0; i < oText.length(); ++i)
					ar_widths.push_back(left_avg_width);

				m_pCurrCont->AddTextBack(oText, ar_widths, gids, origin_lefts);
				is_added = true;

			}
			// more_text+some_text
			else if (fabs(m_pCurrCont->m_dLeft - dRight) < space_width && dLeft < m_pCurrCont->m_dLeft)
			{
				double right_avg_width = (m_pCurrCont->m_dLeft - dLeft) / oText.length();
				std::vector<double> ar_widths;
				for (size_t i = 0; i < oText.length(); ++i)
					ar_widths.push_back(right_avg_width);

				m_pCurrCont->AddTextFront(oText, ar_widths, gids, origin_lefts);
				is_added = true;
			}

			if (is_added)
			{
				m_pCurrCont->m_dTop = std::min(m_pCurrCont->m_dTop, dTop);
				m_pCurrCont->m_dBot = std::max(m_pCurrCont->m_dBot, dBot);
				m_pCurrCont->m_dHeight = m_pCurrCont->m_dBot - m_pCurrCont->m_dTop;
				m_pCurrCont->m_dWidth = m_pCurrCont->m_dRight - m_pCurrCont->m_dLeft;
				return;
			}
		}

		auto pCont = std::make_shared<CContText>(pFontManager);
		const auto& oParams = pFontManager->GetFontSelectParams();
		const auto& oMetrics = pFontManager->GetFontMetrics();
		m_pFontSelector->SelectFont(oParams, oMetrics, oText);

		pCont->m_dBot    = dBot;
		pCont->m_dTop    = dTop;
		pCont->m_dHeight = dHeight;
		pCont->m_dLeft   = dLeft;

		// первичное получение стиля для текущего символа
		// при дальнейшем анализе может измениться
		pCont->m_pFontStyle = m_pFontStyleManager->GetOrAddFontStyle(
		            oBrush,
		            m_pFontSelector->GetSelectedName(),
		            oFont.Size,
		            m_pFontSelector->IsSelectedItalic(),
		            m_pFontSelector->IsSelectedBold() || bForcedBold);

		// just in case if oText contains more than 1 symbol
		std::vector<double> ar_widths;
		double avg_width = abs(dRight - dLeft) / oText.length();
		for (size_t i = 0; i < oText.length(); ++i)
		{
			if (oText.at(i) == c_SPACE_SYM) pCont->m_pFontStyle->UpdateAvgSpaceWidth(avg_width);
			ar_widths.push_back(avg_width);
		}

		pCont->m_bCollectMetaInfo = bCollectMetaInfo;
		pCont->SetText(oText, ar_widths, std::move(gids), std::move(origin_lefts));
		pCont->m_bIsRtl = CContText::IsUnicodeRtl(oText.at(0));

		pCont->m_dWidth = dWidth;
		pCont->m_dRight = dRight;

		double font_size = oFont.Size;
		double em_height = oMetrics.dEmHeight;
		double ratio = font_size / em_height * c_dPtToMM;

		pCont->m_dTopWithAscent = pCont->m_dBot - (oMetrics.dAscent * ratio);
		pCont->m_dBotWithDescent = pCont->m_dBot + (oMetrics.dDescent * ratio);
		pCont->m_dSpaceWidthMM = pFontManager->GetSpaceWidthMM();

		pCont->m_wsOriginFontName = oFont.Name;
		pCont->m_nOriginFontFaceIndex = oFont.FaceIndex;

		if (bUseDefaultFont)
		{
			pCont->m_oSelectedFont.Name = oFont.Name;
			pCont->m_oSelectedFont.Path = oFont.Path;
			pCont->m_oSelectedFont.Size = oFont.Size;
			pCont->m_oSelectedFont.FaceIndex = oFont.FaceIndex;
		}
		else
		{
			pCont->m_oSelectedFont.Name = m_pFontSelector->GetSelectedName();
			pCont->m_oSelectedFont.Size = oFont.Size;
			pCont->m_oSelectedFont.Bold = m_pFontSelector->IsSelectedBold();
			pCont->m_oSelectedFont.Italic = m_pFontSelector->IsSelectedItalic();
		}
		pCont->m_bWriteStyleRaw = bWriteStyleRaw;
		pCont->m_bFontSubstitution = bFontSubstitution;

		if (pCont->IsDiacritical())
		{
			m_arDiacs.push_back(std::move(pCont));
		}
        else
        {
            m_arConts.push_back(pCont);
        }

		m_pCurrCont = pCont;
		m_oPrevFont = oFont;
		m_oPrevBrush = oBrush;
	}

	void CContTextBuilder::NullCurrCont()
	{
		m_pCurrCont = nullptr;
	}
	void CContTextBuilder::Clear()
	{
		m_pCurrCont = nullptr;
		m_arConts.clear();
		m_arDiacs.clear();
		m_oPrevFont.SetDefaultParams();
		m_oPrevBrush.SetDefaultParams();
	}
}
