#include "CXmlElement.h"

#include <algorithm>
#include <vector>
#include <cwctype>

#include <iostream>

bool IsDigit(const std::wstring& sValue)
{
    if (sValue.empty())
        return false;

    return std::all_of(sValue.begin(), sValue.end(), [] (const wchar_t& cChar) { return iswdigit(cChar); });

    return true;
}

CXmlElement::CXmlElement() : m_bB(false), m_bI(false), m_bQFormat(false),
                             m_bSemiHidden(false), m_bUnhideWhenUsed(false), m_bKeepNext(false),
                             m_bContextualSpacing(false), m_bKeepLines(false)
{}

CXmlElement::CXmlElement(const std::wstring& sNameDefaultElement)
{
    CreateDefaultElement(sNameDefaultElement);
}

bool CXmlElement::Empty() const
{
    return m_sType.empty() && m_sStyleId.empty() && m_sDefault.empty() && m_sCustomStyle.empty() &&
           m_sS.empty() && m_sName.empty() && m_sBasedOn.empty() && m_sLink.empty() && m_sUiPriority.empty() &&
           !m_bQFormat && !m_bSemiHidden && !m_bUnhideWhenUsed && !m_bB && !m_bI && m_sU.empty() && m_sRFonts.empty() &&
           m_sColor.empty() && m_sSz.empty() && !m_bKeepLines && !m_bKeepNext && m_sSpacing.empty() &&
           m_sOutlineLvl.empty() && !m_bContextualSpacing && m_sInd.empty() && m_sJc.empty() &&
           m_sTblInd.empty() && m_sCellTop.empty() && m_sCellLeft.empty() && m_sCellBottom.empty() &&
           m_sCellRight.empty() && m_sBorderTop.empty() && m_sBorderLeft.empty() && m_sBorderBottom.empty() &&
           m_sBorderRight.empty() && m_sBorderInsideH.empty() && m_sBorderInsideV.empty() && m_sShd.empty() &&
           m_sTopBorder.empty() && m_sLeftBorder.empty() && m_sBottomBorder.empty() && m_sRightBorder.empty();
}

