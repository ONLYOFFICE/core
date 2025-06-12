#include "StyleConverter.h"
#include "../Common/WriterContext.h"
#include "Transform.h"
#include <cmath>

namespace HWP
{
#define DEFAULT_FONT_FAMILY std::wstring(L"Arial")
#define DEFAULT_FONT_SIZE 18
#define DEFAULT_LANGUAGE std::wstring(L"en")
#define DEFAULT_STYLE_NAME L"Style"
#define DEFAULT_SPACING 240

#define SPACING_SCALE_MS_WORD 1.21

#define ADD_COLOR(r, g, b, enum_value) {{r, g, b}, enum_value}

static const std::vector<std::pair<TColor, EHighlightColors>> s_arHighlightColors
{
	ADD_COLOR(  0,   0,   0, EHighlightColors::Black),
	ADD_COLOR(  0,   0, 255, EHighlightColors::Blue),
	ADD_COLOR(  0, 255, 255, EHighlightColors::Cyan),
	ADD_COLOR(  0, 255,   0, EHighlightColors::Green),
	ADD_COLOR(255,   0, 255, EHighlightColors::Magenta),
	ADD_COLOR(255,   0,   0, EHighlightColors::Red),
	ADD_COLOR(255, 255,   0, EHighlightColors::Yellow),
	ADD_COLOR(255, 255, 255, EHighlightColors::White),

	ADD_COLOR(  0,   0, 139, EHighlightColors::DarkBlue),
	ADD_COLOR(  0, 139, 139, EHighlightColors::DarkCyan),
	ADD_COLOR(  0, 100,   0, EHighlightColors::DarkGreen),
	ADD_COLOR(139,   0, 139, EHighlightColors::DarkMagenta),
	ADD_COLOR(139,   0,   0, EHighlightColors::DarkRed),
	ADD_COLOR(128, 128,   0, EHighlightColors::DarkYellow),
	ADD_COLOR(169, 169, 169, EHighlightColors::DarkGray),

	ADD_COLOR(211, 211, 211, EHighlightColors::LightGray)
};

CStyleConverter::CStyleConverter()
	: m_ushLastParaShapeId(0), m_ushLastCharShapeId(0)
{
	m_mUsedStyleNames.insert(std::make_pair(DEFAULT_STYLE_NAME, 0));
}

std::wstring CStyleConverter::CreateStyle(short shParaShapeId, short shParaStyleId, CWriterContext& oContext, TConversionState& oState)
{
	std::map<unsigned short, std::wstring>::const_iterator itFound = m_mUsedStyles.find(shParaStyleId);

	if (m_mUsedStyles.cend() != itFound)
		return itFound->second;

	const CHWPRecordStyle *pParaStyle = dynamic_cast<const CHWPRecordStyle*>(oContext.GetParaStyle(shParaStyleId));

	if (nullptr == pParaStyle)
		return std::wstring();

	const CHWPRecordParaShape* pParaShapeRecord = dynamic_cast<const CHWPRecordParaShape*>(oContext.GetParaShape(pParaStyle->GetParaShapeId()));

	if (nullptr == pParaShapeRecord)
		return  std::wstring();

	std::wstring wsParaStyleName = !pParaStyle->GetEngName().empty() ? pParaStyle->GetEngName() : pParaStyle->GetName();

	if (wsParaStyleName.empty())
		wsParaStyleName = DEFAULT_STYLE_NAME;

	const std::wstring wsStyleName = GenerateUniqueID(wsParaStyleName);

	m_oStylesXml.WriteString(L"<w:style w:type=\"paragraph\" w:customStyle=\"1\" w:styleId=\"");
	m_oStylesXml.WriteEncodeXmlString(wsStyleName);
	m_oStylesXml.WriteString(L"\">");
	m_oStylesXml.WriteString(L"<w:name w:val=\"");
	m_oStylesXml.WriteEncodeXmlString(wsStyleName);
	m_oStylesXml.WriteString(L"\"/>");
	m_oStylesXml.WriteString(L"<w:qFormat/>");
	m_oStylesXml.WriteString(L"<w:pPr>");
	WriteParagraphProperties(GenerateParagraphStyle(*pParaShapeRecord), m_oStylesXml);
	m_oStylesXml.WriteString(L"</w:pPr>");

	const CHWPRecordCharShape *pCharShapeRecord = dynamic_cast<const CHWPRecordCharShape*>(oContext.GetCharShape(pParaStyle->GetCharShapeId()));

	if (nullptr != pCharShapeRecord)
	{
		m_oStylesXml.WriteString(L"<w:rPr>");
		WriteRunnerProperties(GenerateRunnerStyle(*pCharShapeRecord), m_oStylesXml);
		m_oStylesXml.WriteString(L"</w:rPr>");
	}

	m_oStylesXml.WriteString(L"</w:style>");

	m_mUsedStyles.insert(std::make_pair(shParaStyleId, wsStyleName));

	m_ushLastParaShapeId = pParaStyle->GetParaShapeId();
	m_ushLastCharShapeId = pParaStyle->GetCharShapeId();

	return wsStyleName;
}

bool CStyleConverter::WriteDifferenceParagraphStyles(short shFirtsParaShapeId, short shSecondParaShapeId, CWriterContext& oContext, NSStringUtils::CStringBuilder& oBuilder)
{
	const CHWPRecordParaShape* pFirstParaShape = dynamic_cast<const CHWPRecordParaShape*>(oContext.GetParaShape(shFirtsParaShapeId));

	if (nullptr == pFirstParaShape)
		return false;

	const CHWPRecordParaShape* pSecondParaShape = dynamic_cast<const CHWPRecordParaShape*>(oContext.GetParaShape(shSecondParaShapeId));

	if (nullptr == pSecondParaShape)
		return false;

	CParagraphsStyle oFirstParagraphStyle {GenerateParagraphStyle(*pFirstParaShape )};
	CParagraphsStyle oSecondParagraphStyle{GenerateParagraphStyle(*pSecondParaShape)};

	oSecondParagraphStyle -= oFirstParagraphStyle;

	WriteParagraphProperties(oSecondParagraphStyle, oBuilder);

	return true;
}

bool CStyleConverter::WriteDifferenceRunnerStyles(short shFirtsCharShapeId, short shSecondCharShapeId, CWriterContext& oContext, NSStringUtils::CStringBuilder& oBuilder)
{
	const CHWPRecordCharShape* pFirstCharShape = dynamic_cast<const CHWPRecordCharShape*>(oContext.GetCharShape(shFirtsCharShapeId));

	if (nullptr == pFirstCharShape)
		return false;

	const CHWPRecordCharShape* pSecondCharShape = dynamic_cast<const CHWPRecordCharShape*>(oContext.GetCharShape(shSecondCharShapeId));

	if (nullptr == pSecondCharShape)
		return false;

	CRunnerStyle oFirstRunnerStyle {GenerateRunnerStyle(*pFirstCharShape )};
	CRunnerStyle oSecondRunnerStyle{GenerateRunnerStyle(*pSecondCharShape)};

	oSecondRunnerStyle -= oFirstRunnerStyle;

	WriteRunnerProperties(oSecondRunnerStyle, oBuilder);

	return true;
}

unsigned short CStyleConverter::GetLastParaShapeId() const
{
	return m_ushLastParaShapeId;
}

unsigned short CStyleConverter::GetLastCharShapeId() const
{
	return m_ushLastCharShapeId;
}

std::wstring CStyleConverter::GenerateUniqueID(const std::wstring& wsName)
{
	std::unordered_map<std::wstring, int>::const_iterator itFound = m_mUsedStyleNames.find(wsName);

	if (itFound == m_mUsedStyleNames.cend())
	{
		m_mUsedStyleNames.insert(std::make_pair(wsName, 0));
		return wsName;
	}

	return wsName + std::to_wstring(++m_mUsedStyleNames[wsName]);
}

CParagraphsStyle CStyleConverter::GenerateParagraphStyle(const CHWPRecordParaShape& oParaShape)
{
	CParagraphsStyle oParagraphsStyle;

	if (oParaShape.KeepWithNext())
		oParagraphsStyle.SetKeepNext(true);

	oParagraphsStyle.SetFirstLine(oParaShape.GetIndent());
	oParagraphsStyle.SetLeftInd(oParaShape.GetLeftIndent());
	oParagraphsStyle.SetRightInd(oParaShape.GetRightIndent());

	switch(oParaShape.GetHorizantalAlign())
	{
		case EHorizontalAlign::JUSTIFY: oParagraphsStyle.SetJs(EJs::Both); break;
		case EHorizontalAlign::LEFT: oParagraphsStyle.SetJs(EJs::Left); break;
		case EHorizontalAlign::RIGHT: oParagraphsStyle.SetJs(EJs::Right); break;
		case EHorizontalAlign::CENTER: oParagraphsStyle.SetJs(EJs::Center); break;
		case EHorizontalAlign::DISTRIBUTE:
		case EHorizontalAlign::DISTRIBUTE_SPACE: oParagraphsStyle.SetJs(EJs::Distribute); break;
	}

	switch(oParaShape.GetVerticalAlign())
	{
		case EVerticalAlign::BOTTOM: oParagraphsStyle.SetTextAlignment(ETextAlignment::Bottom); break;
		case EVerticalAlign::TOP:oParagraphsStyle.SetTextAlignment(ETextAlignment::Top); break;
		case EVerticalAlign::CENTER: oParagraphsStyle.SetTextAlignment(ETextAlignment::Center); break;
		case EVerticalAlign::BASELINE:
			break;
	}

	switch(oParaShape.GetLineSpacingType())
	{
		case 0x0:
		{
			oParagraphsStyle.SetSpacingLineRule(ELineRule::Auto);
			oParagraphsStyle.SetSpacing(static_cast<int>(2.4 * (double)oParaShape.GetLineSpacing())); // 240 / 100
			break;
		}
		case 0x01:
		{
			oParagraphsStyle.SetSpacingLineRule(ELineRule::Exact);
			oParagraphsStyle.SetSpacing(static_cast<int>((double)oParaShape.GetLineSpacing() / 10.)); /*0.352778*/; //(1pt=0.352778mm) //TODO:: проверить, как найдется пример
			break;
		}
		case 0x02:
		case 0x03:
		{
			oParagraphsStyle.SetSpacingLineRule(ELineRule::AtLeast);
			oParagraphsStyle.SetSpacing(static_cast<int>((double)oParaShape.GetLineSpacing() / 10.)); //TODO:: проверить, как найдется пример
			break;
		}
		default:
			break;
	}

	if (0 != oParaShape.GetMarginPrev())
		oParagraphsStyle.SetSpacingBefore(static_cast<int>((double)oParaShape.GetMarginPrev() / 10.));

	if (0 != oParaShape.GetMarginNext())
		oParagraphsStyle.SetSpacingAfter(static_cast<int>((double)oParaShape.GetMarginNext() / 10.));

	return oParagraphsStyle;
}

CRunnerStyle CStyleConverter::GenerateRunnerStyle(const CHWPRecordCharShape& oCharShape)
{
	CRunnerStyle oRunnerStyle;

	oRunnerStyle.SetAscii(oCharShape.GetFontName(ELang::LATIN));
	oRunnerStyle.SetEastAsia(oCharShape.GetFontName(ELang::HANGUL));
	oRunnerStyle.SetRatio(oCharShape.GetRatio(ELang::LATIN));
	oRunnerStyle.SetSpacing(static_cast<short>((double)oCharShape.GetSpacing(ELang::LATIN) * SPACING_SCALE_MS_WORD));

	if (oCharShape.Bold())
		oRunnerStyle.SetBold(true);

	if (oCharShape.Italic())
		oRunnerStyle.SetItalic(true);

	if (oCharShape.StrikeOut())
		oRunnerStyle.SetStrike(EStrikeType::Single);

	if (oCharShape.SuperScript())
		oRunnerStyle.SetVerticalAlign(EVerticalAlignRun::Superscript);
	else if (oCharShape.SubScript())
		oRunnerStyle.SetVerticalAlign(EVerticalAlignRun::Subscript);

	const int nHeight = static_cast<int>(((double)(std::abs)(oCharShape.GetHeight()) * ((double)oCharShape.GetRelSize(ELang::HANGUL) / 100.) / 100.) * 2.);
	oRunnerStyle.SetSz(nHeight);

	oRunnerStyle.SetColor(oCharShape.GetTextColor());

	if (oCharShape.Underline())
	{
		EUnderline eUnderlineType   = oCharShape.GetUnderlineType();
		ELineStyle1 eUnderlineStyle = oCharShape.GetUnderlineStyle();

		if (EUnderline::BOTTOM == eUnderlineType)
		{
			switch (eUnderlineStyle)
			{
				case ELineStyle1::SOLID: oRunnerStyle.SetU({EUType::Single, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DASH: oRunnerStyle.SetU({EUType::Dash, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DOT: oRunnerStyle.SetU({EUType::Dotted, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DASH_DOT: oRunnerStyle.SetU({EUType::DotDash, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DASH_DOT_DOT: oRunnerStyle.SetU({EUType::DotDotDash, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::LONG_DASH: oRunnerStyle.SetU({EUType::DotDash, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::CIRCLE: oRunnerStyle.SetU({EUType::Dotted, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DOUBLE_SLIM:
				case ELineStyle1::SLIM_THICK:
				case ELineStyle1::THICK_SLIM:
				case ELineStyle1::SLIM_THICK_SLIM: oRunnerStyle.SetU({EUType::Double, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::WAVE: oRunnerStyle.SetU({EUType::Wave, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::DOUBLE_WAVE: oRunnerStyle.SetU({EUType::WavyDouble, oCharShape.GetUnderlineColor()}); break;
				case ELineStyle1::THICK_3D:
				case ELineStyle1::THICK_3D_REVERS_LI:
				case ELineStyle1::SOLID_3D:
				case ELineStyle1::SOLID_3D_REVERS_LI: oRunnerStyle.SetU({EUType::Thick, oCharShape.GetUnderlineColor()}); break;
			}
		}
		else if (EUnderline::CENTER == eUnderlineType)
		{
			if (eUnderlineStyle == ELineStyle1::DOUBLE_SLIM ||
				eUnderlineStyle == ELineStyle1::DOUBLE_WAVE)
				oRunnerStyle.SetStrike(EStrikeType::Double);
			else
				oRunnerStyle.SetStrike(EStrikeType::Single);
		}
	}

	if (0xFFFFFFFF != oCharShape.GetShadeColor())
		oRunnerStyle.SetShadeColor(oCharShape.GetShadeColor());

	return oRunnerStyle;
}

void CStyleConverter::WriteParagraphProperties(const CParagraphsStyle& oParagraphsStyle, NSStringUtils::CStringBuilder& oBuilder)
{
	if (oParagraphsStyle.Empty())
		return;

	if (oParagraphsStyle.KeepNextIsSet() && oParagraphsStyle.KeepNext())
		oBuilder.WriteString(L"<w:keepNext w:val=\"true\"/>");

	if ((oParagraphsStyle.FirstLineIsSet() && 0 != oParagraphsStyle.GetFirstLine()) ||
	    (oParagraphsStyle.LeftIndIsSet() && 0 != oParagraphsStyle.GetLeftInd()) ||
	    (oParagraphsStyle.RightIndIsSet() && 0 != oParagraphsStyle.GetRightInd()))
	{
		oBuilder.WriteString(L"<w:ind");

		const int nLeftInd {(oParagraphsStyle.LeftIndIsSet())  ? oParagraphsStyle.GetLeftInd()  : 0};

		if (oParagraphsStyle.FirstLineIsSet() && 0 != oParagraphsStyle.GetFirstLine())
		{
			if (oParagraphsStyle.GetFirstLine() > 0)
			{
				oBuilder.WriteString(L" w:firstLine=\"" + std::to_wstring(static_cast<int>(std::round(oParagraphsStyle.GetFirstLine() / 10.))) + L"\"");

				if (0 != nLeftInd)
					oBuilder.WriteString(L" w:left=\"" + std::to_wstring(static_cast<int>(std::round(nLeftInd / 10.))) + L"\"");
			}
			else
			{
				const int nHanging{static_cast<int>(std::round(-oParagraphsStyle.GetFirstLine() / 10.))};

				oBuilder.WriteString(L" w:hanging=\"" + std::to_wstring(nHanging) + L"\"");
				oBuilder.WriteString(L" w:left=\"" + std::to_wstring(static_cast<int>(std::round(nLeftInd / 10.)) + nHanging) + L"\"");
			}
		}
		else if (0 != nLeftInd)
			oBuilder.WriteString(L" w:left=\"" + std::to_wstring(static_cast<int>(std::round(oParagraphsStyle.GetLeftInd() / 10.))) + L"\"");

		if (oParagraphsStyle.RightIndIsSet() && 0 != oParagraphsStyle.GetRightInd())
			oBuilder.WriteString(L" w:right=\"" + std::to_wstring(static_cast<int>(std::round(oParagraphsStyle.GetRightInd() / 10.))) + L"\"");

		oBuilder.WriteString(L"/>");
	}

	if (oParagraphsStyle.JsIsSet())
	{
		switch(oParagraphsStyle.GetJs())
		{
			case EJs::Left: oBuilder.WriteString(L"<w:jc w:val=\"left\"/>"); break;
			case EJs::Center: oBuilder.WriteString(L"<w:jc w:val=\"center\"/>"); break;
			case EJs::Right: oBuilder.WriteString(L"<w:jc w:val=\"right\"/>"); break;
			case EJs::Both: oBuilder.WriteString(L"<w:jc w:val=\"both\"/>"); break;
			case EJs::Distribute:oBuilder.WriteString(L"<w:jc w:val=\"distribute\"/>"); break;
		}
	}

	if (oParagraphsStyle.TextAlignmentIsSet())
	{
		switch(oParagraphsStyle.GetTextAlignment())
		{
			case ETextAlignment::Top: oBuilder.WriteString(L"<w:textAlignment w:val=\"top\"/>"); break;
			case ETextAlignment::Center: oBuilder.WriteString(L"<w:textAlignment w:val=\"center\"/>"); break;
			case ETextAlignment::Bottom: oBuilder.WriteString(L"<w:textAlignment w:val=\"bottom\"/>"); break;
		}
	}

	if (oParagraphsStyle.SpacingLineRuleIsSet() || oParagraphsStyle.SpacingIsSet() || oParagraphsStyle.SpacingBeforeIsSet() || oParagraphsStyle.SpacingAfterIsSet())
	{
		oBuilder.WriteString(L"<w:spacing w:lineRule=\"");

		if (oParagraphsStyle.SpacingLineRuleIsSet())
		{
			switch (oParagraphsStyle.GetSpacingLineRule())
			{
				case ELineRule::Auto: oBuilder.WriteString(L"auto"); break;
				case ELineRule::Exact:  oBuilder.WriteString(L"exact"); break;
				case ELineRule::AtLeast: oBuilder.WriteString(L"atLeast"); break;
			}
		}
		else
			oBuilder.WriteString(L"auto");

		oBuilder.WriteString(L"\" w:line=\"" + std::to_wstring((oParagraphsStyle.SpacingIsSet()) ? oParagraphsStyle.GetSpacing() : DEFAULT_SPACING) + L"\"");

		if (oParagraphsStyle.SpacingBeforeIsSet())
			oBuilder.WriteString(L" w:before=\"" + std::to_wstring(oParagraphsStyle.GetSpacingBefore()) + L"\"");

		if (oParagraphsStyle.SpacingAfterIsSet())
			oBuilder.WriteString(L" w:after=\"" + std::to_wstring(oParagraphsStyle.GetSpacingAfter()) + L"\"");

		oBuilder.WriteString(L"/>");
	}
}

void CStyleConverter::WriteParagraphsStyle(const CParagraphsStyle& oParagraphsStyle, const std::wstring& wsName, const std::wstring& wsLink, NSStringUtils::CStringBuilder& oBuilder)
{
	oBuilder.WriteString(L"<w:style w:type=\"paragraph\" w:customStyle=\"1\" w:styleId=\"");
	oBuilder.WriteEncodeXmlString(wsName);
	oBuilder.WriteString(L"\">");
	oBuilder.WriteString(L"<w:name w:val=\"");
	oBuilder.WriteEncodeXmlString(wsName);
	oBuilder.WriteString(L"\"/>");
	oBuilder.WriteString(L"<w:link w:val=\"");
	oBuilder.WriteEncodeXmlString(wsLink);
	oBuilder.WriteString(L"\"/>");
	oBuilder.WriteString(L"<w:qFormat/>");
	oBuilder.WriteString(L"<w:pPr>");
	WriteParagraphProperties(oParagraphsStyle, oBuilder);
	oBuilder.WriteString(L"</w:pPr>");
	oBuilder.WriteString(L"</w:style>");
}

void CStyleConverter::WriteRunnerProperties(const CRunnerStyle& oRunnerStyle, NSStringUtils::CStringBuilder& oBuilder)
{
	if (oRunnerStyle.Empty())
		return;

	std::wstring wsAScii, wsEastAsia;

	if (oRunnerStyle.AsciiIsSet())
		wsAScii = oRunnerStyle.GetAscii();

	if (oRunnerStyle.EastAsiaIsSet())
		wsEastAsia = oRunnerStyle.GetEastAsia();

	if (!wsAScii.empty() || !wsEastAsia.empty())
	{
		oBuilder.WriteString(L"<w:rFonts");

		if (!wsAScii.empty())
			oBuilder.WriteString(L" w:ascii=\"" + wsAScii +  L"\" w:hAnsi=\"" + wsAScii + L"\" w:cs=\"" + wsAScii + L'"');

		if (!wsEastAsia.empty())
			oBuilder.WriteString(L" w:eastAsia=\"" + wsEastAsia + L'"');

		oBuilder.WriteString(L"/>");
	}

	if (oRunnerStyle.BoldIsSet() && oRunnerStyle.Bold())
		oBuilder.WriteString(L"<w:b/><w:bCs/>");

	if (oRunnerStyle.ItalicIsSet() && oRunnerStyle.Italic())
		oBuilder.WriteString(L"<w:i/><w:iCs/>");

	if (oRunnerStyle.SzIsSet())
		oBuilder.WriteString(L"<w:sz w:val=\"" + std::to_wstring(oRunnerStyle.GetSz()) + L"\"/><w:szCs w:val=\"" + std::to_wstring(oRunnerStyle.GetSz()) + L"\"/>");

	if (oRunnerStyle.ColorIsSet())
		oBuilder.WriteString(L"<w:color w:val=\"" + oRunnerStyle.GetColor().ToHEX() + L"\"/>");

	if (oRunnerStyle.RatioIsSet() && 100 != oRunnerStyle.GetRatio())
		oBuilder.WriteString(L"<w:w w:val=\"" + std::to_wstring(oRunnerStyle.GetRatio()) + L"\"/>");

	if (oRunnerStyle.SpacingIsSet() && 0 != oRunnerStyle.GetSpacing())
		oBuilder.WriteString(L"<w:spacing w:val=\"" + std::to_wstring(oRunnerStyle.GetSpacing()) + L"\"/>");

	if (oRunnerStyle.UIsSet())
	{
		oBuilder.WriteString(L"<w:u w:val=\"");

		switch (oRunnerStyle.GetU().m_eVal)
		{
			case EUType::Single: oBuilder.WriteString(L"single"); break;
			case EUType::Dash: oBuilder.WriteString(L"dash"); break;
			case EUType::Dotted: oBuilder.WriteString(L"dotted"); break;
			case EUType::DotDash: oBuilder.WriteString(L"dotDash"); break;
			case EUType::DotDotDash: oBuilder.WriteString(L"dotDotDash"); break;
			case EUType::Double: oBuilder.WriteString(L"double"); break;
			case EUType::Wave: oBuilder.WriteString(L"wave"); break;
			case EUType::WavyDouble: oBuilder.WriteString(L"wavyDouble"); break;
			case EUType::Thick: oBuilder.WriteString(L"thick"); break;
		}

		oBuilder.WriteString(L"\" w:color=\"" + oRunnerStyle.GetU().m_oColor.ToHEX() + L"\"/>");
	}

	if (oRunnerStyle.StrikeIsSet())
	{
		switch (oRunnerStyle.GetStrike())
		{
			case EStrikeType::Single: oBuilder.WriteString(L"<w:strike/>"); break;
			case EStrikeType::Double: oBuilder.WriteString(L"<w:dstrike/>"); break;
		}
	}

	if (oRunnerStyle.HighlightIsSet())
	{
		oBuilder.WriteString(L"<w:highlight w:val=\"");

		switch (oRunnerStyle.GetHighlight())
		{
			case EHighlightColors::Black: oBuilder.WriteString(L"black"); break;
			case EHighlightColors::Blue: oBuilder.WriteString(L"blue"); break;
			case EHighlightColors::Cyan: oBuilder.WriteString(L"cyan"); break;
			case EHighlightColors::Green: oBuilder.WriteString(L"green"); break;
			case EHighlightColors::Magenta: oBuilder.WriteString(L"magenta"); break;
			case EHighlightColors::Red: oBuilder.WriteString(L"red"); break;
			case EHighlightColors::Yellow: oBuilder.WriteString(L"yellow"); break;
			case EHighlightColors::White: oBuilder.WriteString(L"white"); break;
			case EHighlightColors::DarkBlue: oBuilder.WriteString(L"darkBlue"); break;
			case EHighlightColors::DarkCyan: oBuilder.WriteString(L"darkCyan"); break;
			case EHighlightColors::DarkGreen: oBuilder.WriteString(L"darkGreen"); break;
			case EHighlightColors::DarkMagenta: oBuilder.WriteString(L"darkMagenta"); break;
			case EHighlightColors::DarkRed: oBuilder.WriteString(L"darkRed"); break;
			case EHighlightColors::DarkYellow: oBuilder.WriteString(L"darkYellow"); break;
			case EHighlightColors::DarkGray: oBuilder.WriteString(L"darkGray"); break;
			case EHighlightColors::LightGray: oBuilder.WriteString(L"lightGray"); break;
				break;
		}

		oBuilder.WriteString(L"\"/>");
	}

	if (oRunnerStyle.ShadeColorIsSet())
		oBuilder.WriteString(L"<w:shd w:val=\"clear\" w:fill=\"" + Transform::IntColorToHEX(oRunnerStyle.GetShadeColor()) + L"\"/>");

	if (oRunnerStyle.VerticalAlignIsSet())
	{
		switch (oRunnerStyle.GetVerticalAlign())
		{
			case EVerticalAlignRun::Baseline: break;
			case EVerticalAlignRun::Superscript: oBuilder.WriteString(L"<w:vertAlign w:val=\"superscript\"/>"); break;
			case EVerticalAlignRun::Subscript: oBuilder.WriteString(L"<w:vertAlign w:val=\"subscript\"/>"); break;
		}
	}
}

void CStyleConverter::WriteRunnerStyle(const CRunnerStyle& oRunnerStyle, const std::wstring& wsName, const std::wstring& wsLink, NSStringUtils::CStringBuilder& oBuilder)
{
	oBuilder.WriteString(L"<w:style w:type=\"character\" w:customStyle=\"1\" w:styleId=\"");
	oBuilder.WriteEncodeXmlString(wsName);
	oBuilder.WriteString(L"-c\">");
	oBuilder.WriteString(L"<w:name w:val=\"");
	oBuilder.WriteEncodeXmlString(wsName);
	oBuilder.WriteString(L" Character\"/>");
	oBuilder.WriteString(L"<w:link w:val=\"");
	oBuilder.WriteEncodeXmlString(wsLink);
	oBuilder.WriteString(L"\"/>");
	oBuilder.WriteString(L"<w:rPr>");
	WriteRunnerProperties(oRunnerStyle, oBuilder);
	oBuilder.WriteString(L"</w:rPr>");
	oBuilder.WriteString(L"</w:style>");
}

bool CStyleConverter::SaveToFile(const std::wstring& wsDirectory)
{
	NSFile::CFileBinary oStylesWriter;
	if (!oStylesWriter.CreateFileW(wsDirectory + L"styles.xml"))
		return false;

	oStylesWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" mc:Ignorable=\"w14 w15\">");
	oStylesWriter.WriteStringUTF8(L"<w:rPrDefault><w:rPr>");
	oStylesWriter.WriteStringUTF8(L"<w:rFonts w:ascii=\"" + DEFAULT_FONT_FAMILY + L"\" w:eastAsia=\"" + DEFAULT_FONT_FAMILY + L"\"  w:hAnsi=\"" + DEFAULT_FONT_FAMILY + L"\" w:cs=\"" + DEFAULT_FONT_FAMILY + L"\"/>");
	oStylesWriter.WriteStringUTF8(L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>");
	oStylesWriter.WriteStringUTF8(L"<w:lang w:val=\"" + DEFAULT_LANGUAGE + L"\" w:eastAsia=\"en-US\" w:bidi=\"ar-SA\"/>");
	oStylesWriter.WriteStringUTF8(L"</w:rPr></w:rPrDefault><w:pPrDefault/>");

	oStylesWriter.WriteStringUTF8(L"<w:style w:type=\"paragraph\" w:styleId=\"normal\" w:default=\"1\"><w:name w:val=\"Normal\"/><w:qFormat/><w:rPr><w:rFonts w:eastAsiaTheme=\"minorEastAsia\"/>");
	oStylesWriter.WriteStringUTF8(L"<w:sz w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/><w:szCs w:val=\"" + std::to_wstring(DEFAULT_FONT_SIZE) + L"\"/>");
	oStylesWriter.WriteStringUTF8(L"</w:rPr></w:style>");

	oStylesWriter.WriteStringUTF8(L"<w:style w:type=\"paragraph\" w:styleId=\"li\"><w:name w:val=\"List Paragraph\"/><w:basedOn w:val=\"normal\"/><w:qFormat/><w:uiPriority w:val=\"34\"/><w:pPr><w:contextualSpacing w:val=\"true\"/><w:ind w:left=\"720\"/></w:pPr></w:style>");
	oStylesWriter.WriteStringUTF8(L"<w:style w:type=\"character\" w:styleId=\"a\"><w:name w:val=\"Hyperlink\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:color w:val=\"0000FF\" w:themeColor=\"hyperlink\"/><w:u w:val=\"single\"/></w:rPr></w:style>");
	oStylesWriter.WriteStringUTF8(L"<w:style w:type=\"character\" w:styleId=\"footnote\"><w:name w:val=\"footnote reference\"/><w:uiPriority w:val=\"99\"/><w:unhideWhenUsed/><w:rPr><w:vertAlign w:val=\"superscript\"/></w:rPr></w:style><w:style w:type=\"paragraph\" w:styleId=\"footnote-p\"><w:name w:val=\"footnote text\"/><w:basedOn w:val=\"normal\"/><w:link w:val=\"footnote-c\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:rPr><w:sz w:val=\"18\"/></w:rPr><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"40\"/></w:pPr></w:style><w:style w:type=\"character\" w:styleId=\"footnote-c\" w:customStyle=\"1\"><w:name w:val=\"footnote text character\"/><w:link w:val=\"footnote-p\"/><w:uiPriority w:val=\"99\"/><w:rPr><w:sz w:val=\"18\"/></w:rPr></w:style>");
	oStylesWriter.WriteStringUTF8(L"<w:style w:type=\"paragraph\" w:styleId=\"normal-web\"><w:name w:val=\"Normal (Web)\"/><w:basedOn w:val=\"normal\"/><w:uiPriority w:val=\"99\"/><w:semiHidden/><w:unhideWhenUsed/><w:pPr><w:spacing w:before=\"100\" w:beforeAutospacing=\"1\" w:after=\"100\" w:afterAutospacing=\"1\"/></w:pPr></w:style>");

	oStylesWriter.WriteStringUTF8(m_oStylesXml.GetData());

	oStylesWriter.WriteStringUTF8(L"</w:styles>");

	return true;
}

CParagraphsStyle::CParagraphsStyle()
{}

CParagraphsStyle& CParagraphsStyle::operator-=(const CParagraphsStyle& oParagraphStyle)
{
	if (Empty() || oParagraphStyle.Empty())
		return *this;

	m_bKeepNext      -= oParagraphStyle.m_bKeepNext;
	m_oInd           -= oParagraphStyle.m_oInd;
	m_eJs            -= oParagraphStyle.m_eJs;
	m_eTextAlignment -= oParagraphStyle.m_eTextAlignment;
	m_oSpacing       -= oParagraphStyle.m_oSpacing;

	return *this;
}

void CParagraphsStyle::Clear()
{
	m_bKeepNext.UnSet();
	m_eJs.UnSet();
	m_eTextAlignment.UnSet();

	m_oInd.m_nFirstLine.UnSet();
	m_oInd.m_nLeft.UnSet();
	m_oInd.m_nRight.UnSet();

	m_oSpacing.m_eLineRule.UnSet();
	m_oSpacing.m_nLine.UnSet();
	m_oSpacing.m_nAfter.UnSet();
	m_oSpacing.m_nBefore.UnSet();
}

bool CParagraphsStyle::Empty() const
{
	return !m_bKeepNext.IsSet() && !m_oInd.m_nFirstLine.IsSet() && !m_oInd.m_nLeft.IsSet() &&
	       !m_oInd.m_nRight.IsSet() && !m_eJs.IsSet() && !m_eTextAlignment.IsSet() &&
	       !m_oSpacing.m_eLineRule.IsSet() && !m_oSpacing.m_nLine.IsSet() &&
	       !m_oSpacing.m_nAfter.IsSet() && !m_oSpacing.m_nBefore.IsSet();
}

#define CREATE_BODY_METHODS_FOR_PROPERTY(class_name, type, name, variable_name)\
	void class_name::Set##name(type oValue)\
	{\
		variable_name = oValue;\
	}\
	bool class_name::name##IsSet() const\
	{\
		return variable_name.IsSet();\
	}\
	type class_name::Get##name() const\
	{\
		return variable_name;\
	}

#define CREATE_BODY_METHODS_FOR_PROPERTY_BOOL(class_name, name, variable_name)\
	void class_name::Set##name(bool oValue)\
	{\
		variable_name = oValue;\
	}\
	bool class_name::name##IsSet() const\
	{\
		return variable_name.IsSet();\
	}\
	bool class_name::name() const\
	{\
		return variable_name;\
	}

CREATE_BODY_METHODS_FOR_PROPERTY_BOOL(CParagraphsStyle, KeepNext, m_bKeepNext)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, FirstLine, m_oInd.m_nFirstLine)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, LeftInd, m_oInd.m_nLeft)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, RightInd, m_oInd.m_nRight)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, EJs, Js, m_eJs)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, ETextAlignment, TextAlignment, m_eTextAlignment)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, ELineRule, SpacingLineRule, m_oSpacing.m_eLineRule)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, Spacing, m_oSpacing.m_nLine)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, SpacingBefore, m_oSpacing.m_nBefore)
CREATE_BODY_METHODS_FOR_PROPERTY(CParagraphsStyle, int, SpacingAfter, m_oSpacing.m_nAfter)

template<class T>
CProperty<T>::CProperty()
	: m_bIsSet(false)
{}

template<class T>
void CProperty<T>::UnSet()
{
	m_bIsSet = false;
}

template<class T>
void CProperty<T>::SetValue(T oValue)
{
	m_oValue = oValue;
	m_bIsSet = true;
}

template<class T>
bool CProperty<T>::IsSet() const
{
	return m_bIsSet;
}

template<class T>
T CProperty<T>::GetValue() const
{
	return m_oValue;
}

template<class T>
CProperty<T>& CProperty<T>::operator=(T oValue)
{
	SetValue(oValue);
	return *this;
}

template<class T>
CProperty<T>& CProperty<T>::operator-=(const CProperty<T>& oProperty)
{
	if (!m_bIsSet || !oProperty.m_bIsSet || m_oValue != oProperty.m_oValue)
		return *this;

	m_bIsSet = false;

	return *this;
}

template<class T>
CProperty<T>::operator T() const
{
	return m_oValue;
}

TRFonts& TRFonts::operator-=(const TRFonts& oRFonts)
{
	m_wsAscii    -= oRFonts.m_wsAscii;
	m_wsEastAsia -= oRFonts.m_wsEastAsia;

	return *this;
}

CRunnerStyle::CRunnerStyle()
{}

CRunnerStyle& CRunnerStyle::operator-=(const CRunnerStyle& oRunnerStyle)
{
	m_oRFonts        -= oRunnerStyle.m_oRFonts;
	m_bBold          -= oRunnerStyle.m_bBold;
	m_bItalic        -= oRunnerStyle.m_bItalic;
	m_nSz            -= oRunnerStyle.m_nSz;
	m_oColor         -= oRunnerStyle.m_oColor;
	m_nShadeColor    -= oRunnerStyle.m_nShadeColor;
	m_shRatio        -= oRunnerStyle.m_shRatio;
	m_oU             -= oRunnerStyle.m_oU;
	m_eStrike        -= oRunnerStyle.m_eStrike;
	m_shSpacing      -= oRunnerStyle.m_shSpacing;
	m_oHighlight     -= oRunnerStyle.m_oHighlight;
	m_eVerticalAlign -= oRunnerStyle.m_eVerticalAlign;

	return *this;
}

void CRunnerStyle::Clear()
{
	m_oRFonts.m_wsAscii.UnSet();
	m_oRFonts.m_wsEastAsia.UnSet();
	m_bBold.UnSet();
	m_bItalic.UnSet();
	m_nSz.UnSet();
	m_oColor.UnSet();
	m_nShadeColor.UnSet();
	m_shRatio.UnSet();
	m_oU.UnSet();
	m_eStrike.UnSet();
	m_shSpacing.UnSet();
	m_oHighlight.UnSet();
	m_eVerticalAlign.UnSet();
}

bool CRunnerStyle::Empty() const
{
	return !m_oRFonts.m_wsAscii.IsSet() && !m_oRFonts.m_wsEastAsia.IsSet() && !m_bBold.IsSet() && !m_bItalic.IsSet() &&
	       !m_nSz.IsSet() && !m_oColor.IsSet() && !m_nShadeColor.IsSet() && !m_shRatio.IsSet() && !m_oU.IsSet() &&
	       !m_eStrike.IsSet() && !m_shSpacing.IsSet() && !m_oHighlight.IsSet() && !m_eVerticalAlign.IsSet();
}

CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, std::wstring, Ascii, m_oRFonts.m_wsAscii);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, std::wstring, EastAsia, m_oRFonts.m_wsEastAsia);
CREATE_BODY_METHODS_FOR_PROPERTY_BOOL(CRunnerStyle, Bold, m_bBold);
CREATE_BODY_METHODS_FOR_PROPERTY_BOOL(CRunnerStyle, Italic, m_bItalic);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, int, Sz, m_nSz);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, TColor, Color, m_oColor);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, int, ShadeColor, m_nShadeColor)
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, short, Ratio, m_shRatio)
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, TU, U, m_oU);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, EStrikeType, Strike, m_eStrike);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, short, Spacing, m_shSpacing);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, EHighlightColors, Highlight, m_oHighlight);
CREATE_BODY_METHODS_FOR_PROPERTY(CRunnerStyle, EVerticalAlignRun, VerticalAlign, m_eVerticalAlign);

bool TU::operator!=(const TU& oU) const
{
	return m_eVal != oU.m_eVal || m_oColor != oU.m_oColor;
}

EHighlightColors NormalizeHighlightColor(const TColor& oCurrentColor)
{
	EHighlightColors eSelectedColor{EHighlightColors::Yellow};
	double dMinDistance = 999.;
	double dDistance;

	for (const std::pair<TColor, EHighlightColors>& oColor : s_arHighlightColors)
	{
		dDistance = sqrt(pow(oCurrentColor.m_uchRed - oColor.first.m_uchRed, 2) + pow(oCurrentColor.m_uchGreen - oColor.first.m_uchGreen, 2) + pow(oCurrentColor.m_uchBlue - oColor.first.m_uchBlue, 2));

		if (dDistance < dMinDistance)
		{
			dMinDistance = dDistance;
			eSelectedColor = oColor.second;
		}
	}

	return eSelectedColor;
}

TLineSpacing& TLineSpacing::operator-=(const TLineSpacing& oLineSpacing)
{
	m_eLineRule -= oLineSpacing.m_eLineRule;
	m_nLine     -= oLineSpacing.m_nLine;
	m_nAfter    -= oLineSpacing.m_nAfter;
	m_nBefore   -= oLineSpacing.m_nBefore;

	return *this;
}

TInd& TInd::operator-=(const TInd& oInd)
{
	m_nFirstLine -= oInd.m_nFirstLine;
	m_nLeft -= oInd.m_nLeft;
	m_nRight -= oInd.m_nRight;

	return *this;
}
}
