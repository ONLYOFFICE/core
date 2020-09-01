#include "CCompiledStyle.h"

#include <cctype>
#include <vector>
#include <codecvt>

#include <iostream>

namespace NSCSS
{
    CCompiledStyle::CCompiledStyle()
    {
    }

    CCompiledStyle::CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle)
    {
        m_mStyle = mStyle;
    }

    CCompiledStyle::CCompiledStyle(const CCompiledStyle& oStyle)
    {
        m_mStyle = oStyle.m_mStyle;
        m_sId = oStyle.m_sId;
        m_arParentsStyles = oStyle.m_arParentsStyles;
    }

    CCompiledStyle::~CCompiledStyle()
    {
    }

    CCompiledStyle& CCompiledStyle::operator+= (const CCompiledStyle &oElement)
    {
        if (oElement.m_mStyle.size() == 0)
            return *this;

        for(auto& item : oElement.m_mStyle)
        {
            std::wstring& sValue = m_mStyle[item.first];

            if (sValue != L"inherit" && !item.second.empty())
                sValue = item.second;

            auto posImportant = item.second.find(L"!important");

            if (posImportant != std::wstring::npos)
                sValue = item.second.substr(0, posImportant);
        }

        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator-= (const CCompiledStyle &oElement)
    {
        std::map<std::wstring, std::wstring> oStyle;

        for (auto item : m_mStyle)
        {
            if (oElement.m_mStyle.find(item.first) != oElement.m_mStyle.cend())
                oStyle.emplace(item.first, item.second);
        }
        m_mStyle = oStyle;

        return *this;
    }

    CCompiledStyle& CCompiledStyle::operator= (const CCompiledStyle &oElement)
    {
        m_mStyle = oElement.m_mStyle;
        m_sId = oElement.m_sId;
//        m_arParentsStyles = oElement.m_arParentsStyles;
        return *this;
    }


//    bool CCompiledStyle::operator==(const CCompiledStyle &oElement)
//    {
//        std::wstring sThisName = this->GetId();

//        auto posDash = sThisName.find(L'-');

//        if (posDash != std::wstring::npos)
//            sThisName = sThisName.substr(0, posDash);

//        auto posLattice = sThisName.find(L'#');

//        if (posLattice != std::wstring::npos)
//            sThisName = sThisName.substr(0, posLattice);

//        auto posPoint = sThisName.find(L'.');

//        if (posPoint != std::wstring::npos)
//            sThisName = sThisName.substr(0, posPoint);

//        std::wstring sElementName = oElement.m_sId;

//        posDash = sElementName.find(L'-');

//        if (posDash != std::wstring::npos)
//            sElementName = sElementName.substr(0, posDash);

//        posLattice = sElementName.find(L'#');

//        if (posLattice != std::wstring::npos)
//            sElementName = sElementName.substr(0, posLattice);

//        posPoint = sElementName.find(L'.');

//        if (posPoint != std::wstring::npos)
//            sElementName = sElementName.substr(0, posPoint);

//        if (sThisName != sElementName)
//            return false;

//        if (this->m_arParentsStyles.size() != oElement.m_arParentsStyles.size())
//            return false;

//        for (size_t i = 0; i < this->m_arParentsStyles.size(); i++)
//            if (this->m_arParentsStyles[i] != oElement.m_arParentsStyles[i])
//                return false;

//        if (this->m_mStyle.size() != oElement.m_mStyle.size())
//            return false;

//        auto iterLeft = this->m_mStyle.begin();
//        auto iterRight = oElement.m_mStyle.begin();

//        while (iterLeft != this->m_mStyle.cend())
//        {
//            if (iterLeft->first != iterRight->first ||
//                iterLeft->second != iterRight->second)
//                return false;

//            iterLeft++;
//            iterRight++;
//        }

//        return true;
//    }

    bool CCompiledStyle::operator!=(const CCompiledStyle &oElement)
    {
        if (*this > oElement)
            return false;

        if (*this < oElement)
            return false;

        return true;
    }

    bool CCompiledStyle::operator>(const CCompiledStyle &oElement)
    {
        return (m_mStyle.size() > oElement.m_mStyle.size());
    }

    bool CCompiledStyle::operator<(const CCompiledStyle &oElement)
    {
        return (m_mStyle.size() < oElement.m_mStyle.size());
    }

    const std::map<std::wstring, std::wstring>& CCompiledStyle::GetStyleMap() const
    {
        return m_mStyle;
    }

    std::wstring CCompiledStyle::GetStyleW() const
    {
        std::wstring sStyle;
        for (auto iter = m_mStyle.begin(); iter != m_mStyle.cend(); iter++)
            sStyle += iter->first + L":" + iter->second + L";";
        return sStyle;
    }

    std::string CCompiledStyle::GetStyle() const
    {
        std::wstring sStyle = GetStyleW();
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(sStyle);
    }

    const size_t& CCompiledStyle::GetSize() const
    {
        return m_mStyle.size();
    }

    bool CCompiledStyle::Empty() const
    {
        if (m_mStyle.size() == 0)
            return true;

        if (m_mStyle.size() == 0 && m_arParentsStyles.size() == 0)
            return true;

        return false;
    }

    void CCompiledStyle::Clear()
    {
        m_mStyle.clear();
        m_sId.clear();
        m_arParentsStyles.clear();
    }

    void CCompiledStyle::AddPropSel(const std::wstring& sProperty, const std::wstring& sValue)
    {
        m_mStyle.emplace(sProperty, sValue);
    }

    void CCompiledStyle::InsertStyle(const std::map<std::wstring, std::wstring>& mStyle)
    {
        m_mStyle.insert(mStyle.begin(), mStyle.end());
    }

    void CCompiledStyle::SetStyle(const std::map<std::wstring, std::wstring>& mStyle)
    {
        m_mStyle = mStyle;
    }

    void CCompiledStyle::AddStyle(const std::wstring& sStyle)
    {
        std::wstring sProperty;
        std::wstring sValue;
        size_t nPosition = 0;

        wchar_t wc;

        while(nPosition < sStyle.length())
        {
            wc = sStyle[nPosition];
            while (nPosition < sStyle.length() && wc != L':')
            {
                if (!iswspace(wc))
                    sProperty += wc;
                wc = sStyle[++nPosition];
            }
            wc = sStyle[++nPosition];

            while (nPosition < sStyle.length() && sStyle[nPosition] != L';')
            {
                sValue += sStyle[nPosition];
                wc = sStyle[++nPosition];
            }

            if (nPosition < sStyle.length())
                wc = sStyle[++nPosition];

            if (!sProperty.empty() && !sValue.empty())
            {
                auto posExclamation = sValue.find(L'!');
                auto posSemicolon = sValue.find(L';');
                auto posColon = sValue.find(L':');

                if (posExclamation != std::wstring::npos)
                    sValue = sValue.substr(0, posExclamation - 1);

                if (posSemicolon != std::wstring::npos)
                    sValue = sValue.substr(0, posSemicolon);

                if (posColon != std::wstring::npos)
                    sValue = sValue.substr(0, posColon);

                AddPropSel(sProperty, sValue);
                sProperty.clear();
                sValue.clear();
            }
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

    const std::map<std::wstring, std::wstring>::iterator& CCompiledStyle::GetBegin()
    {
        return m_mStyle.begin();
    }

    const std::map<std::wstring, std::wstring>::iterator& CCompiledStyle::GetEnd()
    {
        return m_mStyle.end();
    }

    const double CCompiledStyle::GetWeidth()
    {
        double dWidth;
        for (auto sValue : m_mStyle)
        {
            dWidth += sValue.first.length();
            dWidth += sValue.second.length();
        }

        for (std::wstring sValue : m_arParentsStyles)
        {
            dWidth += sValue.length() / 2;
        }

        return dWidth;
    }

    /* FONT */

        std::wstring CCompiledStyle::GetFont()
        {
            if (m_mStyle.find(L"font") != m_mStyle.cend())
                return m_mStyle[L"font"];

            std::wstring sValue = GetFontStyle()   + L" " +
                                  GetFontVariant() + L" " +
                                  GetFontWeight()  + L" " +
                                  GetFontSize()    + L"/" +
                                  GetLineHeight()  + L" " +
                                  GetFontFamily();
        }

        std::wstring CCompiledStyle::GetFontFamily()
        {
            if (m_mStyle.find(L"font-family") != m_mStyle.cend())
            {
                std::wstring sFontFamily = m_mStyle[L"font-family"];

                if (sFontFamily.find(L',') != std::wstring::npos)
                    sFontFamily = sFontFamily.substr(0, sFontFamily.find(L','));

                if (sFontFamily.find(L'"') != std::wstring::npos || sFontFamily.find(L'\'') != std::wstring::npos)
                    return sFontFamily;

                return  L'"' + sFontFamily + L'"';
            }
            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            int nPos1;

            for (int i = (int)sFont.length() - 1; i >= 0; i--)
            {
                if(iswdigit(sFont[i]))
                {
                    nPos1 = i;
                    break;
                }
            }

            std::wstring sValue = sFont.substr(nPos1 + 1);

            auto posComma = sValue.find(L',');

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
            return L"";
        }

        std::wstring CCompiledStyle::GetFontSize()
        {
            if (m_mStyle.find(L"font-size") != m_mStyle.cend())
                return m_mStyle[L"font-size"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

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
            size_t posUnitMeasure;

            for (std::wstring sUnitMesure : arUnitMeasure)
            {
                posUnitMeasure = sFont.find(sUnitMesure);
                if (posUnitMeasure != std::wstring::npos)
                {
                    sTempUnitMeasure = sUnitMesure;
                    wchar_t wc = sFont[posUnitMeasure - 1];

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
                    wchar_t wc = sFont[i];

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
            else if (posUnitMeasure != 0)
            {
                int num = 0;
                std::wstring sValue;
                while ((posUnitMeasure - num) > 0 &&
                       (isdigit(sFont[posUnitMeasure - num]) ||
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

        std::wstring CCompiledStyle::GetFontSizeAdjust()
        {
            if (m_mStyle.find(L"font-size-adjust") != m_mStyle.cend())
                return m_mStyle[L"font-size-adjust"];
            return L"";
        }

        std::wstring CCompiledStyle::GetFontStretch()
        {
            if (m_mStyle.find(L"font-stretch") != m_mStyle.cend())
                return m_mStyle[L"font-stretch"];
            return L"";
        }

        std::wstring CCompiledStyle::GetFontStyle()
        {
            if (m_mStyle.find(L"font-style") != m_mStyle.cend())
                return m_mStyle[L"font-style"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            std::vector<std::wstring> arValues = {L"italic", L"oblique"};

            for (std::wstring sValue : arValues)
                if (sFont.find(sValue))
                    return sValue;

            if (iswdigit(sFont[0]))
                return L"";

            std::wstring sValue;
            auto posSpace = sFont.find(L' ');

            if (posSpace != std::wstring::npos)
                sValue = sFont.substr(0, posSpace);

            if(!sValue.empty())
                return L"";

            return L"";
        }

        std::wstring CCompiledStyle::GetFontVariant()
        {
            if (m_mStyle.find(L"font-variant") != m_mStyle.cend())
                return m_mStyle[L"font-variant"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            if (sFont.find(L"small-caps"))
                return L"small-caps";

            if (isdigit(sFont[0]))
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

            if(!sValue.empty())
                return L"";

            return L"";
        }

        std::wstring CCompiledStyle::GetFontWeight()
        {
            if (m_mStyle.find(L"font-weight") != m_mStyle.cend())
                return m_mStyle[L"font-weight"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            const std::vector<std::wstring> arValues = {L"bold",   L"bolder",
                                                        L"lighter"};

            for (std::wstring sValue : arValues)
                if (sFont.find(sValue))
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

            if(!sValue.empty())
                return L"";

            return L"";
        }

        std::wstring CCompiledStyle::GetLineHeight()
        {
            if (m_mStyle.find(L"line-height") != m_mStyle.cend())
                return m_mStyle[L"line-height"];

            std::wstring sFont;

            if (m_mStyle.find(L"font") != m_mStyle.cend())
                sFont = m_mStyle[L"font"];

            if (sFont.empty())
                return L"";

            if (sFont.find(L"/") == std::wstring::npos)
                return L"";

            auto posSlash = sFont.find(L"/");

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

            auto posLastComma = sFontNames.find_last_of(L',');

            if (posLastComma != std::wstring::npos)
                sFontNames = sFontNames.substr(0, posLastComma);

            std::wstring sTemp;

            for (wchar_t wc : sFontNames)
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
        std::wstring CCompiledStyle::GetMargin()
        {
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                return m_mStyle[L"margin"];

            std::wstring sValue;

            std::wstring sTop    = GetMarginTop();
            std::wstring sLeft   = GetMarginLeft();
            std::wstring sRight  = GetMarginRight();
            std::wstring sBottom = GetMarginBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginTop()
        {
            if (m_mStyle.find(L"margin-top") != m_mStyle.cend())
                return m_mStyle[L"margin-top"];

            if (!GetMarginBlockStart().empty())
                return GetMarginBlockStart();

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetMarginBlockStart()
        {
            if (m_mStyle.find(L"margin-block-start") != m_mStyle.cend())
                return m_mStyle[L"margin-block-start"];

            return L"";
        }

        std::wstring CCompiledStyle::GetMarginLeft()
        {
            if (m_mStyle.find(L"margin-left") != m_mStyle.cend())
                return m_mStyle[L"margin-left"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetMarginRight()
        {
            if (m_mStyle.find(L"margin-right") != m_mStyle.cend())
                return m_mStyle[L"margin-right"];

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetMarginBottom()
        {
            if (m_mStyle.find(L"margin-bottom") != m_mStyle.cend())
                return m_mStyle[L"margin-bottom"];

            if (!GetMarginBlockEnd().empty())
                return GetMarginBlockEnd();

            std::wstring sValue;
            if (m_mStyle.find(L"margin") != m_mStyle.cend())
                sValue = m_mStyle[L"margin"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetMarginBlockEnd()
        {
            if (m_mStyle.find(L"margin-block-end") != m_mStyle.cend())
                return m_mStyle[L"margin-block-end"];

            return L"";
        }

    /* PADDING */
        std::wstring CCompiledStyle::GetPadding()
        {
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                return m_mStyle[L"padding"];

            std::wstring sValue;

            std::wstring sTop    = GetPaddingTop();
            std::wstring sLeft   = GetPaddingLeft();
            std::wstring sRight  = GetPaddingRight();
            std::wstring sBottom = GetPaddingBottom();

            if ((sTop == sLeft) && (sLeft == sRight) && (sRight == sBottom))
                return sTop;

            if ((sTop == sBottom) && (sLeft == sRight))
                return  sTop + L" " + sLeft;

            if (sLeft == sRight)
                return sTop + L" " + sLeft + L" " + sBottom;

            return sTop + L" " + sRight + L" " + sBottom + L" " + sLeft;
        }

        std::wstring CCompiledStyle::GetPaddingTop()
        {
            if (m_mStyle.find(L"padding-top") != m_mStyle.cend())
                return m_mStyle[L"padding-top"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetPaddingLeft()
        {
            if (m_mStyle.find(L"padding-left") != m_mStyle.cend())
                return m_mStyle[L"padding-left"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetPaddingRight()
        {
            if (m_mStyle.find(L"padding-right") != m_mStyle.cend())
                return m_mStyle[L"padding-right"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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

        std::wstring CCompiledStyle::GetPaddingBottom()
        {
            if (m_mStyle.find(L"padding-bottom") != m_mStyle.cend())
                return m_mStyle[L"padding-bottom"];

            std::wstring sValue;
            if (m_mStyle.find(L"padding") != m_mStyle.cend())
                sValue = m_mStyle[L"padding"];

            if (sValue.empty())
                return L"";

            std::vector<std::wstring> arValues;

            std::wstring sTemp;

            for (wchar_t wc : sValue)
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
        std::wstring CCompiledStyle::GetLetterSpacing()
        {
            if (m_mStyle.find(L"letter-spacing") != m_mStyle.cend())
                return m_mStyle[L"letter-spacing"];
            return L"";
        }

        std::wstring CCompiledStyle::GetWordSpacing()
        {
            if (m_mStyle.find(L"word-spacing") != m_mStyle.cend())
                return m_mStyle[L"word-spacing"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBorderSpacing()
        {
            if (m_mStyle.find(L"border-spacing") != m_mStyle.cend())
                return m_mStyle[L"border-spacing"];
            return L"";
        }

    /* COLOR */
        std::wstring CCompiledStyle::GetTextDecorationColor()
        {
            if (m_mStyle.find(L"text-decoration-color") != m_mStyle.cend())
                return m_mStyle[L"text-decoration-color"];
            return L"";
        }

        std::wstring CCompiledStyle::GetBackgroundColor()
        {
            if (m_mStyle.find(L"background-color") != m_mStyle.cend())
                return m_mStyle[L"background-color"];

            std::wstring sBackground = GetBackground();
            auto posLattice = sBackground.find(L'#');

            if (!sBackground.empty() && posLattice != std::wstring::npos)
            {
                auto posSpace = sBackground.find(L' ', posLattice);
                if (posSpace != std::wstring::npos)
                    return sBackground.substr(posLattice, posSpace);
                else
                    sBackground.substr(posLattice);
            }
            return L"";
        }

        std::wstring CCompiledStyle::GetColor()
        {
            if (m_mStyle.find(L"color") != m_mStyle.cend())
                return m_mStyle[L"color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderBottomColor()
        {
            if (m_mStyle.find(L"border-bottom-color") != m_mStyle.cend())
                return m_mStyle[L"border-bottom-color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderLeftColor()
        {
            if (m_mStyle.find(L"border-left-color") != m_mStyle.cend())
                return m_mStyle[L"border-left-color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderRightColor()
        {
            if (m_mStyle.find(L"border-right-color") != m_mStyle.cend())
                return m_mStyle[L"border-right-color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetOutlineColor()
        {
            if (m_mStyle.find(L"outline-color") != m_mStyle.cend())
                return m_mStyle[L"outline-color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetColumnRuleColor()
        {
            if (m_mStyle.find(L"column-rule-color") != m_mStyle.cend())
                return m_mStyle[L"column-rule-color"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBackground()
        {
            if (m_mStyle.find(L"background") != m_mStyle.cend())
                return  m_mStyle[L"background"];

            return L"";
        }

    /* TEXT */
        std::wstring CCompiledStyle::GetTextAlign()
        {
            if (m_mStyle.find(L"text-align") != m_mStyle.cend())
                return m_mStyle[L"text-align"];

            return L"";
        }

        std::wstring CCompiledStyle::GetTextIndent()
        {
            if (m_mStyle.find(L"text-indent") != m_mStyle.cend())
                return m_mStyle[L"text-indent"];

            return L"";
        }

        std::wstring CCompiledStyle::GetTextDecoration()
        {
            if (m_mStyle.find(L"text-decoration") != m_mStyle.cend())
                return m_mStyle[L"text-decoration"];

            return L"";
        }

    /* BORDER */

        std::wstring CCompiledStyle::GetBorder()
        {
            if (m_mStyle.find(L"border") != m_mStyle.cend())
                return m_mStyle[L"border"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderWidth()
        {
            if (m_mStyle.find(L"border-width") != m_mStyle.cend())
                return m_mStyle[L"border-width"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderStyle()
        {
            if (m_mStyle.find(L"border-style") != m_mStyle.cend())
                return m_mStyle[L"border-style"];

            return L"";
        }

        std::wstring CCompiledStyle::GetBorderColor()
        {
            if (m_mStyle.find(L"border-color") != m_mStyle.cend())
                return m_mStyle[L"border-color"];

            return L"";
        }

}