void CXmlElement::CreateDefaultElement(const std::wstring& sNameDefaultElement)
{
    if (!Empty())
        Clear();

    if (sNameDefaultElement == L"li")
    {
        SetType(L"paragraph");
        SetStyleId(L"li");
        SetName(L"List Paragraph");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetUiPriority(L"34");
        SetContextualSpacing(L"true");
        SetInd(L"w:left=\"720\"");
    }
    else if (sNameDefaultElement == L"h1")
    {
        SetType(L"paragraph");
        SetStyleId(L"h1");
        SetName(L"Heading 1");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"0");
    }
    else if (sNameDefaultElement == L"h2")
    {
        SetType(L"paragraph");
        SetStyleId(L"h2");
        SetName(L"Heading 2");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"1");
    }
    else if (sNameDefaultElement == L"h3")
    {
        SetType(L"paragraph");
        SetStyleId(L"h3");
        SetName(L"Heading 3");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"2");
    }
    else if (sNameDefaultElement == L"h4")
    {
        SetType(L"paragraph");
        SetStyleId(L"h4");
        SetName(L"Heading 4");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"3");
    }
    else if (sNameDefaultElement == L"h5")
    {
        SetType(L"paragraph");
        SetStyleId(L"h5");
        SetName(L"Heading 5");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"4");
    }
    else if (sNameDefaultElement == L"h6")
    {
        SetType(L"paragraph");
        SetStyleId(L"h6");
        SetName(L"Heading 6");
        SetBasedOn(L"normal");
        SetQFormat(true);
        SetOutlineLvl(L"5");
    }
    else if (sNameDefaultElement == L"h1-c")
    {
        SetType(L"character");
        SetStyleId(L"h1-c");
        SetCustomStyle(L"1");
        SetName(L"Title 1 Sign");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetB(true);
        SetColor(L"000000");
        SetSz(L"48");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"480\"");
    }
    else if (sNameDefaultElement == L"h2-c")
    {
        SetType(L"character");
        SetStyleId(L"h2-c");
        SetCustomStyle(L"1");
        SetName(L"Title 2 Sign");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetUnhideWhenUsed(true);
        SetB(true);
        SetColor(L"000000");
        SetSz(L"40");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
    }
    else if (sNameDefaultElement == L"h3-c")
    {
        SetType(L"character");
        SetStyleId(L"h3-c");
        SetCustomStyle(L"1");
        SetName(L"Title 3 Sign");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetUnhideWhenUsed(true);
        SetB(true);
//        SetI(true);
        SetColor(L"000000");
        SetSz(L"36");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
    }
    else if (sNameDefaultElement == L"h4-c")
    {
        SetType(L"character");
        SetStyleId(L"h4-c");
        SetCustomStyle(L"1");
        SetName(L"Title 4 Sign");
        SetLink(L"h4");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetB(true);
        SetUnhideWhenUsed(true);
        SetColor(L"232323");
        SetSz(L"32");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
    }
    else if (sNameDefaultElement == L"h5-c")
    {
        SetType(L"character");
        SetStyleId(L"h5-c");
        SetCustomStyle(L"1");
        SetName(L"Title 5 Sign");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetUnhideWhenUsed(true);
        SetB(true);
        SetColor(L"444444");
        SetSz(L"28");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
    }
    else if (sNameDefaultElement == L"h6-c")
    {
        SetType(L"character");
        SetStyleId(L"h6-c");
        SetCustomStyle(L"1");
        SetName(L"Title 6 Sign");
        SetUiPriority(L"9");;
        SetRFonts(L"\"Arial\"");
        SetUnhideWhenUsed(true);
        SetI(true);
        SetB(true);
        SetColor(L"232323");
        SetSz(L"28");
        SetKeepLines(true);
        SetKeepNext(true);
        SetSpacing(L"w:after=\"0\" w:before=\"200\"");
    }
    else if (sNameDefaultElement == L"p-c")
    {
        SetType(L"character");
        SetStyleId(L"p-c");
        SetCustomStyle(L"1");
        SetName(L"Paragraph_character");
        SetRFonts(L"\"Arial\"");
    }
    else if (sNameDefaultElement == L"p")
    {
        SetType(L"paragraph");
        SetStyleId(L"p");
        SetCustomStyle(L"1");
        SetName(L"Paragraph");
        SetBasedOn(L"normal");
        SetQFormat(true);
//        SetInd(L"w:firstLine=\"567\"");
//        SetJc(L"both");
    }
    else if (sNameDefaultElement == L"div-c")
    {
        SetType(L"character");
        SetStyleId(L"div-c");
        SetCustomStyle(L"1");
        SetName(L"Paragraph_character");
        SetLink(L"div");
        SetRFonts(L"\"Arial\"");
    }
    else if (sNameDefaultElement == L"div")
    {
        SetType(L"paragraph");
        SetStyleId(L"div");
        SetCustomStyle(L"1");
        SetName(L"Paragraph");
        SetBasedOn(L"normal");
        SetLink(L"div-c");
        SetQFormat(true);
//        SetInd(L"w:firstLine=\"567\"");
//        SetJc(L"both");
    }
    else if (sNameDefaultElement == L"a-c")
    {
        SetType(L"character");
        SetStyleId(L"a-character");
        SetName(L"Hyperlink");
        SetUiPriority(L"99");
        SetUnhideWhenUsed(true);
        SetColor(L"0000FF");
        SetU(L"single");
        SetSz(L"22");
        SetQFormat(true);
        SetRFonts(L"\"Arial\"");
    }
    else if (sNameDefaultElement == L"a")
    {
        SetType(L"character");
        SetBasedOn(L"a-c");
        SetStyleId(L"a");
        SetName(L"Hyperlink");
        SetQFormat(true);
    }
}

