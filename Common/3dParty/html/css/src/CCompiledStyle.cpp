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
                    std::wcout << pPropertie.second << L" -> " << m_pText.GetColor() << std::endl;
                    break;
                }
                //BORDER
                CASE(L"border"):
                CASE(L"mso-border-alt"):
                {
                    const NSConstValues::NSCssProperties::BorderSide oBorderSide = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    if (oBorderSide.fWidth > 0)
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
                    m_pBorder.stTop = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    break;
                }
                CASE(L"border-top-width"):
                {
                    m_pBorder.stTop.fWidth = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
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
                    m_pBorder.stRight = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    break;
                }
                CASE(L"border-right-width"):
                {
                    m_pBorder.stRight.fWidth = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
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
                    m_pBorder.stBottom = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    break;
                }
                CASE(L"border-bottom-width"):
                {
                    m_pBorder.stBottom.fWidth = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
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
                    m_pBorder.stLeft = NSConstValues::NSCssProperties::BorderSide::GetCorrectSide(ConvertUnitMeasure(pPropertie.second, 0.0f));
                    break;
                }
                CASE(L"border-left-width"):
                {
                    m_pBorder.stLeft.fWidth = wcstof(ConvertUnitMeasure(pPropertie.second, 0.0f).c_str(), NULL);
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

    /*
    const std::map<std::wstring, std::wstring>::iterator& CCompiledStyle::GetBegin()
    {
        return m_mStyle.begin();
    }

    const std::map<std::wstring, std::wstring>::iterator& CCompiledStyle::GetEnd()
    {
        return m_mStyle.end();
    }

    double CCompiledStyle::GetWeidth() const
    {
        double dWidth = 0.0;
        dWidth = std::accumulate(m_mStyle.begin(), m_mStyle.end(), dWidth,
            [] (double dW, const std::pair<std::wstring, std::wstring>& sValue) { return dW + sValue.first.length() + sValue.second.length(); });
        dWidth = std::accumulate(m_arParentsStyles.begin(), m_arParentsStyles.end(), dWidth,
            [] (double dW, const std::wstring& sValue) { return dW + sValue.length() / 2; });
        return dWidth;
    }
    */

//    /* FONT */

//        std::wstring CCompiledStyle::GetFont() const
//        {
//            styles_iterator oFont = m_mStyle.find(L"font");
//            if (oFont != m_mStyle.end())
//                return oFont->second;

//            std::wstring sValue = GetFontStyle()   + L" " +
//                                  GetFontVariant() + L" " +
//                                  GetFontWeight()  + L" " +
//                                  GetFontSize()    + L"/" +
//                                  GetLineHeight()  + L" " +
//                                  GetFontFamily();

//            return sValue.length() == 5 ? std::wstring() : sValue;
//        }

//        std::wstring CCompiledStyle::GetFontFamily() const
//        {
//            styles_iterator oFontFamily = m_mStyle.find(L"font-family");

//            if (oFontFamily != m_mStyle.end())
//            {
//                const std::wstring& sFont = oFontFamily->second;
//                size_t nLeftQuote = sFont.find_first_of(L"'\"");
//                while (nLeftQuote != std::wstring::npos)
//                {
//                    const size_t nRightQuote = sFont.find_first_of(L"'\"", nLeftQuote + 1);
//                    const size_t nNewLeftQuote = sFont.find_first_of(L"'\"", nRightQuote + 1);
//                    if (nNewLeftQuote == std::wstring::npos)
//                        return sFont.substr(nLeftQuote, nRightQuote - nLeftQuote + 1);
//                    nLeftQuote = nNewLeftQuote;
//                }
//            }

//            std::wstring sFont;
//            styles_iterator oFont = m_mStyle.find(L"font");

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            size_t nLeftQuote = sFont.find_first_of(L"'\"");
//            while (nLeftQuote != std::wstring::npos)
//            {
//                const size_t nRightQuote = sFont.find_first_of(L"'\"", nLeftQuote + 1);
//                const size_t nNewLeftQuote = sFont.find_first_of(L"'\"", nRightQuote + 1);
//                if (nNewLeftQuote == std::wstring::npos)
//                    return sFont.substr(nLeftQuote, nRightQuote - nLeftQuote + 1);
//                nLeftQuote = nNewLeftQuote;
//            }

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetFontSize() const
//        {
//            styles_iterator oFontSize = m_mStyle.find(L"font-size");

//            if (oFontSize != m_mStyle.end())
//                return oFontSize->second;

//            std::wstring sFont;

//            styles_iterator oFont = m_mStyle.find(L"font");

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFont);

//            std::wstring sValue;

//            for (const std::wstring& sWord : arWords)
//                if (isdigit(sWord[0]))
//                    sValue = sWord;
//                else if (sWord == L"/")
//                    return sValue;

//            return sValue;
//        }

//        std::wstring CCompiledStyle::GetFontSizeAdjust() const
//        {
//            styles_iterator oFontSizeAdj = m_mStyle.find(L"font-size-adjust");
//            return oFontSizeAdj != m_mStyle.end() ? oFontSizeAdj->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetFontStretch() const
//        {
//            styles_iterator oFontStretch = m_mStyle.find(L"font-stretch");
//            return oFontStretch != m_mStyle.end() ? oFontStretch->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetFontStyle() const
//        {
//            styles_iterator oFontStyle = m_mStyle.find(L"font-style");

