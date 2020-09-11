#include "CCompiledStyle.h"

#include <cwctype>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>

#include <iostream>
#include "../../../../../DesktopEditor/common/File.h"
#include "StaticFunctions.h"
#include "ConstValues.h"

namespace NSCSS
{
    CCompiledStyle::CCompiledStyle(){}

    CCompiledStyle::CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle) : m_mStyle(mStyle) {}

    CCompiledStyle::CCompiledStyle(const CCompiledStyle& oStyle) :
        m_mStyle(oStyle.m_mStyle), m_sId(oStyle.m_sId), m_arParentsStyles(oStyle.m_arParentsStyles) {}

    CCompiledStyle::~CCompiledStyle()
    {
        m_mStyle.clear();
        m_arParentsStyles.clear();
    }

    CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
    {
        if (oElement.m_mStyle.empty())
            return *this;

        for(const auto& oItem : oElement.m_mStyle)
        {
            const auto& oFindPr = m_mStyle.find(oItem.first);

            if (oFindPr != m_mStyle.end())
            {
                if (!oItem.second.empty() && oItem.second != L"inherit" && oFindPr->second.find(L"!imp") == std::wstring::npos)
                    m_mStyle[oFindPr->first] = oItem.second;
            }
            else
                m_mStyle[oItem.first] = oItem.second;
        }
        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator= (const CCompiledStyle &oElement)
    {
        m_mStyle = oElement.m_mStyle;
        m_sId = oElement.m_sId;
        // m_arParentsStyles = oElement.m_arParentsStyles;
        return *this;
    }

    bool CCompiledStyle::operator== (const CCompiledStyle& oStyle) const
    {
        return GetId()[0]        == oStyle.GetId()[0]        &&
               m_arParentsStyles == oStyle.m_arParentsStyles &&
               m_mStyle          == oStyle.m_mStyle;
    }

    std::wstring CCompiledStyle::GetStyleW() const
    {
        if (m_mStyle.empty())
            return std::wstring();

        std::wstring sStyle;
        return std::accumulate(m_mStyle.begin(), m_mStyle.end(), sStyle,
            [] (std::wstring& sRes, const auto& oIter) { return sRes + oIter.first + L":" + oIter.second + L";"; });
    }

    std::map<std::wstring, std::wstring> CCompiledStyle::GetStyleMap() const
    {
        return m_mStyle;
    }

    /*
    bool CCompiledStyle::operator!=(const CCompiledStyle &oElement) const
    {
        if (*this > oElement)
            return false;

        if (*this < oElement)
            return false;

        return true;
    }

    bool CCompiledStyle::operator>(const CCompiledStyle &oElement) const
    {
        return GetWeidth() > oElement.GetWeidth();
    }

    bool CCompiledStyle::operator<(const CCompiledStyle &oElement) const
    {
        return GetWeidth() < oElement.GetWeidth();
    }

    std::string CCompiledStyle::GetStyle() const
    {
        std::wstring sStyle = GetStyleW();
        return U_TO_UTF8(sStyle);
    }

    size_t CCompiledStyle::GetSize() const
    {
        return m_mStyle.size();
    }
    */

    bool CCompiledStyle::Empty() const
    {
        return m_mStyle.empty();
    }

    /*
    void CCompiledStyle::Clear()
    {
        m_mStyle.clear();
        m_sId.clear();
        m_arParentsStyles.clear();
    }
    */

    void CCompiledStyle::AddPropSel(const std::wstring& sProperty, const std::wstring& sValue)
    {
        m_mStyle[sProperty] = sValue;
    }

    /*
    void CCompiledStyle::InsertStyle(const std::map<std::wstring, std::wstring>& mStyle)
    {
        m_mStyle.insert(mStyle.begin(), mStyle.end());
    }

    void CCompiledStyle::SetStyle(const std::map<std::wstring, std::wstring>& mStyle)
    {
        m_mStyle = mStyle;
    }
    */

    void CCompiledStyle::AddStyle(const std::wstring& sStyle)
    {
        size_t posColon = sStyle.find(L':');
        size_t posLastSemicolon = size_t(0);
        while (posColon != std::wstring::npos)
        {
            const size_t& posSemicolon = sStyle.find(L';', posColon);
            AddPropSel(sStyle.substr(posLastSemicolon, posColon- posLastSemicolon), sStyle.substr(posColon + 1, posSemicolon - posColon - 1));
            posColon = sStyle.find(L':', posSemicolon);
            posLastSemicolon = posSemicolon + 1;
        }
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

    const std::wstring& CCompiledStyle::GetId() const
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
            [] (double dW, const auto& sValue) { return dW + sValue.first.length() + sValue.second.length(); });
        dWidth = std::accumulate(m_arParentsStyles.begin(), m_arParentsStyles.end(), dWidth,
            [] (double dW, const auto& sValue) { return dW + sValue.length() / 2; });
        return dWidth;
    }
    */

    /* FONT */

        std::wstring CCompiledStyle::GetFont() const
        {
            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                return oFont->second;

            const std::wstring& sValue = GetFontStyle()   + L" " +
                                         GetFontVariant() + L" " +
                                         GetFontWeight()  + L" " +
                                         GetFontSize()    + L"/" +
                                         GetLineHeight()  + L" " +
                                         GetFontFamily();

            return (sValue.length() == 5 ? std::wstring() : sValue);
        }

        std::wstring CCompiledStyle::GetFontFamily() const
        {
            const auto& oFontFamily = m_mStyle.find(L"font-family");

            if (oFontFamily != m_mStyle.end())
            {
                std::wstring sFontFamily = oFontFamily->second;

                const auto& posComma = sFontFamily.find(L',');

                if (posComma != std::wstring::npos)
                    sFontFamily = sFontFamily.substr(0, posComma);

                if (sFontFamily.find(L'"') != std::wstring::npos || sFontFamily.find(L'\'') != std::wstring::npos)
                    return sFontFamily;

                if (!sFontFamily.empty())
                    return  L'"' + sFontFamily + L'"';
            }
            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                sFont = oFont->second;
            else
                return std::wstring();

            if (sFont.empty())
                return std::wstring();

            int nPos1 = -1;

            for (int i = (int)sFont.length() - 1; i >= 0; --i)
            {
                if(iswdigit(sFont[i]))
                {
                    nPos1 = i;
                    break;
                }
            }

            std::wstring sValue = sFont.substr(nPos1 + 1);

            const auto& posComma = sValue.find(L',');

            if (posComma != std::wstring::npos)
                sValue.substr(0, posComma - 1);

            if (!sValue.empty())
            {
                if (sValue.find(L'"') != std::wstring::npos || sValue.find(L'\'') != std::wstring::npos)
                    return sValue;

                return L'"' + sValue + L'"';
            }

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontSize() const
        {
            const auto& oFontSize = m_mStyle.find(L"font-size");

            if (oFontSize != m_mStyle.end())
                return oFontSize->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                sFont = oFont->second;
            else
                return std::wstring();

            if (sFont.empty())
                return std::wstring();

            const std::vector<std::wstring> arValues = { L"xx-small",L"x-small", L"small",
                                                         L"medium",  L"large",   L"x-large",
                                                         L"xx-large",L"larger",  L"smaller"};

            for (const std::wstring& sValue : arValues)
                if (sFont.find(sValue) != std::wstring::npos)
                    return sValue;

            const std::vector<std::wstring> arUnitMeasure = {L"mm", L"cm",
                                                             L"in", L"pt",
                                                             L"pc", L"%",
                                                             L"em", L"rem",
                                                             L"ex"};
            std::wstring sTempUnitMeasure;
            size_t posUnitMeasure = std::wstring::npos;

            for (std::wstring sUnitMesure : arUnitMeasure)
            {
                posUnitMeasure = sFont.find(sUnitMesure);
                if (posUnitMeasure != std::wstring::npos)
                {
                    sTempUnitMeasure = sUnitMesure;
                    const wchar_t& wc = sFont[posUnitMeasure - 1];

                    if (iswdigit(wc) || wc == L'.')
                        break;
                    else
                    {
                        posUnitMeasure = 0;
                        sTempUnitMeasure.clear();
                    }
                }
            }

            if (posUnitMeasure == 0)
            {
                std::wstring sValue;
                for (unsigned int  i = sFont.length() - 1; i >= 0; --i)
                {
                    const wchar_t& wc = sFont[i];

                    if (iswdigit(wc))
                    {
                        while (iswdigit(wc) && i >= 0)
                        {
                            sValue += wc;
                            --i;
                        }
                        break;
                    }
                }
                if (!sValue.empty())
                    return sValue;
            }
            else if (posUnitMeasure != std::wstring::npos)
            {
                unsigned int num = 0;
                std::wstring sValue;
                while ((posUnitMeasure - num) > 0 &&
                       (iswdigit(sFont[posUnitMeasure - num]) ||
                        sFont[posUnitMeasure - num] == '.'))
                {
                    sValue = sFont[posUnitMeasure - num] + sValue;
                    ++num;
                }
                if (!sValue.empty())
                    return sValue + sTempUnitMeasure;
            }

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetFontSizeAdjust() const
        {
            const auto& oFontSizeAdj = m_mStyle.find(L"font-size-adjust");

            if (oFontSizeAdj != m_mStyle.end())
                return oFontSizeAdj->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetFontStretch() const
        {
            const auto& oFontStretch = m_mStyle.find(L"font-stretch");

            if (oFontStretch != m_mStyle.end())
                return oFontStretch->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetFontStyle() const
        {
            const auto& oFontStyle = m_mStyle.find(L"font-style");

            if (oFontStyle != m_mStyle.end())
                return oFontStyle->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                sFont = oFont->second;
            else
                return std::wstring();

            if (sFont.empty())
                return std::wstring();

            const std::vector<std::wstring>& arValues = {L"italic", L"oblique"};

            if (sFont == L"italic" || sFont == L"oblique")
                return sFont;

            if (iswdigit(sFont[0]))
                return std::wstring();

            std::wstring sValue;
            const auto& posSpace = sFont.find(L' ');

            if (posSpace != std::wstring::npos)
                sValue = sFont.substr(0, posSpace);

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontVariant() const
        {
            const auto& oFontVariant = m_mStyle.find(L"font-variant");

            if (oFontVariant != m_mStyle.end())
                return oFontVariant->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                sFont = oFont->second;
            else
                return std::wstring();

            if (sFont.empty())
                return std::wstring();

            if (sFont.find(L"small-caps") != std::wstring::npos)
                return L"small-caps";

            if (iswdigit(sFont[0]))
                return std::wstring();

            std::wstring sValue;
            auto posSpace = sFont.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sFont[0]))
                sValue = sFont.substr(posSpace + 1);
            else
                return std::wstring();

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(0, posSpace);
            else
                return std::wstring();

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontWeight() const
        {
            const auto& oFontWeight = m_mStyle.find(L"font-weight");

            if (oFontWeight != m_mStyle.end())
                return oFontWeight->second;

            const auto& oFont = m_mStyle.find(L"font");
            if (oFont == m_mStyle.end())
                return std::wstring();

            const std::wstring& sFont = oFont->second;
            if (sFont.empty())
                return std::wstring();

            const std::vector<std::wstring> arValues = {L"bold", L"bolder", L"lighter"};

            for (const std::wstring& sValue : arValues)
                if (sFont.find(sValue) != std::wstring::npos)
                    return sValue;

            if (iswdigit(sFont[0]))
                return std::wstring();

            std::wstring sValue = sFont;
            auto posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(posSpace + 1);
            else
                return std::wstring();

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(posSpace + 1);
            else
                return std::wstring();

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(0, posSpace);
            else
                return std::wstring();

            return sValue;
        }

        std::wstring CCompiledStyle::GetLineHeight() const
        {
            const auto& oLineHeight = m_mStyle.find(L"line-height");

            if (oLineHeight != m_mStyle.end())
                return oLineHeight->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.end())
                sFont = oFont->second;
            else
                return std::wstring();

            if (sFont.empty())
                return std::wstring();

            const auto& posSlash = sFont.find(L'/');

            if (posSlash == std::wstring::npos)
                return std::wstring();

            std::wstring sValue = sFont.substr(posSlash + 1);


            auto posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos)
                sValue = sValue.substr(0, posSpace);

            if (sValue.length() > 1 && iswalpha(sValue.back()))
                return sValue;

            posSpace = sFont.find(sValue) + sValue.length();
            sValue += L" ";

            while (posSpace < sFont.length() &&
                   iswalpha(sFont[posSpace]))
            {
                sValue += sFont[posSpace];
                ++posSpace;
            }

            return sValue;

        }

        std::vector<std::wstring> CCompiledStyle::GetFontNames() const
        {
            std::wstring sFontFamily = GetFontFamily();
            if (sFontFamily.empty())
                return std::vector<std::wstring>();


            auto posFirst = sFontFamily.find_first_of(L"'\"");
                if (posFirst == std::wstring::npos)
                    return std::vector<std::wstring>({sFontFamily});

            std::vector<std::wstring> arWords;
            while (posFirst != std::wstring::npos)
            {
                const auto& posSecond = sFontFamily.find_first_of(L"'\"", posFirst + 1);
                arWords.push_back(sFontFamily.substr(posFirst, posSecond - posFirst + 1));
                posFirst = sFontFamily.find_first_of(L"'\"", posSecond + 1);
            }

            return arWords;
        }

    /* MARGIN */
        std::wstring CCompiledStyle::GetMargin() const
        {
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.end())
                return oMargin->second;

            const std::wstring& sTop    = GetMarginTop();
            const std::wstring& sLeft   = GetMarginLeft();
            const std::wstring& sRight  = GetMarginRight();
            const std::wstring& sBottom = GetMarginBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginTop() const
        {
            const auto& oMarginTop = m_mStyle.find(L"margin-top");

            if (oMarginTop != m_mStyle.end())
                return oMarginTop->second;

            const std::wstring& sMarginBlockStart = GetMarginBlockStart();

            if (!sMarginBlockStart.empty())
                return sMarginBlockStart;

            std::wstring sMargin;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.end())
                sMargin = oMargin->second;
            else
                return std::wstring();

            if (sMargin.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sMargin)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (!arValues.empty())
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginBlockStart() const
        {
            const auto& oMarginBlockStart = m_mStyle.find(L"margin-block-start");

            if (oMarginBlockStart != m_mStyle.end())
                return oMarginBlockStart->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginLeft() const
        {
            const auto& oMarginLeft = m_mStyle.find(L"margin-left");

            if (oMarginLeft != m_mStyle.end())
                return oMarginLeft->second;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.end())
                sValue = oMargin->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() == 4)
                return arValues[3];
            else if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginRight() const
        {
            const auto& oMarginRight = m_mStyle.find(L"margin-right");

            if (oMarginRight != m_mStyle.end())
                return oMarginRight->second;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.end())
                sValue = oMargin->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginBottom() const
        {
            const auto& oMarginBottom = m_mStyle.find(L"margin-bottom");

            if (oMarginBottom != m_mStyle.end())
                return oMarginBottom->second;

            const std::wstring& sMarginBlockEnd = GetMarginBlockEnd();

            if (!sMarginBlockEnd.empty())
                return sMarginBlockEnd;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.end())
                sValue = oMargin->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 2)
                return arValues[2];
            else if (arValues.size() > 0)
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetMarginBlockEnd() const
        {
            const auto& oMarginBlockEnd = m_mStyle.find(L"margin-block-end");

            if (oMarginBlockEnd != m_mStyle.end())
                return oMarginBlockEnd->second;

            return std::wstring();
        }

    /* PADDING */
        std::wstring CCompiledStyle::GetPadding() const
        {
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.end())
                return oPadding->second;

            const std::wstring& sTop    = GetPaddingTop();
            const std::wstring& sLeft   = GetPaddingLeft();
            const std::wstring& sRight  = GetPaddingRight();
            const std::wstring& sBottom = GetPaddingBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;
        }

        std::wstring CCompiledStyle::GetPaddingTop() const
        {
            const auto& oPaddingTop = m_mStyle.find(L"padding-top");

            if (oPaddingTop != m_mStyle.end())
                return oPaddingTop->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.end())
                sValue = oPadding->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }
            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (!arValues.empty())
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetPaddingLeft() const
        {
            const auto& oPaddingLeft = m_mStyle.find(L"padding-left");

            if (oPaddingLeft != m_mStyle.end())
                return oPaddingLeft->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.end())
                sValue = oPadding->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() == 4)
                return arValues[3];
            else if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetPaddingRight() const
        {
            const auto& oPaddingRight = m_mStyle.find(L"padding-right");

            if (oPaddingRight != m_mStyle.end())
                return oPaddingRight->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.end())
                sValue = oPadding->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 1)
                return arValues[1];
            else if (arValues.size() == 1)
                return arValues[0];

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetPaddingBottom() const
        {
            const auto& oPaddingBottom = m_mStyle.find(L"padding-bottom");

            if (oPaddingBottom != m_mStyle.end())
                return oPaddingBottom->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.end())
                sValue = oPadding->second;
            else
                return std::wstring();

            if (sValue.empty())
                return std::wstring();

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (const wchar_t& wc : sValue)
            {
                sTemp += wc;

                if (wc == L' ')
                {
                    arValues.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arValues.push_back(sTemp);

            if (arValues.size() > 2)
                return arValues[2];
            else if (arValues.size() > 0)
                return arValues[0];

            return std::wstring();
        }

    /*  SPACING */
        std::wstring CCompiledStyle::GetLetterSpacing() const
        {
            const auto& oLetterSpacing = m_mStyle.find(L"letter-spacing");

            if (oLetterSpacing != m_mStyle.end())
                return oLetterSpacing->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetWordSpacing() const
        {
            const auto& oWordSpacing = m_mStyle.find(L"word-spacing");

            if (oWordSpacing != m_mStyle.end())
                return oWordSpacing->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetBorderSpacing() const
        {
            const auto& oBorderSpacing = m_mStyle.find(L"border-spacing");

            if (oBorderSpacing != m_mStyle.end())
                return oBorderSpacing->second;

            return std::wstring();
        }

    /* COLOR */
        std::wstring CCompiledStyle::GetTextDecorationColor() const
        {
            const auto& oTextDecorationColor = m_mStyle.find(L"text-decoration-color");

            if (oTextDecorationColor != m_mStyle.end())
                return oTextDecorationColor->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetBackgroundColor() const
        {
            const auto& oBackgroundColor = m_mStyle.find(L"background-color");

            if (oBackgroundColor != m_mStyle.end())
                return oBackgroundColor->second;

            const std::wstring& sBackground = GetBackground();

            if (sBackground.empty())
                return std::wstring();

            const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBackground);

            for (std::wstring sColor : arWords)
            {
                if (sColor[0] == L'#')
                {
                    if (sColor.length() < 7)
                        sColor += L"000000";
                    return sColor.substr(1, 6);
                }
                std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                if (oHEX != NS_CONST_VALUES::mColors.end())
                    return oHEX->second;
            }

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetColor() const
        {
            const auto& oColor = m_mStyle.find(L"color");

            if (oColor != m_mStyle.end())
                return oColor->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetOutlineColor() const
        {
            const auto& oOutlineColor = m_mStyle.find(L"outline-color");

            if (oOutlineColor != m_mStyle.end())
                return oOutlineColor->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetColumnRuleColor() const
        {
            const auto& oColumnRuleColor = m_mStyle.find(L"column-rule-color");

            if (oColumnRuleColor != m_mStyle.end())
                return oColumnRuleColor->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetBackground() const
        {
            const auto& oBackground = m_mStyle.find(L"background");

            if (oBackground != m_mStyle.end())
                return  oBackground->second;

            return std::wstring();
        }

    /* TEXT */
        std::wstring CCompiledStyle::GetTextAlign() const
        {
            const auto& oTextAlign = m_mStyle.find(L"text-align");

            if (oTextAlign != m_mStyle.end())
                return oTextAlign->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetTextIndent() const
        {
            const auto& oTextIndent = m_mStyle.find(L"text-indent");

            if (oTextIndent != m_mStyle.end())
                return oTextIndent->second;

            return std::wstring();
        }

        std::wstring CCompiledStyle::GetTextDecoration() const
        {
            const auto& oTextDecoration = m_mStyle.find(L"text-decoration");

            if (oTextDecoration != m_mStyle.end())
                return oTextDecoration->second;

            return std::wstring();
        }

        /* BORDER */

            std::wstring CCompiledStyle::GetBorder() const
            {
                const auto& oBorder = m_mStyle.find(L"border");

                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
                    return oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
                        return oMsoBorder->second;
                }

                const std::wstring& sBorderWidth = GetBorderWidth();
                const std::wstring& sBorderStyle = GetBorderStyle();
                const std::wstring& sBorderColor = GetBorderColor();

                if (sBorderWidth.empty() + sBorderStyle.empty() + sBorderColor.empty() == 0)
                    return sBorderWidth + L" " + sBorderStyle + L" " + sBorderColor;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderWidth() const
            {
                const auto& oBorderWidth = m_mStyle.find(L"border-width");

                if (oBorderWidth != m_mStyle.end())
                    return oBorderWidth->second;

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return std::wstring();
                }

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderStyle() const
            {
                const auto& oBorderStyle = m_mStyle.find(L"border-style");

                if (oBorderStyle != m_mStyle.end())
                    return oBorderStyle->second;

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return std::wstring();
                }

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.end())
                        return oStyle->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderColor() const
            {
                const auto& oBorderColor = m_mStyle.find(L"border-color");

                if (oBorderColor != m_mStyle.end())
                    return oBorderColor->second;

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.end() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.end() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return std::wstring();
                }

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorder);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.end())
                        return oHEX->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderBottom() const
            {
                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                if (oBorderBottom != m_mStyle.end())
                    return oBorderBottom->second;

                const std::wstring& sBorderBottomWidth = GetBorderBottomWidth();
                const std::wstring& sBorderBottomStyle = GetBorderBottomStyle();
                const std::wstring& sBorderBottomColor = GetBorderBottomColor();

                if (sBorderBottomWidth.empty() + sBorderBottomStyle.empty() + sBorderBottomColor.empty() == 0)
                    return sBorderBottomWidth + L" " + sBorderBottomStyle + L" " + sBorderBottomColor;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderBottomWidth() const
            {
                const auto& oBorderBottomWidth = m_mStyle.find(L"border-bottom-width");

                if (oBorderBottomWidth != m_mStyle.end())
                    return oBorderBottomWidth->second;

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.end())
                    sBorderBottom = oBorderBottom->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderBottomStyle() const
            {
                const auto& oBorderBottomStyle = m_mStyle.find(L"border-bottom-style");

                if (oBorderBottomStyle != m_mStyle.end())
                    return oBorderBottomStyle->second;

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.end())
                    sBorderBottom = oBorderBottom->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.end())
                        return oStyle->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderBottomColor() const
            {
                const auto& oBorderBottomColor = m_mStyle.find(L"border-bottom-color");

                if (oBorderBottomColor != m_mStyle.end())
                    return oBorderBottomColor->second;\

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.end())
                    sBorderBottom = oBorderBottom->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderBottom);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.end())
                        return oHEX->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderLeft() const
            {
                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                if (oBorderLeft != m_mStyle.end())
                    return oBorderLeft->second;

                const std::wstring& sBorderLeftWidth = GetBorderLeftWidth();
                const std::wstring& sBorderLeftStyle = GetBorderLeftStyle();
                const std::wstring& sBorderLeftColor = GetBorderLeftColor();

                if (sBorderLeftWidth.empty() + sBorderLeftStyle.empty() + sBorderLeftColor.empty() == 0)
                    return sBorderLeftWidth + L" " + sBorderLeftStyle + L" " + sBorderLeftColor;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderLeftWidth() const
            {
                const auto& oBorderLeftWidth = m_mStyle.find(L"border-left-width");

                if (oBorderLeftWidth != m_mStyle.end())
                    return oBorderLeftWidth->second;

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.end())
                    sBorderLeft = oBorderLeft->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderLeftStyle() const
            {
                const auto& oBorderLeftStyle = m_mStyle.find(L"border-left-style");

                if (oBorderLeftStyle != m_mStyle.end())
                    return oBorderLeftStyle->second;

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.end())
                    sBorderLeft = oBorderLeft->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.end())
                        return oStyle->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderLeftColor() const
            {
                const auto& oBorderLeftColor = m_mStyle.find(L"border-left-color");

                if (oBorderLeftColor != m_mStyle.end())
                    return oBorderLeftColor->second;

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.end())
                    sBorderLeft = oBorderLeft->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderLeft);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.end())
                        return oHEX->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderRight() const
            {
                const auto& oBorderRight = m_mStyle.find(L"border-right");

                if (oBorderRight != m_mStyle.end())
                    return oBorderRight->second;

                const std::wstring& sBorderRightWidth = GetBorderRightWidth();
                const std::wstring& sBorderRightStyle = GetBorderRightStyle();
                const std::wstring& sBorderRightColor = GetBorderRightColor();

                if (sBorderRightWidth.empty() + sBorderRightStyle.empty() + sBorderRightColor.empty() == 0)
                    return sBorderRightWidth + L" " + sBorderRightStyle + L" " + sBorderRightColor;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderRightWidth() const
            {
                const auto& oBorderRightWidth = m_mStyle.find(L"border-right-width");

                if (oBorderRightWidth != m_mStyle.end())
                    return oBorderRightWidth->second;

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.end())
                    sBorderRight = oBorderRight->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderRightStyle() const
            {
                const auto& oBorderRightStyle = m_mStyle.find(L"border-right-style");

                if (oBorderRightStyle != m_mStyle.end())
                    return oBorderRightStyle->second;

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.end())
                    sBorderRight = oBorderRight->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.end())
                        return oStyle->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderRightColor() const
            {
                const auto& oBorderRightColor = m_mStyle.find(L"border-right-color");

                if (oBorderRightColor != m_mStyle.end())
                    return oBorderRightColor->second;

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.end())
                    sBorderRight = oBorderRight->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderRight);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.end())
                        return oHEX->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderTop() const
            {
                const auto& oBorderTop = m_mStyle.find(L"border-top");

                if (oBorderTop != m_mStyle.end())
                    return oBorderTop->second;

                const std::wstring& sBorderTopWidth = GetBorderTopWidth();
                const std::wstring& sBorderTopStyle = GetBorderTopStyle();
                const std::wstring& sBorderTopColor = GetBorderTopColor();

                if (sBorderTopWidth.empty() + sBorderTopStyle.empty() + sBorderTopColor.empty() == 0)
                    return sBorderTopWidth + L" " + sBorderTopStyle + L" " + sBorderTopColor;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderTopWidth() const
            {
                const auto& oBorderTopWidth = m_mStyle.find(L"border-top-width");

                if (oBorderTopWidth != m_mStyle.end())
                    return oBorderTopWidth->second;

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.end())
                    sBorderTop = oBorderTop->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderTopStyle() const
            {
                const auto& oBorderTopStyle = m_mStyle.find(L"border-top-style");

                if (oBorderTopStyle != m_mStyle.end())
                    return oBorderTopStyle->second;

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.end())
                    sBorderTop = oBorderTop->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.end())
                        return oStyle->second;
                }

                return std::wstring();
            }

            std::wstring CCompiledStyle::GetBorderTopColor() const
            {
                const auto& oBorderTopColor = m_mStyle.find(L"border-top-color");

                if (oBorderTopColor != m_mStyle.end())
                    return oBorderTopColor->second;

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.end())
                    sBorderTop = oBorderTop->second;
                else
                    return std::wstring();

                const std::vector<std::wstring>& arWords = NS_STATIC_FUNCTIONS::GetWordsW(sBorderTop);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.end())
                        return oHEX->second;
                }

                return std::wstring();
            }
    }

