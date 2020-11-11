#include "CDocumentStyle.h"

#include <iostream>
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
        for (std::vector<std::wstring>::const_reverse_iterator iStyleName = arStyles.rbegin(); iStyleName != arStyles.rend(); ++iStyleName)
        {
            sId += *iStyleName + L'+';
            CXmlElement oTempElement;
            CreateStandardStyle(*iStyleName, oTempElement);
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), *iStyleName) == m_arStandardStylesUsed.end())
            {
                m_sStyle += oTempElement.GetStyle();
                m_arStandardStylesUsed.push_back(*iStyleName);
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
            const std::wstring sParentsStyleID = oParentStyle.GetStyleId();
            if (std::find(m_arStandardStylesUsed.begin(), m_arStandardStylesUsed.end(), sParentsStyleID) == m_arStandardStylesUsed.end())
            {
                if (bIsPStyle)
                {
                    oParentStyle.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_QFormat, L"true");
                    m_sStyle += oParentStyle.GetPStyle();
                }
                else
                    m_sStyle += oParentStyle.GetRStyle();

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

                if (bIsPStyle)
                {
                    oTempElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_QFormat, L"true");
                    m_sStyle += oTempElement.GetPStyle();
                }
                else
                    m_sStyle += oTempElement.GetRStyle();

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
                if (bIsPStyle)
                {
                    oStandardXmlElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_QFormat, L"true");
                    m_sStyle += oStandardXmlElement.GetPStyle();
                }
                else
                    m_sStyle += oStandardXmlElement.GetRStyle();

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
            return;
        }

        m_sId = oStyle.GetId();
        if(!bIsPStyle)
            m_sId += L"-c";
        else
            oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_QFormat, L"true");

        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_StyleId, m_sId);
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Name, m_sId);
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_Type, bIsPStyle ? L"paragraph" : L"character");
        oElement.AddBasicProperties(NS_CONST_VALUES::BasicProperties::B_CustomStyle, L"1");
    }

    void CDocumentStyle::SetPStyle   (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement)
    {
        ConvertStyle(oStyle, oXmlElement, true);
        if (oStyle.Empty() || oXmlElement.Empty())
            return;

        std::wstring sTextAlign = oStyle.GetTextAlign();

        if (sTextAlign == L"justify")
            sTextAlign = L"both";
        else if (sTextAlign == L"middle")
            sTextAlign = L"center";
        else if (sTextAlign != L"left" && sTextAlign != L"center" && sTextAlign != L"right" && sTextAlign != L"both")
            sTextAlign.clear();

        oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Jc, sTextAlign);

        std::wstring sInfValue;
        sInfValue.reserve(64);

        const std::wstring& sTextIndent = oStyle.GetTextIndent();
        if (!sTextIndent.empty() && oStyle.GetId().find(L"table") == std::wstring::npos)
        {
            const float fFirstLineValue = wcstof(sTextIndent.c_str(), NULL) * 10.0f;
            sInfValue += L"w:firstLine=\"" + std::to_wstring(static_cast<unsigned short int>(fFirstLineValue + 0.5f)) + L"\" ";
        }

        const std::vector<std::wstring>& sMargins = oStyle.GetMargins();
        if (!sMargins.empty())
        {
            const float fLeftValue  = wcstof(sMargins[3].c_str(), NULL) * 10.0f;
            const float fRightValue = wcstof(sMargins[1].c_str(), NULL) * 10.0f;
            sInfValue += L"w:left=\""  + std::to_wstring(static_cast<short int>(fLeftValue  + 0.5f)) + L"\" ";
            sInfValue += L"w:right=\"" + std::to_wstring(static_cast<short int>(fRightValue + 0.5f)) + L"\" ";
        }

        oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_Ind, sInfValue);

        std::wstring sSpacingValue;
        sSpacingValue.reserve(128);

        if (!sMargins.empty())
        {
            const float fAfterValue  = wcstof(sMargins[0].c_str(), NULL);
            const float fBeforeValue = wcstof(sMargins[2].c_str(), NULL);

            sSpacingValue += L"w:after=\""  + std::to_wstring(static_cast<unsigned short int>((fAfterValue > 0.0f) ? (fAfterValue  + 0.5f) : 0.0f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring(static_cast<unsigned short int>((fBeforeValue > 0.0f) ? (fBeforeValue  + 0.5f) : 0.0f)) + L"\" ";
        }

        const std::wstring sLineHeight = oStyle.GetLineHeight();
        if (!sLineHeight.empty())
        {
            float fValue = wcstof(oStyle.GetFontSize().c_str(), NULL);
            if (fValue <= 0.0f)
                fValue = NS_CONST_VALUES::FONT_SIZE;

            float fLineHeight = wcstof(sLineHeight.c_str(), NULL);

            if (fLineHeight >= 1.0f)
            {
                fLineHeight *= (fLineHeight < fValue / 2) ? 10.0f : (10.0f / fLineHeight);
                float fLine = fLineHeight * fValue;

                sSpacingValue += L"w:line=\"" + std::to_wstring(static_cast<unsigned short int>((fLine < fValue * 20.0f) ? fLine : fValue * 5.0f + 0.5f)) + L"\" ";
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
        const float fValue = wcstof(oStyle.GetBorderWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderWidth = std::to_wstring(static_cast<short int>(fValue + 0.5f));
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
        const float fValueTop = wcstof(oStyle.GetBorderTopWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderTopWidth = std::to_wstring(static_cast<unsigned short int>(fValueTop + 0.5f));
        if (!sBorderTopColor.empty() && !sBorderTopWidth.empty() && !sBorderTopStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_TopBorder,
                                         L" w:color=\"" + sBorderTopColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderTopWidth + L"\" w:val=\"" + sBorderTopStyle + L"\"");

        const std::wstring sBorderLeftColor = oStyle.GetBorderLeftColor();
        const std::wstring sBorderLeftStyle = oStyle.GetBorderLeftStyle();
        const float fValueLeft = wcstof(oStyle.GetBorderLeftWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderLeftWidth = std::to_wstring(static_cast<unsigned short int>(fValueLeft + 0.5f));
        if (!sBorderLeftColor.empty() && !sBorderLeftWidth.empty() && !sBorderLeftStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_LeftBorder,
                                         L" w:color=\"" + sBorderLeftColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderLeftWidth + L"\" w:val=\"" + sBorderLeftStyle + L"\"");

        const std::wstring sBorderBottomColor = oStyle.GetBorderBottomColor();
        const std::wstring sBorderBottomStyle = oStyle.GetBorderBottomStyle();
        const float fValueBottom = wcstof(oStyle.GetBorderBottomWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderBottomWidth = std::to_wstring(static_cast<unsigned short int>(fValueBottom + 0.5f));
        if (!sBorderBottomColor.empty() && !sBorderBottomWidth.empty() && !sBorderBottomStyle.empty())
            oXmlElement.AddPropertiesInP(NS_CONST_VALUES::ParagraphProperties::P_BottomBorder,
                                         L" w:color=\"" + sBorderBottomColor + L"\" w:space=\"4\" w:sz=\"" +
                                         sBorderBottomWidth + L"\" w:val=\"" + sBorderBottomStyle + L"\"");

        const std::wstring sBorderRightColor = oStyle.GetBorderRightColor();
        const std::wstring sBorderRightStyle = oStyle.GetBorderRightStyle();
        const float fValueRight = wcstof(oStyle.GetBorderRightWidth().c_str(), NULL) * 4.0f;
        const std::wstring sBorderRightWidth = std::to_wstring(static_cast<unsigned short int>(fValueRight + 0.5f));
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
            const float fValue = wcstof(sFontSize.c_str(), NULL);
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_Sz, std::to_wstring(static_cast<unsigned short int>((fValue > 0.0f) ? fValue : NS_CONST_VALUES::FONT_SIZE)));
        }

        const std::wstring sFontFamily = oStyle.GetFontFamily();
        if (!sFontFamily.empty())
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_RFonts, sFontFamily);

        const std::wstring sFontWeight = oStyle.GetFontWeight();

        if (sFontWeight == L"bold")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_B, L"true");
        else if (sFontWeight == L"normal")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_B, L"false");

        if (oStyle.GetFontStyle() == L"italic")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_I, L"true");
        else if (oStyle.GetFontStyle() == L"normal")
            oXmlElement.AddPropertiesInR(NS_CONST_VALUES::RunnerProperties::R_I, L"false");

        std::wstring sSpacingValue;
        const std::vector<std::wstring> sMargins = oStyle.GetMargins();
        if (!sMargins.empty())
        {
            const float fAfterValue = wcstof(sMargins[0].c_str(), NULL) * 10.0f;
            const float fBeforeValue = wcstof(sMargins[2].c_str(), NULL) * 10.0f;
            sSpacingValue += L"w:after=\"" + std::to_wstring(static_cast<unsigned short int>(fAfterValue + 0.5f)) + L"\" ";
            sSpacingValue += L"w:before=\"" + std::to_wstring(static_cast<unsigned short int>(fBeforeValue + 0.5f)) + L"\" ";
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

        std::list<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

        if (oItem != m_arStyleUsed.end())
        {
            m_sId = (*oItem).getId();
            return;
        }
        CXmlElement oXmlElement;
        SetRStyle(oStyle, oXmlElement);

        if (!oStyle.Empty() || !oXmlElement.Empty())
        {
            structStyle.setId(oXmlElement.GetStyleId());
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
        std::list<CStyleUsed>::iterator oItem = std::find(m_arStyleUsed.begin(), m_arStyleUsed.end(), structStyle);

        if (oItem != m_arStyleUsed.end())
        {
            m_sId = (*oItem).getId();
            return;
        }

        CXmlElement oXmlElement;
        SetPStyle(oStyle, oXmlElement);

        if (!oStyle.Empty() || !oXmlElement.Empty())
        {
            structStyle.setId(oXmlElement.GetStyleId());
            m_arStyleUsed.push_back(structStyle);
            m_sStyle += oXmlElement.GetPStyle();
        }
    }
}