//            if (oFontStyle != m_mStyle.end())
//                return oFontStyle->second;

//            std::wstring sFont;

//            styles_iterator oFont = m_mStyle.find(L"font");

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFont);

//            for (const std::wstring& sWord : arWords)
//                if (sWord == L"italic" || sWord == L"oblique")
//                    return sWord;

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetFontVariant() const
//        {
//            styles_iterator oFontVariant = m_mStyle.find(L"font-variant");

//            if (oFontVariant != m_mStyle.end())
//                return oFontVariant->second;

//            styles_iterator oFont = m_mStyle.find(L"font");
//            std::wstring sFont;

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFont);

//            for (const std::wstring& sWord : arWords)
//                if (sWord == L"small-caps")
//                    return sWord;

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetFontWeight() const
//        {
//            styles_iterator oFontWeight = m_mStyle.find(L"font-weight");

//            if (oFontWeight != m_mStyle.end())
//                return oFontWeight->second;

//            styles_iterator oFont = m_mStyle.find(L"font");
//            std::wstring sFont;

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFont);

//            for (const std::wstring& sWord : arWords)
//                if (sWord == L"bold" || sWord == L"bolder" || L"lighter" || (sWord.length() == 3 && isdigit(sWord[0]) && isdigit(sWord[1]) && isdigit(sWord[2])))
//                    return std::wstring(L"bold");

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetLineHeight() const
//        {
//            styles_iterator oLineHeight = m_mStyle.find(L"line-height");

//            if (oLineHeight != m_mStyle.end())
//                return oLineHeight->second;

//            std::wstring sFont;

//            styles_iterator oFont = m_mStyle.find(L"font");

//            if (oFont != m_mStyle.end())
//                sFont = oFont->second;

//            if (sFont.empty())
//                return std::wstring();

//            const std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFont);

//            std::wstring sValue;

//            for (size_t i = 0; i < arWords.size() - 1; ++i)
//                if (arWords[i] == L"/" && i < arWords.size() - 2)
//                    return arWords[i + 1];

//            return std::wstring();

//        }

//        std::vector<std::wstring> CCompiledStyle::GetFontNames() const
//        {
//            const std::wstring sFontFamily = GetFontFamily();
//            if (sFontFamily.empty())
//                return std::vector<std::wstring>();


//            size_t posFirst = sFontFamily.find_first_of(L"'\"");
//                if (posFirst == std::wstring::npos)
//                    return std::vector<std::wstring>({sFontFamily});

//            std::vector<std::wstring> arWords;
//            while (posFirst != std::wstring::npos)
//            {
//                const size_t& posSecond = sFontFamily.find_first_of(L"'\"", posFirst + 1);
//                arWords.push_back(sFontFamily.substr(posFirst, posSecond - posFirst + 1));
//                posFirst = sFontFamily.find_first_of(L"'\"", posSecond + 1);
//            }

//            return arWords;
//        }

//        std::vector<std::wstring> CCompiledStyle::GetFontNames2(const std::wstring& sFontFamily) const
//        {
//            if (sFontFamily.empty())
//                return std::vector<std::wstring>();

//            size_t posFirst = sFontFamily.find_first_of(L"'\"");
//            if (posFirst == std::wstring::npos)
//                return std::vector<std::wstring>({sFontFamily});

//            std::vector<std::wstring> arWords;
//            while (posFirst != std::wstring::npos)
//            {
//                const size_t& posSecond = sFontFamily.find_first_of(L"'\"", posFirst + 1);
//                arWords.push_back(sFontFamily.substr(posFirst, posSecond - posFirst + 1));
//                posFirst = sFontFamily.find_first_of(L"'\"", posSecond + 1);
//            }
//            return arWords;
//        }

//    /* MARGIN */
//        std::wstring CCompiledStyle::GetMargin() const
//        {
//            styles_iterator oMargin = m_mStyle.find(L"margin");
//            if (oMargin != m_mStyle.end())
//                return oMargin->second;

//            const std::wstring& sTop    = GetMarginTop();
//            const std::wstring& sLeft   = GetMarginLeft();
//            const std::wstring& sRight  = GetMarginRight();
//            const std::wstring& sBottom = GetMarginBottom();

//            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
//                return sTop;

//            if ((sTop == sBottom) && (sLeft == sRight))
//                return  sTop + L" " + sLeft;

//            if (sLeft == sRight)
//                return sTop + L" " + sLeft + L" " + sBottom;

//            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;

//            return std::wstring();
//        }

//        std::vector<std::wstring> CCompiledStyle::GetMargins() const
//        {
//            if (m_pMargin.fTopSide != 0 || m_pMargin.fRightSide != 0 ||
//                m_pMargin.fBottomSide != 0 || m_pMargin.fLeftSide != 0 )
//            {
//                std::wcout << L"_____________________________" << std::endl;
//                return {std::to_wstring(m_pMargin.fTopSide), std::to_wstring(m_pMargin.fRightSide), std::to_wstring(m_pMargin.fBottomSide), std::to_wstring(m_pMargin.fLeftSide)};
//            }
//            styles_iterator oMargin = m_mStyle.find(L"margin");

