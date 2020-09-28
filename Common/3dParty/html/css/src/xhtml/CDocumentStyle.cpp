#include "CDocumentStyle.h"

#include <iostream>
#include <set>
#include <wchar.h>
#include <math.h>
#include <algorithm>
#include <list>

namespace NSCSS
{
    CDocumentStyle::CDocumentStyle() : m_arStandardStyles({L"a", L"li", L"h1", L"h2", L"h3", L"h4", L"h5", L"h6", L"h1-c",
        L"h2-c", L"h3-c", L"h4-c", L"h5-c", L"h6-c", L"p-c", L"p", L"div-c", L"div", L"a-c"}) {}

    CDocumentStyle::~CDocumentStyle()
    {
        m_arStandardStyles.    clear();
        m_arStyleUsed.         clear();
        m_arStandardStylesUsed.clear();
    }

    std::wstring CDocumentStyle::GetStyle() const
    {
        return m_sId.empty() ? std::wstring() : m_sStyle;
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

    void CDocumentStyle::SetStyle(const std::wstring& sStyle)
    {
        if (sStyle.empty())
            return;
        m_sStyle = sStyle;
    }

    void CDocumentStyle::SetId   (const std::wstring& sId)
    {
        if (sId.empty())
            return;
        m_sId = sId;
    }

    void CDocumentStyle::CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement)
    {
        if (arStandartedStyles.empty())
            return;

        std::vector<std::wstring> arStyles;
        for (const std::wstring& sStyleName : arStandartedStyles)
        {
            if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sStyleName) != m_arStandardStyles.end())
                arStyles.push_back(sStyleName);
        }

        if (arStyles.empty())
            return;

        std::wstring sId;
        for (const std::wstring& sStyleName : arStyles)
        {
            sId += sStyleName + L'+';
            CXmlElement oTempElement;
            CreateStandardStyle(sStyleName, oTempElement);
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStyleName) == m_arStandardStylesUsed.end())
            {
                m_sStyle += oTempElement.GetStyle();
                m_arStandardStylesUsed.push_back(sStyleName);
            }
            oElement += oTempElement;
        }
        sId.pop_back();

        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Name, sId);
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, sId);
    }

    void CDocumentStyle::CreateStandardStyle(const std::wstring& sNameStyle, CXmlElement& oElement)
    {
        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sNameStyle) != m_arStandardStyles.end())
            oElement.CreateDefaultElement(sNameStyle);
    }

    void CDocumentStyle::ConvertStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle)
    {
        if (oStyle.GetId().empty())
            return;

        std::wstring sName = oStyle.GetId();
        const size_t posPoint = sName.find(L'.');

        if (posPoint != std::wstring::npos)
            sName = sName.substr(0, posPoint);
        else
        {
            const size_t posLattice = sName.find(L'#');
            if (posLattice != std::wstring::npos)
                sName = sName.substr(0, posLattice);
            else
            {
                const size_t posDash = sName.find(L'-');
                if (posDash != std::wstring::npos)
                    sName = sName.substr(0, posDash);
            }
        }

        if (!bIsPStyle)
            sName += L"-c";

        CXmlElement oParentStyle;
        std::vector<std::wstring> arParentsName = oStyle.GetParentsName();

        if (!bIsPStyle)
            for (std::wstring& sParentName : arParentsName)
                sParentName += L"-c";

        if (!arParentsName.empty())
        {
            CombineStandardStyles(arParentsName, oParentStyle);

            if (!oParentStyle.Empty())
            {
                oParentStyle.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, L"(" + oParentStyle.GetStyleId() + L")");
                if (!bIsPStyle)
                {
                    oParentStyle.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, oParentStyle.GetStyleId() + L"-c");
                    oParentStyle.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Type, L"character");
                }
            }
        }

        CXmlElement oStandardXmlElement;
        if (std::find(m_arStandardStyles.begin(), m_arStandardStyles.end(), sName) != m_arStandardStyles.end())
            CreateStandardStyle(sName, oStandardXmlElement);

        if (oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            std::wstring sParentsStyleID = oParentStyle.GetStyleId();
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) == m_arStandardStylesUsed.end())
            {
                m_sStyle += bIsPStyle ? oParentStyle.GetPStyle() : oParentStyle.GetRStyle();
                m_arStandardStylesUsed.push_back(sParentsStyleID);
            }
            if (oStyle.Empty())
            {
                m_sId = sParentsStyleID;
                return;
            }
            oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_BasedOn, sParentsStyleID);
        }
        else if (!oStandardXmlElement.Empty() && !oParentStyle.Empty())
        {
            std::wstring sStandPlusParent = oStandardXmlElement.GetStyleId() + L"+" + oParentStyle.GetStyleId();
            if(std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandPlusParent) != m_arStandardStylesUsed.end())
            {
                if (oStyle.Empty())
                {
                    m_sId = sStandPlusParent;
                    return;
                }
                oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_BasedOn, sStandPlusParent);
            }
            else
            {
                CXmlElement oTempElement = oParentStyle;
                oTempElement += oStandardXmlElement;

                oTempElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, sStandPlusParent);

                m_sStyle += bIsPStyle ? oTempElement.GetPStyle() :  oTempElement.GetRStyle();
                m_arStandardStylesUsed.push_back(sStandPlusParent);
                if (oStyle.Empty())
                {
                    m_sId = sStandPlusParent;
                    return;
                }
                oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_BasedOn, oTempElement.GetStyleId());
            }
        }
        else if (!oStandardXmlElement.Empty() && oParentStyle.Empty())
        {
            std::wstring sStandartStyleID = oStandardXmlElement.GetStyleId();
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sStandartStyleID) == m_arStandardStylesUsed.end())
            {
                m_sStyle += bIsPStyle ? oStandardXmlElement.GetPStyle() : oStandardXmlElement.GetRStyle();
                m_arStandardStylesUsed.push_back(sStandartStyleID);
            }
            if (oStyle.Empty())
            {
                m_sId = sStandartStyleID;
                return;
            }
            oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_BasedOn, sStandartStyleID);
        }

        if (oStyle.Empty() && oElement.Empty())
        {
            m_sId = L"normal";
//            (oXmlElement.GetBasedOn().empty() || (oStandardXmlElement.Empty() && oParentStyle.Empty()))
//                    ? m_sId = L"normal" : m_sId = oXmlElement.GetBasedOn();
            return;
        }

        m_sId = oStyle.GetId();
        if(!bIsPStyle)
            m_sId += L"-c";
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, m_sId);
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Name, m_sId);
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_QFormat, L"true");
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Type, bIsPStyle ? L"paragraph" : L"character");
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_CustomStyle, L"1");
    }

    void CDocumentStyle::SetPStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
    {
        ConvertStyle(oStyle, oXmlElement, true);
        if (oStyle.Empty() || oXmlElement.Empty())
            return;

        std::wstring sTextAlign = oStyle.GetTextAlign();
        std::transform(sTextAlign.begin(), sTextAlign.end(), sTextAlign.begin(), towlower);

        if (sTextAlign == L"justify")
            sTextAlign = L"both";
        else if (sTextAlign == L"middle")
            sTextAlign = L"center";
        else if (sTextAlign != L"left" && sTextAlign != L"center" && sTextAlign != L"right" && sTextAlign != L"both")
            sTextAlign.clear();

        oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Jc, sTextAlign);

        std::wstring sIndValue;
        sIndValue.reserve(64);

        const std::wstring& sTextIndent = oStyle.GetTextIndent();
        if (!sTextIndent.empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            const float dFirstLineValue = wcstof(sTextIndent.c_str(), NULL) * 10.0f;
            sIndValue += L"w:firstLine=\"" + std::to_wstring(static_cast<unsigned short int>(dFirstLineValue + 0.5f)) + L"\" ";
        }

        const std::vector<std::wstring>& sMargins = oStyle.GetMargins();
        if (!sMargins.empty())
        {
            const float dLeftValue  = wcstof(sMargins[1].c_str(), NULL);
            const float dRightValue = wcstof(sMargins[3].c_str(), NULL);
            sIndValue += L"w:left=\""  + std::to_wstring(static_cast<short int>(dLeftValue  + 0.5f)) + L"\" ";
            sIndValue += L"w:right=\"" + std::to_wstring(static_cast<short int>(dRightValue + 0.5f)) + L"\" ";
        }

        oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Ind, sIndValue);

        std::wstring sSpacingValue;
        sSpacingValue.reserve(128);

        if (!sMargins.empty())
        {
            const float dAfterValue  = wcstof(sMargins[0].c_str(), NULL);
            const float dBeforeValue = wcstof(sMargins[2].c_str(), NULL);

            sSpacingValue += L"w:after=\""  + std::to_wstring(static_cast<unsigned short int>((dAfterValue > 0.0f) ? (dAfterValue  + 0.5f) : 0.0f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring(static_cast<unsigned short int>((dBeforeValue > 0.0f) ? (dBeforeValue  + 0.5f) : 0.0f)) + L"\" ";
        }

        const std::wstring sLineHeight = oStyle.GetLineHeight();
        if (!sLineHeight.empty())
        {
            float dValue = wcstof(oStyle.GetFontSize().c_str(), NULL);
            if (dValue == 0.0f)
                dValue = 22.0f;

            float dLineHeight = wcstof(sLineHeight.c_str(), NULL);

            if (dLineHeight > 7.0f || dLineHeight <= 0.0f)
                dLineHeight /= dValue;

            if (dLineHeight < 1.0f)
                dLineHeight = 1.0f;

            dLineHeight *= dValue * 10.0f;

            if (dLineHeight > 0)
            {
                sSpacingValue += L"w:line=\"" + std::to_wstring(static_cast<unsigned short int>((dLineHeight > 0.0f) ? (dLineHeight  + 0.5f) : 0.0f)) + L"\" ";
                sSpacingValue += L"w:lineRule=\"auto\"";
            }
        }
        if (!sSpacingValue.empty())
        {
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Spacing, sSpacingValue);
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_ContextualSpacing, L"true");
        }

        oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Shd, oStyle.GetBackgroundColor());

