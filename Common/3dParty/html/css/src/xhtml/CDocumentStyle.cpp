#include "CDocumentStyle.h"

#include <iostream>
#include <algorithm>
#include <wchar.h>
#include <math.h>

static std::vector<std::wstring> GetWordsW(const std::wstring& sLine)
{
    if (sLine.empty())
        return {};

    std::vector<std::wstring> arWords;
    size_t posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,");

    while (posFirstNotSpace != std::wstring::npos)
    {
        size_t posLastNotSpace = sLine.find_first_of(L" \n\r\t\f\v:;,", posFirstNotSpace);
        arWords.push_back(sLine.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
        posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,", posLastNotSpace);
    }
    return arWords;
}

namespace NSCSS
{
    CDocumentStyle::CDocumentStyle(): m_arStandardStyles({L"a",         L"li",
                                                          L"h1",        L"h2",
                                                          L"h3",        L"h4",
                                                          L"h5",        L"h6",
                                                          L"title1-c",  L"title2-c",
                                                          L"title3-c",  L"title4-c",
                                                          L"title5-c",  L"title6-c",
                                                          L"p-c",       L"p",
                                                          L"div-c",     L"div",
                                                          L"a-c"})
    {
    }

    CDocumentStyle::~CDocumentStyle()
    {
        m_arStandardStyles.clear();
        m_arStyleUsed.clear();
        m_arStandardStylesUsed.clear();
    }

    const std::wstring CDocumentStyle::GetStyle() const
    {
        if (m_sId.empty())
            return m_sId;

        return m_sStyle;
    }

    std::wstring CDocumentStyle::GetIdAndClear()
    {
        std::wstring sId = m_sId;
        Clear();
        return sId;
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
            return oXmlElement;

        std::vector<std::wstring> arStyles;

        for (const std::wstring& sStyleName : arStandartedStyles)
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.cend())
                arStyles.push_back(sStyleName);
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

        for (const std::wstring& sStyleName : arStyles)
        {
            const CXmlElement& oTempXmlElement = CreateStandardStyle(sStyleName);
            if (!oTempXmlElement.Empty())
                oXmlElement += oTempXmlElement;
        }

        if (!oXmlElement.Empty())
        {
            oXmlElement.SetName(sId);
            oXmlElement.SetStyleId(sId);
        }

        return oXmlElement;
    }

    CXmlElement CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle)
    {
        CXmlElement oXmlElement;

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) == m_arStandardStyles.cend())
            return oXmlElement;

        CXmlElement oCharXmlElement;

        oXmlElement.CreateDefaultElement(sNameStyle);

        if (sNameStyle == L"p" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"p-c") != m_arStandardStyles.cend())
            oCharXmlElement.CreateDefaultElement(L"p-c");
        else if (sNameStyle == L"div" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"div-c") != m_arStandardStyles.cend())
            oCharXmlElement.CreateDefaultElement(L"div-c");
        else if (sNameStyle == L"a" && std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), L"a-c") != m_arStandardStyles.cend())
            oCharXmlElement.CreateDefaultElement(L"a-c");

        if (!oCharXmlElement.Empty())
        {
            oXmlElement.SetLink(oCharXmlElement.GetStyleId());
            oCharXmlElement.SetLink(oXmlElement.GetStyleId());
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
        {
            if (sName[0] == L'h' && sName.length() == 2 && iswdigit(sName[1]))
            {
                sName = sName[1];
                sName = L"title" + sName;
                sName += L"-c";
            }
            else
                sName += L"-c";
        }
        CXmlElement oXmlElement;
        CXmlElement oStandardXmlElement;
        CXmlElement oParentStyle;

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

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.cend())
        {
            oStandardXmlElement = CreateStandardStyle(sName);
        }

        if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            const std::wstring& sStandPlusParent = oStandardXmlElement.GetStyleId() + oParentStyle.GetStyleId();

            if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.cend())
                oXmlElement.SetBasedOn(sStandPlusParent);
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
                oXmlElement.SetBasedOn(sStandartStyleID);
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
                oXmlElement.SetBasedOn(sParentsStyleID);
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

    void CDocumentStyle::SetPStyle(const NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        oXmlElement = ConvertStyle(oStyle, true);

        if (oStyle.Empty())
            return;

        oXmlElement.SetJc(oStyle.GetTextAlign());

        std::wstring sIndValue;
        if (!oStyle.GetTextIndent().empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            const double& dFirstLineValue = wcstod(oStyle.GetTextIndent().c_str(), NULL) * 10;
            sIndValue += L"w:firstLine=\"" +std::to_wstring((int)floor(dFirstLineValue + 0.5)) + L"\" ";
        }

        if (!oStyle.GetMargin().empty())
        {
            const double& dLeftValue = wcstod(oStyle.GetMarginLeft().c_str(), NULL);
            const double& dRightValue = wcstod(oStyle.GetMarginRight().c_str(), NULL);

            sIndValue += L"w:left=\"" + std::to_wstring((int)floor(dLeftValue + 0.5)) + L"\" ";
            sIndValue += L"w:right=\"" + std::to_wstring((int)floor(dRightValue + 0.5)) + L"\"";
        }

        oXmlElement.SetInd(sIndValue);

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            const double& dAfterValue = wcstod(oStyle.GetMarginTop().c_str(), NULL);
            const double& dBeforeValue = wcstod(oStyle.GetMarginBottom().c_str(), NULL);

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floor(dAfterValue + 0.5)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floor(dBeforeValue + 0.5)) + L"\" ";
        }

        if (!oStyle.GetLineHeight().empty())
        {
            double dValue = wcstod(oStyle.GetFontSize().c_str(), NULL);
            if (dValue == 0)
                dValue = 24;

            const double& dLineHeight = wcstod(oStyle.GetLineHeight().c_str(), NULL) / 24 * dValue * 10;

            sSpacingValue += L"w:line=\"" + std::to_wstring((int)floor(dLineHeight + 0.5)) + L"\" ";
            sSpacingValue += L"w:lineRule=\"auto\"";
        }

        if (!sSpacingValue.empty())
        {
            oXmlElement.SetSpacing(sSpacingValue);
            oXmlElement.SetContextualSpacing(true);
        }

        oXmlElement.SetShd(oStyle.GetBackgroundColor());