//            std::wstring sMargin;
//            if (oMargin != m_mStyle.end())
//                sMargin = oMargin->second;

//            std::wstring sTop    = GetMarginTop2();
//            std::wstring sBottom = GetMarginBottom2();
//            std::wstring sLeft   = GetMarginLeft2  ();
//            std::wstring sRight  = GetMarginRight2 ();

//            if(!sMargin.empty())
//            {
//                std::vector<std::wstring> arValues;
//                size_t pre = 0;
//                size_t find = sMargin.find(' ');
//                while(find != std::wstring::npos)
//                {
//                    arValues.push_back(sMargin.substr(pre, find - pre));
//                    pre = find + 1;
//                    find = sMargin.find(' ', pre);
//                }
//                if(pre < sMargin.size())
//                    arValues.push_back(sMargin.substr(pre));

//                if(sTop.empty())
//                    sTop = arValues[0];
//                if(sLeft.empty())
//                    sLeft = arValues.size() == 4 ? arValues[3] : (arValues.size() > 1 ? arValues[1] : arValues[0]);
//                if(sRight.empty())
//                    sRight = arValues.size() > 1 ? arValues[1] : arValues[0];
//                if(sBottom.empty())
//                    sBottom = arValues.size() > 2 ? arValues[2] : arValues[0];
//            }
//            else if (sTop.empty() && sRight.empty() && sBottom.empty() && sLeft.empty())
//                return std::vector<std::wstring>();

//            return {sTop, sRight, sBottom, sLeft};
//        }

//        std::wstring CCompiledStyle::GetMarginTop2() const
//        {
//            styles_iterator oMarginTop = m_mStyle.find(L"margin-top");
//            if (oMarginTop != m_mStyle.end())
//                return oMarginTop->second;

//            const std::wstring& sMarginBlockStart = GetMarginBlockStart();
//            return sMarginBlockStart.empty() ? std::wstring() : sMarginBlockStart;
//        }

//        std::wstring CCompiledStyle::GetMarginTop() const
//        {
//            styles_iterator oMarginTop = m_mStyle.find(L"margin-top");

//            if (oMarginTop != m_mStyle.end())
//                return oMarginTop->second;

//            const std::wstring& sMarginBlockStart = GetMarginBlockStart();

//            if (!sMarginBlockStart.empty())
//                return sMarginBlockStart;

//            std::wstring sMargin;
//            styles_iterator oMargin = m_mStyle.find(L"margin");

//            if (oMargin != m_mStyle.end())
//                sMargin = oMargin->second;
//            else
//                return std::wstring();

//            if (sMargin.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sMargin)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }
//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (!arValues.empty())
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginBlockStart() const
//        {
//            styles_iterator oMarginBlockStart = m_mStyle.find(L"margin-block-start");
//            return oMarginBlockStart != m_mStyle.end() ? oMarginBlockStart->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginLeft2() const
//        {
//            styles_iterator oMarginLeft = m_mStyle.find(L"margin-left");
//            return oMarginLeft != m_mStyle.end() ? oMarginLeft->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginLeft() const
//        {
//            styles_iterator oMarginLeft = m_mStyle.find(L"margin-left");

//            if (oMarginLeft != m_mStyle.end())
//                return oMarginLeft->second;

//            std::wstring sValue;
//            styles_iterator oMargin = m_mStyle.find(L"margin");

//            if (oMargin != m_mStyle.end())
//                sValue = oMargin->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }
//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() == 4)
//                return arValues[3];
//            else if (arValues.size() > 1)
//                return arValues[1];
//            else if (arValues.size() == 1)
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginRight2() const
//        {
//            styles_iterator oMarginRight = m_mStyle.find(L"margin-right");

//            if (oMarginRight != m_mStyle.end())
//                return oMarginRight->second;

//            return L"";
//        }

//        std::wstring CCompiledStyle::GetMarginRight() const
//        {
//            styles_iterator oMarginRight = m_mStyle.find(L"margin-right");

//            if (oMarginRight != m_mStyle.end())
//                return oMarginRight->second;

//            std::wstring sValue;
//            styles_iterator oMargin = m_mStyle.find(L"margin");

//            if (oMargin != m_mStyle.end())
//                sValue = oMargin->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }

//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() > 1)
//                return arValues[1];
//            else if (arValues.size() == 1)
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginBottom2() const
//        {
//            styles_iterator oMarginBottom = m_mStyle.find(L"margin-bottom");
//            if (oMarginBottom != m_mStyle.end())
//                return oMarginBottom->second;

//            const std::wstring& sMarginBlockEnd = GetMarginBlockEnd();
//            return sMarginBlockEnd.empty() ? std::wstring() : sMarginBlockEnd;
//        }

//        std::wstring CCompiledStyle::GetMarginBottom() const
//        {
//            styles_iterator oMarginBottom = m_mStyle.find(L"margin-bottom");