//        if (!oStyle.GetBorder().empty() &&  oStyle.GetBorder() != L"none")

        const std::wstring sBorderColor = oStyle.GetBorderColor();
        const std::wstring sBorderStyle = oStyle.GetBorderStyle();
        const float dValue = wcstof(oStyle.GetBorderWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderWidth = std::to_wstring(static_cast<short int>(dValue + 0.5f));
        if (!sBorderColor.empty() && !sBorderWidth.empty() && !sBorderStyle.empty())
        {
            const std::wstring sBorder = L" w:color=\"" + sBorderColor + L"\" w:space=\"4\" w:sz=\"" +
                    sBorderWidth + L"\" w:val=\"" + sBorderStyle + L"\"";

            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_TopBorder, sBorder);
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_LeftBorder, sBorder);
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_BottomBorder, sBorder);
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_RightBorder, sBorder);

            return;
        }

        const std::wstring sBorderTopColor = oStyle.GetBorderTopColor();
        const std::wstring sBorderTopStyle = oStyle.GetBorderTopStyle();
        const float dValueTop = wcstof(oStyle.GetBorderTopWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderTopWidth = std::to_wstring(static_cast<unsigned short int>(dValueTop + 0.5f));
        if (!sBorderTopColor.empty() && !sBorderTopWidth.empty() && !sBorderTopStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_TopBorder,
                                         L" w:color=\"" + sBorderTopColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderTopWidth + L"\" w:val=\"" + sBorderTopStyle + L"\"");

        const std::wstring sBorderLeftColor = oStyle.GetBorderLeftColor();
        const std::wstring sBorderLeftStyle = oStyle.GetBorderLeftStyle();
        const float dValueLeft = wcstof(oStyle.GetBorderLeftWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderLeftWidth = std::to_wstring(static_cast<unsigned short int>(dValueLeft + 0.5f));
        if (!sBorderLeftColor.empty() && !sBorderLeftWidth.empty() && !sBorderLeftStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_LeftBorder,
                                         L" w:color=\"" + sBorderLeftColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderLeftWidth + L"\" w:val=\"" + sBorderLeftStyle + L"\"");

        const std::wstring sBorderBottomColor = oStyle.GetBorderBottomColor();
        const std::wstring sBorderBottomStyle = oStyle.GetBorderBottomStyle();
        const float dValueBottom = wcstof(oStyle.GetBorderBottomWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderBottomWidth = std::to_wstring(static_cast<unsigned short int>(dValueBottom + 0.5f));
        if (!sBorderBottomColor.empty() && !sBorderBottomWidth.empty() && !sBorderBottomStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_BottomBorder,
                                         L" w:color=\"" + sBorderBottomColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderBottomWidth + L"\" w:val=\"" + sBorderBottomStyle + L"\"");

        const std::wstring sBorderRightColor = oStyle.GetBorderRightColor();
        const std::wstring sBorderRightStyle = oStyle.GetBorderRightStyle();
        const float dValueRight = wcstof(oStyle.GetBorderRightWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderRightWidth = std::to_wstring(static_cast<unsigned short int>(dValueRight + 0.5f));
        if (!sBorderRightColor.empty() && !sBorderRightWidth.empty() && !sBorderRightStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_RightBorder,
                                         L" w:color=\"" + sBorderRightColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderRightWidth + L"\" w:val=\"" + sBorderRightStyle + L"\"");

    }

    void CDocumentStyle::SetRStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
    {
        ConvertStyle(oStyle, oXmlElement, false);
        if (oStyle.Empty() || oXmlElement.Empty())
            return;

        oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_Color, oStyle.GetColor());
        oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_Shd, oStyle.GetBackgroundColor());
        oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_U, oStyle.GetTextDecoration());

        const std::wstring sFontSize = oStyle.GetFontSize();
        if (!sFontSize.empty())
        {
            const float dValue = wcstof(sFontSize.c_str(), NULL);
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_Sz, std::to_wstring(static_cast<unsigned short int>((dValue > 0.0f) ? dValue : 22.0f)));
        }

        const std::wstring sFontFamily = oStyle.GetFontFamily();
        if (!sFontFamily.empty())
        {
            const std::vector<std::wstring> sFontsFamily = oStyle.GetFontNames2(sFontFamily);
            if (!sFontsFamily.empty())
                oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_RFonts, sFontsFamily.back());
        }

        if (oStyle.GetFontWeight() == L"bold")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_B, L"true");

        if (oStyle.GetFontStyle() == L"italic")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_I, L"true");

        std::wstring sSpacingValue;
        const std::vector<std::wstring> sMargins = oStyle.GetMargins();
        if (!sMargins.empty())
        {
            const float dAfterValue = wcstof(sMargins[0].c_str(), NULL) * 10.0f;
            const float dBeforeValue = wcstof(sMargins[2].c_str(), NULL) * 10.0f;
            sSpacingValue += L"w:after=\"" + std::to_wstring(static_cast<unsigned short int>(dAfterValue + 0.5f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring(static_cast<unsigned short int>(dBeforeValue + 0.5f)) + L"\" ";
        }

        // spacing в rPr
    }

    void CDocumentStyle::WriteRStyle (const NSCSS::CCompiledStyle& oStyle)
    {
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            return;
        }

        CStyleUsed structStyle(oStyle, false);

        std::list<CStyleUsed>::const_iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

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

    void CDocumentStyle::WritePStyle (const NSCSS::CCompiledStyle& oStyle)
    {   
        if(oStyle.GetId().empty())
        {
            m_sId = L"normal";
            return;
        }

        CStyleUsed structStyle(oStyle, true);
        std::list<CStyleUsed>::const_iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

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
