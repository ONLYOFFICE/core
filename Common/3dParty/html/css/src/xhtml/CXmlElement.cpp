#include "CXmlElement.h"

#include <algorithm>
#include <vector>
#include <cwctype>
#include <functional>

#include <iostream>
#include "../StaticFunctions.h"
#include "../ConstValues.h"

CXmlElement::CXmlElement()
{}

CXmlElement::~CXmlElement()
{
    m_mBasicValues.clear();
    m_mPStyleValues.clear();
    m_mRStyleValues.clear();
}

CXmlElement::CXmlElement(const std::wstring& sNameDefaultElement)
{
    CreateDefaultElement(sNameDefaultElement);
}

bool CXmlElement::Empty() const
{
    return m_mBasicValues.empty() && m_mPStyleValues.empty() && m_mRStyleValues.empty();
}

void CXmlElement::CreateDefaultElement(const std::wstring& sNameDefaultElement)
{
    if (!Empty())
        Clear();

    if (sNameDefaultElement == L"li")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"li");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"List Paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"34");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_ContextualSpacing, L"true");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Ind, L"w:left=\"720\"");
    }
    else if (sNameDefaultElement == L"h1")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h1-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"0");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"480\"");
    }
    else if (sNameDefaultElement == L"h2")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h2");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 2");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h2-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"1");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"400\"");
    }
    else if (sNameDefaultElement == L"h3")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h3");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 3");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h3-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"2");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"360\"");
    }
    else if (sNameDefaultElement == L"h4")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h4");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 4");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h4-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"3");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"320\"");
    }
    else if (sNameDefaultElement == L"h5")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h5");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 5");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h5-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"4");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"280\"");

    }
    else if (sNameDefaultElement == L"h6")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h6");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Heading 6");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h6-c");

        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl, L"5");
        AddPropertiesInP(NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing, L"w:after=\"0\" w:before=\"280\"");
    }
    else if (sNameDefaultElement == L"h1-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h1-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 1 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h1");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"44");
    }
    else if (sNameDefaultElement == L"h2-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h2-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 2 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h2");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"33");
    }
    else if (sNameDefaultElement == L"h3-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h3-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 3 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h3");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"26");
    }
    else if (sNameDefaultElement == L"h4-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h4-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 4 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h4");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"22");
    }
    else if (sNameDefaultElement == L"h5-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h5-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 5 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h5");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"18");
    }
    else if (sNameDefaultElement == L"h6-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"h6-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Title 6 Sign");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"9");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"h6");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B, L"bold");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz, L"15");
    }
    else if (sNameDefaultElement == L"p-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"p-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Paragraph character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"p");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
    }
    else if (sNameDefaultElement == L"p")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"p");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"p-c");
    }
    else if (sNameDefaultElement == L"div-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"div-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Div character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"div");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
    }
    else if (sNameDefaultElement == L"div")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"div");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle, L"1");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Div paragraph");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"normal");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link, L"div-c");
    }
    else if (sNameDefaultElement == L"a-c")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"a-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Hyperlink character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority, L"99");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed, L"true");

        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Color, L"0000FF");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_U, L"single");
        AddPropertiesInR(NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts, L"\"Times New Roman\"");
    }
    else if (sNameDefaultElement == L"a")
    {
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type, L"character");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn, L"a-c");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId, L"a");
        AddBasicProperties(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name, L"Hyperlink");
    }
}

void CXmlElement::Clear()
{
    m_mBasicValues.clear();
    m_mPStyleValues.clear();
    m_mRStyleValues.clear();
}

void CXmlElement::AddPropertiesInP(const NSCSS::NSConstValues::NSProperties::ParagraphProperties &nProperties, const std::wstring &sValue)
{
    if (sValue.empty())
        return;

    m_mPStyleValues[nProperties] = sValue;
}

void CXmlElement::AddPropertiesInR(const NSCSS::NSConstValues::NSProperties::RunnerProperties &nProperties, const std::wstring &sValue)
{
    if (sValue.empty())
        return;

    m_mRStyleValues[nProperties] = sValue;
}

void CXmlElement::AddBasicProperties(const NSCSS::NSConstValues::NSProperties::BasicProperties &nProperties, const std::wstring &sValue)
{
    if (sValue.empty())
        return;

    m_mBasicValues[nProperties] = sValue;
}

CXmlElement& CXmlElement::operator+=(const CXmlElement& oElement)
{
    if (oElement.Empty())
        return *this;

    for (const std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::value_type& oBasicElement : oElement.m_mBasicValues)
        m_mBasicValues[oBasicElement.first] = oBasicElement.second;

    for (const std::map<NSCSS::NSConstValues::NSProperties::ParagraphProperties, std::wstring>::value_type& oPElement : oElement.m_mPStyleValues)
        m_mPStyleValues[oPElement.first] = oPElement.second;

    for (const std::map<NSCSS::NSConstValues::NSProperties::RunnerProperties, std::wstring>::value_type& oRElement : oElement.m_mRStyleValues)
        m_mRStyleValues[oRElement.first] = oRElement.second;

    return *this;
}

