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

        return m_sStyle;
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

    void CDocumentStyle::WriteStyle(NSCSS::CCompiledStyle &oStyle)
    {
        if(oStyle.GetId().empty())
        {
            CXmlElement oXmlElement;
            oXmlElement.CreateDefaultElement(L"normal");
            m_sId = L"normal";
            m_sStyle = oXmlElement.GetStyle();
            return;
        }


        CXmlElement oXmlElement;

        bool bIsStandard = false;
        std::wstring sId = oStyle.GetId();

        if (sId.find(L'.') != std::wstring::npos)
            sId = sId.substr(0, sId.find(L'.'));

        if (oStyle.GetNeedSave() == false ||
            std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), oStyle.GetId()) != m_arUsedStyles.cend())
        {
            m_sId = oStyle.GetId();
            m_sStyle = L"";
            return;
        }

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sId) != m_arStandardStyles.end())
        {
            bIsStandard = true;

            CXmlElement oTempXmlElement;

            if (sId[0] == L'h' && isdigit(sId[1]))
            {
                std::wstring sName = L"title";
                sName += sId[1];
                sName += L"-c";
                if (std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), sName) == m_arUsedStyles.cend())
                {
                    oTempXmlElement.CreateDefaultElement(sName);
                    m_arUsedStyles.push_back(sName);
                }
            }
            else if (sId == L"p" && std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), L"p-c") == m_arUsedStyles.cend())
            {
                oTempXmlElement.CreateDefaultElement(L"p-c");
                m_arUsedStyles.push_back(L"p-c");
            }
            else if (sId == L"div" && std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), L"div-c") == m_arUsedStyles.cend())
            {
                oTempXmlElement.CreateDefaultElement(L"div-c");
                m_arUsedStyles.push_back(L"div-c");
            }
            m_sStyle = oTempXmlElement.GetStyle();

            oXmlElement.CreateDefaultElement(sId);

            m_arUsedStyles.push_back(sId);
            bIsStandard = true;
        }

        if (oStyle.Empty() && bIsStandard)
        {
            m_sId = oStyle.GetId();
            m_sStyle += oXmlElement.GetStyle();
            return;
        }

        if (oStyle.Empty() && !bIsStandard)
        {
            m_sId = L"normal";
            m_sStyle = L"";
            return;
        }

        m_sId = oStyle.GetId();

        oXmlElement.SetStyleId(m_sId);
        oXmlElement.SetName(m_sId);

        if (!oStyle.GetColor().empty())
            oXmlElement.SetColor(oStyle.GetColor());

        if (!oStyle.GetFontSize().empty())
        {
            std::wstring sFontSize = oStyle.GetFontSize();
            if (isdigit(sFontSize[0]))
            {
                oXmlElement.SetSz(sFontSize);
            }
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
                oXmlElement.SetI(true);
        }

        if (!oStyle.GetFontWeight().empty())
        {
            std::wstring sFontWeight = oStyle.GetFontWeight();
            if (sFontWeight == L"bold")
                oXmlElement.SetB(true);
        }

        if (!oStyle.GetTextIndent().empty())
            oXmlElement.SetInd(L"w:firstLine=\"" + oStyle.GetTextIndent() + L"\"");
//            sPPr += L"<w:ind w:firstLine=\"" + oStyle.GetTextIndent() + L"\"/>";

//        if (!oStyle.GetPadding().empty())
//        {
//            std::wstring sInd = L"<w:ind ";
//            std::wstring sPaddingLeft = oStyle.GetPaddingLeft();
//            std::wstring sPaddingRight = oStyle.GetPaddingRight();
//            bool bFl = false;


//            if (sPaddingLeft != L"0")
//            {
//                bFl = true;
//                sInd += L"w:left=\"" + sPaddingLeft + L"\" ";
//            }
//            if (sPaddingRight != L"0")
//            {
//                bFl = true;
//                sInd += L"w:right=\"" + sPaddingRight + L"\"";
//            }
//            sInd += L"/>";

//            if (bFl)
//                m_sStyle += sInd;
//        }

//        if (true)
//        {
//            std::wstring sSpacing = L"<w:spacing ";
//            std::wstring sBefore = oStyle.GetMarginTop();
//            std::wstring sAfter = oStyle.GetMarginBottom();
//            std::wstring sLineHeight = oStyle.GetLineHeight();

//            if (isdigit(sBefore[0]))
//                sSpacing += L"w:before=\"" + sBefore + L"\" ";

//            if (isdigit(sAfter[0]))
//                sSpacing += L"w:after=\"" + sAfter + L"\" ";

//            if (isdigit(sLineHeight[0]))
//                sSpacing += L"w:line=\"" + sAfter + L"\" ";

//            sSpacing += L"w:lineRule=\"auto\"/>";

//            m_sStyle += sSpacing;
//        }

//        if (!oStyle.GetTextDecoration().empty())
//        {
//            std::wstring sTextDecoration = oStyle.GetTextDecoration();
//            if (sTextDecoration == L"underline")
//                m_sStyle += L"<w:u w:val=\"single\"/>";
//        }

//        if (!oStyle.GetBackgroundColor().empty())
//            m_sStyle += L"<w:background w:color=\"" + oStyle.GetBackgroundColor() + L"\"/>";

        m_sStyle += oXmlElement.GetStyle();
    }
}
