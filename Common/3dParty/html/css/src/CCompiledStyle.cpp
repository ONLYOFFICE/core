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

namespace NSCSS
{
	typedef std::map<std::wstring, std::wstring>::const_iterator styles_iterator;

	CCompiledStyle::CCompiledStyle() : m_nDpi(96), m_UnitMeasure(Default){}

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
		m_oBackground   += oElement.m_oBackground;
		m_oBorder       += oElement.m_oBorder;
		m_oFont         += oElement.m_oFont;
		m_oMargin       += oElement.m_oMargin;
		m_oPadding      += oElement.m_oPadding;
		m_oText         += oElement.m_oText;
		m_oDisplay      += oElement.m_oDisplay;

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
		return GetId()[0]        == oStyle.GetId()[0]        &&
		       m_arParentsStyles == oStyle.m_arParentsStyles &&
		       m_oBackground     == oStyle.m_oBackground     &&
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

	void CCompiledStyle::SetSizeSourceWindow(const CSizeWindow &oSizeWindow)
	{
		m_oSourceWindow = oSizeWindow;
	}

	void CCompiledStyle::SetSizeDeviceWindow(const CSizeWindow &oSizeWindow)
	{
		m_oDeviceWindow = oSizeWindow;
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
		const double dSize = m_oFont.GetSize().ToDouble();

		for (std::pair<std::wstring, std::wstring> pPropertie : mStyle)
		{
			std::transform(pPropertie.first.begin(), pPropertie.first.end(), pPropertie.first.begin(), tolower);
			SWITCH(pPropertie.first)
			{
				//FONT
				CASE(L"font"):
				{
					m_oFont.SetValue(ConvertUnitMeasure(pPropertie.second.c_str(), dSize), unLevel, bHardMode);
					break;
				}
				CASE(L"font-size"):
				CASE(L"font-size-adjust"):
				{
					m_oFont.SetSize(ConvertUnitMeasure(pPropertie.second, dSize), unLevel, bHardMode);
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

					m_oMargin.AddValue(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				CASE(L"margin-top"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.AddTop(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionY), unLevel, bHardMode);
					break;
				}
				CASE(L"margin-right"):
				CASE(L"margin-block-end"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.AddRight(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				CASE(L"margin-bottom"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.AddBottom(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionY), unLevel, bHardMode);
					break;
				}
				CASE(L"margin-left"):
				CASE(L"margin-block-start"):
				{
					if (bIsThereBorder)
						break;

					m_oMargin.AddLeft(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				//PADDING
				CASE(L"padding"):
				CASE(L"mso-padding-alt"):
				{
					m_oPadding.AddValue(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				CASE(L"padding-top"):
				CASE(L"mso-padding-top-alt"):
				{
					m_oPadding.AddTop(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionY), unLevel, bHardMode);
					break;
				}
				CASE(L"padding-right"):
				CASE(L"mso-padding-right-alt"):
				{
					m_oPadding.AddRight(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				CASE(L"padding-bottom"):
				CASE(L"mso-padding-bottom-alt"):
				{
					m_oPadding.AddBottom(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionY), unLevel, bHardMode);
					break;
				}
				CASE(L"padding-left"):
				CASE(L"mso-padding-left-alt"):
				{
					m_oPadding.AddLeft(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
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
					m_oText.SetIndent(ConvertUnitMeasure(pPropertie.second, 540.0f, ScalingDirectionX), unLevel, bHardMode);
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
					m_oBorder.SetSides(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
					break;
				}
				CASE(L"border-width"):
				{
					m_oBorder.SetWidth(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
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
				//BORDER TOP
				CASE(L"border-top"):
				{
					m_oBorder.SetTopSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
					break;
				}
				CASE(L"border-top-width"):
				{
					m_oBorder.SetWidthTopSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
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
					m_oBorder.SetRightSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
					break;
				}
				CASE(L"border-right-width"):
				{
					m_oBorder.SetWidthRightSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
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
					m_oBorder.SetBottomSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
					break;
				}
				CASE(L"border-bottom-width"):
				{
					m_oBorder.SetWidthBottomSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
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
					m_oBorder.SetLeftSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
					break;
				}
				CASE(L"border-left-width"):
				{
					m_oBorder.SetWidthLeftSide(ConvertUnitMeasure(pPropertie.second, 0.0f), unLevel, bHardMode);
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

					if (bIsThereBorder)
						m_oBackground.InBorder();

					break;
				}
				CASE(L"background"):
				{
					m_oBackground.SetBackground(pPropertie.second, unLevel, bHardMode);

					if (bIsThereBorder)
						m_oBackground.InBorder();

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
					double dParentValue = m_oDisplay.GetWidth().ToDouble();

					if (0 >= dParentValue)
						dParentValue =  m_oDeviceWindow.m_ushWidth;

					m_oDisplay.SetWidth(ConvertUnitMeasure(pPropertie.second, dParentValue, ScalingDirectionX), unLevel, bHardMode);
					break;
				}
				CASE(L"height"):
				{
					double dParentValue = m_oDisplay.GetHeight().ToDouble();

					if (0 >= dParentValue)
						dParentValue =  m_oDeviceWindow.m_ushHeight;

					m_oDisplay.SetHeight(ConvertUnitMeasure(pPropertie.second, dParentValue, ScalingDirectionY), unLevel, bHardMode);
					break;
				}
				CASE(L"align"):
				{
					m_oDisplay.SetHAlign(pPropertie.second, unLevel, bHardMode);
					break;
				}
				CASE(L"vertical-align"):
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

		const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsWithSigns(sStyle, L":;");

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

				if (L' ' == sValue.front())
					    sValue.erase(0, 1);

				if (!sValue.empty() && ((*iWord).back() == L';' || iWord == (arWords.end() - 1)))
				{
					if (sValue.back() == L';')
						sValue.pop_back();

					std::transform(sProperty.begin(), sProperty.end(), sProperty.begin(), tolower);
					std::transform(sValue.begin(), sValue.end(), sValue.begin(), tolower);
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

	void CCompiledStyle::SetID(const std::wstring& sId)
	{
		m_sId = sId;
	}

	std::wstring CCompiledStyle::GetId() const
	{
		return m_sId;
	}

	std::wstring CCompiledStyle::ConvertUnitMeasure(const std::wstring &sValue, const float& fPreviousValue, ScalingDirection enScalingDirection) const
	{
		if (sValue.empty())
			return sValue;

		std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsWithSigns(sValue);

		std::wstring sValueString;

		for (std::wstring& sValueTemp : arValues)
		{
			std::transform(sValueTemp.begin(), sValueTemp.end(), sValueTemp.begin(), tolower);

			if (sValueTemp == L"important")
			{
				sValueString += L"!important";
				continue;
			}

			size_t nPosGrid = sValueTemp.find(L'#');

			if (nPosGrid != std::wstring::npos || !NS_STATIC_FUNCTIONS::NumberInWString(sValueTemp))
			{
				if (!NS_STATIC_FUNCTIONS::ConvertAbsoluteValue(sValueTemp, fPreviousValue))
				{
					sValueString += sValueTemp;
					continue;
				}
			}

			const size_t posPercent = sValueTemp.find(L'%');

			if (posPercent != std::wstring::npos)
			{
				const float dValue = wcstof(sValueTemp.substr(0, posPercent).c_str(), NULL) * fPreviousValue / 100;

				sValueString += std::to_wstring(static_cast<short int>(dValue + 0.5f));

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"px") != std::wstring::npos)
			{
				int nValue = ConvertPx(sValueTemp);

				Scale(nValue, enScalingDirection);

				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"cm") != std::wstring::npos)
			{
				int nValue = ConvertCm(sValueTemp);

				Scale(nValue, enScalingDirection);

				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"mm") != std::wstring::npos)
			{
				int nValue = ConvertMm(sValueTemp);
				std::wcout << sValueTemp << L" -> " << nValue;
				Scale(nValue, enScalingDirection);
				std::wcout << L" -> " << nValue << std::endl;
				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"in") != std::wstring::npos)
			{
				int nValue = ConvertIn(sValueTemp);

				Scale(nValue, enScalingDirection);

				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"pt") != std::wstring::npos)
			{
				int nValue = ConvertPt(sValueTemp);

				Scale(nValue, enScalingDirection);

				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"pc") != std::wstring::npos)
			{
				int nValue = ConvertPc(sValueTemp);

				Scale(nValue, enScalingDirection);

				sValueString += std::to_wstring(nValue);

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else if (sValueTemp.find(L"em") != std::wstring::npos)
			{
				const float fValue = wcstof(sValueTemp.c_str(), NULL) * m_oFont.GetSize().ToDouble();

				sValueString += std::to_wstring(static_cast<short int>(fValue + 0.5f));

				if (sValueTemp.find(L';') != std::wstring::npos)
					sValueString += L';';
				else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
					sValueString += L' ';
			}
			else
			{
				if (iswdigit(sValueTemp[0]))
				{
					int nValue = static_cast<int>(wcstof(sValueTemp.c_str(), NULL) + 0.5f);

					Scale(nValue, enScalingDirection);

					sValueString += std::to_wstring(nValue);
				}
				else
					sValueString += sValueTemp;

				if (sValueTemp.find(L";") != std::wstring::npos)
					sValueString += L';';

				continue;
			}

			if (sValueTemp.back() != L';' && sValueTemp.back() != L':' && sValueTemp.back() != L' ')
				sValueTemp += L' ';
		}

		return sValueString;
	}

	void CCompiledStyle::Scale(int &nValue, ScalingDirection enScalingDirection) const
	{
		    if (ScalingDirectionNone == enScalingDirection)
				    return;
			else if (ScalingDirectionX == enScalingDirection && 0 != m_oSourceWindow.m_ushWidth &&
			    0 != m_oDeviceWindow.m_ushWidth && m_oSourceWindow.m_ushWidth != m_oDeviceWindow.m_ushWidth)
			{
				    nValue = static_cast<int>((double)nValue / m_oSourceWindow.m_ushWidth * m_oDeviceWindow.m_ushWidth + 0.5f);
			}
			else if (ScalingDirectionY == enScalingDirection && 0 != m_oSourceWindow.m_ushHeight &&
			         0 != m_oDeviceWindow.m_ushHeight && m_oSourceWindow.m_ushHeight != m_oDeviceWindow.m_ushHeight)
			{
				    nValue = static_cast<int>((double)nValue / m_oSourceWindow.m_ushHeight * m_oDeviceWindow.m_ushHeight + 0.5f);
			}
	}

	inline int CCompiledStyle::ConvertPx(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"px") - 1);
		const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Pixel:
				return static_cast<int>(dValue);
			case Default:
			case Point:
				return ConvertPxToPt(dValue);
			case Cantimeter:
				return ConvertPxToCm(dValue);
			case Millimeter:
				return ConvertPxToMm(dValue);
			case Inch:
				return ConvertPxToIn(dValue);
			case Peak:
				return ConvertPxToPc(dValue);
		}

		return 0;
	}

	inline int CCompiledStyle::ConvertPxToCm(const float& dValue) const
	{
		return static_cast<int>(dValue / m_nDpi * 2.54f);
	}

	inline int CCompiledStyle::ConvertPxToIn(const float& dValue) const
	{
		return static_cast<int>(1.0f / static_cast<float>(m_nDpi) * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertPxToMm(const float& dValue) const
	{
		return static_cast<int>(dValue / static_cast<float>(m_nDpi) * 25.4f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPxToPc(const float& dValue) const
	{
		return static_cast<int>(0.16667f / static_cast<float>(m_nDpi) * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertPxToPt(const float& dValue) const
	{
		return static_cast<int>(72.0f /  static_cast<float>(m_nDpi) * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertCm(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"cm") - 1);
		const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Default:
			case Point:
				return ConvertCmToPt(dValue);
			case Pixel:
				return ConvertCmToPx(dValue);
			case Cantimeter:
				return static_cast<int>(dValue);
			case Millimeter:
				return ConvertCmToMm(dValue);
			case Inch:
				return ConvertCmToIn(dValue);
			case Peak:
				return ConvertCmToPc(dValue);
		}

		return 0;
	}

	inline int CCompiledStyle::ConvertCmToIn(const float& dValue) const
	{
		return static_cast<int>(dValue / 2.54f + 0.5f);
	}

	inline int CCompiledStyle::ConvertCmToMm(const float& dValue) const
	{
		return static_cast<int>(dValue * 10.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertCmToPc(const float& dValue) const
	{
		return static_cast<int>(2.36f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertCmToPt(const float& dValue) const
	{
		return static_cast<int>(28.35f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertCmToPx(const float& dValue) const
	{
		return static_cast<int>(static_cast<float>(m_nDpi) / 2.54f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertMm(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"mm") - 1);
		const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Pixel:
				return ConvertMmToPx(dValue);
			case Default:
			case Point:
				return ConvertMmToPt(dValue);
			case Cantimeter:
				return ConvertMmToCm(dValue);
			case Millimeter:
				return static_cast< int>(dValue);
			case Inch:
				return ConvertMmToIn(dValue);
			case Peak:
				return ConvertMmToPc(dValue);
		}
		return 0;
	}

	inline int CCompiledStyle::ConvertMmToIn(const float& dValue) const
	{
		return static_cast<int>(dValue / 25.4f + 0.5f);
	}

	inline int CCompiledStyle::ConvertMmToCm(const float& dValue) const
	{
		return static_cast<int>(dValue / 10.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertMmToPc(const float& dValue) const
	{
		return static_cast<int>(2.8346f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertMmToPt(const float& dValue) const
	{
		return static_cast<int>(0.23262f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertMmToPx(const float& dValue) const
	{
		return static_cast<int>(static_cast<float>(m_nDpi) / 25.4f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertIn(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"in") - 1);
		const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Pixel:
				return  ConvertInToPx(dValue);
			case Default:
			case Point:
				return ConvertInToPt(dValue);
			case Cantimeter:
				return ConvertInToCm(dValue);
			case Millimeter:
				return ConvertInToMm(dValue);
			case Inch:
				return static_cast<int>(dValue);
			case Peak:
				return ConvertInToPc(dValue);
		}
		return 0;
	}

	inline int CCompiledStyle::ConvertInToMm(const float& dValue) const
	{
		return static_cast<int>(dValue * 25.4f + 0.5f);
	}

	inline int CCompiledStyle::ConvertInToCm(const float& dValue) const
	{
		return static_cast<int>(dValue * 2.54f + 0.5f);
	}

	inline int CCompiledStyle::ConvertInToPc(const float& dValue) const
	{
		return static_cast<int>(dValue / 72.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertInToPt(const float& dValue) const
	{
		return static_cast<int>(dValue / 6.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertInToPx(const float& dValue) const
	{
		return static_cast<short int>(dValue * static_cast<float>(m_nDpi) + 0.5f);
	}

	inline int CCompiledStyle::ConvertPt(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pt") - 1);
		float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Pixel:
				return ConvertPtToPx(dValue);
			case Default:
			case Point:
				return static_cast<int>(dValue + 0.5f);
			case Cantimeter:
				return ConvertPtToCm(dValue);
			case Millimeter:
				return ConvertPtToMm(dValue);
			case Inch:
				return ConvertPtToIn(dValue);
			case Peak:
				return ConvertPtToPc(dValue);
		}

		return 0;
	}

	inline int CCompiledStyle::ConvertPtToIn(const float& dValue) const
	{
		return static_cast<int>(dValue / 72.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPtToCm(const float& dValue) const
	{
		return static_cast<int>(dValue * 0.03528f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPtToPc(const float& dValue) const
	{
		return static_cast<int>(dValue / 12.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPtToMm(const float& dValue) const
	{
		return static_cast<int>(dValue * 0.3528f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPtToPx(const float& dValue) const
	{
		return static_cast<short int>(static_cast<float>(m_nDpi) / 72.0f * dValue + 0.5f);
	}

	inline int CCompiledStyle::ConvertPc(const std::wstring& sValue) const
	{
		if (sValue.empty())
			return 0;

		const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pc") - 1);
		const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

		switch (m_UnitMeasure)
		{
			case Pixel:
				return ConvertPcToPx(dValue);
			case Default:
			case Point:
				return ConvertPcToPt(dValue);
			case Cantimeter:
				return ConvertPcToCm(dValue);
			case Millimeter:
				return ConvertPcToMm(dValue);
			case Inch:
				return ConvertPcToIn(dValue);
			case Peak:
				return static_cast<int>(dValue);
		}

		return 0;
	}

	inline int CCompiledStyle::ConvertPcToIn(const float& dValue) const
	{
		return static_cast<int>(dValue / 6.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPcToCm(const float& dValue) const
	{
		return static_cast<int>(dValue * 0.423f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPcToPt(const float& dValue) const
	{
		return static_cast<int>(dValue * 12.0f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPcToMm(const float& dValue) const
	{
		return static_cast<int>(dValue * 4.23f + 0.5f);
	}

	inline int CCompiledStyle::ConvertPcToPx(const float& dValue) const
	{
		return static_cast<int>(static_cast<float>(m_nDpi) / 6.0f * dValue + 0.5f);
	}
}

