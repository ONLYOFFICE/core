#include "CDocumentStyle.h"

#include <iostream>
#include <set>
#include <wchar.h>
#include <math.h>

namespace NSCSS
{
    CDocumentStyle::CDocumentStyle(): m_arStandardStyles({L"a", L"li", L"h1", L"h2", L"h3", L"h4", L"h5", L"h6", L"h1-c",
    L"h2-c", L"h3-c", L"h4-c", L"h5-c", L"h6-c", L"p-c", L"p", L"div-c", L"div", L"a-c"}) {}

    CDocumentStyle::~CDocumentStyle()
    {
        m_arStandardStyles.clear();
        m_arStyleUsed.clear();
        m_arStandardStylesUsed.clear();
    }

    const std::wstring CDocumentStyle::GetStyle() const
    {
        if (m_sId.empty())
            return std::wstring();

        return m_sStyle;
    }

    std::wstring CDocumentStyle::GetIdAndClear()
    {
        const std::wstring sId = m_sId;
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

    void CDocumentStyle::CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement)
    {
        if (arStandartedStyles.empty())
            return;

        std::set<std::wstring> arStyles;
//        arStyles.reserve(arStandartedStyles.size());

        for (const std::wstring& sStyleName : arStandartedStyles)
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.end())
                arStyles.insert(sStyleName);
        }

        if (arStyles.empty())
            return;

        std::wstring sId;

        for (const std::wstring& sStyleName : arStyles)
        {
            sId += sStyleName + L'+';
            CXmlElement oTempElement;
            CreateStandardStyle(sStyleName, oTempElement);
            oElement += oTempElement;
        }

        sId.pop_back();
        oElement.SetName(sId);
        oElement.SetStyleId(sId);
    }

    void CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle, CXmlElement& oElement)
    {
        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) == m_arStandardStyles.end())
            return;

        oElement.CreateDefaultElement(sNameStyle);
    }

    void CDocumentStyle::ConvertStyle(const NSCSS::CCompiledStyle &oStyle, bool bIsPStyle, CXmlElement& oElement)
    {   
        if (oStyle.GetId().empty())
            return;

        std::wstring sName = oStyle.GetId();

        const size_t& posPoint = sName.find(L'.');

        if (posPoint != std::wstring::npos)
            sName = sName.substr(0, posPoint);
        else
        {
            const size_t& posLattice = sName.find(L'#');
            if (posLattice != std::wstring::npos)
                sName = sName.substr(0, posLattice);
            else
            {
                const size_t& posDash = sName.find(L'-');
                if (posDash != std::wstring::npos)
                    sName = sName.substr(0, posDash);
            }
        }


        if (!bIsPStyle)
            sName += L"-c";

        CXmlElement oStandardXmlElement;
        CXmlElement oParentStyle;

        std::vector<std::wstring> arParentsName = oStyle.GetParentsName();

        if (!arParentsName.empty())
        {
            std::wstring sParentName;
            for (std::wstring& sParent : arParentsName)
            {
                if(!bIsPStyle)
                    sParent += L"-c";
                sParentName += sParent + L"+";
            }

            sParentName.pop_back();

            CombineStandardStyles(arParentsName, oParentStyle);

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

        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.end())
            CreateStandardStyle(sName, oStandardXmlElement);

        if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            const std::wstring& sStandPlusParent = oStandardXmlElement.GetStyleId() + oParentStyle.GetStyleId();

            if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.end())
            {
                if (oStyle.Empty())
                {
                    m_sId = sStandPlusParent;
                    return;
                }

                oElement.SetBasedOn(sStandPlusParent);
            }
            else
            {
                CXmlElement oTempElement = oParentStyle;
                oTempElement += oStandardXmlElement;
                oTempElement.SetStyleId(sStandPlusParent);

                m_sStyle += bIsPStyle ? oTempElement.GetPStyle() :  oTempElement.GetRStyle();
                m_arStandardStylesUsed.push_back(sStandPlusParent);

                if (oStyle.Empty())
                {
                    m_sId = sStandPlusParent;
                    return;
                }

                oElement.SetBasedOn(oTempElement.GetStyleId());
            }
        }
        else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
        {
            const std::wstring& sStandartStyleID = oStandardXmlElement.GetStyleId();

            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) != m_arStandardStylesUsed.end())
            {
                if (oStyle.Empty())
                {
                    m_sId = sStandartStyleID;
                    return;
                }

                oElement.SetBasedOn(sStandartStyleID);
            }
            else
            {
                m_sStyle += bIsPStyle ? oStandardXmlElement.GetPStyle() : oStandardXmlElement.GetRStyle();
                m_arStandardStylesUsed.push_back(sStandartStyleID);

                if (oStyle.Empty())
                {
                    m_sId = sStandartStyleID;
                    return;
                }

                oElement.SetBasedOn(sStandartStyleID);
            }
        }
        else if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            const std::wstring& sParentsStyleID = oParentStyle.GetStyleId();

            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) != m_arStandardStylesUsed.end())
            {
                if (oStyle.Empty())
                {
                    m_sId = sParentsStyleID;
                    return;
                }

                oElement.SetBasedOn(sParentsStyleID);
            }
            else
            {
                m_sStyle += bIsPStyle ? oParentStyle.GetPStyle() : oParentStyle.GetRStyle();
                m_arStandardStylesUsed.push_back(sParentsStyleID);

                if (oStyle.Empty())
                {
                    m_sId = sParentsStyleID;
                    return;
                }

                oElement.SetBasedOn(sParentsStyleID);
            }
        }

        if (oStyle.Empty() && oElement.Empty())
        {
            m_sId = L"normal";

//            (oXmlElement.GetBasedOn().empty() || (oStandardXmlElement.Empty() && oParentStyle.Empty()))
//                    ? m_sId = L"normal" : m_sId = oXmlElement.GetBasedOn();

            return;
        }


        m_sId = oStyle.GetId();

        bIsPStyle ? m_sId = oStyle.GetId() : m_sId = oStyle.GetId() + L"-c";

        oElement.SetStyleId(m_sId);
        oElement.SetName(m_sId);
        oElement.SetQFormat(true);

        oElement.SetType(bIsPStyle ? L"paragraph" : L"character");

        oElement.SetCustomStyle(L"1");
    }

    void CDocumentStyle::SetPStyle(const NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
         ConvertStyle(oStyle, true, oXmlElement);

        if (oStyle.Empty() || oXmlElement.Empty())
            return;

        oXmlElement.SetJc(oStyle.GetTextAlign());

        std::wstring sIndValue;
        if (!oStyle.GetTextIndent().empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            const float& dFirstLineValue = wcstof(oStyle.GetTextIndent().c_str(), NULL) * 10.0f;
            sIndValue += L"w:firstLine=\"" +std::to_wstring((int)floor(dFirstLineValue + 0.5f)) + L"\" ";
        }

        if (!oStyle.GetMargin().empty())
        {
            const float& dLeftValue = wcstof(oStyle.GetMarginLeft().c_str(), NULL);
            const float& dRightValue = wcstof(oStyle.GetMarginRight().c_str(), NULL);

            sIndValue += L"w:left=\"" + std::to_wstring((int)floor(dLeftValue + 0.5f)) + L"\" ";
            sIndValue += L"w:right=\"" + std::to_wstring((int)floor(dRightValue + 0.5f)) + L"\"";
        }

        oXmlElement.SetInd(sIndValue);

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            const float& dAfterValue = wcstof(oStyle.GetMarginTop().c_str(), NULL);
            const float& dBeforeValue = wcstof(oStyle.GetMarginBottom().c_str(), NULL);

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floorf(dAfterValue + 0.5f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floorf(dBeforeValue + 0.5f)) + L"\" ";
        }

        if (!oStyle.GetLineHeight().empty())
        {
            float dValue = wcstof(oStyle.GetFontSize().c_str(), NULL);
            if (dValue == 0)
                dValue = 24;

            const float& dLineHeight = wcstof(oStyle.GetLineHeight().c_str(), NULL) / 2.4f * dValue;

            if (dLineHeight > 0)
            {
                sSpacingValue += L"w:line=\"" + std::to_wstring((int)floorf(dLineHeight + 0.5f)) + L"\" ";
                sSpacingValue += L"w:lineRule=\"auto\"";
            }
        }

        if (!sSpacingValue.empty())
        {
            oXmlElement.SetSpacing(sSpacingValue);
            oXmlElement.SetContextualSpacing(true);
        }

        oXmlElement.SetShd(oStyle.GetBackgroundColor());