//            if (oMarginBottom != m_mStyle.end())
//                return oMarginBottom->second;

//            const std::wstring& sMarginBlockEnd = GetMarginBlockEnd();

//            if (!sMarginBlockEnd.empty())
//                return sMarginBlockEnd;

//            std::wstring sValue;
//            styles_iterator oMargin = m_mStyle.find(L"margin");

//            if (oMargin != m_mStyle.end())
//                sValue = oMargin->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }
//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() > 2)
//                return arValues[2];
//            else if (arValues.size() > 0)
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetMarginBlockEnd() const
//        {
//            styles_iterator oMarginBlockEnd = m_mStyle.find(L"margin-block-end");
//            return oMarginBlockEnd != m_mStyle.end() ? oMarginBlockEnd->second : std::wstring();
//        }

//    /* PADDING */
//        std::wstring CCompiledStyle::GetPadding() const
//        {
//            styles_iterator oPadding = m_mStyle.find(L"padding");

//            if (oPadding != m_mStyle.end())
//                return oPadding->second;

//            const std::wstring& sTop    = GetPaddingTop();
//            const std::wstring& sLeft   = GetPaddingLeft();
//            const std::wstring& sRight  = GetPaddingRight();
//            const std::wstring& sBottom = GetPaddingBottom();

//            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
//                return sTop;

//            if ((sTop == sBottom) && (sLeft == sRight))
//                return  sTop + L" " + sLeft;

//            if (sLeft == sRight)
//                return sTop + L" " + sLeft + L" " + sBottom;

//            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;
//        }

//        std::wstring CCompiledStyle::GetPaddingTop() const
//        {
//            styles_iterator oPaddingTop = m_mStyle.find(L"padding-top");

//            if (oPaddingTop != m_mStyle.end())
//                return oPaddingTop->second;

//            std::wstring sValue;
//            styles_iterator oPadding = m_mStyle.find(L"padding");

//            if (oPadding != m_mStyle.end())
//                sValue = oPadding->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }
//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (!arValues.empty())
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetPaddingLeft() const
//        {
//            styles_iterator oPaddingLeft = m_mStyle.find(L"padding-left");

//            if (oPaddingLeft != m_mStyle.end())
//                return oPaddingLeft->second;

//            std::wstring sValue;
//            styles_iterator oPadding = m_mStyle.find(L"padding");

//            if (oPadding != m_mStyle.end())
//                sValue = oPadding->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }

//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() == 4)
//                return arValues[3];
//            else if (arValues.size() > 1)
//                return arValues[1];
//            else if (arValues.size() == 1)
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetPaddingRight() const
//        {
//            styles_iterator
//                  oPaddingRight = m_mStyle.find(L"padding-right");

//            if (oPaddingRight != m_mStyle.end())
//                return oPaddingRight->second;

//            std::wstring sValue;
//            styles_iterator oPadding = m_mStyle.find(L"padding");

//            if (oPadding != m_mStyle.end())
//                sValue = oPadding->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }

//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() > 1)
//                return arValues[1];
//            else if (arValues.size() == 1)
//                return arValues[0];

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetPaddingBottom() const
//        {
//            styles_iterator oPaddingBottom = m_mStyle.find(L"padding-bottom");

//            if (oPaddingBottom != m_mStyle.end())
//                return oPaddingBottom->second;

//            std::wstring sValue;
//            styles_iterator oPadding = m_mStyle.find(L"padding");

//            if (oPadding != m_mStyle.end())
//                sValue = oPadding->second;
//            else
//                return std::wstring();

//            if (sValue.empty())
//                return std::wstring();

//            std::vector<std::wstring> arValues;

//            std::wstring sTemp;

//            for (const wchar_t& wc : sValue)
//            {
//                sTemp += wc;

//                if (wc == L' ')
//                {
//                    arValues.push_back(sTemp);
//                    sTemp.clear();
//                }
//            }

//            if (!sTemp.empty())
//                arValues.push_back(sTemp);

//            if (arValues.size() > 2)
//                return arValues[2];
//            else if (arValues.size() > 0)
//                return arValues[0];

//            return std::wstring();
//        }

//    /*  SPACING */
//        std::wstring CCompiledStyle::GetLetterSpacing() const
//        {
//            styles_iterator oLetterSpacing = m_mStyle.find(L"letter-spacing");

//            if (oLetterSpacing != m_mStyle.end())
//                return oLetterSpacing->second;

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetWordSpacing() const
//        {
//            styles_iterator oWordSpacing = m_mStyle.find(L"word-spacing");

//            if (oWordSpacing != m_mStyle.end())
//                return oWordSpacing->second;

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetBorderSpacing() const
//        {
//            styles_iterator oBorderSpacing = m_mStyle.find(L"border-spacing");

//            if (oBorderSpacing != m_mStyle.end())
//                return oBorderSpacing->second;

//            return std::wstring();
//        }

//    /* COLOR */
//        std::wstring CCompiledStyle::GetTextDecorationColor() const
//        {
//            styles_iterator oTextDecorationColor = m_mStyle.find(L"text-decoration-color");

