#include "CDocumentStyle.h"

#include <iostream>
#include <unordered_set>
#include <wchar.h>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <list>

#define DEFAULT_LINEHEIGHT 240
#define LINEHEIGHTSCALE 10 // Значение LineHeight в OOXML должно быть в 10 раз больше чем указано в стиле

#define VALUE_TO_INT(value, unit_measure) \
	(NSCSS::UnitMeasure::None != value.GetUnitMeasure()) ? \
		value.ToInt(unit_measure) : \
		static_cast<int>(NSCSS::CUnitMeasureConverter::ConvertPx(value.ToDouble(), unit_measure, 96) + 0.5)

namespace NSCSS
{
	CStyleUsed::CStyleUsed(const CCompiledStyle &oStyle, bool bIsPStyle)
		: m_oStyle(oStyle), m_bIsPStyle(bIsPStyle)
	{}

	bool CheckArrays(const std::vector<std::wstring>& arInitial, const std::set<std::wstring>& arFirst, const std::set<std::wstring>& arSecond)
	{
		std::unordered_set<std::wstring> arInitialSet(arInitial.begin(), arInitial.end());

		std::vector<std::wstring> arCommonElements1;
		std::vector<std::wstring> arCommonElements2;

		for (const std::wstring& wsValue : arFirst)
		{
			if (arInitialSet.count(wsValue) > 0)
				arCommonElements1.push_back(wsValue);
		}

		for (const std::wstring& wsValue : arSecond)
		{
			if (arInitialSet.count(wsValue) > 0)
				arCommonElements2.push_back(wsValue);
		}

		if (arCommonElements1.size() != arCommonElements2.size())
			return false;

		std::sort(arCommonElements1.begin(), arCommonElements1.end());
		std::sort(arCommonElements2.begin(), arCommonElements2.end());

		return arCommonElements1 == arCommonElements2;
	}

	bool CStyleUsed::operator==(const CStyleUsed &oUsedStyle) const
	{
		return m_bIsPStyle == oUsedStyle.m_bIsPStyle &&
		       CheckArrays(Names_Standard_Styles, m_oStyle.GetParentsNamesSet(), oUsedStyle.m_oStyle.GetParentsNamesSet()) &&
		       m_oStyle == oUsedStyle.m_oStyle;
	}

	std::wstring CStyleUsed::getId()
	{
		if (m_bIsPStyle)
			return m_oStyle.GetId();

		return m_oStyle.GetId() + L"-c";
	}

	CDocumentStyle::CDocumentStyle() : m_arStandardStyles(Names_Standard_Styles)
	{
		for (const std::wstring& oNameStandardStyle : Names_Standard_Styles)
			m_arStandardStyles.push_back(oNameStandardStyle + L"-c");
	}

	CDocumentStyle::~CDocumentStyle()
	{
		m_arStandardStyles.    clear();
		m_arStyleUsed.         clear();
		m_arStandardStylesUsed.clear();
	}

	std::wstring CDocumentStyle::GetStyle() const
	{
		return m_sStyle;
	}

	std::wstring CDocumentStyle::GetIdAndClear()
	{
		std::wstring sId = m_sId;
		Clear();
		return sId;
	}

	void CDocumentStyle::Clear()
	{
		m_sId.clear();
		m_sStyle.clear();
	}

	void CDocumentStyle::SetStyle(const std::wstring& sStyle)
	{
		if (sStyle.empty())
			return;
		m_sStyle = sStyle;
	}

	void CDocumentStyle::SetId   (const std::wstring& sId)
	{
		if (sId.empty())
			return;
		m_sId = sId;
	}

