#ifndef CCOMPILEDSTYLE_H
#define CCOMPILEDSTYLE_H

#include "CssCalculator_global.h"

#include <map>
#include <vector>
#include <string>

namespace NSCSS
{
    class CSSCALCULATOR_EXPORT CCompiledStyle
    {
        std::wstring m_sId;
        std::vector<std::wstring> m_arParentsStyles;
        std::map<std::wstring, std::wstring> m_mStyle;

    public:
        CCompiledStyle();
        explicit CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle);
        CCompiledStyle(const CCompiledStyle& oStyle);

        ~CCompiledStyle();

         std::map<std::wstring, std::wstring> GetStyleMap() const;
         std::wstring GetStyleW() const;
        // std::string GetStyle() const;

        // size_t GetSize() const;
        bool Empty() const;
        // void Clear();

        void AddPropSel(const std::wstring& sProperty, const std::wstring& sValue);
        // void InsertStyle(const std::map<std::wstring, std::wstring>& mStyle);
        // void SetStyle(const std::map<std::wstring, std::wstring>& mStyle);
        void AddStyle(const std::wstring& sStyle);
        void AddParent(const std::wstring& sParentName);

        std::vector<std::wstring> GetParentsName() const;

        void SetID(const std::wstring& sId);
        std::wstring GetId() const;

        // const std::map<std::wstring, std::wstring>::iterator& GetBegin();
        // const std::map<std::wstring, std::wstring>::iterator& GetEnd();

        // double GetWeidth() const;

        /* FONT */
        std::wstring GetFont() const;
        std::wstring GetFontFamily() const;
        std::vector<std::wstring> GetFontNames() const;
        std::vector<std::wstring> GetFontNames2(const std::wstring& sFontFamily) const;
        std::wstring GetFontSize() const;
        std::wstring GetFontSizeAdjust() const;
        std::wstring GetFontStretch() const;
        std::wstring GetFontStyle() const;
        std::wstring GetFontVariant() const;
        std::wstring GetFontWeight() const;

        std::wstring GetLineHeight() const;

        /* MARGIN */
        std::wstring GetMargin() const;
        std::wstring GetMarginBottom() const;
        std::wstring GetMarginBlockEnd() const;
        std::wstring GetMarginLeft() const;
        std::wstring GetMarginRight() const;
        std::wstring GetMarginTop() const;
        std::wstring GetMarginBlockStart() const;

        std::map<unsigned short int, std::wstring> GetMargins() const;
        std::wstring GetMarginTop2   () const;
        std::wstring GetMarginBottom2() const;
        std::wstring GetMarginLeft2  () const;
        std::wstring GetMarginRight2 () const;

        /* PADDING */
        std::wstring GetPadding() const;
        std::wstring GetPaddingBottom() const;
        std::wstring GetPaddingLeft() const;
        std::wstring GetPaddingRight() const;
        std::wstring GetPaddingTop() const;

        /*  SPACING */
        std::wstring GetLetterSpacing() const;
        std::wstring GetWordSpacing() const;
        std::wstring GetBorderSpacing() const;

        /* COLOR */
        std::wstring GetTextDecorationColor() const;
        std::wstring GetBackgroundColor() const;
        std::wstring GetColor() const;
        std::wstring GetOutlineColor() const;
        std::wstring GetColumnRuleColor() const;
        std::wstring GetBackground() const;

        /* TEXT */
        std::wstring GetTextAlign() const;
        std::wstring GetTextIndent() const;
        std::wstring GetTextDecoration() const;

        /* BORDER */
        std::wstring GetBorder() const;
        std::wstring GetBorderWidth() const;
        std::wstring GetBorderStyle() const;
        std::wstring GetBorderColor() const;

        std::wstring GetBorderBottom() const;
        std::wstring GetBorderBottomWidth() const;
        std::wstring GetBorderBottomStyle() const;
        std::wstring GetBorderBottomColor() const;

        std::wstring GetBorderLeft() const;
        std::wstring GetBorderLeftWidth() const;
        std::wstring GetBorderLeftStyle() const;
        std::wstring GetBorderLeftColor() const;

        std::wstring GetBorderRight() const;
        std::wstring GetBorderRightWidth() const;
        std::wstring GetBorderRightStyle() const;
        std::wstring GetBorderRightColor() const;

        std::wstring GetBorderTop() const;
        std::wstring GetBorderTopWidth() const;
        std::wstring GetBorderTopStyle() const;
        std::wstring GetBorderTopColor() const;


        CCompiledStyle& operator += (const CCompiledStyle& oElement);
        CCompiledStyle& operator = (const CCompiledStyle& oElement);

        bool operator == (const CCompiledStyle& oStyle)   const;
        // bool operator != (const CCompiledStyle& oElement) const;
        // bool operator >  (const CCompiledStyle& oElement) const;
        // bool operator <  (const CCompiledStyle& oElement) const;

    };
}

#endif // CCOMPILEDSTYLE_H