void CXmlElement::Clear()
{
    m_sType.clear();
    m_sStyleId.clear();
    m_sDefault.clear();
    m_sCustomStyle.clear();
    m_sS.clear();

    m_sName.clear();
    m_sBasedOn.clear();
    m_sLink.clear();
    m_sUiPriority.clear();
    m_bQFormat = false;
    m_bSemiHidden = false;
    m_bUnhideWhenUsed = false;

    // <rPr>
        m_bB = false;
        m_bI = false;
        m_sU.clear();
        m_sRFonts.clear();
        m_sColor.clear();
        m_sSz.clear();
    // </rPr>

    // <pPr>
        m_bKeepLines = false;
        m_bKeepNext = false;
        m_sSpacing.clear();
        m_bContextualSpacing = false;
        m_sOutlineLvl.clear();
        m_sInd.clear();
        m_sJc.clear();
        m_sShd.clear();

        // <pBdr>
            m_sTopBorder.clear();
            m_sLeftBorder.clear();
            m_sBottomBorder.clear();
            m_sRightBorder.clear();
        // </pBdr>

    // </pPr>

    // <tblPr>
        m_sTblInd.clear();
        // <tblCellMar>
            m_sCellTop.clear();
            m_sCellLeft.clear();
            m_sCellBottom.clear();
            m_sCellRight.clear();
        // <tblCellMar>

        // <tblBorders>
            m_sBorderTop.clear();
            m_sBorderLeft.clear();
            m_sBorderBottom.clear();
            m_sBorderRight.clear();
            m_sBorderInsideH.clear();
            m_sBorderInsideV.clear();
        // </tblBorders>
    // </tblPr>
}

void CXmlElement::SetType(const std::wstring& sType)
{
    m_sType = sType;
}

void CXmlElement::SetStyleId(const std::wstring& sStyleId)
{
    m_sStyleId = sStyleId;
}

void CXmlElement::SetDefault(const std::wstring& sDefault)
{
    m_sDefault = sDefault;
}

void CXmlElement::SetCustomStyle(const std::wstring& sCustomStyle)
{
    m_sCustomStyle = sCustomStyle;
}

void CXmlElement::SetS(const std::wstring& sS)
{
    m_sS = sS;
}

void CXmlElement::SetName(const std::wstring& sName)
{
    m_sName = sName;
}

void CXmlElement::SetBasedOn(const std::wstring& sBasedOn)
{
    m_sBasedOn = sBasedOn;
}

void CXmlElement::SetLink(const std::wstring& sLink)
{
    m_sLink = sLink;
}

void CXmlElement::SetUiPriority(const std::wstring& sUiPriority)
{
    m_sUiPriority = sUiPriority;
}

void CXmlElement::SetQFormat(bool bQFormat)
{
    m_bQFormat = bQFormat;
}

void CXmlElement::SetUnhideWhenUsed(bool bUnhideWhenUsed)
{
    m_bUnhideWhenUsed = bUnhideWhenUsed;
}

void CXmlElement::SetB(bool bB)
{
    m_bB = bB;
}

void CXmlElement::SetI(bool bI)
{
    m_bI = bI;
}

void CXmlElement::SetU(const std::wstring& sU)
{
    m_sU = sU;
}

void CXmlElement::SetRFonts(const std::wstring& sRFonts)
{
    m_sRFonts = sRFonts;
}

void CXmlElement::SetColor(const std::wstring& sColor)
{
    m_sColor = sColor;
}

void CXmlElement::SetSz(const std::wstring& sSz)
{
    if (IsDigit(sSz))
        m_sSz = sSz;
}

void CXmlElement::SetKeepLines(bool bKeepLines)
{
    m_bKeepLines = bKeepLines;
}

void CXmlElement::SetKeepNext(bool bKeepNext)
{
    m_bKeepNext = bKeepNext;
}

void CXmlElement::SetSpacing(const std::wstring& sSpacing)
{
    m_sSpacing = sSpacing;
}

void CXmlElement::SetOutlineLvl(const std::wstring& sOutlineLvl)
{
    m_sOutlineLvl = sOutlineLvl;
}

void CXmlElement::SetContextualSpacing(bool bContextualSpacing)
{
    m_bContextualSpacing = bContextualSpacing;
}

void CXmlElement::SetInd(const std::wstring& sInd)
{
    m_sInd = sInd;
}

void CXmlElement::SetJc(const std::wstring& sJc)
{
    std::wstring sNewJc = sJc;
    std::transform(sNewJc.begin(), sNewJc.end(), sNewJc.begin(), towlower);

    if (sNewJc == L"justify")
        m_sJc = L"both";
    else if (sNewJc == L"middle")
        m_sJc = L"center";
    else if (sNewJc == L"left" || sNewJc == L"center" || sNewJc == L"right" || sNewJc == L"both")
        m_sJc = sNewJc;
}

