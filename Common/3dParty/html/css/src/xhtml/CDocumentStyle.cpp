#include "CDocumentStyle.h"

#include <iostream>
#include <algorithm>
#include <wchar.h>

struct ParentStyle
{
    CXmlElement oParentsXmlElement;
    std::wstring oNameParent;
};

namespace NSCSS
{
CDocumentStyle::CDocumentStyle()
    {
        m_arStandardStyles = {L"normal",    L"li",
                              L"h1",        L"h2",
                              L"h3",        L"h4",
                              L"h5",        L"h6",
                              L"title1-c",  L"title2-c",
                              L"title3-c",  L"title4-c",
                              L"title5-c",  L"title6-c",
                              L"p-c",       L"p",
                              L"div-c",     L"div",
                              L"a"};
    }

    CDocumentStyle::~CDocumentStyle()
    {
    }

    std::wstring CDocumentStyle::GetStyle()
    {
        if (m_sId.empty())
            return L"";
        std::wstring sStyle = m_sStyle;
        Clear();
        return sStyle;
    }

    std::wstring CDocumentStyle::GetId()
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

    CXmlElement CDocumentStyle::CombineStandardStyles(std::vector<std::wstring> arStandartedStyles)
    {
        if (arStandartedStyles.size() == 0)
            return CXmlElement();

        std::vector<std::wstring> arStyles;

        for (std::wstring sStyleName : arStandartedStyles)
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.cend() &&
                std::find(arStyles.begin(), arStyles.end(), sStyleName) == arStyles.cend())
            {
                arStyles.push_back(sStyleName);
            }
        }

        CXmlElement oXmlElement;
        oXmlElement.Clear();

        if (arStyles.empty())
            return oXmlElement;

        std::wstring sId;

        for (std::wstring sStyleName : arStyles)
        {
            sId += sStyleName;
            if (sStyleName != arStyles[arStyles.size() - 1])
                sId += L'+';
        }

        for (std::wstring sStyleName : arStyles)
        {
            CXmlElement oTempXmlElement = CreateStandardStyle(sStyleName);
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
        return oXmlElement;
    }

    CXmlElement CDocumentStyle::CreateStandardStyle(std::wstring sNameStyle)
    {
        CXmlElement oXmlElement;

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) == m_arStandardStyles.cend())
        {
            oXmlElement.Clear();
            return oXmlElement;
        }

        CXmlElement oCharXmlElement;

        oXmlElement.CreateDefaultElement(sNameStyle);

        if (sNameStyle[0] == L'h' && isdigit(sNameStyle[1]) && sNameStyle.length() == 2)
        {
            std::wstring sCharName = L"title";
            sCharName += sNameStyle[1];
            sCharName += L"-c";

            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sCharName) != m_arStandardStyles.cend())
            {
                oCharXmlElement.CreateDefaultElement(sCharName);
                m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), sCharName), m_arStandardStyles.end());
            }
        }
        else if (sNameStyle == L"p" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"p-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"p-c");
            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"p-c"), m_arStandardStyles.end());
        }
        else if (sNameStyle == L"div" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"div-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"div-c");
            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"div-c"), m_arStandardStyles.end());
        }
        else if (sNameStyle == L"a" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"a-c") != m_arStandardStyles.cend())
        {
            oCharXmlElement.CreateDefaultElement(L"a-c");
            m_arStandardStyles.erase(std::remove(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"a-c"), m_arStandardStyles.end());
        }
        if (!oCharXmlElement.Empty())
        {
            m_sStyle += oCharXmlElement.GetPStyle();
        }

        return oXmlElement;
    }

    CXmlElement CDocumentStyle::ConvertStyle(NSCSS::CCompiledStyle &oStyle, bool bIsPStyle)
    {
        std::wstring sName = oStyle.GetId();

        if (sName.find(L'-') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'-'));
        if (sName.find(L'#') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'#'));
        if (sName.find(L'.') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'.'));

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
            for (std::wstring sParent : arParentsName)
            {
                sParentName += sParent;
                if (sParent != arParentsName[arParentsName.size() - 1])
                {
                    sParentName += L"+";
                }
            }
            oParentStyle = CombineStandardStyles(arParentsName);
        }

        if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), oStandardXmlElement.GetStyleId() + L"(" + oParentStyle.GetStyleId() + L")") != m_arStandardStylesUsed.cend())
            {
                oXmlElement.SetBasedOn(oStandardXmlElement.GetStyleId() + L"(" + oParentStyle.GetStyleId() + L")");
            }
            else
            {
                CXmlElement oTempXmlElement;
                oTempXmlElement = oParentStyle;
                oTempXmlElement += oStandardXmlElement;
                oTempXmlElement.SetStyleId(oStandardXmlElement.GetStyleId() + L"(" + oParentStyle.GetStyleId() + L")");

                m_sStyle = oTempXmlElement.GetStyle();

                m_arStandardStylesUsed.push_back(oTempXmlElement.GetStyleId());

                oXmlElement.SetBasedOn(oTempXmlElement.GetStyleId());
            }
        }
        else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
        {
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), oStandardXmlElement.GetStyleId()) != m_arStandardStylesUsed.cend())
            {
                oXmlElement.SetBasedOn(oStandardXmlElement.GetStyleId());
            }
            else
            {
                m_sStyle = oStandardXmlElement.GetStyle();
                m_arStandardStylesUsed.push_back(oStandardXmlElement.GetStyleId());
                oXmlElement.SetBasedOn(oStandardXmlElement.GetStyleId());
            }
        }
        else if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), oParentStyle.GetStyleId()) == m_arStandardStylesUsed.cend())
            {
                m_arStandardStylesUsed.push_back(oParentStyle.GetStyleId());
                m_sStyle = oParentStyle.GetStyle();
            }
            oXmlElement.SetBasedOn(oParentStyle.GetStyleId());
        }

        if (oStyle.Empty())
        {
            m_sId = oXmlElement.GetBasedOn();
            oXmlElement.Clear();
            return oXmlElement;
        }

        m_sId = oStyle.GetId();
        oXmlElement.SetStyleId(m_sId);
        oXmlElement.SetName(m_sId);
        oXmlElement.SetQFormat(true);

        if (bIsPStyle)
            oXmlElement.SetType(L"paragraph");
        else
            oXmlElement.SetType(L"character");

        oXmlElement.SetCustomStyle(L"1");

        return oXmlElement;

    }

    void CDocumentStyle::SetPStyle(NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        oXmlElement = ConvertStyle(oStyle, true);

        if (oStyle.Empty())
            return;

        if (!oStyle.GetColor().empty())
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

        if (!oStyle.GetTextAlign().empty())
        {
            // Сделать обработку в SetJc
            std::wstring sTextAlign = oStyle.GetTextAlign();
            if (sTextAlign != L"justify")
                oXmlElement.SetJc(oStyle.GetTextAlign());
            else if (sTextAlign == L"justify")
                oXmlElement.SetJc(L"both");
        }

        if (!oStyle.GetFontStyle().empty())
        {
            std::wstring sFontStyle = oStyle.GetFontStyle();
            if (sFontStyle == L"italic")
            {
                oXmlElement.SetI(true);
            }
        }

        if (!oStyle.GetFontWeight().empty())
        {
            std::wstring sFontWeight = oStyle.GetFontWeight();
            if (sFontWeight == L"bold")
                oXmlElement.SetB(true);
        }

        std::wstring sValue;
        if (!oStyle.GetTextIndent().empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            sValue += L"w:firstLine=\"" + oStyle.GetTextIndent() + L"\" ";
        }
        if (!oStyle.GetMargin().empty())
        {
            double dLeftValue = wcstod(oStyle.GetMarginLeft().c_str(), NULL);
            dLeftValue = dLeftValue * 1440 * 2.54 / 72;

            double dRightValue = wcstod(oStyle.GetMarginRight().c_str(), NULL);
            dRightValue = dRightValue * 1440 * 2.54 / 72;

            sValue += L"w:left=\"" + std::to_wstring((int)dLeftValue) + L"\" ";
            sValue += L"w:right=\"" + std::to_wstring((int)dRightValue) + L"\"";
        }

        if (!sValue.empty())
        {
            oXmlElement.SetInd(sValue);
        }

        if (!oStyle.GetBackgroundColor().empty())
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

        if (!oStyle.GetColor().empty())
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

        if (!oStyle.GetFontStyle().empty())
        {
            std::wstring sFontStyle = oStyle.GetFontStyle();
            if (sFontStyle == L"italic")
            {
                oXmlElement.SetI(true);
            }
        }

        if (!oStyle.GetFontWeight().empty())
        {
            std::wstring sFontWeight = oStyle.GetFontWeight();
            if (sFontWeight == L"bold")
                oXmlElement.SetB(true);
        }

        if (!oStyle.GetBorder().empty() && oStyle.GetId().find(L"caption") == std::wstring::npos)
        {
            oXmlElement.SetTopBorder(L"true");
            oXmlElement.SetLeftBorder(L"true");
            oXmlElement.SetBottomBorder(L"true");
            oXmlElement.SetRightBorder(L"true");
        }
    }

    void CDocumentStyle::WriteRStyle(NSCSS::CCompiledStyle &oStyle)
    {
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            m_sStyle.clear();
            return;
        }

        for (auto oItem : m_arStyleUsed)
        {
            if(oItem.first == oStyle && !oItem.second)
            {
                m_sId = oItem.first.GetId();
                m_sStyle.clear();
                return;
            }
        }

        CXmlElement oXmlElement;
        SetRStyle(oStyle, oXmlElement);

        if (!oStyle.Empty())
            m_arStyleUsed.push_back(std::make_pair(oStyle, false));

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

        for (auto oItem : m_arStyleUsed)
        {
            if(oItem.first == oStyle && oItem.second)
            {
                m_sId = oItem.first.GetId();
                m_sStyle.clear();
                return;
            }
        }

        CXmlElement oXmlElement;
        SetPStyle(oStyle, oXmlElement);

        if (!oStyle.Empty())
            m_arStyleUsed.push_back(std::make_pair(oStyle, true));

        m_sStyle += oXmlElement.GetPStyle();
    }
}
