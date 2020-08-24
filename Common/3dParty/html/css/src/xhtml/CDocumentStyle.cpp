#include "CDocumentStyle.h"

#include <iostream>

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
                m_arStandardStylesUsed.push_back(sStyleName);
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

            oCharXmlElement.CreateDefaultElement(sCharName);
        }
        else if (sNameStyle == L"p" )
        {
            oCharXmlElement.CreateDefaultElement(L"p-c");
        }
        else if (sNameStyle == L"div")
        {
            oCharXmlElement.CreateDefaultElement(L"div-c");
        }
        else if (sNameStyle == L"a")
        {
            oCharXmlElement.CreateDefaultElement(L"a-c");
        }
        if (!oCharXmlElement.Empty())
        {
            m_sStyle += oCharXmlElement.GetPStyle();
        }

        return oXmlElement;
    }

    CXmlElement CDocumentStyle::ConvertStyle(CCompiledStyle &oStyle, bool bIsPStyle)
    {
        std::wstring sName = oStyle.GetId();

        if (sName.find(L'-') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'-'));
        if (sName.find(L'#') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'#'));
        if (sName.find(L'.') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'.'));

        CXmlElement oXmlElement;
        CXmlElement oParentStyle = CombineStandardStyles(oStyle.GetParentsName());

        if (!oParentStyle.Empty())
            m_arStandardStylesUsed.push_back(oParentStyle.GetStyleId());

        CXmlElement oStandardStyle = CreateStandardStyle(sName);
        CXmlElement oCharStyle;

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) == m_arStandardStyles.cend() &&
            std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sName + L"-c") == m_arStandardStylesUsed.cend())
        {
            oCharStyle.CreateDefaultElement(sName + L"-c");
            m_arStandardStylesUsed.push_back(sName + L"-c");
        }

        if(!oStandardStyle.Empty() && oStyle.Empty() && !oParentStyle.Empty())
        {
            if (!oCharStyle.Empty())
            {
                m_sStyle += oCharStyle.GetPStyle();
                oStandardStyle.SetLink(oCharStyle.GetStyleId());
            }
            m_sStyle = oParentStyle.GetPStyle();
            oStandardStyle.SetBasedOn(oParentStyle.GetStyleId());

            if (bIsPStyle)
                m_sStyle += oStandardStyle.GetPStyle();
            else
                m_sStyle += oStandardStyle.GetRStyle();

            m_sId = oStandardStyle.GetStyleId();
            return oXmlElement;
        }
        if (oStandardStyle.Empty() && !oStyle.Empty() && !oParentStyle.Empty())
        {
            m_sStyle += oParentStyle.GetPStyle();
            oXmlElement.SetBasedOn(oParentStyle.GetStyleId());
            if (!oCharStyle.Empty())
            {
                m_sStyle += oCharStyle.GetPStyle();
                oXmlElement.SetLink(oCharStyle.GetStyleId());
            }
            goto unification;
        }

        if (oStandardStyle.Empty() && oStyle.Empty() && !oParentStyle.Empty())
        {
            oXmlElement += oParentStyle;
            if (!oCharStyle.Empty())
            {
                m_sStyle += oCharStyle.GetPStyle();
                oXmlElement.SetLink(oCharStyle.GetStyleId());
            }
            goto unification;
        }

        if (oStandardStyle.Empty() && oStyle.Empty())
        {
            if (!oParentStyle.Empty())
            {
                m_sStyle = oParentStyle.GetPStyle();
                oXmlElement.SetBasedOn(oParentStyle.GetStyleId());
                if (!oCharStyle.Empty())
                {
                    m_sStyle += oCharStyle.GetPStyle();
                    oXmlElement.SetLink(oCharStyle.GetStyleId());
                }
            }
            goto unification;
        }

        if(!oStyle.Empty())
        {
            CXmlElement oTempXmlElement;
            std::wstring sTempName;

            if (!oParentStyle.Empty())
            {
                oTempXmlElement = oParentStyle;
                sTempName = oParentStyle.GetStyleId() + L"+";
            }
            if (!oStandardStyle.Empty())
            {
                oTempXmlElement += oStandardStyle;
                sTempName += oStandardStyle.GetStyleId();
            }

            oTempXmlElement.SetName(sTempName);
            oTempXmlElement.SetStyleId(sTempName);

            m_sStyle += oTempXmlElement.GetPStyle();
            if (!oCharStyle.Empty())
            {
                m_sStyle += oCharStyle.GetPStyle();
                oXmlElement.SetLink(oCharStyle.GetStyleId());
            }
            oXmlElement.SetBasedOn(oTempXmlElement.GetStyleId());
            goto unification;
        }

        if (oStandardStyle.Empty() && !oParentStyle.Empty())
        {
            m_sStyle += oParentStyle.GetPStyle();
            oXmlElement.SetBasedOn(oParentStyle.GetStyleId());
            if (!oCharStyle.Empty())
            {
                m_sStyle += oCharStyle.GetPStyle();
                oXmlElement.SetLink(oCharStyle.GetStyleId());
            }
        }

        unification:
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
                oXmlElement.SetSz(sFontSize);
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

        if (!oStyle.GetTextIndent().empty())
            oXmlElement.SetInd(L"w:firstLine=\"" + oStyle.GetTextIndent() + L"\"");

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
                oXmlElement.SetSz(sFontSize);
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

        if (!oStyle.GetBorder().empty())
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

//        for (auto oUsedStyle : m_arStyleUsed)
//        {
//            if (oUsedStyle.second.first == oStyle &&
//                !oUsedStyle.second.second)
//            {
//                m_sId = oUsedStyle.first;
//                m_sStyle.clear();
//                return;
//            }
//        }

        CXmlElement oXmlElement;
        SetRStyle(oStyle, oXmlElement);

        m_arStyleUsed.emplace(oStyle.GetId(), std::make_pair(oStyle, false));

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

        for (auto oUsedStyle : m_arStyleUsed)
        {
            if (oUsedStyle.second.first == oStyle &&
                oUsedStyle.second.second)
            {
                m_sId = oUsedStyle.first;
                m_sStyle.clear();
                return;
            }
        }

        CXmlElement oXmlElement;
        SetPStyle(oStyle, oXmlElement);

        m_sStyle += oXmlElement.GetPStyle();
    }
}