//            if (oTextDecorationColor != m_mStyle.end())
//                return oTextDecorationColor->second;

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetBackgroundColor() const
//        {
//            styles_iterator oBackgroundColor = m_mStyle.find(L"background-color");

////            if (oBackgroundColor != m_mStyle.end())
////            {
////                std::wstring sColor = oBackgroundColor->second;

////                if (sColor[0] == L'#')
////                {
////                    if (sColor.length() == 7)
////                        return sColor.substr(1, 7);
////                    else if (sColor.length() == 4)
////                    {
////                        std::wstring sRetColor;
////                        sRetColor += sColor[1];
////                        sRetColor += sColor[1];
////                        sRetColor += sColor[2];
////                        sRetColor += sColor[2];
////                        sRetColor += sColor[3];
////                        sRetColor += sColor[3];
////                        return sRetColor;
////                    }
////                    else
////                        return L"auto";
////                }

////                std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                if (oHEX != NSConstValues::mColors.end())
////                    return oHEX->second;
////            }
////            const std::wstring& sBackground = GetBackground();

////            if (sBackground.empty())
////                return std::wstring();

////            const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBackground);

////            for (std::wstring sColor : arWords)
////            {
////                if (sColor[0] == L'#')
////                {
////                    if (sColor.length() == 7)
////                        return sColor.substr(1, 7);
////                    else if (sColor.length() == 4)
////                    {
////                        std::wstring sRetColor;
////                        sRetColor += sColor[1];
////                        sRetColor += sColor[1];
////                        sRetColor += sColor[2];
////                        sRetColor += sColor[2];
////                        sRetColor += sColor[3];
////                        sRetColor += sColor[3];
////                        return sRetColor;
////                    }
////                    else
////                        return L"auto";
////                }

////                std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                if (oHEX != NSConstValues::mColors.end())
////                    return oHEX->second;
////            }

//            return std::wstring();
//        }

//        std::wstring CCompiledStyle::GetColor() const
//        {
//            styles_iterator oColor = m_mStyle.find(L"color");
////            if (oColor == m_mStyle.end())
////                return std::wstring();

////            std::wstring sColor = oColor->second;

////            if (sColor[0] == L'#')
////            {
////                if (sColor.length() == 7)
////                    return sColor.substr(1, 7);
////                else if (sColor.length() == 4)
////                {
////                    std::wstring sRetColor;
////                    sRetColor = sColor[1] + sColor[1] + sColor[2] + sColor[2] + sColor[3] + sColor[3];
////                    return sRetColor;
////                }
////                else
////                    return L"";
////            }

////            std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////            styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////            if (oHEX != NSConstValues::mColors.end())
////                return oHEX->second;

//            return L"";
//        }

//        std::wstring CCompiledStyle::GetOutlineColor() const
//        {
//            styles_iterator oOutlineColor = m_mStyle.find(L"outline-color");
//            return oOutlineColor != m_mStyle.end() ? oOutlineColor->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetColumnRuleColor() const
//        {
//            styles_iterator oColumnRuleColor = m_mStyle.find(L"column-rule-color");
//            return oColumnRuleColor != m_mStyle.end() ? oColumnRuleColor->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetBackground() const
//        {
//            styles_iterator oBackground = m_mStyle.find(L"background");
//            return oBackground != m_mStyle.end() ? oBackground->second : std::wstring();
//        }

//    /* TEXT */
//        std::wstring CCompiledStyle::GetTextAlign() const
//        {
//            styles_iterator oTextAlign = m_mStyle.find(L"text-align");
//            return oTextAlign != m_mStyle.end() ? oTextAlign->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetTextIndent() const
//        {
//            styles_iterator oTextIndent = m_mStyle.find(L"text-indent");
//            return oTextIndent != m_mStyle.end() ? oTextIndent->second : std::wstring();
//        }

//        std::wstring CCompiledStyle::GetTextDecoration() const
//        {
//            styles_iterator oTextDecoration = m_mStyle.find(L"text-decoration");

//            if (oTextDecoration != m_mStyle.end())
//            {
//                if (oTextDecoration->second == L"underline")
//                    return std::wstring(L"single");
//                else if (oTextDecoration->second == L"none")
//                    return oTextDecoration->second;
//            }

//            return L"";

////            return oTextDecoration != m_mStyle.end() ? oTextDecoration->second : std::wstring();
//        }

//        /* BORDER */

//            std::wstring CCompiledStyle::GetBorder() const
//            {
//                styles_iterator oBorder = m_mStyle.find(L"border");

//                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
//                    return oBorder->second;
//                else
//                {
//                    styles_iterator oMsoBorder = m_mStyle.find(L"mso-border-alt");
//                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
//                        return oMsoBorder->second;
//                }

//                const std::wstring& sBorderWidth = GetBorderWidth();
//                const std::wstring& sBorderStyle = GetBorderStyle();
//                const std::wstring& sBorderColor = GetBorderColor();

