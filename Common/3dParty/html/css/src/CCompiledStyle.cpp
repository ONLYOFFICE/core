#include "CCompiledStyle.h"

#include <cwctype>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>

#include <iostream>
#include "../../../../../DesktopEditor/common/File.h"
#include "ConstValues.h"

inline static std::vector<std::wstring> GetWordsW(const std::wstring& sLine)
{
    if (sLine.empty())
        return {};

    std::vector<std::wstring> arWords;
    std::wstring sTempWord = sLine;

    while (true)
    {
        const auto& posFirstNotSpace = sTempWord.find_first_not_of(L" \n\r\t\f\v:;");
        const auto& posLastNotSpace = sTempWord.find_first_of(L" \n\r\t\f\v", posFirstNotSpace);
        const auto& posFirstSign = sTempWord.find_first_of(L":;,", posFirstNotSpace);

        if (posFirstNotSpace != std::wstring::npos && posFirstSign != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1));
            sTempWord.erase(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
            sTempWord.erase(posFirstNotSpace, posLastNotSpace - posFirstNotSpace);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace == std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace));
            break;
        }
        else
            break;
    }

    return arWords;
}

namespace NSCSS
{
    CCompiledStyle::CCompiledStyle(){}

    CCompiledStyle::CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle) : m_mStyle(mStyle) {}

    CCompiledStyle::CCompiledStyle(const CCompiledStyle& oStyle) : m_mStyle(oStyle.m_mStyle),
                                                                   m_sId(oStyle.m_sId),
                                                                   m_arParentsStyles(oStyle.m_arParentsStyles) {}

    CCompiledStyle::~CCompiledStyle()
    {
        m_mStyle.clear();
        m_arParentsStyles.clear();
    }

    CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
    {
        if (oElement.m_mStyle.size() == 0)
            return *this;
        for(const auto& oItem : oElement.m_mStyle)
        {
            std::wstring& sValue = m_mStyle[oItem.first];
            if (sValue != L"inherit" && !oItem.second.empty())
                sValue = oItem.second;
            size_t posImportant = oItem.second.find(L"!important");
            if (posImportant != std::wstring::npos)
                sValue = oItem.second.substr(0, posImportant);
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
        if (GetId()[0] != oStyle.GetId()[0])
            return false;

        if (m_arParentsStyles.size() != oStyle.m_arParentsStyles.size())
            return false;
        for (size_t i = 0; i < m_arParentsStyles.size(); i++)
            if (m_arParentsStyles[i] != oStyle.m_arParentsStyles[i])
                return false;

        if (m_mStyle.size() != oStyle.m_mStyle.size())
            return false;

        auto iterLeft = m_mStyle.begin();
        auto iterRight = oStyle.m_mStyle.begin();
        while (iterLeft != m_mStyle.cend())
        {
            if (iterLeft->first != iterRight->first || iterLeft->second != iterRight->second)
                return false;
            iterLeft++;
            iterRight++;
        }
        return true;
    }

    std::wstring CCompiledStyle::GetStyleW() const
    {
        std::wstring sStyle1;
        for (const auto& oIter : m_mStyle)
            sStyle1 += oIter.first + L":" + oIter.second + L";";
        return sStyle1;
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

    std::map<std::wstring, std::wstring> CCompiledStyle::GetStyleMap() const
    {
        return m_mStyle;
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
        return m_mStyle.empty() && m_arParentsStyles.empty();
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
        std::wstring sTempStyle = sStyle;

        std::wstring sTempProperty;
        std::wstring sValue;

        while (!sTempStyle.empty())
        {
            const auto& posColon = sTempStyle.find(L':');
            const auto& posSemicolon = sTempStyle.find(L';', posColon);

            if (posColon != std::wstring::npos && posSemicolon != std::wstring::npos)
            {
                AddPropSel(sTempStyle.substr(0, posColon), sTempStyle.substr(posColon + 1, posSemicolon - posColon - 1));
                sTempStyle.erase(0, posSemicolon + 1);
            }
            else if (posColon != std::wstring::npos)
            {
                AddPropSel(sTempStyle.substr(0, posColon), sTempStyle.substr(posColon + 1));
                break;
            }
            if (posColon == std::wstring::npos && posSemicolon == std::wstring::npos)
                break;
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

            if (oFont != m_mStyle.cend())
                return oFont->second;

            std::wstring sValue = GetFontStyle()   + L" " +
                                  GetFontVariant() + L" " +
                                  GetFontWeight()  + L" " +
                                  GetFontSize()    + L"/" +
                                  GetLineHeight()  + L" " +
                                  GetFontFamily();

            return (sValue.length() == 5 ? L"" : sValue);
        }

        std::wstring CCompiledStyle::GetFontFamily() const
        {
            const auto& oFontFamily = m_mStyle.find(L"font-family");

            if (oFontFamily != m_mStyle.cend())
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

            if (oFont != m_mStyle.cend())
                sFont = oFont->second;
            else
                return sFont;

            if (sFont.empty())
                return sFont;

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
                sValue = sValue.substr(0, posComma - 1);

            if (!sValue.empty())
            {
                if (sValue.find(L'"') != std::wstring::npos || sValue.find(L'\'') != std::wstring::npos)
                {
                    return sValue;
                }
                return L'"' + sValue + L'"';
            }

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontSize() const
        {
            const auto& oFontSize = m_mStyle.find(L"font-size");

            if (oFontSize != m_mStyle.cend())
                return oFontSize->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.cend())
                sFont = oFont->second;
            else
                return sFont;

            if (sFont.empty())
                return sFont;

            const std::vector<std::wstring> arValues = { L"xx-small",L"x-small", L"small",
                                                         L"medium",  L"large",   L"x-large",
                                                         L"xx-large",L"larger",  L"smaller"};

            for (std::wstring sValue : arValues)
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
                        sTempUnitMeasure = L"";
                    }
                }
            }

            if (posUnitMeasure == 0)
            {
                std::wstring sValue;
                for (int  i = sFont.length() - 1; i >= 0; --i)
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
                int num = 0;
                std::wstring sValue;
                while ((posUnitMeasure - num) > 0 &&
                       (iswdigit(sFont[posUnitMeasure - num]) ||
                        sFont[posUnitMeasure - num] == '.'))
                {
                    sValue = sFont[posUnitMeasure - num] + sValue;
                    num++;
                }
                if (!sValue.empty())
                    return sValue + sTempUnitMeasure;
            }

            return L"";
        }

        std::wstring CCompiledStyle::GetFontSizeAdjust() const
        {
            const auto& oFontSizeAdj = m_mStyle.find(L"font-size-adjust");

            if (oFontSizeAdj != m_mStyle.cend())
                return oFontSizeAdj->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetFontStretch() const
        {
            const auto& oFontStretch = m_mStyle.find(L"font-stretch");

            if (oFontStretch != m_mStyle.cend())
                return oFontStretch->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetFontStyle() const
        {
            const auto& oFontStyle = m_mStyle.find(L"font-style");

            if (oFontStyle != m_mStyle.cend())
                return oFontStyle->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.cend())
                sFont = oFont->second;
            else
                return sFont;

            if (sFont.empty())
                return sFont;

            std::vector<std::wstring> arValues = {L"italic", L"oblique"};

            for (const std::wstring& sValue : arValues)
                if (sFont.find(sValue) != std::wstring::npos)
                    return sValue;

            if (iswdigit(sFont[0]))
                return L"";

            std::wstring sValue;
            const auto& posSpace = sFont.find(L' ');

            if (posSpace != std::wstring::npos)
                sValue = sFont.substr(0, posSpace);

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontVariant() const
        {
            const auto& oFontVariant = m_mStyle.find(L"font-variant");

            if (oFontVariant != m_mStyle.cend())
                return oFontVariant->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.cend())
                sFont = oFont->second;
            else
                return sFont;

            if (sFont.empty())
                return sFont;

            if (sFont.find(L"small-caps") != std::wstring::npos)
                return L"small-caps";

            if (iswdigit(sFont[0]))
                return L"";

            std::wstring sValue;
            auto posSpace = sFont.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sFont[0]))
                sValue = sFont.substr(posSpace + 1);
            else
                return L"";

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(0, posSpace);
            else
                return L"";

            return sValue;
        }

        std::wstring CCompiledStyle::GetFontWeight() const
        {
            const auto& oFontWeight = m_mStyle.find(L"font-weight");

            if (oFontWeight != m_mStyle.cend())
                return oFontWeight->second;

            const auto& oFont = m_mStyle.find(L"font");
            if (oFont == m_mStyle.cend())
                return L"";

            std::wstring sFont = oFont->second;
            if (sFont.empty())
                return L"";

            const std::vector<std::wstring> arValues = {L"bold", L"bolder", L"lighter"};

            for (const std::wstring& sValue : arValues)
                if (sFont.find(sValue) != std::wstring::npos)
                    return sValue;

            if (iswdigit(sFont[0]))
                return L"";

            std::wstring sValue = sFont;
            auto posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(posSpace + 1);
            else
                return L"";

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(posSpace + 1);
            else
                return L"";

            posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos && !iswdigit(sValue[0]))
                sValue = sValue.substr(0, posSpace);
            else
                return L"";

            return sValue;
        }

        std::wstring CCompiledStyle::GetLineHeight() const
        {
            const auto& oLineHeight = m_mStyle.find(L"line-height");

            if (oLineHeight != m_mStyle.cend())
                return oLineHeight->second;

            std::wstring sFont;

            const auto& oFont = m_mStyle.find(L"font");

            if (oFont != m_mStyle.cend())
                sFont = oFont->second;
            else
                return sFont;

            if (sFont.empty())
                return sFont;

            const auto& posSlash = sFont.find(L'/');

            if (posSlash == std::wstring::npos)
                return L"";

            std::wstring sValue = sFont.substr(posSlash + 1);
            auto posSpace = sValue.find(L' ');

            if (posSpace != std::wstring::npos)
                sValue = sValue.substr(0, posSpace);

            if (iswalpha(sValue[sValue.length() - 1]))
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

        std::vector<std::wstring> CCompiledStyle::GetFontNames()
        {
            std::wstring sFontNames = GetFontFamily();
            std::vector<std::wstring> arFontNames;

            if (sFontNames.empty())
                return {};

            const auto& posLastComma = sFontNames.find_last_of(L',');

            if (posLastComma != std::wstring::npos)
                sFontNames = sFontNames.substr(0, posLastComma);

            std::wstring sTemp;

            for (const wchar_t& wc : sFontNames)
            {
                if (wc != L',' && wc != L' ')
                    sTemp += wc;

                if (wc == L',')
                {
                    arFontNames.push_back(sTemp);
                    sTemp.clear();
                }
            }

            if (!sTemp.empty())
                arFontNames.push_back(sTemp);

            return arFontNames;
        }

    /* MARGIN */
        std::wstring CCompiledStyle::GetMargin() const
        {
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.cend())
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

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginTop() const
        {
            const auto& oMarginTop = m_mStyle.find(L"margin-top");

            if (oMarginTop != m_mStyle.cend())
                return oMarginTop->second;

            const std::wstring& sMarginBlockStart = GetMarginBlockStart();

            if (!sMarginBlockStart.empty())
                return sMarginBlockStart;

            std::wstring sMargin;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.cend())
                sMargin = oMargin->second;
            else
                return sMargin;

            if (sMargin.empty())
                return sMargin;

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

            if (arValues.size() != 0)
                return arValues[0];

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginBlockStart() const
        {
            const auto& oMarginBlockStart = m_mStyle.find(L"margin-block-start");

            if (oMarginBlockStart != m_mStyle.cend())
                return oMarginBlockStart->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginLeft() const
        {
            const auto& oMarginLeft = m_mStyle.find(L"margin-left");

            if (oMarginLeft != m_mStyle.cend())
                return oMarginLeft->second;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.cend())
                sValue = oMargin->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginRight() const
        {
            const auto& oMarginRight = m_mStyle.find(L"margin-right");

            if (oMarginRight != m_mStyle.cend())
                return oMarginRight->second;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.cend())
                sValue = oMargin->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginBottom() const
        {
            const auto& oMarginBottom = m_mStyle.find(L"margin-bottom");

            if (oMarginBottom != m_mStyle.cend())
                return oMarginBottom->second;

            const std::wstring& sMarginBlockEnd = GetMarginBlockEnd();

            if (!sMarginBlockEnd.empty())
                return sMarginBlockEnd;

            std::wstring sValue;
            const auto& oMargin = m_mStyle.find(L"margin");

            if (oMargin != m_mStyle.cend())
                sValue = oMargin->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginBlockEnd() const
        {
            const auto& oMarginBlockEnd = m_mStyle.find(L"margin-block-end");

            if (oMarginBlockEnd != m_mStyle.cend())
                return oMarginBlockEnd->second;

            return L"";
        }

    /* PADDING */
        std::wstring CCompiledStyle::GetPadding() const
        {
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.cend())
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

            if (oPaddingTop != m_mStyle.cend())
                return oPaddingTop->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.cend())
                sValue = oPadding->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            if (arValues.size() != 0)
                return arValues[0];

            return L"";
        }

        std::wstring CCompiledStyle::GetPaddingLeft() const
        {
            const auto& oPaddingLeft = m_mStyle.find(L"padding-left");

            if (oPaddingLeft != m_mStyle.cend())
                return oPaddingLeft->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.cend())
                sValue = oPadding->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

        std::wstring CCompiledStyle::GetPaddingRight() const
        {
            const auto& oPaddingRight = m_mStyle.find(L"padding-right");

            if (oPaddingRight != m_mStyle.cend())
                return oPaddingRight->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.cend())
                sValue = oPadding->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

        std::wstring CCompiledStyle::GetPaddingBottom() const
        {
            const auto& oPaddingBottom = m_mStyle.find(L"padding-bottom");

            if (oPaddingBottom != m_mStyle.cend())
                return oPaddingBottom->second;

            std::wstring sValue;
            const auto& oPadding = m_mStyle.find(L"padding");

            if (oPadding != m_mStyle.cend())
                sValue = oPadding->second;
            else
                return sValue;

            if (sValue.empty())
                return sValue;

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

            return L"";
        }

    /*  SPACING */
        std::wstring CCompiledStyle::GetLetterSpacing() const
        {
            const auto& oLetterSpacing = m_mStyle.find(L"letter-spacing");

            if (oLetterSpacing != m_mStyle.cend())
                return oLetterSpacing->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetWordSpacing() const
        {
            const auto& oWordSpacing = m_mStyle.find(L"word-spacing");

            if (oWordSpacing != m_mStyle.cend())
                return oWordSpacing->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderSpacing() const
        {
            const auto& oBorderSpacing = m_mStyle.find(L"border-spacing");

            if (oBorderSpacing != m_mStyle.cend())
                return oBorderSpacing->second;

            return L"";
        }

    /* COLOR */
        std::wstring CCompiledStyle::GetTextDecorationColor() const
        {
            const auto& oTextDecorationColor = m_mStyle.find(L"text-decoration-color");

            if (oTextDecorationColor != m_mStyle.cend())
                return oTextDecorationColor->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetBackgroundColor() const
        {
            const auto& oBackgroundColor = m_mStyle.find(L"background-color");

            if (oBackgroundColor != m_mStyle.cend())
                return oBackgroundColor->second;

            const std::wstring& sBackground = GetBackground();

            if (sBackground.empty())
                return L"";

            const std::vector<std::wstring>& arWords = GetWordsW(sBackground);

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

                if (oHEX != NS_CONST_VALUES::mColors.cend())
                    return oHEX->second;
            }

            return L"";
        }

        std::wstring CCompiledStyle::GetColor() const
        {
            const auto& oColor = m_mStyle.find(L"color");

            if (oColor != m_mStyle.cend())
                return oColor->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetOutlineColor() const
        {
            const auto& oOutlineColor = m_mStyle.find(L"outline-color");

            if (oOutlineColor != m_mStyle.cend())
                return oOutlineColor->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetColumnRuleColor() const
        {
            const auto& oColumnRuleColor = m_mStyle.find(L"column-rule-color");

            if (oColumnRuleColor != m_mStyle.cend())
                return oColumnRuleColor->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetBackground() const
        {
            const auto& oBackground = m_mStyle.find(L"background");

            if (oBackground != m_mStyle.cend())
                return  oBackground->second;

            return L"";
        }

    /* TEXT */
        std::wstring CCompiledStyle::GetTextAlign() const
        {
            const auto& oTextAlign = m_mStyle.find(L"text-align");

            if (oTextAlign != m_mStyle.cend())
                return oTextAlign->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetTextIndent() const
        {
            const auto& oTextIndent = m_mStyle.find(L"text-indent");

            if (oTextIndent != m_mStyle.cend())
                return oTextIndent->second;

            return L"";
        }

        std::wstring CCompiledStyle::GetTextDecoration() const
        {
            const auto& oTextDecoration = m_mStyle.find(L"text-decoration");

            if (oTextDecoration != m_mStyle.cend())
                return oTextDecoration->second;

            return L"";
        }

        /* BORDER */

            std::wstring CCompiledStyle::GetBorder() const
            {
                const auto& oBorder = m_mStyle.find(L"border");

                if (oBorder != m_mStyle.cend() && oBorder->second != L"none")
                    return oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.cend() && oMsoBorder->second != L"none")
                        return oMsoBorder->second;
                }

                const std::wstring& sBorderWidth = GetBorderWidth();
                const std::wstring& sBorderStyle = GetBorderStyle();
                const std::wstring& sBorderColor = GetBorderColor();

                if (sBorderWidth.empty() + sBorderStyle.empty() + sBorderColor.empty() == 0)
                    return sBorderWidth + L" " + sBorderStyle + L" " + sBorderColor;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderWidth() const
            {
                const auto& oBorderWidth = m_mStyle.find(L"border-width");

                if (oBorderWidth != m_mStyle.cend())
                    return oBorderWidth->second;

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.cend() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.cend() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return sBorder;
                }

                const std::vector<std::wstring>& arWords = GetWordsW(sBorder);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderStyle() const
            {
                const auto& oBorderStyle = m_mStyle.find(L"border-style");

                if (oBorderStyle != m_mStyle.cend())
                    return oBorderStyle->second;

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.cend() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.cend() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return sBorder;
                }

                const std::vector<std::wstring>& arWords = GetWordsW(sBorder);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.cend())
                        return oStyle->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderColor() const
            {
                const auto& oBorderColor = m_mStyle.find(L"border-color");

                if (oBorderColor != m_mStyle.cend())
                    return oBorderColor->second;\

                const auto& oBorder = m_mStyle.find(L"border");

                std::wstring sBorder;

                if (oBorder != m_mStyle.cend() && oBorder->second != L"none")
                    sBorder = oBorder->second;
                else
                {
                    const auto& oMsoBorder = m_mStyle.find(L"mso-border-alt");
                    if (oMsoBorder != m_mStyle.cend() && oMsoBorder->second != L"none")
                        sBorder =  oMsoBorder->second;
                    else
                        return sBorder;
                }

                const std::vector<std::wstring>& arWords = GetWordsW(sBorder);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.cend())
                        return oHEX->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderBottom() const
            {
                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                if (oBorderBottom != m_mStyle.cend())
                    return oBorderBottom->second;

                const std::wstring& sBorderBottomWidth = GetBorderBottomWidth();
                const std::wstring& sBorderBottomStyle = GetBorderBottomStyle();
                const std::wstring& sBorderBottomColor = GetBorderBottomColor();

                if (sBorderBottomWidth.empty() + sBorderBottomStyle.empty() + sBorderBottomColor.empty() == 0)
                    return sBorderBottomWidth + L" " + sBorderBottomStyle + L" " + sBorderBottomColor;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderBottomWidth() const
            {
                const auto& oBorderBottomWidth = m_mStyle.find(L"border-bottom-width");

                if (oBorderBottomWidth != m_mStyle.cend())
                    return oBorderBottomWidth->second;

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.cend())
                    sBorderBottom = oBorderBottom->second;
                else
                    return sBorderBottom;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderBottom);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderBottomStyle() const
            {
                const auto& oBorderBottomStyle = m_mStyle.find(L"border-bottom-style");

                if (oBorderBottomStyle != m_mStyle.cend())
                    return oBorderBottomStyle->second;

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.cend())
                    sBorderBottom = oBorderBottom->second;
                else
                    return sBorderBottom;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderBottom);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.cend())
                        return oStyle->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderBottomColor() const
            {
                const auto& oBorderBottomColor = m_mStyle.find(L"border-bottom-color");

                if (oBorderBottomColor != m_mStyle.cend())
                    return oBorderBottomColor->second;\

                const auto& oBorderBottom = m_mStyle.find(L"border-bottom");

                std::wstring sBorderBottom;

                if (oBorderBottom != m_mStyle.cend())
                    sBorderBottom = oBorderBottom->second;
                else
                    return sBorderBottom;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderBottom);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.cend())
                        return oHEX->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderLeft() const
            {
                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                if (oBorderLeft != m_mStyle.cend())
                    return oBorderLeft->second;

                const std::wstring& sBorderLeftWidth = GetBorderLeftWidth();
                const std::wstring& sBorderLeftStyle = GetBorderLeftStyle();
                const std::wstring& sBorderLeftColor = GetBorderLeftColor();

                if (sBorderLeftWidth.empty() + sBorderLeftStyle.empty() + sBorderLeftColor.empty() == 0)
                    return sBorderLeftWidth + L" " + sBorderLeftStyle + L" " + sBorderLeftColor;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderLeftWidth() const
            {
                const auto& oBorderLeftWidth = m_mStyle.find(L"border-left-width");

                if (oBorderLeftWidth != m_mStyle.cend())
                    return oBorderLeftWidth->second;

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.cend())
                    sBorderLeft = oBorderLeft->second;
                else
                    return sBorderLeft;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderLeft);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderLeftStyle() const
            {
                const auto& oBorderLeftStyle = m_mStyle.find(L"border-left-style");

                if (oBorderLeftStyle != m_mStyle.cend())
                    return oBorderLeftStyle->second;

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.cend())
                    sBorderLeft = oBorderLeft->second;
                else
                    return sBorderLeft;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderLeft);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.cend())
                        return oStyle->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderLeftColor() const
            {
                const auto& oBorderLeftColor = m_mStyle.find(L"border-left-color");

                if (oBorderLeftColor != m_mStyle.cend())
                    return oBorderLeftColor->second;\

                const auto& oBorderLeft = m_mStyle.find(L"border-left");

                std::wstring sBorderLeft;

                if (oBorderLeft != m_mStyle.cend())
                    sBorderLeft = oBorderLeft->second;
                else
                    return sBorderLeft;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderLeft);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.cend())
                        return oHEX->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderRight() const
            {
                const auto& oBorderRight = m_mStyle.find(L"border-right");

                if (oBorderRight != m_mStyle.cend())
                    return oBorderRight->second;

                const std::wstring& sBorderRightWidth = GetBorderRightWidth();
                const std::wstring& sBorderRightStyle = GetBorderRightStyle();
                const std::wstring& sBorderRightColor = GetBorderRightColor();

                if (sBorderRightWidth.empty() + sBorderRightStyle.empty() + sBorderRightColor.empty() == 0)
                    return sBorderRightWidth + L" " + sBorderRightStyle + L" " + sBorderRightColor;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderRightWidth() const
            {
                const auto& oBorderRightWidth = m_mStyle.find(L"border-right-width");

                if (oBorderRightWidth != m_mStyle.cend())
                    return oBorderRightWidth->second;

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.cend())
                    sBorderRight = oBorderRight->second;
                else
                    return sBorderRight;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderRight);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderRightStyle() const
            {
                const auto& oBorderRightStyle = m_mStyle.find(L"border-right-style");

                if (oBorderRightStyle != m_mStyle.cend())
                    return oBorderRightStyle->second;

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.cend())
                    sBorderRight = oBorderRight->second;
                else
                    return sBorderRight;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderRight);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.cend())
                        return oStyle->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderRightColor() const
            {
                const auto& oBorderRightColor = m_mStyle.find(L"border-right-color");

                if (oBorderRightColor != m_mStyle.cend())
                    return oBorderRightColor->second;\

                const auto& oBorderRight = m_mStyle.find(L"border-right");

                std::wstring sBorderRight;

                if (oBorderRight != m_mStyle.cend())
                    sBorderRight = oBorderRight->second;
                else
                    return sBorderRight;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderRight);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.cend())
                        return oHEX->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderTop() const
            {
                const auto& oBorderTop = m_mStyle.find(L"border-top");

                if (oBorderTop != m_mStyle.cend())
                    return oBorderTop->second;

                const std::wstring& sBorderTopWidth = GetBorderTopWidth();
                const std::wstring& sBorderTopStyle = GetBorderTopStyle();
                const std::wstring& sBorderTopColor = GetBorderTopColor();

                if (sBorderTopWidth.empty() + sBorderTopStyle.empty() + sBorderTopColor.empty() == 0)
                    return sBorderTopWidth + L" " + sBorderTopStyle + L" " + sBorderTopColor;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderTopWidth() const
            {
                const auto& oBorderTopWidth = m_mStyle.find(L"border-top-width");

                if (oBorderTopWidth != m_mStyle.cend())
                    return oBorderTopWidth->second;

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.cend())
                    sBorderTop = oBorderTop->second;
                else
                    return sBorderTop;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderTop);

                for (const std::wstring& sWidth : arWords)
                    if (iswdigit(sWidth[0]))
                        return sWidth;

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderTopStyle() const
            {
                const auto& oBorderTopStyle = m_mStyle.find(L"border-top-style");

                if (oBorderTopStyle != m_mStyle.cend())
                    return oBorderTopStyle->second;

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.cend())
                    sBorderTop = oBorderTop->second;
                else
                    return sBorderTop;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderTop);

                for (std::wstring sStyle : arWords)
                {
                    std::transform(sStyle.begin(), sStyle.end(), sStyle.begin(), tolower);
                    const auto& oStyle = NS_CONST_VALUES::mStyles.find(sStyle);

                    if (oStyle != NS_CONST_VALUES::mStyles.cend())
                        return oStyle->second;
                }

                return L"";
            }

            std::wstring CCompiledStyle::GetBorderTopColor() const
            {
                const auto& oBorderTopColor = m_mStyle.find(L"border-top-color");

                if (oBorderTopColor != m_mStyle.cend())
                    return oBorderTopColor->second;\

                const auto& oBorderTop = m_mStyle.find(L"border-top");

                std::wstring sBorderTop;

                if (oBorderTop != m_mStyle.cend())
                    sBorderTop = oBorderTop->second;
                else
                    return sBorderTop;

                const std::vector<std::wstring>& arWords = GetWordsW(sBorderTop);

                for (std::wstring sColor : arWords)
                {
                    if (sColor[0] == L'#')
                        return sColor.substr(1);

                    std::transform(sColor.begin(), sColor.end(), sColor.begin(), tolower);

                    const auto& oHEX = NS_CONST_VALUES::mColors.find(sColor);

                    if (oHEX != NS_CONST_VALUES::mColors.cend())
                        return oHEX->second;
                }

                return L"";
            }
    }

