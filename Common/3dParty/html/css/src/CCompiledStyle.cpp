#include "CCompiledStyle.h"

#include <cwctype>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>

#include <iostream>
#include "../../../../../DesktopEditor/common/File.h"
#include "StaticFunctions.h"
#include "ConstValues.h"

#define DEFAULT_FONT_SIZE 14

namespace NSCSS
{
	typedef std::map<std::wstring, std::wstring>::const_iterator styles_iterator;

	CCompiledStyle::CCompiledStyle() : m_nDpi(96), m_UnitMeasure(Point)
	{}

	CCompiledStyle::CCompiledStyle(const CCompiledStyle& oStyle) :
		m_arParentsStyles(oStyle.m_arParentsStyles), m_sId(oStyle.m_sId),
		m_nDpi(oStyle.m_nDpi), m_UnitMeasure(oStyle.m_UnitMeasure),
		m_oFont(oStyle.m_oFont), m_oMargin(oStyle.m_oMargin), m_oPadding(oStyle.m_oPadding), m_oBackground(oStyle.m_oBackground),
		m_oText(oStyle.m_oText), m_oBorder(oStyle.m_oBorder), m_oDisplay(oStyle.m_oDisplay){}

	CCompiledStyle::~CCompiledStyle()
	{
		m_arParentsStyles.clear();
	}

	CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
	{
		if (oElement.Empty())
			return *this;

		m_oBackground   += oElement.m_oBackground;
		m_oBorder       += oElement.m_oBorder;
		m_oFont         += oElement.m_oFont;
		m_oMargin       += oElement.m_oMargin;
		m_oPadding      += oElement.m_oPadding;
		m_oText         += oElement.m_oText;
		m_oDisplay      += oElement.m_oDisplay;

		if (!oElement.m_sId.empty())
			m_sId += L'+' + oElement.m_sId;

		return *this;
	}

	CCompiledStyle& CCompiledStyle::operator= (const CCompiledStyle &oElement)
	{
		m_sId               = oElement.m_sId;
		m_arParentsStyles   = oElement.m_arParentsStyles;

		m_nDpi          = oElement.m_nDpi;
		m_UnitMeasure   = oElement.m_UnitMeasure;

		m_oBackground   = oElement.m_oBackground;
		m_oBorder       = oElement.m_oBorder;
		m_oFont         = oElement.m_oFont;
		m_oMargin       = oElement.m_oMargin;
		m_oPadding      = oElement.m_oPadding;
		m_oText         = oElement.m_oText;
		m_oDisplay      = oElement.m_oDisplay;

		return *this;
	}

	bool CCompiledStyle::operator== (const CCompiledStyle& oStyle) const
	{
		return m_oBackground     == oStyle.m_oBackground     &&
		       m_oBorder         == oStyle.m_oBorder         &&
		       m_oFont           == oStyle.m_oFont           &&
		       m_oMargin         == oStyle.m_oMargin         &&
		       m_oPadding        == oStyle.m_oPadding        &&
		       m_oText           == oStyle.m_oText           &&
		       m_oDisplay        == oStyle.m_oDisplay;
	}

	void CCompiledStyle::StyleEquation(CCompiledStyle &oFirstStyle, CCompiledStyle &oSecondStyle)
	{
		NSProperties::CFont      ::Equation(oFirstStyle.m_oFont,       oSecondStyle.m_oFont);
		NSProperties::CIndent    ::Equation(oFirstStyle.m_oMargin,     oSecondStyle.m_oMargin);
		NSProperties::CIndent    ::Equation(oFirstStyle.m_oPadding,    oSecondStyle.m_oPadding);
		NSProperties::CBackground::Equation(oFirstStyle.m_oBackground, oSecondStyle.m_oBackground);
		NSProperties::CText      ::Equation(oFirstStyle.m_oText,       oSecondStyle.m_oText);
		NSProperties::CBorder    ::Equation(oFirstStyle.m_oBorder,     oSecondStyle.m_oBorder);
		NSProperties::CDisplay   ::Equation(oFirstStyle.m_oDisplay,    oSecondStyle.m_oDisplay);
	}

	void CCompiledStyle::SetDpi(const unsigned short &uiDpi)
	{
		m_nDpi = uiDpi;
	}

	void CCompiledStyle::SetUnitMeasure(const UnitMeasure &enUnitMeasure)
	{
		m_UnitMeasure = enUnitMeasure;
	}

	bool CCompiledStyle::Empty() const
	{
		return m_oBackground.Empty() && m_oBorder.Empty() && m_oFont.Empty() &&
		       m_oMargin.Empty() && m_oPadding.Empty() && m_oText.Empty() && m_oDisplay.Empty();
	}

