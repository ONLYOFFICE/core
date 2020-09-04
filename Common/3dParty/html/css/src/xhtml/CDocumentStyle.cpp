#include "CDocumentStyle.h"

#include <iostream>
#include <algorithm>
#include <wchar.h>
#include <math.h>

namespace NSCSS
{
    CDocumentStyle::CDocumentStyle()
    {
        m_arStandardStyles = {L"a",         L"li",
                              L"h1",        L"h2",
                              L"h3",        L"h4",
                              L"h5",        L"h6",
                              L"title1-c",  L"title2-c",
                              L"title3-c",  L"title4-c",
                              L"title5-c",  L"title6-c",
                              L"p-c",       L"p",
                              L"div-c",     L"div",
                              L"a-c"};
    }

    CDocumentStyle::~CDocumentStyle()
    {
    }

    std::wstring CDocumentStyle::GetStyle()
    {
        if (m_sId.empty())
            return m_sId;

        const std::wstring sStyle = m_sStyle;
        Clear();
        return sStyle;
    }

    std::wstring CDocumentStyle::GetId() const
    {
        return m_sId;
    }

    void CDocumentStyle::Clear()
    {
        m_sId.clear();
        m_sStyle.clear();
    }

    void CDocumentStyle::SetStyle(const std::wstring &sStyle)
    {
        if (sStyle.empty())
            return;

        m_sStyle = sStyle;
    }

    void CDocumentStyle::SetId(const std::wstring &sId)
    {
        if (sId.empty())
            return;
        m_sId = sId;
    }

    CXmlElement CDocumentStyle::CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles)
    {
        CXmlElement oXmlElement;

        if (arStandartedStyles.size() == 0)
        {
            return oXmlElement;
        }

        std::vector<std::wstring> arStyles;

        for (const std::wstring& sStyleName : arStandartedStyles)
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.cend()/* &&
                std::find(arStyles.begin(), arStyles.end(), sStyleName) == arStyles.cend()*/)
            {
                arStyles.push_back(sStyleName);
            }
        }

        if (arStyles.empty())
            return oXmlElement;

        std::wstring sId;

        for (const std::wstring& sStyleName : arStyles)
        {
            sId += sStyleName;
            if (sStyleName != arStyles[arStyles.size() - 1])
                sId += L'+';
        }

//        if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sId) != m_arStandardStylesUsed.cend())
//            return oXmlElement;

        for (const std::wstring& sStyleName : arStyles)
        {
            const CXmlElement& oTempXmlElement = CreateStandardStyle(sStyleName);
            if (!oTempXmlElement.Empty())
            {
                oXmlElement += oTempXmlElement;
//                m_arStandardStylesUsed.push_back(sStyleName);
            }
        }

        if (!oXmlElement.Empty())
        {
            oXmlElement.SetName(sId);
            oXmlElement.SetStyleId(sId);
        }

