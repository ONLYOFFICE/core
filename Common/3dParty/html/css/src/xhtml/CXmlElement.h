#ifndef CXMLELEMENT_H
#define CXMLELEMENT_H

#include <string>

class CXmlElement
{
    bool m_bQFormat;
    bool m_bSemiHidden;
    bool m_bUnhideWhenUsed;
        bool m_bB;                  // <rPr>
        bool m_bI;                  // <rPr>
        bool m_bKeepLines;          // <pPr>
        bool m_bKeepNext;           // <pPr>
        bool m_bContextualSpacing;  // <pPr>
    std::wstring m_sType;
    std::wstring m_sStyleId;
    std::wstring m_sDefault;
    std::wstring m_sCustomStyle;
    std::wstring m_sS;

    std::wstring m_sName;
    std::wstring m_sBasedOn;
    std::wstring m_sLink;
    std::wstring m_sUiPriority;

    // <rPr>
        std::wstring m_sU;
        std::wstring m_sRFonts;
        std::wstring m_sColor;
        std::wstring m_sSz;
    // </rPr>

    // <pPr>
        std::wstring m_sSpacing;
        std::wstring m_sOutlineLvl;
        std::wstring m_sInd;
        std::wstring m_sJc;
        std::wstring m_sShd;

        // <pBdr>
            std::wstring m_sTopBorder;
            std::wstring m_sLeftBorder;
            std::wstring m_sBottomBorder;
            std::wstring m_sRightBorder;
        // </pBdr>

    // </pPr>

    // <tblPr>
        std::wstring m_sTblInd;
        // <tblCellMar>
            std::wstring m_sCellTop;
            std::wstring m_sCellLeft;
            std::wstring m_sCellBottom;
            std::wstring m_sCellRight;
        // <tblCellMar>

        // <tblBorders>
            std::wstring m_sBorderTop;
            std::wstring m_sBorderLeft;
            std::wstring m_sBorderBottom;
            std::wstring m_sBorderRight;
            std::wstring m_sBorderInsideH;
            std::wstring m_sBorderInsideV;
        // </tblBorders>
    // </tblPr>

    std::wstring ConvertPStyle() const;
    std::wstring ConvertRStyle() const;
    std::wstring ConvertBasicInfoStyle() const;

public:
    CXmlElement();
    CXmlElement(const std::wstring& sNameDefaultElement);

    bool Empty() const;

    void CreateDefaultElement(const std::wstring& sNameDefaultElement);
    void Clear();

    void SetType(const std::wstring& sType);
    void SetStyleId(const std::wstring& sStyleId);
    void SetDefault(const std::wstring& sDefault);
    void SetCustomStyle(const std::wstring& sCustomStyle);
    void SetS(const std::wstring& sS);

    void SetName(const std::wstring& sName);
    void SetBasedOn(const std::wstring& sBasedOn);
    void SetLink(const std::wstring& sLink);
    void SetUiPriority(const std::wstring& sUiPriority);
    void SetQFormat(bool bQFormat);
    void SetUnhideWhenUsed(bool bUnhideWhenUsed);

    // <rPr>
        void SetB(bool bB);
        void SetI(bool bI);
        void SetU(const std::wstring& sU);
        void SetRFonts(const std::wstring& sRFonts);
        void SetColor(const std::wstring& sColor);
        void SetSz(const std::wstring& sSz);
    // </rPr>

    // <pPr>
        void SetKeepLines(bool bKeepLines);
        void SetKeepNext(bool bKeepNext);
        void SetContextualSpacing(bool bContextualSpacing);
        void SetSpacing(const std::wstring& sSpacing);
        void SetOutlineLvl(const std::wstring& sOutlineLvl);
        void SetInd(const std::wstring& sInd);
        void SetJc(const std::wstring& sJc);
        void SetShd(const std::wstring& sShd);

        // <pBdr>
            void SetTopBorder(const std::wstring& sTopBorder);
            void SetLeftBorder(const std::wstring& sLeftBorder);
            void SetBottomBorder(const std::wstring& sBottomBorder);
            void SetRightBorder(const std::wstring& sRightBorder);
        // </pBdr>
    // </pPr>

    // <tblPr>
        void SetTblInd(const std::wstring& sTblInd);
        // <tblCellMar>
            void SetCellTop(const std::wstring& sCellTop);
            void SetCellLeft(const std::wstring& sCellLeft);
            void SetCellBottom(const std::wstring& sCellBottom);
            void SetCellRight(const std::wstring& sCellRight);
        // <tblCellMar>

        // <tblBorders>
            void SetBorderTop(const std::wstring& sBorderTop);
            void SetBorderLeft(const std::wstring& sBorderLeft);
            void SetBorderBottom(const std::wstring& sBorderBottom);
            void SetBorderRight(const std::wstring& sBorderRight);
            void SetBorderInsideH(const std::wstring& sBorderInsideH);
            void SetBorderInsideV(const std::wstring& sBorderInsideV);
        // </tblBorders>
    // </tblPr>

    std::wstring GetPStyle() const;
    std::wstring GetRStyle() const;
    std::wstring GetStyle() const;

    std::wstring GetBasedOn() const;
    std::wstring GetStyleId() const;
    std::wstring GetName() const;

    CXmlElement& operator+=(const CXmlElement& oElement);
    CXmlElement& operator= (const CXmlElement& oelement);
    bool operator== (const CXmlElement& oElement);
};

#endif // CXMLELEMENT_H