//                if (!sBorderWidth.empty() && !sBorderStyle.empty() && !sBorderColor.empty())
//                    return sBorderWidth + L" " + sBorderStyle + L" " + sBorderColor;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderWidth() const
//            {
//                styles_iterator oBorderWidth = m_mStyle.find(L"border-width");

//                if (oBorderWidth != m_mStyle.end())
//                    return oBorderWidth->second;

//                styles_iterator oBorder = m_mStyle.find(L"border");

//                std::wstring sBorder;

//                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
//                    sBorder = oBorder->second;
//                else
//                {
//                    styles_iterator oMsoBorder = m_mStyle.find(L"mso-border-alt");
//                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
//                        sBorder =  oMsoBorder->second;
//                    else
//                        return std::wstring();
//                }

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

//                for (const std::wstring& sWidth : arWords)
//                    if (iswdigit(sWidth[0]))
//                        return sWidth;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderStyle() const
//            {
//                styles_iterator oBorderStyle = m_mStyle.find(L"border-style");

////                if (oBorderStyle != m_mStyle.end())
////                    return oBorderStyle->second;

////                styles_iterator oBorder = m_mStyle.find(L"border");

////                std::wstring sBorder;

////                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
////                    sBorder = oBorder->second;
////                else
////                {
////                    styles_iterator oMsoBorder = m_mStyle.find(L"mso-border-alt");
////                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
////                        sBorder =  oMsoBorder->second;
////                    else
////                        return std::wstring();
////                }

////                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

////                for (std::wstring sStyle : arWords)
////                {
////                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
////                    styles_iterator oStyle = NSConstValues::mStyles.find(sStyle);

////                    if (oStyle != NSConstValues::mStyles.end())
////                        return oStyle->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderColor() const
//            {
//                styles_iterator oBorderColor = m_mStyle.find(L"border-color");

////                if (oBorderColor != m_mStyle.end())
////                    return oBorderColor->second;

////                styles_iterator oBorder = m_mStyle.find(L"border");

////                std::wstring sBorder;

////                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
////                    sBorder = oBorder->second;
////                else
////                {
////                    styles_iterator oMsoBorder = m_mStyle.find(L"mso-border-alt");
////                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
////                        sBorder =  oMsoBorder->second;
////                    else
////                        return std::wstring();
////                }

////                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

////                for (std::wstring sColor : arWords)
////                {
////                    if (sColor[0] == L'#')
////                    {
////                        if (sColor.length() == 7)
////                            return sColor.substr(1, 7);
////                        else if (sColor.length() == 4)
////                        {
////                            std::wstring sRetColor;
////                            sRetColor += sColor[1];
////                            sRetColor += sColor[1];
////                            sRetColor += sColor[2];
////                            sRetColor += sColor[2];
////                            sRetColor += sColor[3];
////                            sRetColor += sColor[3];
////                            return sRetColor;
////                        }
////                        else
////                            return L"auto";
////                    }

////                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                    styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                    if (oHEX != NSConstValues::mColors.end())
////                        return oHEX->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderBottom() const
//            {
//                styles_iterator oBorderBottom = m_mStyle.find(L"border-bottom");

//                if (oBorderBottom != m_mStyle.end())
//                    return oBorderBottom->second;

//                const std::wstring& sBorderBottomWidth = GetBorderBottomWidth();
//                const std::wstring& sBorderBottomStyle = GetBorderBottomStyle();
//                const std::wstring& sBorderBottomColor = GetBorderBottomColor();

//                if (!sBorderBottomWidth.empty() && !sBorderBottomStyle.empty() && !sBorderBottomColor.empty())
//                    return sBorderBottomWidth + L" " + sBorderBottomStyle + L" " + sBorderBottomColor;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderBottomWidth() const
//            {
//                styles_iterator oBorderBottomWidth = m_mStyle.find(L"border-bottom-width");

//                if (oBorderBottomWidth != m_mStyle.end())
//                    return oBorderBottomWidth->second;

//                styles_iterator oBorderBottom = m_mStyle.find(L"border-bottom");

//                std::wstring sBorderBottom;

//                if (oBorderBottom != m_mStyle.end())
//                    sBorderBottom = oBorderBottom->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

//                for (const std::wstring& sWidth : arWords)
//                    if (iswdigit(sWidth[0]))
//                        return sWidth;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderBottomStyle() const
//            {
//                styles_iterator oBorderBottomStyle = m_mStyle.find(L"border-bottom-style");

////                if (oBorderBottomStyle != m_mStyle.end())
////                    return oBorderBottomStyle->second;

////                styles_iterator oBorderBottom = m_mStyle.find(L"border-bottom");

////                std::wstring sBorderBottom;

////                if (oBorderBottom != m_mStyle.end())
////                    sBorderBottom = oBorderBottom->second;
////                else
////                    return std::wstring();

////                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

////                for (std::wstring sStyle : arWords)
////                {
////                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
////                    styles_iterator oStyle = mStyles.find(sStyle);

////                    if (oStyle != NSConstValues::mStyles.end())
////                        return oStyle->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderBottomColor() const
//            {
//                styles_iterator oBorderBottomColor = m_mStyle.find(L"border-bottom-color");