//        m_arStandardStylesUsed.push_back(oXmlElement.GetStyleId());

        return oXmlElement;
    }

    CXmlElement CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle)
    {
        CXmlElement oXmlElement;

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) == m_arStandardStyles.cend())
        {
            return oXmlElement;
        }

        CXmlElement oCharXmlElement;

        oXmlElement.CreateDefaultElement(sNameStyle);

        if (sNameStyle[0] == L'h' && sNameStyle.length() == 2 && iswdigit(sNameStyle[1]) )
        {
            std::wstring sCharName = L"title";
            sCharName += sNameStyle[1];
            sCharName += L"-c";

            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sCharName) != m_arStandardStyles.cend())
            {
                oCharXmlElement.CreateDefaultElement(sCharName);
//                m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), sCharName), m_arStandardStyles.end());
            }
        }
        else if (sNameStyle == L"p" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"p-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"p-c");
//            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"p-c"), m_arStandardStyles.end());
        }
        else if (sNameStyle == L"div" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"div-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"div-c");
//            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"div-c"), m_arStandardStyles.end());
        }
        else if (sNameStyle == L"a" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"a-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"a-c");
//            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"a-c"), m_arStandardStyles.end());
        }

        if (!oCharXmlElement.Empty())
        {
            m_sStyle += oCharXmlElement.GetRStyle();
        }

        return oXmlElement;
    }

    CXmlElement CDocumentStyle::ConvertStyle(const NSCSS::CCompiledStyle &oStyle, bool bIsPStyle)
    {
        std::wstring sName = oStyle.GetId();

        const auto& posDash = sName.find(L'-');
        const auto& posLattice = sName.find(L'#');
        const auto& posPoint = sName.find(L'.');

        if (posDash != std::wstring::npos)
            sName = sName.substr(0, posDash);
        if (posLattice != std::wstring::npos)
            sName = sName.substr(0, posLattice);
        if (posPoint != std::wstring::npos)
            sName = sName.substr(0, posPoint);

        if (!bIsPStyle)
            sName += L"-c";

        CXmlElement oXmlElement;
        CXmlElement oStandardXmlElement;
        CXmlElement oParentStyle;

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.cend())
        {
            oStandardXmlElement = CreateStandardStyle(sName);
        }

        if (oStyle.GetParentsName().size() > 0)
        {
            std::vector<std::wstring> arParentsName = oStyle.GetParentsName();

            std::wstring sParentName;
            for (std::wstring& sParent : arParentsName)
            {
                if(!bIsPStyle)
                    sParent += L"-c";

                sParentName += sParent + L"+";
            }

            sParentName.erase(sParentName.length() - 1, 1);

//            std::wcout << oStyle.GetId() <<  L" - " << sParentName << std::endl;

            oParentStyle = CombineStandardStyles(arParentsName);

            if (!oParentStyle.Empty())
            {
                oParentStyle.SetStyleId(L"(" + oParentStyle.GetStyleId() + L")");

                if (!bIsPStyle)
                {
                    oParentStyle.SetStyleId(oParentStyle.GetStyleId() + L"-c");
                    oParentStyle.SetType(L"character");
                }
            }
        }

        if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            const std::wstring& sStandPlusParent = oStandardXmlElement.GetStyleId() + oParentStyle.GetStyleId();

            if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.cend())
            {
                oXmlElement.SetBasedOn(sStandPlusParent);
            }
            else
            {
                CXmlElement oTempElement = oParentStyle;
                oTempElement += oStandardXmlElement;

                oTempElement.SetStyleId(sStandPlusParent);

                bIsPStyle ? m_sStyle += oTempElement.GetPStyle() : m_sStyle += oTempElement.GetRStyle();

                oXmlElement.SetBasedOn(oTempElement.GetStyleId());

                m_arStandardStylesUsed.push_back(sStandPlusParent);
            }
        }
        else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
        {
            const std::wstring& sStandartStyleID = oStandardXmlElement.GetStyleId();

            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) != m_arStandardStylesUsed.cend())
            {
                oXmlElement.SetBasedOn(sStandartStyleID);
            }
            else
            {
                bIsPStyle ? m_sStyle += oStandardXmlElement.GetPStyle() : m_sStyle += oStandardXmlElement.GetRStyle();

                m_arStandardStylesUsed.push_back(sStandartStyleID);

                oXmlElement.SetBasedOn(sStandartStyleID);
            }
        }
        else if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            const std::wstring& sParentsStyleID = oParentStyle.GetStyleId();

            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) != m_arStandardStylesUsed.cend())
            {
                oXmlElement.SetBasedOn(sParentsStyleID);
            }
            else
            {
                bIsPStyle ? m_sStyle += oParentStyle.GetPStyle() : m_sStyle += oParentStyle.GetRStyle();

                oXmlElement.SetBasedOn(sParentsStyleID);
                m_arStandardStylesUsed.push_back(sParentsStyleID);
            }
        }

        if (oStyle.Empty())
        {
            (oXmlElement.GetBasedOn().empty() || (oStandardXmlElement.Empty() && oParentStyle.Empty()))
                    ? m_sId = L"normal" : m_sId = oXmlElement.GetBasedOn();

            oXmlElement.Clear();

            return oXmlElement;
        }

        m_sId = oStyle.GetId();
        if (!bIsPStyle)
            m_sId += L"-c";

        oXmlElement.SetStyleId(m_sId);
        oXmlElement.SetName(m_sId);
        oXmlElement.SetQFormat(true);

        bIsPStyle ? oXmlElement.SetType(L"paragraph") : oXmlElement.SetType(L"character");

        oXmlElement.SetCustomStyle(L"1");

        return oXmlElement;

    }

    void CDocumentStyle::SetPStyle(NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        oXmlElement = ConvertStyle(oStyle, true);

        if (oStyle.Empty())
            return;

        oXmlElement.SetJc(oStyle.GetTextAlign());

        std::wstring sIndValue;
        if (!oStyle.GetTextIndent().empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            sIndValue += L"w:firstLine=\"" + oStyle.GetTextIndent() + L"\" ";
        }
        if (!oStyle.GetMargin().empty())
        {
            double dLeftValue = wcstod(oStyle.GetMarginLeft().c_str(), NULL);
            dLeftValue = dLeftValue * 1440 / 72;

            double dRightValue = wcstod(oStyle.GetMarginRight().c_str(), NULL);
            dRightValue = dRightValue * 1440 / 72;

            sIndValue += L"w:left=\"" + std::to_wstring((int)floor(dLeftValue + 0.5)) + L"\" ";
            sIndValue += L"w:right=\"" + std::to_wstring((int)floor(dRightValue + 0.5)) + L"\"";
        }

        oXmlElement.SetInd(sIndValue);

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            double dAfterValue = wcstod(oStyle.GetMarginTop().c_str(), NULL);
            dAfterValue *= 10;

            double dBeforeValue = wcstod(oStyle.GetMarginBottom().c_str(), NULL);
            dBeforeValue *= 10;

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floor(dAfterValue + 0.5)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floor(dBeforeValue + 0.5)) + L"\" ";
        }