void CXmlElement::SetShd(const std::wstring& sShd)
{
    m_sShd = sShd;
}

void CXmlElement::SetTopBorder(const std::wstring& sTopBorder)
{
    m_sTopBorder = sTopBorder;
}

void CXmlElement::SetLeftBorder(const std::wstring& sLeftBorder)
{
    m_sLeftBorder = sLeftBorder;
}

void CXmlElement::SetBottomBorder(const std::wstring& sBottomBorder)
{
    m_sBottomBorder = sBottomBorder;
}

void CXmlElement::SetRightBorder(const std::wstring& sRightBorder)
{
    m_sRightBorder = sRightBorder;
}

CXmlElement& CXmlElement::operator+=(const CXmlElement& oElement)
{
    if (oElement.Empty())
        return *this;

    if (!oElement.m_sType.empty())
        m_sType = oElement.m_sType;

    if (!oElement.m_sStyleId.empty())
        m_sStyleId = oElement.m_sStyleId;

    if (!oElement.m_sDefault.empty())
        m_sDefault = oElement.m_sDefault;

    if (!oElement.m_sCustomStyle.empty())
        m_sCustomStyle = oElement.m_sCustomStyle;

    if (!oElement.m_sS.empty())
        m_sS = oElement.m_sS;

    if (!oElement.m_sName.empty())
        m_sName = oElement.m_sName;

    if (!oElement.m_sBasedOn.empty())
        m_sBasedOn = oElement.m_sBasedOn;

    if (!oElement.m_sLink.empty())
        m_sLink = oElement.m_sLink;

    if (!oElement.m_sUiPriority.empty())
        m_sUiPriority = oElement.m_sUiPriority;

    m_bQFormat = oElement.m_bQFormat;
    m_bUnhideWhenUsed = oElement.m_bUnhideWhenUsed;
    m_bB = oElement.m_bB;
    m_bI = oElement.m_bI;

    if (!oElement.m_sRFonts.empty())
        m_sRFonts = oElement.m_sRFonts;

    if (!oElement.m_sColor.empty())
        m_sColor = oElement.m_sColor;

    if (!oElement.m_sU.empty())
        m_sU = oElement.m_sU;

    if (!oElement.m_sSz.empty())
        m_sSz = oElement.m_sSz;

    m_bKeepLines = oElement.m_bKeepLines;
    m_bKeepNext = oElement.m_bKeepNext;

    if (!oElement.m_sSpacing.empty())
        m_sSpacing = oElement.m_sSpacing;

    if (!oElement.m_sOutlineLvl.empty())
        m_sOutlineLvl = oElement.m_sOutlineLvl;

    m_bContextualSpacing = oElement.m_bContextualSpacing;

    if (!oElement.m_sInd.empty())
        m_sInd = oElement.m_sInd;

    if (!oElement.m_sJc.empty())
        m_sJc = oElement.m_sJc;

    if (!oElement.m_sShd.empty())
        m_sShd = oElement.m_sShd;

    if (!oElement.m_sTblInd.empty())
        m_sTblInd = oElement.m_sTblInd;

    if (!oElement.m_sCellTop.empty())
        m_sCellTop = oElement.m_sCellTop;

    if (!oElement.m_sCellLeft.empty())
        m_sCellLeft = oElement.m_sCellLeft;

    if (!oElement.m_sCellBottom.empty())
        m_sCellBottom = oElement.m_sCellBottom;

    if (!oElement.m_sCellRight.empty())
        m_sCellRight = oElement.m_sCellRight;

    if (!oElement.m_sBorderTop.empty())
        m_sBorderTop = oElement.m_sBorderTop;

    if (!oElement.m_sBorderLeft.empty())
        m_sBorderLeft = oElement.m_sBorderLeft;

    if (!oElement.m_sBorderBottom.empty())
        m_sBorderBottom = oElement.m_sBorderBottom;

    if (!oElement.m_sBorderRight.empty())
        m_sBorderRight = oElement.m_sBorderRight;

    if (!oElement.m_sBorderInsideH.empty())
        m_sBorderInsideH = oElement.m_sBorderInsideH;

    if (!oElement.m_sBorderInsideV.empty())
        m_sBorderInsideV = oElement.m_sBorderInsideV;

    if (!oElement.m_sTopBorder.empty())
        m_sTopBorder = oElement.m_sTopBorder;

    if (!oElement.m_sLeftBorder.empty())
        m_sLeftBorder = oElement.m_sLeftBorder;

    if (!oElement.m_sBottomBorder.empty())
        m_sBottomBorder = oElement.m_sBottomBorder;

    if (!oElement.m_sRightBorder.empty())
        m_sRightBorder = oElement.m_sRightBorder;

    return *this;
}