	void CCompiledStyle::AddPropSel(const std::wstring& sProperty, const std::wstring& sValue, const unsigned int unLevel, const bool& bHardMode)
	{
		AddStyle({{sProperty, sValue}}, unLevel, bHardMode);
	}

	void CCompiledStyle::AddStyle(const std::map<std::wstring, std::wstring>& mStyle, const unsigned int unLevel, const bool& bHardMode)
	{
		const bool bIsThereBorder = (m_oBorder.Empty()) ? false : true;
		const double dFontSize = (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Point) : DEFAULT_FONT_SIZE;

		for (std::pair<std::wstring, std::wstring> pPropertie : mStyle)
		{
			std::transform(pPropertie.first.begin(), pPropertie.first.end(), pPropertie.first.begin(), tolower);

			SWITCH(pPropertie.first)
			{
				//FONT
				CASE(L"font"):
				{
					m_oFont.SetValue(pPropertie.second, unLevel, bHardMode);
					m_oFont.UpdateSize(dFontSize);
					m_oFont.UpdateLineHeight(dFontSize);
					break;
				}
				CASE(L"font-size"):
				CASE(L"font-size-adjust"):
				{
					m_oFont.SetSize(pPropertie.second, unLevel, bHardMode);
					m_oFont.UpdateSize(dFontSize);
					break;
				}
				CASE(L"font-stretch"):
				{
					m_oFont.SetStretch(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"font-style"):
				{
					m_oFont.SetStyle(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"font-variant"):
				{
					m_oFont.SetVariant(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"font-weight"):
				{
					m_oFont.SetWeight(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"font-family"):
				{
					m_oFont.SetFamily(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"line-height"):
				{
					m_oFont.SetLineHeight(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//MARGIN
				CASE(L"margin"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.SetValues(pPropertie.second, unLevel, bHardMode);
					m_oMargin.UpdateAll(dFontSize);
					break;
				}
				CASE(L"margin-top"):
				CASE(L"topmargin"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.SetTop(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"margin-right"):
				CASE(L"margin-block-end"):
				CASE(L"rightmargin"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.SetRight(pPropertie.second, unLevel, bHardMode);
					m_oMargin.UpdateRight(dFontSize);
					break;
				}
				CASE(L"margin-bottom"):
				CASE(L"bottommargin"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.SetBottom(pPropertie.second, unLevel, bHardMode);
					m_oMargin.UpdateBottom(dFontSize);
					break;
				}
				CASE(L"margin-left"):
				CASE(L"margin-block-start"):
				CASE(L"leftmargin"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.SetLeft(pPropertie.second, unLevel, bHardMode);
					m_oMargin.UpdateLeft(dFontSize);
					break;
				}
				//PADDING
				CASE(L"padding"):
				CASE(L"mso-padding-alt"):
				{
					m_oPadding.SetValues(pPropertie.second, unLevel, bHardMode);
					m_oPadding.UpdateAll(dFontSize);
					break;
				}
				CASE(L"padding-top"):
				CASE(L"mso-padding-top-alt"):
				{
					m_oPadding.SetTop(pPropertie.second, unLevel, bHardMode);
					m_oPadding.UpdateTop(dFontSize);
					break;
				}
				CASE(L"padding-right"):
				CASE(L"mso-padding-right-alt"):
				{
					m_oPadding.SetRight(pPropertie.second, unLevel, bHardMode);
					m_oPadding.UpdateRight(dFontSize);
					break;
				}
				CASE(L"padding-bottom"):
				CASE(L"mso-padding-bottom-alt"):
				{
					m_oPadding.SetBottom(pPropertie.second, unLevel, bHardMode);
					m_oPadding.UpdateBottom(dFontSize);
					break;
				}
				CASE(L"padding-left"):
				CASE(L"mso-padding-left-alt"):
				{
					m_oPadding.SetLeft(pPropertie.second, unLevel, bHardMode);
					m_oPadding.UpdateLeft(dFontSize);
					break;
				}
				// TEXT
				CASE(L"text-align"):
				{
					m_oText.SetAlign(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"text-indent"):
				{
					m_oText.SetIndent(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"text-decoration"):
				{
					m_oText.SetDecoration(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"text-color"):
				CASE(L"color"):
				{
					m_oText.SetColor(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//BORDER
				CASE(L"border"):
				CASE(L"mso-border-alt"):
				{
					m_oBorder.SetSides(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-width"):
				{
					m_oBorder.SetWidth(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-style"):
				{
					m_oBorder.SetStyle(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-color"):
				{
					m_oBorder.SetColor(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-collapse"):
				{
					m_oBorder.SetCollapse(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//BORDER TOP
				CASE(L"border-top"):
				{
					m_oBorder.SetTopSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-top-width"):
				{
					m_oBorder.SetWidthTopSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-top-style"):
				{
					m_oBorder.SetStyleTopSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-top-color"):
				{
					m_oBorder.SetColorTopSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//BORDER RIGHT
				CASE(L"border-right"):
				{
					m_oBorder.SetRightSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-right-width"):
				{
					m_oBorder.SetWidthRightSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-right-style"):
				{
					m_oBorder.SetStyleRightSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-right-color"):
				{
					m_oBorder.SetColorRightSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//BORDER bottom
				CASE(L"border-bottom"):
				{
					m_oBorder.SetBottomSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-bottom-width"):
				{
					m_oBorder.SetWidthBottomSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-bottom-style"):
				{
					m_oBorder.SetStyleBottomSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-bottom-color"):
				{
					m_oBorder.SetColorBottomSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//BORDER LEFT
				CASE(L"border-left"):
				{
					m_oBorder.SetLeftSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-left-width"):
				{
					m_oBorder.SetWidthLeftSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-left-style"):
				{
					m_oBorder.SetStyleLeftSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"border-left-color"):
				{
					m_oBorder.SetColorLeftSide(pPropertie.second, unLevel, bHardMode);
					break;
				}
				// BACKGROUND
				CASE(L"background-color"):
				{
					m_oBackground.SetColor(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"background"):
				CASE(L"bgcolor"):
				{
					m_oBackground.SetBackground(pPropertie.second, unLevel, bHardMode);
					break;
				}
				//DISPLAY
				CASE(L"display"):
				{
					m_oDisplay.SetDisplay(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"width"):
				{
					m_oDisplay.SetWidth(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"height"):
				{
					m_oDisplay.SetHeight(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"align"):
				{
					m_oDisplay.SetHAlign(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"vertical-align"):
				CASE(L"valign"):
				{
					m_oDisplay.SetVAlign(pPropertie.second, unLevel, bHardMode);
					break;
				}
				default: AddOtherStyle(pPropertie, unLevel, bHardMode);
			}
		}
	}

	void CCompiledStyle::AddStyle(const std::wstring& sStyle, const unsigned int unLevel, const bool& bHardMode)
	{
		if (sStyle.empty())
			return;

		const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sStyle, true, L":;");

		std::wstring sProperty, sValue;

		for (std::vector<std::wstring>::const_iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
		{
			if ((*iWord).back() == L':')
			{
				sProperty = *iWord;
				sProperty.pop_back();
			}
			else
			{
				sValue += *iWord;

				if (!sValue.empty() && ((*iWord).back() == L';' || iWord == (arWords.end() - 1)))
				{
					if (sValue.back() == L';')
						sValue.pop_back();

					std::transform(sProperty.begin(), sProperty.end(), sProperty.begin(), tolower);
					std::transform(sValue.begin(), sValue.end(), sValue.begin(), tolower);

					NS_STATIC_FUNCTIONS::RemoveSpaces(sProperty);
					NS_STATIC_FUNCTIONS::RemoveSpaces(sValue);

					AddPropSel(sProperty, sValue, unLevel, bHardMode);
					sProperty.clear();
					sValue.clear();
				}
			}
		}

		if (!sProperty.empty() && !sValue.empty())
			AddPropSel(sProperty, sValue, bHardMode);
	}

	void CCompiledStyle::AddParent(const std::wstring& sParentName)
	{
		if (!sParentName.empty())
			m_arParentsStyles.insert(sParentName);
	}

	std::vector<std::wstring> CCompiledStyle::GetParentsName() const
	{
		if (m_arParentsStyles.empty())
			return std::vector<std::wstring>();

		std::vector<std::wstring> arParentsName;

		for (std::set<std::wstring>::iterator iParentName = m_arParentsStyles.begin(); iParentName != m_arParentsStyles.end(); ++iParentName)
			arParentsName.push_back(*iParentName);

		return arParentsName;
	}

	std::set<std::wstring> CCompiledStyle::GetParentsNamesSet() const
	{
		return m_arParentsStyles;
	}

	void CCompiledStyle::SetID(const std::wstring& sId)
	{
		m_sId = sId;
	}

	std::wstring CCompiledStyle::GetId() const
	{
		return m_sId;
	}
	
	bool CCompiledStyle::HaveThisParent(const std::wstring &wsParentName) const
	{
		return m_arParentsStyles.end() != m_arParentsStyles.find(wsParentName);
	}
}
