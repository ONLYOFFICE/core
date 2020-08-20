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

    CXmlElement CDocumentStyle::AddParentsStyle(CCompiledStyle &oStyle)
    {
        CXmlElement oXmlElement;
        for (std::wstring sParentName : oStyle.GetParentsName())
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sParentName) != m_arStandardStyles.cend() &&
                ((sParentName[0] == L'h' && isdigit(sParentName[1]) && sParentName.length() == 2)))
            {
                oXmlElement.Clear();
                if (std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), sParentName) != m_arUsedStyles.cend())
                {
                    oXmlElement.SetBasedOn(sParentName);
                    return oXmlElement;
                }

                oXmlElement.CreateDefaultElement(sParentName);
                CXmlElement oCharXmlElement;
                oCharXmlElement.Clear();
                if (sParentName[0] == L'h' && isdigit(sParentName[1]) && sParentName.length() == 2)
                {
                    std::wstring sCharName = L"title";
                    sCharName += sParentName[1];
                    sCharName += L"-c";

                    oCharXmlElement.CreateDefaultElement(sCharName);

                }
                else if (sParentName == L"p" )
                {
                    oCharXmlElement.CreateDefaultElement(L"p-c");
                }
                else if (sParentName == L"div")
                {
                    oCharXmlElement.CreateDefaultElement(L"div-c");
                }

                if (!oCharXmlElement.Empty())
                    m_sStyle = oCharXmlElement.GetStyle();

                m_sStyle += oXmlElement.GetStyle();
                oXmlElement.Clear();
                oXmlElement.SetBasedOn(sParentName);
                return oXmlElement;
            }
        }
        oXmlElement.Clear();
        return oXmlElement;
    }

    void CDocumentStyle::WriteStyle(NSCSS::CCompiledStyle &oStyle)
    {
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            m_sStyle.clear();

            return;
        }

        CXmlElement oXmlElement;

        std::wstring sName = oStyle.GetId();

        if (sName.find(L'-') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'-'));
        if (sName.find(L'#') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'#'));
        if (sName.find(L'.') != std::wstring::npos)
            sName = sName.substr(0, sName.find(L'.'));

        if (std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), sName) != m_arUsedStyles.cend())
        {
            if (oStyle.Empty())
            {
                m_sId = sName;
                m_sStyle.clear();
                oXmlElement.Clear();
                oXmlElement = AddParentsStyle(oStyle);
                if (!oXmlElement.Empty() && !oXmlElement.GetBasedOn().empty())
                {
                    sName = m_sId + L'+' + oXmlElement.GetBasedOn();

//                    if (std::find(m_arUsedStyles.begin(), m_arUsedStyles.end(), sName) != m_arUsedStyles.cend())

                    oXmlElement.SetStyleId(sName);
                    oXmlElement.SetName(sName);
                    oXmlElement.SetQFormat(true);
                    oXmlElement.SetType(L"paragraph");
                    oXmlElement.SetCustomStyle(L"1");
                    m_arUsedStyles.push_back(sName);
                    m_sStyle = oXmlElement.GetStyle();
                }
                return;
            }
            oXmlElement.Clear();
            oXmlElement.SetBasedOn(sName);

        }
        else if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.cend())
        {
            oXmlElement.CreateDefaultElement(sName);
            CXmlElement oCharXmlElement;
            oCharXmlElement.Clear();

            if (sName[0] == L'h' && isdigit(sName[1]) && sName.length() == 2)
            {
                std::wstring sCharName = L"title";
                sCharName += sName[1];
                sCharName += L"-c";

                oCharXmlElement.CreateDefaultElement(sCharName);
            }
            else if (sName == L"p" )
            {
                oCharXmlElement.CreateDefaultElement(L"p-c");
            }
            else if (sName == L"div")
            {
                oCharXmlElement.CreateDefaultElement(L"div-c");
            }

            if (!oCharXmlElement.Empty())
                m_sStyle = oCharXmlElement.GetStyle();

            m_sStyle += oXmlElement.GetStyle();

            m_sId = sName;

            m_arUsedStyles.push_back(sName);



            if (oStyle.Empty())
                return;


            oXmlElement.Clear();
//            oXmlElement = AddParentsStyle(oStyle);
            oXmlElement.SetBasedOn(sName);
        }
        else if (oStyle.GetParentsName().size() > 0)
        {
            oXmlElement = AddParentsStyle(oStyle);
        }
        else if (oStyle.Empty())
        {
            m_sId = L"normal";
            m_sStyle = L"";
            return;
        }

//        if (oStyle.Empty())
//        {
//            m_sStyle += oXmlElement.GetStyle();
//            return;
//        }

        m_sId = oStyle.GetId();
        oXmlElement.SetStyleId(m_sId);
        oXmlElement.SetName(m_sId);
        oXmlElement.SetQFormat(true);
        oXmlElement.SetType(L"paragraph");
        oXmlElement.SetCustomStyle(L"1");

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

        if (!oStyle.GetBackgroundColor().empty())
            oXmlElement.SetShd(oStyle.GetBackgroundColor());

        if (!oStyle.GetBorder().empty())
        {
            oXmlElement.SetTopBorder(L"true");
            oXmlElement.SetLeftBorder(L"true");
            oXmlElement.SetBottomBorder(L"true");
            oXmlElement.SetRightBorder(L"true");
        }

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

        m_arUsedStyles.push_back(m_sId);
        m_sStyle += oXmlElement.GetStyle();
    }
}