//        if (!oStyle.GetBorder().empty() &&  oStyle.GetBorder() != L"none")
        {
            const std::wstring& sBorderColor = oStyle.GetBorderColor();
            const std::wstring& sBorderStyle = oStyle.GetBorderStyle();

            const double& dValue = wcstod(oStyle.GetBorderWidth().c_str(), NULL) * 4;
            const std::wstring& sBorderWidth = std::to_wstring((int)floor(dValue + 0.5));

            const std::wstring& sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
                                                           sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

            if (sBorderColor.empty() + sBorderWidth.empty() + sBorderStyle.empty() == 0)
            {
                oXmlElement.SetTopBorder(sBorder);
                oXmlElement.SetLeftBorder(sBorder);
                oXmlElement.SetBottomBorder(sBorder);
                oXmlElement.SetRightBorder(sBorder);
                return;
            }
        }
//        else
        {
            const std::wstring& sBorderTopColor = oStyle.GetBorderTopColor();
            const std::wstring& sBorderTopStyle = oStyle.GetBorderTopStyle();

            const double& dValueTop = wcstod(oStyle.GetBorderTopWidth().c_str(), NULL) * 4;
            const std::wstring& sBorderTopWidth = std::to_wstring((int)floor(dValueTop + 0.5));

            const std::wstring& sBorderTop = L" w:color=\"" + sBorderTopColor + L"\" w:space=\"4\" w:sz=\"" +
                                                             sBorderTopWidth + L"\" w:val=\"" + sBorderTopStyle + L"\"";

            if (sBorderTopColor.empty() + sBorderTopWidth.empty() + sBorderTopStyle.empty() == 0)
                oXmlElement.SetTopBorder(sBorderTop);

            const std::wstring& sBorderLeftColor = oStyle.GetBorderLeftColor();
            const std::wstring& sBorderLeftStyle = oStyle.GetBorderLeftStyle();

            const double& dValueLeft = wcstod(oStyle.GetBorderLeftWidth().c_str(), NULL) * 4;
            const std::wstring& sBorderLeftWidth = std::to_wstring((int)floor(dValueLeft + 0.5));

            const std::wstring& sBorderLeft = L" w:color=\"" + sBorderLeftColor + L"\" w:space=\"4\" w:sz=\"" +
                                                              sBorderLeftWidth + L"\" w:val=\"" + sBorderLeftStyle + L"\"";

            if (sBorderLeftColor.empty() + sBorderLeftWidth.empty() + sBorderLeftStyle.empty() == 0)
                oXmlElement.SetLeftBorder(sBorderLeft);

            const std::wstring& sBorderBottomColor = oStyle.GetBorderBottomColor();
            const std::wstring& sBorderBottomStyle = oStyle.GetBorderBottomStyle();

            const double& dValueBottom = wcstod(oStyle.GetBorderBottomWidth().c_str(), NULL) * 4;
            const std::wstring& sBorderBottomWidth = std::to_wstring((int)floor(dValueBottom + 0.5));

            const std::wstring& sBorderBottom = L" w:color=\"" + sBorderBottomColor + L"\" w:space=\"4\" w:sz=\"" +
                                                                 sBorderBottomWidth + L"\" w:val=\"" + sBorderBottomStyle + L"\"";

            if (sBorderBottomColor.empty() + sBorderBottomWidth.empty() + sBorderBottomStyle.empty() == 0)
                oXmlElement.SetBottomBorder(sBorderBottom);

            const std::wstring& sBorderRightColor = oStyle.GetBorderRightColor();
            const std::wstring& sBorderRightStyle = oStyle.GetBorderRightStyle();

            const double& dValueRight = wcstod(oStyle.GetBorderRightWidth().c_str(), NULL) * 4;
            const std::wstring& sBorderRightWidth = std::to_wstring((int)floor(dValueRight + 0.5));

            const std::wstring& sBorderRight = L" w:color=\"" + sBorderRightColor + L"\" w:space=\"4\" w:sz=\"" +
                                                                sBorderRightWidth + L"\" w:val=\"" + sBorderRightStyle + L"\"";

            if (sBorderRightColor.empty() + sBorderRightWidth.empty() + sBorderRightStyle.empty() == 0)
                oXmlElement.SetRightBorder(sBorderRight);
        }

    }

    void CDocumentStyle::SetRStyle(const NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        oXmlElement = ConvertStyle(oStyle, false);

        if (oStyle.Empty())
            return;

        oXmlElement.SetColor(oStyle.GetColor());
        oXmlElement.SetShd(oStyle.GetBackgroundColor());

        if (!oStyle.GetFontSize().empty())
        {
            const double& dValue = wcstod(oStyle.GetFontSize().c_str(), NULL);

            oXmlElement.SetSz(std::to_wstring((int)dValue));
        }
        if (!oStyle.GetFontFamily().empty())
        {
            const std::wstring& sFontFamily = oStyle.GetFontFamily();
            oXmlElement.SetRFonts(sFontFamily);
        }

        if (oStyle.GetFontWeight() == L"bold")
            oXmlElement.SetB(true);

        if (oStyle.GetFontStyle() == L"italic")
            oXmlElement.SetI(true);

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            const double& dAfterValue = wcstod(oStyle.GetMarginTop().c_str(), NULL) * 10;

            const double& dBeforeValue = wcstod(oStyle.GetMarginBottom().c_str(), NULL) * 10;

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floor(dAfterValue + 0.5)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floor(dBeforeValue + 0.5)) + L"\" ";
        }

        if (!oStyle.GetLineHeight().empty())
        {
            double dValue = wcstod(oStyle.GetFontSize().c_str(), NULL);
            if (dValue == 0)
                dValue = 24;

            const double& dLineHeight = wcstod(oStyle.GetLineHeight().c_str(), NULL) / 24 * dValue * 10;

            sSpacingValue += L"w:line=\"" + std::to_wstring((int)floor(dLineHeight + 0.5)) + L"\" ";
            sSpacingValue += L"w:lineRule=\"auto\"";
        }

        if (!sSpacingValue.empty())
        {
            oXmlElement.SetSpacing(sSpacingValue);
            oXmlElement.SetContextualSpacing(true);
        }
    }

    void CDocumentStyle::WriteRStyle(const NSCSS::CCompiledStyle &oStyle)
    {
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

    void CDocumentStyle::WritePStyle(const NSCSS::CCompiledStyle &oStyle)
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
