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
        m_pBorder       += oElement.m_pBorder;
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

    void CCompiledStyle::AddPropSel(const std::wstring& sProperty, const std::wstring& sValue, const bool& bHardMode)
    {
        AddStyle({{sProperty, sValue}});
    }

    void CCompiledStyle::AddStyle(const std::map<std::wstring, std::wstring>& mStyle, const bool& bHardMode)
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
                    m_pFont.SetFont(ConvertUnitMeasure(pPropertie.second.c_str(), fSize));
                    break;
                }
                CASE(L"font-size"):
                CASE(L"font-size-adjust"):
                {
                    m_pFont.SetSize(ConvertUnitMeasure(pPropertie.second, fSize));
                    break;
                }

                CASE(L"font-stretch"):
                {
                    m_pFont.SetStretch(pPropertie.second);
                    break;
                }
                CASE(L"font-style"):
                {
                    m_pFont.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"font-variant"):
                {
                    m_pFont.SetVariant(pPropertie.second);
                    break;
                }
                CASE(L"font-weight"):
                {
                    m_pFont.SetWeight(pPropertie.second);
                    break;
                }
                CASE(L"line-height"):
                {
                    m_pFont.SetLineHeight(ConvertUnitMeasure(pPropertie.second, m_pFont.GetSize()));
                    break;
                }
                //MARGIN
                CASE(L"margin"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                        m_pMargin.AddMargin(sValue);
                    break;
                }
                CASE(L"margin-top"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                        m_pMargin.AddTopMargin(sValue);
                    break;
                }
                CASE(L"margin-right"):
                CASE(L"margin-block-end"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                        m_pMargin.AddRightMargin(sValue);
                    break;
                }
                CASE(L"margin-bottom"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                        m_pMargin.AddBottomMargin(sValue);
                    break;
                }
                CASE(L"margin-left"):
                CASE(L"margin-block-start"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos)
                        m_pMargin.AddLeftMargin(sValue);
                    break;
                }
                //PADDING
                CASE(L"padding"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos && sValue[0] != L'-')
                        m_pMargin.AddMargin(sValue);
                    break;
                }
                CASE(L"padding-top"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos && sValue[0] != L'-')
                        m_pMargin.AddTopMargin(sValue);
                    break;
                }
                CASE(L"padding-right"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos && sValue[0] != L'-')
                        m_pMargin.AddRightMargin(sValue);
                    break;
                }
                CASE(L"padding-bottom"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos && sValue[0] != L'-')
                        m_pMargin.AddBottomMargin(sValue);
                    break;
                }
                CASE(L"padding-left"):
                {
                    if (bIsThereBorder)
                        break;

                    const std::wstring sValue = ConvertUnitMeasure(pPropertie.second, 540.0f);
                    if (sValue.find_first_not_of(L" 0") != std::wstring::npos && sValue[0] != L'-')
                        m_pMargin.AddLeftMargin(sValue);
                    break;
                }
                // TEXT
                CASE(L"text-align"):
                {
                    m_pText.SetAlign(pPropertie.second);
                    break;
                }
                CASE(L"text-indent"):
                {
                    m_pText.SetIndent(ConvertUnitMeasure(pPropertie.second, 540.0f));
                    break;
                }
                CASE(L"text-decoration"):
                {
                    m_pText.SetDecoration(pPropertie.second);
                    break;
                }
                CASE(L"text-color"):
                CASE(L"color"):
                {
                    m_pText.SetColor(pPropertie.second);
                    break;
                }
                //BORDER
                CASE(L"border"):
                CASE(L"mso-border-alt"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() >= 0)
                    {
                        m_pBorder.stTop     = oBorderSide;
                        m_pBorder.stRight   = oBorderSide;
                        m_pBorder.stBottom  = oBorderSide;
                        m_pBorder.stLeft    = oBorderSide;
                    }
                    break;
                }
                CASE(L"border-width"):
                {
                    const float fValue = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
                    m_pBorder.SetWidth(fValue);
                    break;
                }
                CASE(L"border-style"):
                {
                    m_pBorder.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"border-color"):
                {
                    m_pBorder.SetColor(pPropertie.second);
                    break;
                }
                //BORDER TOP
                CASE(L"border-top"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() >= 0)
                        m_pBorder.stTop = oBorderSide;
                    break;
                }
                CASE(L"border-top-width"):
                {
                    m_pBorder.stTop.SetWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL));
                    break;
                }
                CASE(L"border-top-style"):
                {
                    m_pBorder.stTop.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"border-top-color"):
                {
                    m_pBorder.stTop.SetColor(pPropertie.second);
                    break;
                }
                //BORDER RIGHT
                CASE(L"border-right"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() >= 0)
                        m_pBorder.stRight = oBorderSide;

                    break;
                }
                CASE(L"border-right-width"):
                {
                    m_pBorder.stRight.SetWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL));
                    break;
                }
                CASE(L"border-right-style"):
                {
                    m_pBorder.stRight.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"border-right-color"):
                {
                    m_pBorder.stRight.SetColor(pPropertie.second);
                    break;
                }
                //BORDER bottom
                CASE(L"border-bottom"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() >= 0)
                        m_pBorder.stBottom = oBorderSide;
                    break;
                }
                CASE(L"border-bottom-width"):
                {
                    m_pBorder.stBottom.SetWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL));
                    break;
                }
                CASE(L"border-bottom-style"):
                {
                    m_pBorder.stBottom.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"border-bottom-color"):
                {
                    m_pBorder.stBottom.SetColor(pPropertie.second);
                    break;
                }
                //BORDER LEFT
                CASE(L"border-left"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.GetWidth() >= 0)
                        m_pBorder.stLeft = oBorderSide;
                    break;
                }
                CASE(L"border-left-width"):
                {
                    m_pBorder.stLeft.SetWidth(wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL));
                    break;
                }
                CASE(L"border-left-style"):
                {
                    m_pBorder.stLeft.SetStyle(pPropertie.second);
                    break;
                }
                CASE(L"border-left-color"):
                {
                    m_pBorder.stLeft.SetColor(pPropertie.second);
                    break;
                }
                // BACKGROUND
                CASE(L"background-color"):
                {
                    m_pBackground.SetColor(pPropertie.second);
                    break;
                }
                CASE(L"background"):
                {
                    m_pBackground.SetBackground(pPropertie.second);
                    break;
                }
            }
        }
    }

    void CCompiledStyle::AddStyle(const std::wstring& sStyle, const bool& bHardMode)
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
                    AddPropSel(sProperty, sValue);
                    sProperty.clear();
                    sValue.clear();
                }
            }

        if (!sProperty.empty() && !sValue.empty())
            AddPropSel(sProperty, sValue);
    }

    void CCompiledStyle::AddParent(const std::wstring& sParentName)
    {
        if (!sParentName.empty())
            m_arParentsStyles.push_back(sParentName);
    }

    std::vector<std::wstring> CCompiledStyle::GetParentsName() const
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

                if (nPosGrid != std::wstring::npos || !NS_STATIC_FUNCTIONS::ThereIsNumber(sValueTemp))
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
                    const float fValue = wcstof(sValue.c_str(), NULL) * m_pFont.GetSize();
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