CXmlElement& CXmlElement::operator=(const CXmlElement& oElement)
{
//    Clear();
    m_sType               = oElement.m_sType;
    m_sStyleId            = oElement.m_sStyleId;
    m_sDefault            = oElement.m_sDefault;
    m_sCustomStyle        = oElement.m_sCustomStyle;
    m_sS                  = oElement.m_sS;
    m_sName               = oElement.m_sName;
    m_sBasedOn            = oElement.m_sBasedOn;
    m_sLink               = oElement.m_sLink;
    m_sUiPriority         = oElement.m_sUiPriority;
    m_bQFormat            = oElement.m_bQFormat;
    m_bUnhideWhenUsed     = oElement.m_bUnhideWhenUsed;
    m_bB                  = oElement.m_bB;
    m_bI                  = oElement.m_bI;
    m_sRFonts             = oElement.m_sRFonts;
    m_sColor              = oElement.m_sColor;
    m_sU                  = oElement.m_sU;
    m_sSz                 = oElement.m_sSz;
    m_bKeepLines          = oElement.m_bKeepLines;
    m_bKeepNext           = oElement.m_bKeepNext;
    m_sSpacing            = oElement.m_sSpacing;
    m_sOutlineLvl         = oElement.m_sOutlineLvl;
    m_bContextualSpacing  = oElement.m_bContextualSpacing;
    m_sInd                = oElement.m_sInd;
    m_sJc                 = oElement.m_sJc;
    m_sShd                = oElement.m_sShd;
    m_sTblInd             = oElement.m_sTblInd;
    m_sCellTop            = oElement.m_sCellTop;
    m_sCellLeft           = oElement.m_sCellLeft;
    m_sCellBottom         = oElement.m_sCellBottom;
    m_sCellRight          = oElement.m_sCellRight;
    m_sBorderTop          = oElement.m_sBorderTop;
    m_sBorderLeft         = oElement.m_sBorderLeft;
    m_sBorderBottom       = oElement.m_sBorderBottom;
    m_sBorderRight        = oElement.m_sBorderRight;
    m_sBorderInsideH      = oElement.m_sBorderInsideH;
    m_sBorderInsideV      = oElement.m_sBorderInsideV;
    m_sTopBorder          = oElement.m_sTopBorder;
    m_sLeftBorder         = oElement.m_sLeftBorder;
    m_sBottomBorder       = oElement.m_sBottomBorder;
    m_sRightBorder        = oElement.m_sRightBorder;

    return *this;
}