	void CDocumentStyle::CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement)
	{
		if (arStandartedStyles.empty())
			return;

		std::vector<std::wstring> arStyles;
		for (const std::wstring& sStyleName : arStandartedStyles)
		{
			if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.end())
				arStyles.push_back(sStyleName);
		}

		if (arStyles.empty())
			return;

		std::wstring sId;
		for (std::vector<std::wstring>::const_reverse_iterator iStyleName = arStyles.rbegin(); iStyleName != arStyles.rend(); ++iStyleName)
		{
			sId += *iStyleName + L'+';
			CXmlElement oTempElement;
			CreateStandardStyle(*iStyleName, oTempElement);
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), *iStyleName) == m_arStandardStylesUsed.end())
			{
				m_sStyle = oTempElement.GetStyle() + m_sStyle;
				m_arStandardStylesUsed.push_back(*iStyleName);
			}
			oElement += oTempElement;
		}
		sId.pop_back();

		oElement.AddBasicProperties(BProperties::B_Name, sId);
		oElement.AddBasicProperties(BProperties::B_StyleId, sId);
	}

	void CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle, CXmlElement& oElement)
	{
		if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) != m_arStandardStyles.end())
			oElement.CreateDefaultElement(sNameStyle);
	}

	void CDocumentStyle::ConvertStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle)
	{
		if (oStyle.GetId().empty())
			return;

		std::wstring sName = oStyle.GetId();
		const size_t posPoint = sName.find(L'.');

		if (posPoint != std::wstring::npos)
			sName = sName.substr(0, posPoint);
		else
		{
			const size_t posLattice = sName.find(L'#');
			if (posLattice != std::wstring::npos)
				sName = sName.substr(0, posLattice);
			else
			{
				const size_t posDash = sName.find(L'-');
				if (posDash != std::wstring::npos)
					sName = sName.substr(0, posDash);
			}
		}

		if (!bIsPStyle)
			sName += L"-c";

		CXmlElement oParentStyle;
		std::vector<std::wstring> arParentsName = oStyle.GetParentsName();

		if (!bIsPStyle)
			for (std::wstring& sParentName : arParentsName)
				sParentName += L"-c";

		if (!arParentsName.empty())
		{
			CombineStandardStyles(arParentsName, oParentStyle);

			if (!oParentStyle.Empty())
			{
				oParentStyle.AddBasicProperties(BProperties::B_BasedOn, L"normal");
				oParentStyle.AddBasicProperties(BProperties::B_StyleId, L"(" + oParentStyle.GetStyleId() + L")");
				if (!bIsPStyle)
				{
					oParentStyle.AddBasicProperties(BProperties::B_StyleId, oParentStyle.GetStyleId() + L"-c");
					oParentStyle.AddBasicProperties(BProperties::B_Type, L"character");
				}
			}
		}

		CXmlElement oStandardXmlElement;
		if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.end())
			CreateStandardStyle(sName, oStandardXmlElement);

		if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
		{
			const std::wstring sParentsStyleID = oParentStyle.GetStyleId();
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) == m_arStandardStylesUsed.end())
			{
				if (bIsPStyle)
				{
					oParentStyle.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oParentStyle.GetPStyle();
				}
				else
					m_sStyle += oParentStyle.GetRStyle();

				m_arStandardStylesUsed.push_back(sParentsStyleID);
			}
			if (oStyle.Empty())
			{
				m_sId = sParentsStyleID;
				return;
			}

			oElement.AddBasicProperties(BProperties::B_BasedOn, sParentsStyleID);
		}
		else if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
		{
			std::wstring sStandPlusParent = oStandardXmlElement.GetStyleId() + L"+" + oParentStyle.GetStyleId();
			if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.end())
			{
				if (oStyle.Empty())
				{
					m_sId = sStandPlusParent;
					return;
				}
				oElement.AddBasicProperties(BProperties::B_BasedOn, sStandPlusParent);
			}
			else
			{
				CXmlElement oTempElement = oParentStyle;
				oTempElement += oStandardXmlElement;

				oTempElement.AddBasicProperties(BProperties::B_StyleId, sStandPlusParent);

				if (bIsPStyle)
				{
					oTempElement.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oTempElement.GetPStyle();
				}
				else
					m_sStyle += oTempElement.GetRStyle();

				m_arStandardStylesUsed.push_back(sStandPlusParent);

				if (oStyle.Empty())
				{
					m_sId = sStandPlusParent;
					return;
				}
				oElement.AddBasicProperties(BProperties::B_BasedOn, oTempElement.GetStyleId());
			}
		}
		else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
		{
			std::wstring sStandartStyleID = oStandardXmlElement.GetStyleId();
			if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) == m_arStandardStylesUsed.end())
			{
				if (bIsPStyle)
				{
					oStandardXmlElement.AddBasicProperties(BProperties::B_QFormat, L"true");
					m_sStyle += oStandardXmlElement.GetPStyle();
				}
				else
					m_sStyle += oStandardXmlElement.GetRStyle();

				m_arStandardStylesUsed.push_back(sStandartStyleID);
			}
			if (oStyle.Empty())
			{
				m_sId = sStandartStyleID;
				return;
			}
			oElement.AddBasicProperties(BProperties::B_BasedOn, sStandartStyleID);
		}

		if (oStyle.Empty() && oElement.Empty())
		{
			m_sId = L"normal";
			return;
		}

		m_sId = oStyle.GetId();
		if(!bIsPStyle)
			m_sId += L"-c";
		else
			oElement.AddBasicProperties(BProperties::B_QFormat, L"true");

		oElement.AddBasicProperties(BProperties::B_StyleId, m_sId);
		oElement.AddBasicProperties(BProperties::B_Name, m_sId);
		oElement.AddBasicProperties(BProperties::B_Type, bIsPStyle ? L"paragraph" : L"character");
		oElement.AddBasicProperties(BProperties::B_CustomStyle, L"1");
	}

	void CDocumentStyle::SetPStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement, bool bIsLite)
	{
		if (!bIsLite)
			ConvertStyle(oStyle, oXmlElement, true);

		if (oStyle.Empty())
			return;

		const bool bInTable{oStyle.HaveThisParent(L"table")};

		std::wstring wsTextAlign{oStyle.m_oText.GetAlign().ToWString()};

		if (wsTextAlign.empty())
			wsTextAlign = oStyle.m_oDisplay.GetHAlign().ToWString();

		oXmlElement.AddPropertiesInP(PProperties::P_Jc, wsTextAlign);

		std::wstring sInfValue;
		sInfValue.reserve(64);

		if (!oStyle.m_oMargin.GetLeft().Empty() && !oStyle.m_oMargin.GetLeft().Zero())
			sInfValue += L"w:left=\""   + std::to_wstring(oStyle.m_oMargin.GetLeft().ToInt(NSCSS::Twips)) + L"\" ";

		if (!oStyle.m_oMargin.GetRight().Empty() && !oStyle.m_oMargin.GetRight().Zero())
			sInfValue += L"w:right=\""  + std::to_wstring(oStyle.m_oMargin.GetRight().ToInt(NSCSS::Twips)) + L"\" ";

		const int nIndent = oStyle.m_oText.GetIndent().ToInt(NSCSS::Twips);

		if (0 != nIndent)
			sInfValue += L"w:firstLine=\"" + std::to_wstring(nIndent) + L"\" ";

		oXmlElement.AddPropertiesInP(PProperties::P_Ind, sInfValue);

		std::wstring sSpacingValue;
		sSpacingValue.reserve(128);

		if (!oStyle.m_oMargin.GetTop().Empty() && !oStyle.m_oMargin.GetTop().Zero())
			sSpacingValue += L"w:before=\""  + std::to_wstring(VALUE_TO_INT(oStyle.m_oMargin.GetTop(), NSCSS::Twips))    + L"\" w:beforeAutospacing=\"0\"";
		else if (oStyle.m_oMargin.GetBottom().Zero() || bInTable)
			sSpacingValue += L"w:before=\"0\" w:beforeAutospacing=\"0\"";

		if (!oStyle.m_oMargin.GetBottom().Empty() && !oStyle.m_oMargin.GetBottom().Zero())
			sSpacingValue += L" w:after=\""   + std::to_wstring(VALUE_TO_INT(oStyle.m_oMargin.GetBottom(), NSCSS::Twips)) + L"\" w:afterAutospacing=\"0\"";
		else if (oStyle.m_oMargin.GetBottom().Zero() || bInTable)
			sSpacingValue += L" w:after=\"0\" w:afterAutospacing=\"0\"";

		if (!oStyle.m_oFont.GetLineHeight().Empty() && !oStyle.m_oFont.GetLineHeight().Zero())
		{
			const std::wstring wsLine{std::to_wstring(oStyle.m_oFont.GetLineHeight().ToInt(NSCSS::Twips, DEFAULT_LINEHEIGHT))};
			const std::wstring wsLineRule{(NSCSS::Percent == oStyle.m_oFont.GetLineHeight().GetUnitMeasure() ? L"auto" : L"atLeast")};

			sSpacingValue += L" w:line=\"" + wsLine + L"\" w:lineRule=\"" + wsLineRule + L"\"";
		}
		else if (oStyle.m_oFont.GetLineHeight().Zero() || bInTable)
			sSpacingValue += L" w:lineRule=\"auto\" w:line=\"240\"";

		if (!sSpacingValue.empty())
			oXmlElement.AddPropertiesInP(PProperties::P_Spacing, sSpacingValue);

		if (!oStyle.m_oBackground.Empty() && !bInTable)
			oXmlElement.AddPropertiesInP(PProperties::P_Shd, oStyle.m_oBackground.IsNone() ? L"auto" : oStyle.m_oBackground.GetColor().ToWString());

		if (!oStyle.m_oBorder.Empty() && !bInTable)
		{
			if (oStyle.m_oBorder.EqualSides())
			{
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_TopBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_LeftBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_BottomBorder);
				SetBorderStyle(oStyle, oXmlElement, PProperties::P_RightBorder);
			}
			else
			{
				if (!oStyle.m_oBorder.GetTopBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_TopBorder);

				if (!oStyle.m_oBorder.GetRightBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_RightBorder);

				if (!oStyle.m_oBorder.GetBottomBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_BottomBorder);

				if (!oStyle.m_oBorder.GetLeftBorder().Empty())
					SetBorderStyle(oStyle, oXmlElement, PProperties::P_LeftBorder);
			}
		}
	}

	void CDocumentStyle::SetBorderStyle(const CCompiledStyle &oStyle, CXmlElement &oXmlElement, const PProperties &enBorderProperty)
	{
		const NSCSS::NSProperties::CBorderSide* pBorder = NULL;
		const NSCSS::NSProperties::CDigit* pPadding = NULL;

		switch(enBorderProperty)
		{
			case PProperties::P_BottomBorder:
			{
				pBorder  = &oStyle.m_oBorder.GetBottomBorder();
				pPadding = &oStyle.m_oPadding.GetBottom();
				break;
			}
			case PProperties::P_LeftBorder:
			{
				pBorder = &oStyle.m_oBorder.GetLeftBorder();
				pPadding = &oStyle.m_oPadding.GetLeft();
				break;
			}
			case PProperties::P_RightBorder:
			{
				pBorder = &oStyle.m_oBorder.GetRightBorder();
				pPadding = &oStyle.m_oPadding.GetRight();
				break;
			}
			case PProperties::P_TopBorder:
			{
				pBorder = &oStyle.m_oBorder.GetTopBorder();
				pPadding = &oStyle.m_oPadding.GetTop();
				break;
			}
			default:
				return;
		}

		oXmlElement.AddPropertiesInP(enBorderProperty, CalculateBorderStyle(*pBorder, pPadding));
	}

	std::wstring CDocumentStyle::CalculateBorderStyle(const NSProperties::CBorderSide &oBorder, const NSProperties::CDigit *pPadding)
	{
		if (oBorder.Empty())
			return L"";

		std::wstring wsColor = oBorder.GetColor().ToWString();
		std::wstring wsStyle = oBorder.GetStyle().ToWString();

		int nWidth = static_cast<int>(std::round(oBorder.GetWidth().ToDouble(Point) * 8.));

		if (L"double" == wsStyle)
			nWidth /= 3; // в ooxml double граница формируется из трёх линий

		if (nWidth <= 3)
			nWidth = 2;
		else if (nWidth <= 5)
			nWidth = 4;
		else if (nWidth <= 7)
			nWidth = 6;
		else if (nWidth <= 9)
			nWidth = 8;
		else if (nWidth <= 15)
			nWidth = 12;
		else if (nWidth <= 21)
			nWidth = 18;
		else if (nWidth <= 29)
			nWidth = 24;
		else if (nWidth <= 41)
			nWidth = 36;
		else
			nWidth = 48;

		if (wsColor.empty())
			wsColor = L"auto";

		if (wsStyle.empty())
			wsStyle = L"single";

		int nSpace{0};

		return  L"w:val=\"" + wsStyle + L"\" w:sz=\"" + std::to_wstring(nWidth) + + L"\" w:space=\"" + std::to_wstring(nSpace) + L"\" w:color=\"" + wsColor + L"\"";
	}

	void CDocumentStyle::SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement, bool bIsLite)
	{
		if (!bIsLite)
			ConvertStyle(oStyle, oXmlElement, false);

		if (oStyle.Empty() && oXmlElement.Empty())
			return;

		if (!oStyle.m_oFont.GetSize().Empty())
			oXmlElement.AddPropertiesInR(RProperties::R_Sz, std::to_wstring(static_cast<int>(oStyle.m_oFont.GetSize().ToDouble(NSCSS::Point) * 2. * oStyle.m_oTransform.GetMatrix().GetFinalValue().sy() + 0.5))); // Значения шрифта увеличивает на 2

		if (oStyle.m_oText.GetDecoration().m_oLine.Underline())
			oXmlElement.AddPropertiesInR(RProperties::R_U, (!oStyle.m_oText.GetDecoration().m_oStyle.Empty()) ? oStyle.m_oText.GetDecoration().m_oStyle.ToWString() : L"single");

		if (!oStyle.m_oBackground.GetColor().Empty() && !oStyle.m_oBackground.GetColor().None() && !oStyle.m_oBackground.GetColor().Url())
			oXmlElement.AddPropertiesInR(RProperties::R_Shd, oStyle.m_oBackground.GetColor().ToWString());

		const std::wstring wsHighlight{oStyle.m_oText.GetHighlight().EquateToColor({{{0,   0,   0},   L"black"},    {{0,   0,   255}, L"blue"},      {{0,   255, 255}, L"cyan"},
		                                                                            {{0,   255, 0},   L"green"},    {{255, 0,   255}, L"magenta"},   {{255, 0,   0},   L"red"},
		                                                                            {{255, 255, 0},   L"yellow"},   {{255, 255, 255}, L"white"},     {{0,   0,   139}, L"darkBlue"},
		                                                                            {{0,   139, 139}, L"darkCyan"}, {{0,   100, 0},   L"darkGreen"}, {{139, 0,   139}, L"darkMagenta"},
		                                                                            {{139, 0,   0},   L"darkRed"},  {{128, 128, 0},   L"darkYellow"},{{169, 169, 169}, L"darkGray"},
		                                                                            {{211, 211, 211}, L"lightGray"}})};

		if (L"none" != wsHighlight)
			oXmlElement.AddPropertiesInR(RProperties::R_Highlight, wsHighlight);

		oXmlElement.AddPropertiesInR(RProperties::R_Color, oStyle.m_oText.GetColor().ToWString());

		std::wstring wsFontFamily{oStyle.m_oFont.GetFamily().ToWString()};

		if (L"sans-serif" == wsFontFamily)
			wsFontFamily = L"Arial";
		else if (L"serif" == wsFontFamily)
			wsFontFamily = L"Times New Roman";

		oXmlElement.AddPropertiesInR(RProperties::R_RFonts, oStyle.m_oFont.GetFamily().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_I, oStyle.m_oFont.GetStyle().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_B, oStyle.m_oFont.GetWeight().ToWString());
		oXmlElement.AddPropertiesInR(RProperties::R_SmallCaps, oStyle.m_oFont.GetVariant().ToWString());
	}

	bool CDocumentStyle::WriteRStyle(const NSCSS::CCompiledStyle& oStyle)
	{
		Clear();

		if(oStyle.GetId().empty())
			return false;

		CStyleUsed structStyle(oStyle, false);

		std::vector<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

		if (oItem != m_arStyleUsed.end())
		{
			m_sId = (*oItem).getId();
			return true;
		}

		CXmlElement oXmlElement;
		SetRStyle(oStyle, oXmlElement);

		if (oXmlElement.Empty())
			return false;

		m_arStyleUsed.push_back(structStyle);
		m_sStyle += oXmlElement.GetRStyle();

		return true;
	}

	bool CDocumentStyle::WriteLitePStyle(const CCompiledStyle &oStyle)
	{
		Clear();

		if (oStyle.Empty())
			return false;

		CXmlElement oXmlElement;
		SetPStyle(oStyle, oXmlElement, true);

		if (oXmlElement.Empty())
			return false;

		m_sStyle += oXmlElement.GetPStyle(true);
		return true;
	}

	bool CDocumentStyle::WriteLiteRStyle(const CCompiledStyle &oStyle)
	{
		Clear();

		if (oStyle.Empty())
			return false;

		CXmlElement oXmlElement;
		SetRStyle(oStyle, oXmlElement, true);

		if (oXmlElement.Empty())
			return false;

		m_sStyle += oXmlElement.GetRStyle(true);
		return true;
	}

	bool CDocumentStyle::WritePStyle(const NSCSS::CCompiledStyle& oStyle)
	{
		Clear();

		if(oStyle.GetId().empty())
			return false;

		CStyleUsed structStyle(oStyle, true);
		std::vector<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

		if (oItem != m_arStyleUsed.end())
		{
			m_sId = (*oItem).getId();
			return true;
		}

		CXmlElement oXmlElement;
		SetPStyle(oStyle, oXmlElement);

		if (oXmlElement.Empty())
			return false;

		m_arStyleUsed.push_back(structStyle);
		m_sStyle += oXmlElement.GetPStyle();

		return true;
	}
}