//        if (!oStyle.GetLineHeight().empty())
//        {
//            double dLineHeight = wcstod(oStyle.GetLineHeight().c_str(), NULL);
//            dLineHeight *= 22;
//            sSpacingValue += L"w:line=\"" + std::to_wstring((int)floor(dLineHeight + 0.5)) + L"\" ";
//            sSpacingValue += L"w:lineRule=\"auto\"";
//        }

        if (!sSpacingValue.empty())
        {
            oXmlElement.SetSpacing(sSpacingValue);
            oXmlElement.SetContextualSpacing(true);
        }

        oXmlElement.SetShd(oStyle.GetBackgroundColor());

        if (!oStyle.GetBorder().empty())
        {
            oXmlElement.SetTopBorder(L"true");
            oXmlElement.SetLeftBorder(L"true");
            oXmlElement.SetBottomBorder(L"true");
            oXmlElement.SetRightBorder(L"true");
        }
    }

    void CDocumentStyle::SetRStyle(NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        oXmlElement = ConvertStyle(oStyle, false);

        if (oStyle.Empty())
            return;

        oXmlElement.SetColor(oStyle.GetColor());

        if (!oStyle.GetFontSize().empty())
        {
            std::wstring sFontSize = oStyle.GetFontSize();

            double dValue = wcstod(sFontSize.c_str(), NULL);
            dValue *= 2;
            oXmlElement.SetSz(std::to_wstring((int)dValue));
        }
        if (!oStyle.GetFontFamily().empty())
        {
            std::wstring sFontFamily = oStyle.GetFontFamily();
            oXmlElement.SetRFonts(sFontFamily);
        }

        if (oStyle.GetFontWeight() == L"bold")
            oXmlElement.SetB(true);

        if (oStyle.GetFontStyle() == L"italic")
        {
            oXmlElement.SetI(true);
        }

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            double dAfterValue = wcstod(oStyle.GetMarginTop().c_str(), NULL);
            dAfterValue *= 10;

            double dBeforeValue = wcstod(oStyle.GetMarginBottom().c_str(), NULL);
            dBeforeValue *= 10;

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floor(dAfterValue + 0.5)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floor(dBeforeValue + 0.5)) + L"\" ";
        }

        if (!oStyle.GetLineHeight().empty())
        {
            double dLineHeight = wcstod(oStyle.GetLineHeight().c_str(), NULL);
            dLineHeight *= 22;
            sSpacingValue += L"w:line=\"" + std::to_wstring((int)floor(dLineHeight + 0.5)) + L"\" ";
            sSpacingValue += L"w:lineRule=\"auto\"";
        }

        if (!sSpacingValue.empty())
        {
            oXmlElement.SetSpacing(sSpacingValue);
            oXmlElement.SetContextualSpacing(true);
        }
    }

    void CDocumentStyle::WriteRStyle(NSCSS::CCompiledStyle &oStyle)
    {
//        if (!oStyle.GetBasedOn().empty())
//        {
//            m_sId = oStyle.GetBasedOn();
//            return;
//        }

        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            return;
        }

        CStyleUsed structStyle(oStyle, false);

        const auto& oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

        if (oItem != m_arStyleUsed.cend())
        {
            m_sId = oItem->m_sId;
            return;
        }

        CXmlElement oXmlElement;
        SetRStyle(oStyle, oXmlElement);

        if (!oStyle.Empty())
        {
            structStyle.m_sId = oXmlElement.GetStyleId();
            m_arStyleUsed.push_back(structStyle);

        }

        m_sStyle += oXmlElement.GetRStyle();
    }

    void CDocumentStyle::WritePStyle(NSCSS::CCompiledStyle &oStyle)
    {   
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            m_sStyle.clear();
            return;
        }

        CStyleUsed structStyle(oStyle, true);

        const auto& oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

        if (oItem != m_arStyleUsed.cend())
        {
            m_sId = oItem->m_sId;
            return;
        }

        CXmlElement oXmlElement;
        SetPStyle(oStyle, oXmlElement);

        if (!oStyle.Empty())
        {
            structStyle.m_sId = oXmlElement.GetStyleId();

            m_arStyleUsed.push_back(structStyle);
        }

        m_sStyle += oXmlElement.GetPStyle();
    }
}