//        if (!oStyle.GetBorder().empty() &&  oStyle.GetBorder() != L"none")

        const std::wstring& sBorderColor = oStyle.GetBorderColor();
        const std::wstring& sBorderStyle = oStyle.GetBorderStyle();

        const float& dValue = wcstof(oStyle.GetBorderWidth().c_str(), NULL) * 4.0f;
        const std::wstring& sBorderWidth = std::to_wstring((int)floor(dValue + 0.5f));

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


        const std::wstring& sBorderTopColor = oStyle.GetBorderTopColor();
        const std::wstring& sBorderTopStyle = oStyle.GetBorderTopStyle();

        const float& dValueTop = wcstof(oStyle.GetBorderTopWidth().c_str(), NULL) * 4.0f;
        const std::wstring& sBorderTopWidth = std::to_wstring((int)floor(dValueTop + 0.5f));

        const std::wstring& sBorderTop = L" w:color=\"" + sBorderTopColor + L"\" w:space=\"4\" w:sz=\"" +
                                                         sBorderTopWidth + L"\" w:val=\"" + sBorderTopStyle + L"\"";

        if (sBorderTopColor.empty() + sBorderTopWidth.empty() + sBorderTopStyle.empty() == 0)
            oXmlElement.SetTopBorder(sBorderTop);

        const std::wstring& sBorderLeftColor = oStyle.GetBorderLeftColor();
        const std::wstring& sBorderLeftStyle = oStyle.GetBorderLeftStyle();

        const float& dValueLeft = wcstof(oStyle.GetBorderLeftWidth().c_str(), NULL) * 4.0f;
        const std::wstring& sBorderLeftWidth = std::to_wstring((int)floorf(dValueLeft + 0.5f));

        const std::wstring& sBorderLeft = L" w:color=\"" + sBorderLeftColor + L"\" w:space=\"4\" w:sz=\"" +
                                                          sBorderLeftWidth + L"\" w:val=\"" + sBorderLeftStyle + L"\"";

        if (sBorderLeftColor.empty() + sBorderLeftWidth.empty() + sBorderLeftStyle.empty() == 0)
            oXmlElement.SetLeftBorder(sBorderLeft);

        const std::wstring& sBorderBottomColor = oStyle.GetBorderBottomColor();
        const std::wstring& sBorderBottomStyle = oStyle.GetBorderBottomStyle();

        const float& dValueBottom = wcstof(oStyle.GetBorderBottomWidth().c_str(), NULL) * 4.0f;
        const std::wstring& sBorderBottomWidth = std::to_wstring((int)floorf(dValueBottom + 0.5f));

        const std::wstring& sBorderBottom = L" w:color=\"" + sBorderBottomColor + L"\" w:space=\"4\" w:sz=\"" +
                                                             sBorderBottomWidth + L"\" w:val=\"" + sBorderBottomStyle + L"\"";

        if (sBorderBottomColor.empty() + sBorderBottomWidth.empty() + sBorderBottomStyle.empty() == 0)
            oXmlElement.SetBottomBorder(sBorderBottom);

        const std::wstring& sBorderRightColor = oStyle.GetBorderRightColor();
        const std::wstring& sBorderRightStyle = oStyle.GetBorderRightStyle();

        const float& dValueRight = wcstod(oStyle.GetBorderRightWidth().c_str(), NULL) * 4.0f;
        const std::wstring& sBorderRightWidth = std::to_wstring((int)floorf(dValueRight + 0.5f));

        const std::wstring& sBorderRight = L" w:color=\"" + sBorderRightColor + L"\" w:space=\"4\" w:sz=\"" +
                                                            sBorderRightWidth + L"\" w:val=\"" + sBorderRightStyle + L"\"";

        if (sBorderRightColor.empty() + sBorderRightWidth.empty() + sBorderRightStyle.empty() == 0)
            oXmlElement.SetRightBorder(sBorderRight);

    }

    void CDocumentStyle::SetRStyle(const NSCSS::CCompiledStyle &oStyle, CXmlElement& oXmlElement)
    {
        ConvertStyle(oStyle, false, oXmlElement);

        if (oStyle.Empty() || oXmlElement.Empty())
            return;

        oXmlElement.SetColor(oStyle.GetColor());
        oXmlElement.SetShd(oStyle.GetBackgroundColor());

        if (!oStyle.GetFontSize().empty())
        {
            const float& dValue = wcstof(oStyle.GetFontSize().c_str(), NULL);

            oXmlElement.SetSz(std::to_wstring((int)dValue));
        }
        if (!oStyle.GetFontFamily().empty())
        {
            const std::vector<std::wstring> sFontsFamily = oStyle.GetFontNames();
            if (!sFontsFamily.empty())
                oXmlElement.SetRFonts(sFontsFamily.back());
        }

        if (oStyle.GetFontWeight() == L"bold")
            oXmlElement.SetB(true);

        if (oStyle.GetFontStyle() == L"italic")
            oXmlElement.SetI(true);

        std::wstring sSpacingValue;

        if (!oStyle.GetMargin().empty())
        {
            const float& dAfterValue = wcstof(oStyle.GetMarginTop().c_str(), NULL) * 10.0f;

            const float& dBeforeValue = wcstof(oStyle.GetMarginBottom().c_str(), NULL) * 10.0f;

            sSpacingValue += L"w:after=\"" + std::to_wstring((int)floorf(dAfterValue + 0.5f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring((int)floorf(dBeforeValue + 0.5f)) + L"\" ";
        }

        if (!oStyle.GetLineHeight().empty())
        {
            float dValue = wcstof(oStyle.GetFontSize().c_str(), NULL);
            if (dValue == 0.0f)
                dValue = 24.0f;

            const float& dLineHeight = wcstof(oStyle.GetLineHeight().c_str(), NULL) / 2.4f * dValue;

            sSpacingValue += L"w:line=\"" + std::to_wstring((int)floorf(dLineHeight + 0.5f)) + L"\" ";
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

        if (oItem != m_arStyleUsed.end())
        {
            m_sId = oItem->m_sId;
            return;
        }
        CXmlElement oXmlElement;
        SetRStyle(oStyle, oXmlElement);

        if (!oStyle.Empty() || !oXmlElement.Empty())
        {
            structStyle.m_sId = oXmlElement.GetStyleId();
            m_arStyleUsed.push_back(structStyle);
            m_sStyle += oXmlElement.GetRStyle();
        }
    }

    void CDocumentStyle::WritePStyle(const NSCSS::CCompiledStyle &oStyle)
    {   
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            return;
        }

        CStyleUsed structStyle(oStyle, true);
        const auto& oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

        if (oItem != m_arStyleUsed.end())
        {
            m_sId = oItem->m_sId;
            return;
        }

        CXmlElement oXmlElement;
        SetPStyle(oStyle, oXmlElement);

        if (!oStyle.Empty() || !oXmlElement.Empty())
        {
            structStyle.m_sId = oXmlElement.GetStyleId();
            m_arStyleUsed.push_back(structStyle);
            m_sStyle += oXmlElement.GetPStyle();
        }
    }
}