////                if (oBorderBottomColor != m_mStyle.end())
////                    return oBorderBottomColor->second;\

////                styles_iterator oBorderBottom = m_mStyle.find(L"border-bottom");

////                std::wstring sBorderBottom;

////                if (oBorderBottom != m_mStyle.end())
////                    sBorderBottom = oBorderBottom->second;
////                else
////                    return std::wstring();

////                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

////                for (std::wstring sColor : arWords)
////                {
////                    if (sColor[0] == L'#')
////                    {
////                        if (sColor.length() == 7)
////                            return sColor.substr(1, 7);
////                        else if (sColor.length() == 4)
////                        {
////                            std::wstring sRetColor;
////                            sRetColor += sColor[1];
////                            sRetColor += sColor[1];
////                            sRetColor += sColor[2];
////                            sRetColor += sColor[2];
////                            sRetColor += sColor[3];
////                            sRetColor += sColor[3];
////                            return sRetColor;
////                        }
////                        else
////                            return L"auto";
////                    }

////                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                    styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                    if (oHEX != NSConstValues::mColors.end())
////                        return oHEX->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderLeft() const
//            {
//                styles_iterator oBorderLeft = m_mStyle.find(L"border-left");

//                if (oBorderLeft != m_mStyle.end())
//                    return oBorderLeft->second;

//                const std::wstring& sBorderLeftWidth = GetBorderLeftWidth();
//                const std::wstring& sBorderLeftStyle = GetBorderLeftStyle();
//                const std::wstring& sBorderLeftColor = GetBorderLeftColor();

//                if (!sBorderLeftWidth.empty() && !sBorderLeftStyle.empty() && !sBorderLeftColor.empty())
//                    return sBorderLeftWidth + L" " + sBorderLeftStyle + L" " + sBorderLeftColor;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderLeftWidth() const
//            {
//                styles_iterator oBorderLeftWidth = m_mStyle.find(L"border-left-width");

//                if (oBorderLeftWidth != m_mStyle.end())
//                    return oBorderLeftWidth->second;

//                styles_iterator oBorderLeft = m_mStyle.find(L"border-left");

//                std::wstring sBorderLeft;

//                if (oBorderLeft != m_mStyle.end())
//                    sBorderLeft = oBorderLeft->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

//                for (const std::wstring& sWidth : arWords)
//                    if (iswdigit(sWidth[0]))
//                        return sWidth;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderLeftStyle() const
//            {
//                styles_iterator oBorderLeftStyle = m_mStyle.find(L"border-left-style");

//                if (oBorderLeftStyle != m_mStyle.end())
//                    return oBorderLeftStyle->second;

//                styles_iterator oBorderLeft = m_mStyle.find(L"border-left");

//                std::wstring sBorderLeft;

//                if (oBorderLeft != m_mStyle.end())
//                    sBorderLeft = oBorderLeft->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

////                for (std::wstring sStyle : arWords)
////                {
////                    styles_iterator oStyle = NSConstValues::mStyles.find(sStyle);

////                    if (oStyle != NSConstValues::mStyles.end())
////                        return oStyle->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderLeftColor() const
//            {
//                styles_iterator oBorderLeftColor = m_mStyle.find(L"border-left-color");

//                if (oBorderLeftColor != m_mStyle.end())
//                    return oBorderLeftColor->second;

//                styles_iterator oBorderLeft = m_mStyle.find(L"border-left");

//                std::wstring sBorderLeft;

//                if (oBorderLeft != m_mStyle.end())
//                    sBorderLeft = oBorderLeft->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

//                for (std::wstring sColor : arWords)
//                {
//                    if (sColor[0] == L'#')
//                    {
//                        if (sColor.length() == 7)
//                            return sColor.substr(1, 7);
//                        else if (sColor.length() == 4)
//                        {
//                            std::wstring sRetColor;
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[3];
//                            sRetColor += sColor[3];
//                            return sRetColor;
//                        }
//                        else
//                            return L"auto";
//                    }

////                    const styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                    if (oHEX != NSConstValues::mColors.end())
////                        return oHEX->second;
//                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderRight() const
//            {
//                styles_iterator oBorderRight = m_mStyle.find(L"border-right");

//                if (oBorderRight != m_mStyle.end())
//                    return oBorderRight->second;

//                const std::wstring& sBorderRightWidth = GetBorderRightWidth();
//                const std::wstring& sBorderRightStyle = GetBorderRightStyle();
//                const std::wstring& sBorderRightColor = GetBorderRightColor();

//                if (!sBorderRightWidth.empty() && !sBorderRightStyle.empty() && !sBorderRightColor.empty())
//                    return sBorderRightWidth + L" " + sBorderRightStyle + L" " + sBorderRightColor;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderRightWidth() const
//            {
//                styles_iterator oBorderRightWidth = m_mStyle.find(L"border-right-width");

//                if (oBorderRightWidth != m_mStyle.end())
//                    return oBorderRightWidth->second;

//                styles_iterator oBorderRight = m_mStyle.find(L"border-right");

//                std::wstring sBorderRight;

//                if (oBorderRight != m_mStyle.end())
//                    sBorderRight = oBorderRight->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

