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
        m_pFont(oStyle.m_pFont), m_pMargin(oStyle.m_pMargin), m_pBackground(oStyle.m_pBackground),
        m_pText(oStyle.m_pText), m_pBorder(oStyle.m_pBorder){}

    CCompiledStyle::~CCompiledStyle()
    {
        m_arParentsStyles.clear();
    }


    CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
    {
        m_pBackground   += oElement.m_pBackground;
        m_pBorder        = oElement.m_pBorder;
        m_pFont         += oElement.m_pFont;
        m_pMargin       += oElement.m_pMargin;
        m_pText         += oElement.m_pText;

        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator= (const CCompiledStyle &oElement)
    {
        m_sId               = oElement.m_sId;
        m_arParentsStyles   = oElement.m_arParentsStyles;

        m_nDpi          = oElement.m_nDpi;
        m_UnitMeasure   = oElement.m_UnitMeasure;

        m_pBackground   = oElement.m_pBackground;
        m_pBorder       = oElement.m_pBorder;
        m_pFont         = oElement.m_pFont;
        m_pMargin       = oElement.m_pMargin;
        m_pText         = oElement.m_pText;

        return *this;
    }

    bool CCompiledStyle::operator== (const CCompiledStyle& oStyle) const
    {
        return GetId()[0]        == oStyle.GetId()[0]        &&
               m_arParentsStyles == oStyle.m_arParentsStyles &&
               m_pBackground     == oStyle.m_pBackground     &&
               m_pBorder         == oStyle.m_pBorder         &&
               m_pFont           == oStyle.m_pFont           &&
               m_pMargin         == oStyle.m_pMargin         &&
                m_pText           == oStyle.m_pText;
    }

    void CCompiledStyle::StyleEquation(CCompiledStyle &oFirstStyle, CCompiledStyle &oSecondStyle)
    {
        NSConstValues::NSCssProperties::Font::FontEquation(oFirstStyle.m_pFont, oSecondStyle.m_pFont);
        NSConstValues::NSCssProperties::Margin::MarginEquation(oFirstStyle.m_pMargin, oSecondStyle.m_pMargin);
        NSConstValues::NSCssProperties::Background::BackgroundEquation(oFirstStyle.m_pBackground, oSecondStyle.m_pBackground);
        NSConstValues::NSCssProperties::Text::TextEquation(oFirstStyle.m_pText, oSecondStyle.m_pText);
        NSConstValues::NSCssProperties::Border::BorderEquation(oFirstStyle.m_pBorder, oSecondStyle.m_pBorder);

        oFirstStyle.ClearImportants();
        oSecondStyle.ClearImportants();
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
        return m_pBackground.Empty() && m_pBorder.Empty() &&
               m_pFont.Empty() && m_pMargin.Empty() && m_pText.Empty();
    }

    void CCompiledStyle::AddPropSel(const std::wstring& sProperty, const std::wstring& sValue, const unsigned int unLevel, const bool& bHardMode)
    {
        AddStyle({{sProperty, sValue}}, unLevel, bHardMode);
    }

    void CCompiledStyle::AddStyle(const std::map<std::wstring, std::wstring>& mStyle, const unsigned int unLevel, const bool& bHardMode)
    {
        const bool bIsThereBorder = (m_pBorder.Empty()) ? false : true;
        const float fSize = m_pFont.GetSize();

        for (std::pair<std::wstring, std::wstring> pPropertie : mStyle)
        {
            std::transform(pPropertie.first.begin(), pPropertie.first.end(), pPropertie.first.begin(), tolower);
            SWITCH(pPropertie.first)
            {
                //FONT
                CASE(L"font"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetFont(ConvertUnitMeasure(pPropertie.second.c_str(), fSize), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetFont(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), fSize), unLevel, true);
                        m_pFont.SetImportantAll(true);
                    }
                    break;
                }
                CASE(L"font-size"):
                CASE(L"font-size-adjust"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetSize(ConvertUnitMeasure(pPropertie.second, fSize), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetSize(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), fSize), unLevel, true);
                        m_pFont.SetImportantSize(true);
                    }
                    break;
                }

                CASE(L"font-stretch"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetStretch(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetStretch(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pFont.SetImportantenStretch(true);
                    }
                    break;
                }
                CASE(L"font-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pFont.SetImportantenStyle(true);
                    }
                    break;
                }
                CASE(L"font-variant"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetVariant(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetVariant(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pFont.SetImportantenVariant(true);
                    }
                    break;
                }
                CASE(L"font-weight"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetWeight(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetWeight(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pFont.SetImportantenWeight(true);
                    }
                    break;
                }
                CASE(L"font-family"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pFont.SetFontFamily(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pFont.SetFontFamily(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pFont.SetImportantenFamily(true);
                    }
                    break;
                }
                CASE(L"line-height"):
                {
                    size_t unCoefficient = 1;
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    const size_t unPositionSymbol = pPropertie.second.find_first_of(L"abcdefghijklmnopqrstuvwxyz%");

                    if (unPositionSymbol == std::wstring::npos)
                        unCoefficient = m_pFont.GetSize();

                    if (unPositionImp == std::wstring::npos)
                    {
                        const float fValue = wcstof(ConvertUnitMeasure(pPropertie.second, m_pFont.GetSize()).c_str(), NULL);
                        m_pFont.SetLineHeight(std::to_wstring(fValue * unCoefficient), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const float fValue = wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), m_pFont.GetSize()).c_str(), NULL);
                        m_pFont.SetLineHeight(std::to_wstring(fValue * unCoefficient), unLevel, true);
                        m_pFont.SetImportantenLineHeight(true);
                    }
                    break;
                }
                //MARGIN
                CASE(L"margin"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantAll(true);
                    }

                    break;
                }
                CASE(L"margin-top"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddTopMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddTopMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantTopSide(true);
                    }
                    break;
                }
                CASE(L"margin-right"):
                CASE(L"margin-block-end"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddRightMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddRightMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantRightSide(true);
                    }
                    break;
                }
                CASE(L"margin-bottom"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddBottomMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddBottomMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantBottomSide(true);
                    }
                    break;
                }
                CASE(L"margin-left"):
                CASE(L"margin-block-start"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddLeftMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddLeftMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantLeftSide(true);
                    }
                    break;
                }
                //PADDING
                CASE(L"padding"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantAll(true);
                    }

                    break;
                }
                CASE(L"padding-top"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddTopMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddTopMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantTopSide(true);
                    }

                    break;
                }
                CASE(L"padding-right"):
                {
                    if (bIsThereBorder)
                        break;


                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddRightMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddRightMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantRightSide(true);
                    }

                    break;
                }
                CASE(L"padding-bottom"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddBottomMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddBottomMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantBottomSide(true);
                    }

                    break;
                }
                CASE(L"padding-left"):
                {
                    if (bIsThereBorder)
                        break;

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddLeftMargin(sValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const std::wstring sValue = ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f);
                        if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                            m_pMargin.AddLeftMargin(sValue, unLevel, true);

                        m_pMargin.SetImportantLeftSide(true);
                    }

                    break;
                }
                // TEXT
                CASE(L"text-align"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pText.SetAlign(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pText.SetAlign(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pText.SetImportantAlign(true);
                    }

                    break;
                }
                CASE(L"text-indent"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pText.SetIndent(ConvertUnitMeasure(pPropertie.second, 540.0f), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pText.SetIndent(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 540.0f), unLevel, true);
                        m_pText.SetImportantIndent(true);
                    }

                    break;
                }
                CASE(L"text-decoration"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pText.SetDecoration(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pText.SetDecoration(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pText.SetImportantDecoration(true);
                    }

                    break;
                }
                CASE(L"text-color"):
                CASE(L"color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pText.SetColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pText.SetColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pText.SetImportantColor(true);
                    }
                    break;
                }
                //BORDER
                CASE(L"border"):
                CASE(L"mso-border-alt"):
                {
                    NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));

                    if (oBorderSide.GetWidth() < 0)
                        break;

                    oBorderSide.SetAllLevels(unLevel);

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pText.SetAlign(pPropertie.second, bHardMode);

                        m_pBorder.SetTopSide(oBorderSide, bHardMode);
                        m_pBorder.SetRightSide(oBorderSide, bHardMode);
                        m_pBorder.SetBottomSide(oBorderSide, bHardMode);
                        m_pBorder.SetLeftSide(oBorderSide, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        oBorderSide.SetImportantAll(true);
                        m_pBorder.SetTopSide(oBorderSide, true);
                        m_pBorder.SetRightSide(oBorderSide, true);
                        m_pBorder.SetBottomSide(oBorderSide, true);
                        m_pBorder.SetLeftSide(oBorderSide, true);
                    }

                    break;
                }
                CASE(L"border-width"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        const float fValue = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
                        m_pBorder.SetWidth(fValue, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        const float fValue = wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL);
                        m_pBorder.SetWidth(fValue, unLevel, true);
                        m_pBorder.SetImportantWidth(true);
                    }

                    break;
                }
                CASE(L"border-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantStyle(true);
                    }

                    break;
                }
                CASE(L"border-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantColor(true);
                    }

                    break;
                }
                //BORDER TOP
                CASE(L"border-top"):
                {
                    NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() < 0)
                        break;

                    oBorderSide.SetAllLevels(unLevel);

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetTopSide(oBorderSide, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        oBorderSide.SetImportantAll(true);
                        m_pBorder.SetTopSide(oBorderSide, true);
                    }

                    break;
                }
                CASE(L"border-top-width"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetTopSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetTopSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL), unLevel, true);
                        m_pBorder.SetImportantWidthTopSide(true);
                    }

                    break;
                }
                CASE(L"border-top-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetTopSideStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetTopSideStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantStyleTopSide(true);
                    }

                    break;
                }
                CASE(L"border-top-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetTopSideColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetTopSideColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantColorTopSide(true);
                    }

                    break;
                }
                //BORDER RIGHT
                CASE(L"border-right"):
                {
                    NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));

                    if (oBorderSide.GetWidth() < 0)
                        break;

                    oBorderSide.SetAllLevels(unLevel);

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetRightSide(oBorderSide, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        oBorderSide.SetImportantAll(true);
                        m_pBorder.SetRightSide(oBorderSide, true);
                    }

                    break;
                }
                CASE(L"border-right-width"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetRightSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetRightSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL), unLevel, true);
                        m_pBorder.SetImportantWidthRightSide(true);
                    }

                    break;
                }
                CASE(L"border-right-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetRightSideStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetRightSideStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantStyleRightSide(true);
                    }

                    break;
                }
                CASE(L"border-right-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetRightSideColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetRightSideColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantColorRightSide(true);
                    }

                    break;
                }
                //BORDER bottom
                CASE(L"border-bottom"):
                {
                    NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));

                    if (oBorderSide.GetWidth() < 0)
                        break;

                    oBorderSide.SetAllLevels(unLevel);

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetBottomSide(oBorderSide, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        oBorderSide.SetImportantAll(true);
                        m_pBorder.SetBottomSide(oBorderSide, true);
                    }

                    break;
                }
                CASE(L"border-bottom-width"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetBottomSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetBottomSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL), unLevel, true);
                        m_pBorder.SetImportantWidthBottomSide(true);
                    }

                    break;
                }
                CASE(L"border-bottom-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetBottomSideStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetBottomSideStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantStyleBottomSide(true);
                    }

                    break;
                }
                CASE(L"border-bottom-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetBottomSideColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetBottomSideColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantColorBottomSide(true);
                    }

                    break;
                }
                //BORDER LEFT
                CASE(L"border-left"):
                {
                    NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));

                    if (oBorderSide.GetWidth() < 0)
                        break;

                    oBorderSide.SetAllLevels(unLevel);

                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetLeftSide(oBorderSide, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        oBorderSide.SetImportantAll(true);
                        m_pBorder.SetLeftSide(oBorderSide, true);
                    }

                    break;
                }
                CASE(L"border-left-width"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetLeftSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL), unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetLeftSideWidth(wcstof(ConvertUnitMeasure(pPropertie.second.substr(0, unPositionImp - 1), 0.0f).c_str(), NULL), unLevel, true);
                        m_pBorder.SetImportantWidthLeftSide(true);
                    }

                    break;
                }
                CASE(L"border-left-style"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetLeftSideStyle(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetLeftSideStyle(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantStyleLeftSide(true);
                    }

                    break;
                }
                CASE(L"border-left-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBorder.SetLeftSideColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBorder.SetLeftSideColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBorder.SetImportantColorLeftSide(true);
                    }

                    break;
                }
                // BACKGROUND
                CASE(L"background-color"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBackground.SetColor(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBackground.SetColor(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBackground.SetImportantBackground(true);
                    }

                    if (bIsThereBorder)
                        m_pBackground.InBorder();

                    break;
                }
                CASE(L"background"):
                {
                    const size_t unPositionImp = pPropertie.second.find(L"!i");
                    if (unPositionImp == std::wstring::npos)
                    {
                        m_pBackground.SetBackground(pPropertie.second, unLevel, bHardMode);
                    }
                    else if (unPositionImp != 0)
                    {
                        m_pBackground.SetBackground(pPropertie.second.substr(0, unPositionImp - 1), unLevel, true);
                        m_pBackground.SetImportantAll(true);
                    }

                    if (bIsThereBorder)
                        m_pBackground.InBorder();

                    break;
                }
            }
        }
    }

    void CCompiledStyle::AddStyle(const std::wstring& sStyle, const unsigned int unLevel, const bool& bHardMode)
    {
        if (sStyle.empty())
            return;

        const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsWithSigns(sStyle, L" :;");

        std::wstring sProperty, sValue;

        for (std::vector<std::wstring>::const_iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
            if ((*iWord).back() == L':')
            {
                sProperty = *iWord;
                sProperty.pop_back();
            }
            else
            {
                sValue += *iWord;
                if ((*iWord).back() == L';')
                {
                    sValue.pop_back();
                    std::transform(sProperty.begin(), sProperty.end(), sProperty.begin(), tolower);
                    std::transform(sValue.begin(), sValue.end(), sValue.begin(), tolower);
                    AddPropSel(sProperty, sValue, unLevel, bHardMode);
                    sProperty.clear();
                    sValue.clear();
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

    void CCompiledStyle::ClearImportants()
    {
        m_pBackground   .ClearImportants();
        m_pBorder       .ClearImportants();
        m_pFont         .ClearImportants();
        m_pMargin       .ClearImportants();
        m_pText         .ClearImportants();
    }

        std::wstring CCompiledStyle::ConvertUnitMeasure(const std::wstring &sValue, const float& fPreviousValue) const
        {
            if (sValue.empty())
                return sValue;

            std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsWithSigns(sValue);

            std::wstring sValueString;

            for (std::wstring& sValueTemp : arValues)
            {
                const size_t nPosImportant = sValueTemp.find(L'!');
                if (nPosImportant != std::wstring::npos)
                    sValueTemp = sValueTemp.substr(0, nPosImportant);

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
                    sValueString += ConvertPx(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"cm") != std::wstring::npos)
                {
                    sValueString += ConvertCm(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"mm") != std::wstring::npos)
                {
                    sValueString += ConvertMm(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"in") != std::wstring::npos)
                {
                    sValueString += ConvertIn(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"pt") != std::wstring::npos)
                {
                    sValueString += ConvertPt(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"pc") != std::wstring::npos)
                {
                    sValueString += ConvertPc(sValueTemp);

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else if (sValueTemp.find(L"em") != std::wstring::npos)
                {
                    const float fValue = wcstof(sValueTemp.c_str(), NULL) * m_pFont.GetSize();
                    sValueString += std::to_wstring(static_cast<short int>(fValue + 0.5f));

                    if (sValueTemp.find(L';') != std::wstring::npos)
                        sValueString += L';';
                    else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                        sValueString += L' ';
                }
                else
                {
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

        inline std::wstring CCompiledStyle::ConvertPx(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

            const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"px") - 1);
            const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

            switch (m_UnitMeasure)
            {
                case Pixel:
                    return std::to_wstring(static_cast<short int>(dValue));;
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

            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertPxToCm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / static_cast<float>(m_nDpi) * 2.54f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPxToIn(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(1.0f / static_cast<float>(m_nDpi) * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPxToMm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / static_cast<float>(m_nDpi) * 25.4f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPxToPc(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(0.16667f / static_cast<float>(m_nDpi) * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPxToPt(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(72.0f /  static_cast<float>(m_nDpi) * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertCm(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

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
                    return std::to_wstring(static_cast<short int>(dValue));;
                case Millimeter:
                    return ConvertCmToMm(dValue);
                case Inch:
                    return ConvertCmToIn(dValue);
                case Peak:
                    return ConvertCmToPc(dValue);
            }

            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertCmToIn(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 2.54f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertCmToMm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 10.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertCmToPc(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(2.36f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertCmToPt(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(28.35f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertCmToPx(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 2.54f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertMm(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

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
                    return std::to_wstring(static_cast<short int>(dValue));;
                case Inch:
                    return ConvertMmToIn(dValue);
                case Peak:
                    return ConvertMmToPc(dValue);
            }
            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertMmToIn(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 25.4f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertMmToCm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 10.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertMmToPc(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(2.8346f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertMmToPt(const float& dValue) const
        {
            if (dValue == 0)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(0.23262f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertMmToPx(const float& dValue) const
        {
            if (dValue == 0)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 25.4f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertIn(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

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
                    return std::to_wstring(static_cast<short int>(dValue));;
                case Peak:
                    return ConvertInToPc(dValue);
            }
            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertInToMm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 25.4f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertInToCm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 2.54f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertInToPc(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 72.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertInToPt(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 6.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertInToPx(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * static_cast<float>(m_nDpi) + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPt(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

            const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pt") - 1);
            const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

            switch (m_UnitMeasure)
            {
                case Pixel:
                    return ConvertPtToPx(dValue);
                case Default:
                case Point:
                    return std::to_wstring(static_cast<short int>(dValue));
                case Cantimeter:
                    return ConvertPtToCm(dValue);
                case Millimeter:
                    return ConvertPtToMm(dValue);
                case Inch:
                    return ConvertPtToIn(dValue);
                case Peak:
                    return ConvertPtToPc(dValue);
            }

            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertPtToIn(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring((dValue / 72.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPtToCm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 0.03528f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPtToPc(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 12.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPtToMm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring((dValue * 0.3528f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPtToPx(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 72.0f * dValue + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPc(const std::wstring& sValue) const
        {
            if (sValue.empty())
                return std::wstring();

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
                    return std::to_wstring(static_cast<short int>(dValue));
            }

            return std::wstring();
        }

        inline std::wstring CCompiledStyle::ConvertPcToIn(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue / 6.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPcToCm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 0.423f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPcToPt(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 12.0f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPcToMm(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(dValue * 4.23f + 0.5f));
        }

        inline std::wstring CCompiledStyle::ConvertPcToPx(const float& dValue) const
        {
            if (dValue == 0.0f)
                return std::wstring(L"0");

            return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 6.0f * dValue + 0.5f));
        }
    }