bool CXmlElement::operator==(const CXmlElement &oElement)
{
    if (oElement.Empty() && Empty())
        return true;

    if (oElement.Empty() || Empty())
        return false;

        return  m_sType              == oElement.m_sType               &&
                m_sStyleId           == oElement.m_sStyleId            &&
                m_sDefault           == oElement.m_sDefault            &&
                m_sCustomStyle       == oElement.m_sCustomStyle        &&
                m_sS                 == oElement.m_sS                  &&
                m_sName              == oElement.m_sName               &&
                m_sBasedOn           == oElement.m_sBasedOn            &&
                m_sLink              == oElement.m_sLink               &&
                m_sUiPriority        == oElement.m_sUiPriority         &&
                m_bQFormat           == oElement.m_bQFormat            &&
                m_bUnhideWhenUsed    == oElement.m_bUnhideWhenUsed     &&
                m_bB                 == oElement.m_bB                  &&
                m_bI                 == oElement.m_bI                  &&
                m_sRFonts            == oElement.m_sRFonts             &&
                m_sColor             == oElement.m_sColor              &&
                m_sU                 == oElement.m_sU                  &&
                m_sSz                == oElement.m_sSz                 &&
                m_bKeepLines         == oElement.m_bKeepLines          &&
                m_bKeepNext          == oElement.m_bKeepNext           &&
                m_sSpacing           == oElement.m_sSpacing            &&
                m_sOutlineLvl        == oElement.m_sOutlineLvl         &&
                m_bContextualSpacing == oElement.m_bContextualSpacing  &&
                m_sInd               == oElement.m_sInd                &&
                m_sJc                == oElement.m_sJc                 &&
                m_sShd               == oElement.m_sShd                &&
                m_sTblInd            == oElement.m_sTblInd             &&
                m_sCellTop           == oElement.m_sCellTop            &&
                m_sCellLeft          == oElement.m_sCellLeft           &&
                m_sCellBottom        == oElement.m_sCellBottom         &&
                m_sCellRight         == oElement.m_sCellRight          &&
                m_sBorderTop         == oElement.m_sBorderTop          &&
                m_sBorderLeft        == oElement.m_sBorderLeft         &&
                m_sBorderBottom      == oElement.m_sBorderBottom       &&
                m_sBorderRight       == oElement.m_sBorderRight        &&
                m_sBorderInsideH     == oElement.m_sBorderInsideH      &&
                m_sBorderInsideV     == oElement.m_sBorderInsideV      &&
                m_sTopBorder         == oElement.m_sTopBorder          &&
                m_sLeftBorder        == oElement.m_sLeftBorder         &&
                m_sBottomBorder      == oElement.m_sBottomBorder       &&
                m_sRightBorder       == oElement.m_sRightBorder;
}

std::wstring CXmlElement::ConvertPStyle() const
{
    std::wstring sPStyle;

    std::wstring sPPr = L"<w:pPr>";

        if (m_bKeepLines)
            sPPr += L"<w:keepLines/>";

        if (m_bKeepNext)
            sPPr += L"<w:keepNext/>";

        if (!m_sSpacing.empty())
            sPPr += L"<w:spacing " + m_sSpacing + L"/>";

        if (!m_sOutlineLvl.empty())
            sPPr += L"<w:outlineLvl w:val=\"" + m_sOutlineLvl + L"\"/>";

        if (m_bContextualSpacing)
            sPPr += L"<w:contextualSpacing/>";

        if (!m_sInd.empty())
            sPPr += L"<w:ind " + m_sInd + L"/>";

        if (!m_sJc.empty())
            sPPr += L"<w:jc w:val=\"" + m_sJc + L"\"/>";

        if (!m_sShd.empty())
            sPPr += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + m_sShd + L"\"/>";

        std::wstring sPBdr = L"<w:pBdr>";

            if (!m_sTopBorder.empty())
                sPBdr += L"<w:top" + m_sTopBorder + L"/>";

            if (!m_sLeftBorder.empty())
                sPBdr += L"<w:left" + m_sLeftBorder + L"/>";

            if (!m_sBottomBorder.empty())
                sPBdr += L"<w:bottom" + m_sBottomBorder + L"/>";

            if (!m_sRightBorder.empty())
                sPBdr += L"<w:right" + m_sRightBorder + L"/>";

        sPBdr += L"</w:pBdr>";

        if (sPBdr.length() > 17)
            sPPr += sPBdr;

    sPPr += L"</w:pPr>";

    if (sPPr.length() > 15)
        sPStyle += sPPr;

    return sPStyle;
}

std::wstring CXmlElement::ConvertRStyle() const
{
    std::wstring sRStyle;

    std::wstring sRPr = L"<w:rPr>";

        if (m_bB)
            sRPr += L"<w:b/><w:bCs/>";

        if (m_bI)
            sRPr += L"<w:i/><w:iCs/>";

        if (!m_sRFonts.empty())
            sRPr += L"<w:rFonts w:ascii=" + m_sRFonts + L" w:hAnsi=" + m_sRFonts + L" w:cs=" +
                      m_sRFonts + L" w:eastAsia=" + m_sRFonts + L"/>";

        if (!m_sColor.empty())
            sRPr += L"<w:color w:val=\"" + m_sColor + L"\"/>";

        if (!m_sShd.empty())
            sRPr += L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" + m_sShd + L"\"/>";

        if (!m_sU.empty())
            sRPr += L"<w:u w:val=\"" + m_sU + L"\"/>";

        if (!m_sSz.empty())
            sRPr += L"<w:sz w:val=\"" + m_sSz + L"\"/>" + L"<w:szCs w:val=\"" + m_sSz + L"\"/>";

    sRPr += L"</w:rPr>";

    if (sRPr.length() > 15)
        sRStyle += sRPr;

    return sRStyle;
}