//                for (const std::wstring& sWidth : arWords)
//                    if (iswdigit(sWidth[0]))
//                        return sWidth;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderRightStyle() const
//            {
//                styles_iterator oBorderRightStyle = m_mStyle.find(L"border-right-style");

//                if (oBorderRightStyle != m_mStyle.end())
//                    return oBorderRightStyle->second;

//                styles_iterator oBorderRight = m_mStyle.find(L"border-right");

//                std::wstring sBorderRight;

//                if (oBorderRight != m_mStyle.end())
//                    sBorderRight = oBorderRight->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

////                for (std::wstring sStyle : arWords)
////                {
////                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
////                    styles_iterator oStyle = NSConstValues::mStyles.find(sStyle);

////                    if (oStyle != NSConstValues::mStyles.end())
////                        return oStyle->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderRightColor() const
//            {
//                styles_iterator oBorderRightColor = m_mStyle.find(L"border-right-color");

//                if (oBorderRightColor != m_mStyle.end())
//                    return oBorderRightColor->second;

//                styles_iterator oBorderRight = m_mStyle.find(L"border-right");

//                std::wstring sBorderRight;

//                if (oBorderRight != m_mStyle.end())
//                    sBorderRight = oBorderRight->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

//                for (std::wstring sColor : arWords)
//                {
//                    if (sColor[0] == L'#')
//                    {
//                        if (sColor.length() == 7)
//                            return sColor.substr(1, 7);
//                        else if (sColor.length() == 4)
//                        {
//                            std::wstring sRetColor;
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[3];
//                            sRetColor += sColor[3];
//                            return sRetColor;
//                        }
//                        else
//                            return L"auto";
//                    }

//                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                    styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                    if (oHEX != NSConstValues::mColors.end())
////                        return oHEX->second;
//                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderTop() const
//            {
//                styles_iterator oBorderTop = m_mStyle.find(L"border-top");

//                if (oBorderTop != m_mStyle.end())
//                    return oBorderTop->second;

//                const std::wstring& sBorderTopWidth = GetBorderTopWidth();
//                const std::wstring& sBorderTopStyle = GetBorderTopStyle();
//                const std::wstring& sBorderTopColor = GetBorderTopColor();

//                if (!sBorderTopWidth.empty() && !sBorderTopStyle.empty() && !sBorderTopColor.empty())
//                    return sBorderTopWidth + L" " + sBorderTopStyle + L" " + sBorderTopColor;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderTopWidth() const
//            {
//                styles_iterator oBorderTopWidth = m_mStyle.find(L"border-top-width");

//                if (oBorderTopWidth != m_mStyle.end())
//                    return oBorderTopWidth->second;

//                styles_iterator oBorderTop = m_mStyle.find(L"border-top");

//                std::wstring sBorderTop;

//                if (oBorderTop != m_mStyle.end())
//                    sBorderTop = oBorderTop->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

//                for (const std::wstring& sWidth : arWords)
//                    if (iswdigit(sWidth[0]))
//                        return sWidth;

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderTopStyle() const
//            {
//                styles_iterator oBorderTopStyle = m_mStyle.find(L"border-top-style");

//                if (oBorderTopStyle != m_mStyle.end())
//                    return oBorderTopStyle->second;

//                styles_iterator oBorderTop = m_mStyle.find(L"border-top");

//                std::wstring sBorderTop;

//                if (oBorderTop != m_mStyle.end())
//                    sBorderTop = oBorderTop->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

////                for (std::wstring sStyle : arWords)
////                {
////                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
////                    styles_iterator oStyle = NSConstValues::mStyles.find(sStyle);

////                    if (oStyle != NSConstValues::mStyles.end())
////                        return oStyle->second;
////                }

//                return std::wstring();
//            }

//            std::wstring CCompiledStyle::GetBorderTopColor() const
//            {
//                styles_iterator oBorderTopColor = m_mStyle.find(L"border-top-color");

//                if (oBorderTopColor != m_mStyle.end())
//                    return oBorderTopColor->second;

//                styles_iterator oBorderTop = m_mStyle.find(L"border-top");

//                std::wstring sBorderTop;

//                if (oBorderTop != m_mStyle.end())
//                    sBorderTop = oBorderTop->second;
//                else
//                    return std::wstring();

//                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

//                for (std::wstring sColor : arWords)
//                {
//                    if (sColor[0] == L'#')
//                    {
//                        if (sColor.length() == 7)
//                            return sColor.substr(1, 7);
//                        else if (sColor.length() == 4)
//                        {
//                            std::wstring sRetColor;
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[1];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[2];
//                            sRetColor += sColor[3];
//                            sRetColor += sColor[3];
//                            return sRetColor;
//                        }
//                        else
//                            return L"auto";
//                    }
//                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

////                    styles_iterator oHEX = NSConstValues::mColors.find(sColor);

////                    if (oHEX != NSConstValues::mColors.end())
////                        return oHEX->second;
//                }

//                return std::wstring();
//            }

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
                        if (!NS_STATIC_FUNCTIONS::ConvertAbsoluteValue(sValueTemp))
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

