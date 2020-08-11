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
    public:
        CCompiledStyle();
        CCompiledStyle(std::map<std::wstring, std::wstring> mStyle);
        CCompiledStyle(const CCompiledStyle& oStyle);

        ~CCompiledStyle();

        std::map<std::wstring, std::wstring> GetStyleMap();
        std::wstring GetStyleW();
        std::string GetStyle();

        size_t GetSize();
        bool Empty();

        std::map<std::wstring, std::wstring>::iterator GetBegin();
        std::map<std::wstring, std::wstring>::iterator GetEnd();

        void AddPropSel(std::wstring sProperty, std::wstring sValue);
        void InsertStyle(std::map<std::wstring, std::wstring> mStyle);
        void SetStyle(std::map<std::wstring, std::wstring> mStyle);
        void AddStyle(std::wstring sStyle);

        void SetID(std::wstring sId);
        std::wstring GetId();

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
        std::wstring GetMarginLeft();
        std::wstring GetMarginRight();
        std::wstring GetMarginTop();

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

        /* TEXT */
        std::wstring GetTextAlign();
        std::wstring GetTextIndent();
        std::wstring GetTextDecoration();

        /* BORDER */
        std::wstring GetBorder();
        std::wstring GetBorderBottom();
        std::wstring GetBorderLeft();
        std::wstring GetBorderRight();
        std::wstring GetBorderTop();
        std::wstring GetBorderWidth();
        std::wstring GetBorderStyle();
        std::wstring GetBorderColor();
        std::wstring GetBorderBottomColor();
        std::wstring GetBorderLeftColor();
        std::wstring GetBorderRightColor();
        std::wstring GetBordetTopColor();

        CCompiledStyle& operator+= (const CCompiledStyle& oElement);
        CCompiledStyle& operator= (const CCompiledStyle& oElement);
    };


}

#endif // CCOMPILEDSTYLE_H