CXmlElement& CXmlElement::operator=(const CXmlElement& oElement)
{
    m_mBasicValues = oElement.m_mBasicValues;
    m_mPStyleValues = oElement.m_mPStyleValues;
    m_mRStyleValues = oElement.m_mRStyleValues;

    return *this;
}

bool CXmlElement::operator==(const CXmlElement &oElement)
{
    return m_mBasicValues == oElement.m_mBasicValues &&
           m_mPStyleValues == oElement.m_mPStyleValues &&
           m_mRStyleValues == oElement.m_mRStyleValues;
}

std::wstring CXmlElement::ConvertPStyle() const
{
    if (m_mPStyleValues.empty())
        return std::wstring();

    std::wstring sPPr;
    std::wstring sPBdr;

    for (const std::map<NSCSS::NSConstValues::NSProperties::ParagraphProperties, std::wstring>::value_type& oItem : m_mPStyleValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Jc:
            {
                sPPr += L"<w:jc w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Spacing:
            {
                sPPr += L"<w:spacing " + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_ContextualSpacing:
            {
                sPPr += L"<w:contextualSpacing/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Ind:
            {
                sPPr += L"<w:ind " + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_OutlineLvl:
            {
                sPPr += L"<w:outlineLvl w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_Shd:
            {
                sPPr += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + oItem.second + L"\"/>";
                break;
            }

            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_TopBorder:
            {
                sPBdr += L"<w:top" + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_LeftBorder:
            {
                sPBdr += L"<w:left" + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_BottomBorder:
            {
                sPBdr += L"<w:bottom" + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_RightBorder:
            {
                sPBdr += L"<w:right" + oItem.second + L"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_KeepLines:
            {
                sPPr += L"<w:keepNext/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::ParagraphProperties::P_KeepNext:
            {
                sPPr += L"<w:keepLines/>";
                break;
            }
            default:
                break;
        }
    }

    if (!sPBdr.empty())
        sPPr += L"<w:pBdr>" + sPBdr + L"</w:pBdr>";

    return L"<w:pPr>" + sPPr + L"</w:pPr>";
}

std::wstring CXmlElement::ConvertRStyle() const
{
    if (m_mRStyleValues.empty())
        return std::wstring();

    std::wstring sRStyle;

    for (const std::map<NSCSS::NSConstValues::NSProperties::RunnerProperties, std::wstring>::value_type& oItem : m_mRStyleValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_RFonts:
            {
                sRStyle += (oItem.second != L"\"inherit\"") ? L"<w:rFonts w:ascii=" + oItem.second +
                                                          L" w:hAnsi=" + oItem.second +
                                                          L" w:cs=" + oItem.second +
                                                          L" w:eastAsia=" + oItem.second + L"/>"
                                                        : L"<w:rFonts w:ascii=\"Arial\" w:hAnsi=\"Arial\" w:cs=\"Arial\" w:eastAsia=\"Arial\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Sz:
            {
                sRStyle += L"<w:sz w:val=\"" + oItem.second +
                           L"\"/>" + L"<w:szCs w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_B:
            {
                if (oItem.second == L"bold")
                    sRStyle += L"<w:b/><w:bCs/>";
                else if (oItem.second == L"normal")
                    sRStyle += L"<w:b w:val=\"false\"/><w:bCs w:val=\"false\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_I:
            {
                if (oItem.second == L"italic")
                    sRStyle += L"<w:i/><w:iCs/>";
                else if (oItem.second == L"normal")
                    sRStyle += L"<w:i w:val=\"false\"/><w:iCs w:val=\"false\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Color:
            {
                sRStyle += L"<w:color w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_U:
            {
                if (oItem.second == L"line-through")
                    sRStyle += L"<w:strike/>";
                else
                    sRStyle += L"<w:u w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_Highlight:
            {
                sRStyle += L"<w:highlight w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::RunnerProperties::R_SmallCaps:
            {
                if (oItem.second == L"smallCaps")
                    sRStyle += L"<w:smallCaps/>";
                else if (oItem.second == L"normal")
                    sRStyle += L"<w:smallCaps w:val=\"false\"/>";
                break;
            }
            default:
                break;
        }
    }

    return L"<w:rPr>" + sRStyle + L"</w:rPr>";
}

std::wstring CXmlElement::ConvertBasicInfoStyle() const
{
    if (m_mBasicValues.empty())
        return std::wstring();

    std::wstring sBasicInfo;

    for (const std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::value_type& oItem : m_mBasicValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name:
            {
                sBasicInfo += L"<w:name w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn:
            {
                sBasicInfo += L"<w:basedOn w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_QFormat:
            {
                sBasicInfo += L"<w:qFormat/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Link:
            {
                sBasicInfo += L"<w:link w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_UnhideWhenUsed:
            {
                sBasicInfo += L"<w:unhideWhenUsed/>";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_UiPriority:
            {
                sBasicInfo += L"<w:uiPriority w:val=\"" + oItem.second + L"\"/>";
                break;
            }
            default:
                break;
        }
    }

    return sBasicInfo;
}

std::wstring CXmlElement::GetStyle() const
{
    if (Empty())
        return std::wstring();

    std::wstring sStyle = L"<w:style";

    for (const std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::value_type& oItem : m_mBasicValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle:
            {
                sStyle += L" w:customStyle=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId:
            {
                sStyle += L" w:styleId=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type:
            {
                sStyle += L" w:type=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Default:
            {
                sStyle += L" w:default=\"" + oItem.second + L"\"";
                break;
            }
            default:
                break;
        }
    }
    sStyle += L">";
    sStyle += ConvertBasicInfoStyle();
    sStyle += ConvertPStyle();
    sStyle += ConvertRStyle();
    sStyle += L"</w:style>";

    return sStyle.length() > 19 ? sStyle : std::wstring();
}

std::wstring CXmlElement::GetLitePStyle() const
{
    if (Empty())
        return std::wstring();

    const std::wstring &sLitePStyle = ConvertPStyle();

    return (sLitePStyle.empty()) ? sLitePStyle : sLitePStyle.substr(7, sLitePStyle.length() - 15);
}

std::wstring CXmlElement::GetLiteRStyle() const
{
    if (Empty())
        return std::wstring();

    const std::wstring &sLiteRStyle = ConvertRStyle();

    return (sLiteRStyle.empty()) ? sLiteRStyle : sLiteRStyle.substr(7, sLiteRStyle.length() - 15);
}

std::wstring CXmlElement::GetPStyle() const
{
    if (m_mBasicValues.empty() && m_mPStyleValues.empty())
        return std::wstring();

    std::wstring sPStyle = L"<w:style";

    for (const std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::value_type& oItem : m_mBasicValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle:
            {
                sPStyle += L" w:customStyle=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId:
            {
                sPStyle += L" w:styleId=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type:
            {
                sPStyle += L" w:type=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Default:
            {
                sPStyle += L" w:default=\"" + oItem.second + L"\"";
                break;
            }
            default:
                break;
        }
    }

    sPStyle += L">";
    sPStyle += ConvertBasicInfoStyle();
    sPStyle += ConvertPStyle();
    sPStyle += L"</w:style>";

    return sPStyle;
}

std::wstring CXmlElement::GetRStyle() const
{
    if (m_mBasicValues.empty() && m_mRStyleValues.empty())
        return std::wstring();

    std::wstring sRStyle = L"<w:style ";

    for (const std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::value_type& oItem : m_mBasicValues)
    {
        switch (oItem.first)
        {
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_CustomStyle:
            {
                sRStyle += L" w:customStyle=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId:
            {
                sRStyle += L" w:styleId=\"" + oItem.second + L"\"";
                break;
            }
            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Type:
            {
                sRStyle += L" w:type=\"" + oItem.second + L"\"";
                break;
            }

            case NSCSS::NSConstValues::NSProperties::BasicProperties::B_Default:
            {
                sRStyle += L" w:default=\"" + oItem.second + L"\"";
                break;
            }

            default:
                break;
        }
    }
    sRStyle += L">";
    sRStyle += ConvertBasicInfoStyle();
    sRStyle += ConvertRStyle();
    sRStyle += L"</w:style>";

    return sRStyle.length() > 22 ? sRStyle : std::wstring();
}

std::wstring CXmlElement::GetBasedOn() const
{
    std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::const_iterator posBasedOn = m_mBasicValues.find(NSCSS::NSConstValues::NSProperties::BasicProperties::B_BasedOn);
    return  (posBasedOn != m_mBasicValues.end()) ? posBasedOn->second : std::wstring();
}

std::wstring CXmlElement::GetStyleId() const
{
    std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::const_iterator posStyleId = m_mBasicValues.find(NSCSS::NSConstValues::NSProperties::BasicProperties::B_StyleId);
    return  (posStyleId != m_mBasicValues.end()) ? posStyleId->second : std::wstring();
}

std::wstring CXmlElement::GetName() const
{
    std::map<NSCSS::NSConstValues::NSProperties::BasicProperties, std::wstring>::const_iterator posName = m_mBasicValues.find(NSCSS::NSConstValues::NSProperties::BasicProperties::B_Name);
    return  (posName != m_mBasicValues.end()) ? posName->second : std::wstring();
}

