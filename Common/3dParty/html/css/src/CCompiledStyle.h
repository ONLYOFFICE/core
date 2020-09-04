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
        std::map<std::wstring, std::wstring> m_mStyle;
        std::wstring m_sId;

        std::vector<std::wstring> m_arParentsStyles;

    public:
        CCompiledStyle();
        CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle);
        CCompiledStyle(const CCompiledStyle& oStyle);

        ~CCompiledStyle();

        std::map<std::wstring, std::wstring> GetStyleMap() const;
        std::wstring GetStyleW() const;
        std::string GetStyle() const;

        const size_t GetSize() const;
        bool Empty() const;
        void Clear();

        void AddPropSel(const std::wstring& sProperty, const std::wstring& sValue);
        void InsertStyle(const std::map<std::wstring, std::wstring>& mStyle);
        void SetStyle(const std::map<std::wstring, std::wstring>& mStyle);
        void AddStyle(const std::wstring& sStyle);
        void AddParent(const std::wstring& sParentName);

        std::vector<std::wstring> GetParentsName() const;

        void SetID(const std::wstring& sId);
        const std::wstring& GetId() const;

        const std::map<std::wstring, std::wstring>::iterator& GetBegin();
        const std::map<std::wstring, std::wstring>::iterator& GetEnd();

        double GetWeidth() const;

        /* FONT */
        std::wstring GetFont();
        std::wstring GetFontFamily();
        std::vector<std::wstring> GetFontNames();
        std::wstring GetFontSize();
        std::wstring GetFontSizeAdjust();
        std::wstring GetFontStretch();
        std::wstring GetFontStyle();
        std::wstring GetFontVariant();
        std::wstring GetFontWeight();

        std::wstring GetLineHeight();

        /* MARGIN */
        std::wstring GetMargin();
        std::wstring GetMarginBottom();
        std::wstring GetMarginBlockEnd();
        std::wstring GetMarginLeft();
        std::wstring GetMarginRight();
        std::wstring GetMarginTop();
        std::wstring GetMarginBlockStart();

        /* PADDING */
        std::wstring GetPadding();
        std::wstring GetPaddingBottom();
        std::wstring GetPaddingLeft();
        std::wstring GetPaddingRight();
        std::wstring GetPaddingTop();

        /*  SPACING */
        std::wstring GetLetterSpacing();
        std::wstring GetWordSpacing();
        std::wstring GetBorderSpacing();

        /* COLOR */
        std::wstring GetTextDecorationColor();
        std::wstring GetBackgroundColor();
        std::wstring GetColor();
        std::wstring GetOutlineColor();
        std::wstring GetColumnRuleColor();
        std::wstring GetBackground();

        /* TEXT */
        std::wstring GetTextAlign();
        std::wstring GetTextIndent();
        std::wstring GetTextDecoration();

        /* BORDER */
        std::wstring GetBorder();
        std::wstring GetBorderWidth();
        std::wstring GetBorderStyle();
        std::wstring GetBorderColor();

        std::wstring GetBorderBottom();
        std::wstring GetBordeBottomrWidth();
        std::wstring GetBorderBottomStyle();
        std::wstring GetBorderBottomColor();

        std::wstring GetBorderLeft();
        std::wstring GetBorderLeftWidth();
        std::wstring GetBorderLeftStyle();
        std::wstring GetBorderLeftColor();

        std::wstring GetBorderRight();
        std::wstring GetBorderRightWidth();
        std::wstring GetBorderRightStyle();
        std::wstring GetBorderRightColor();

        std::wstring GetBorderTop();
        std::wstring GetBordeToprWidth();
        std::wstring GetBorderTopStyle();
        std::wstring GetBorderTopColor();


        CCompiledStyle& operator+= (const CCompiledStyle& oElement);
        CCompiledStyle& operator=  (const CCompiledStyle& oElement);

        bool operator== (const CCompiledStyle& oStyle)   const;
        bool operator!= (const CCompiledStyle& oElement) const;
        bool operator>  (const CCompiledStyle& oElement) const;
        bool operator<  (const CCompiledStyle& oElement) const;

    };


}

#endif // CCOMPILEDSTYLE_H