std::wstring CXmlElement::ConvertBasicInfoStyle() const
{
    std::wstring sBasicInfo;

    if (!m_sName.empty())
        sBasicInfo += L"<w:name w:val=\"" + m_sName + L"\"/>";

    if (!m_sBasedOn.empty())
        sBasicInfo += L"<w:basedOn w:val=\"" + m_sBasedOn + L"\"/>";

    if (!m_sLink.empty())
        sBasicInfo += L"<w:link w:val=\"" + m_sLink + L"\"/>";

    if (!m_sUiPriority.empty())
        sBasicInfo += L"<w:uiPriority w:val=\"" + m_sUiPriority + L"\"/>";

    if (m_bQFormat)
        sBasicInfo += L"<w:qFormat/>";

    if (m_bUnhideWhenUsed)
        sBasicInfo += L"<w:unhideWhenUsed/>";

    return sBasicInfo;
}

std::wstring CXmlElement::GetStyle() const
{
    std::wstring sStyle = L"<w:style";

    if (!m_sType.empty())
        sStyle += L" w:type=\"" + m_sType + L"\"";

    if (!m_sStyleId.empty())
        sStyle += L" w:styleId=\"" + m_sStyleId + L"\"";

    if (!m_sDefault.empty())
        sStyle += L" w:default=\"" + m_sDefault + L"\"";

    if (!m_sCustomStyle.empty())
        sStyle += L" w:customStyle=\"" + m_sCustomStyle + L"\"";

    if (!m_sS.empty())
        sStyle += L" w:S=\"" + m_sS + L"\"";

    sStyle += L">";

    sStyle += ConvertBasicInfoStyle();
    sStyle += ConvertPStyle();
    sStyle += ConvertRStyle();

    sStyle += L"</w:style>";
    if (sStyle.length() > 19)
        return sStyle;

    return std::wstring();
}

std::wstring CXmlElement::GetPStyle() const
{
    std::wstring sPStyle = L"<w:style";

    if (!m_sType.empty())
        sPStyle += L" w:type=\"" + m_sType + L"\"";

    if (!m_sStyleId.empty())
        sPStyle += L" w:styleId=\"" + m_sStyleId + L"\"";

    if (!m_sDefault.empty())
        sPStyle += L" w:default=\"" + m_sDefault + L"\"";

    if (!m_sCustomStyle.empty())
        sPStyle += L" w:customStyle=\"" + m_sCustomStyle + L"\"";

    if (!m_sS.empty())
        sPStyle += L" w:S=\"" + m_sS + L"\"";

    sPStyle += L">";

    sPStyle += ConvertBasicInfoStyle();
    sPStyle += ConvertPStyle();

    sPStyle += L"</w:style>";

    if (sPStyle.length() > 19)
        return sPStyle;

    return std::wstring();
}

std::wstring CXmlElement::GetRStyle() const
{
    std::wstring sRStyle = L"<w:style ";

    if (!m_sType.empty())
        sRStyle += L" w:type=\"" + m_sType + L"\"";

    if (!m_sStyleId.empty())
        sRStyle += L" w:styleId=\"" + m_sStyleId + L"\"";

    if (!m_sDefault.empty())
        sRStyle += L" w:default=\"" + m_sDefault + L"\"";

    if (!m_sCustomStyle.empty())
        sRStyle += L" w:customStyle=\"" + m_sCustomStyle + L"\"";

    if (!m_sS.empty())
        sRStyle += L" w:S=\"" + m_sS + L"\"";

    sRStyle += L">";

    sRStyle += ConvertBasicInfoStyle();
    sRStyle += ConvertRStyle();

    sRStyle += L"</w:style>";

    if (sRStyle.length() > 22)
        return sRStyle;

    return std::wstring();
}

std::wstring CXmlElement::GetBasedOn() const
{
    return m_sBasedOn;
}

std::wstring CXmlElement::GetStyleId() const
{
    return m_sStyleId;
}

std::wstring CXmlElement::GetName() const
{
    return m_sName;
}

