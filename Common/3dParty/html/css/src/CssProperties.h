#ifndef CSSPROPERTIES_H
#define CSSPROPERTIES_H

#include "StaticFunctions.h"

namespace NSCSS
{
    namespace CssProperties
    {
        enum class FontStretch
        {
            normal = 0,
            ultraCondensed,
            extraCondensed,
            condensed,
            semiCondensed,
            semiExpanded,
            expanded,
            extraExpanded,
            ultraExpanded,
        };

        enum class FontStyle
        {
            normal = 0,
            italic,
            oblique,
        };

        enum class FontVariant
        {
            normal = 0,
            smallCaps,
        };

        enum class FontWeight
        {
            normal = 0,
            bold
        };

        struct Font
        {
            float fSize;
            FontStretch enStretch;
            FontStyle enStyle;
            FontVariant enVariant;
            bool bWeight; //true - font-weight:bold;
            float fLineHeight;

            Font() : fSize(0), enStretch(FontStretch::normal), enStyle(FontStyle::normal),
                     enVariant(FontVariant::normal), bWeight(0), fLineHeight(0) {};

            Font(const Font& oFont) : fSize(oFont.fSize), enStretch(oFont.enStretch), enStyle(oFont.enStyle),
                                      enVariant(oFont.enVariant), bWeight(oFont.bWeight), fLineHeight(oFont.fLineHeight){};
        };

        struct Margin
        {
            float fTopSide;
            float fRightSide;
            float fBottomSide;
            float fLeftSide;

            Margin() : fTopSide(0), fRightSide(0), fBottomSide(0), fLeftSide(0){};
            Margin(const Margin &oMargin) : fTopSide(oMargin.fTopSide), fRightSide(oMargin.fRightSide),
                                            fBottomSide(oMargin.fBottomSide), fLeftSide(oMargin.fLeftSide){};

            void AddMargin(const std::wstring& sMargin)
            {
                const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(sMargin, L" ");
                switch (arValues.size())
                {
                    case 0:
                        return;
                    case 1:
                    {
                        const float fValue = wcstof(arValues[0].c_str(), NULL);
                        fTopSide    += fValue;
                        fRightSide  += fValue;
                        fBottomSide += fValue;
                        fLeftSide   += fValue;
                        break;
                    }
                    case 2:
                    {
                        const float fValue1 = wcstof(arValues[0].c_str(), NULL);
                        const float fValue2 = wcstof(arValues[1].c_str(), NULL);
                        fTopSide    += fValue1;
                        fRightSide  += fValue2;
                        fBottomSide += fValue1;
                        fLeftSide   += fValue2;
                        break;
                    }
                    case 3:
                    {
                        const float fValue = wcstof(arValues[1].c_str(), NULL);
                        fTopSide    += wcstof(arValues[0].c_str(), NULL);
                        fRightSide  += fValue;
                        fBottomSide += wcstof(arValues[2].c_str(), NULL);
                        fLeftSide   += fValue;
                        break;
                    }
                    case 4:
                    {
                        fTopSide    += wcstof(arValues[0].c_str(), NULL);
                        fRightSide  += wcstof(arValues[1].c_str(), NULL);
                        fBottomSide += wcstof(arValues[2].c_str(), NULL);
                        fLeftSide   += wcstof(arValues[3].c_str(), NULL);
                        break;
                    }
                }
            }
        };


        enum class TextDecoration
        {
            none = 0,
            lineThrough,
            overline,
            underline
        };

        struct Text
        {
            float fAlign;
            float fIndent;
            TextDecoration enDecoration;
            std::wstring sColor; //HEX color

            Text(): fAlign(0), fIndent(0), enDecoration(TextDecoration::none), sColor(L""){};
            Text(const Text &oText) : fAlign(oText.fAlign), fIndent(oText.fIndent),
                                      enDecoration(oText.enDecoration), sColor(oText.sColor){};
        };

        enum BorderStyle
        {
            none = 0,
            hidden,
            dotted,
            dashed,
            solid,
            dual,
            groove,
            ridge,
            inset,
            outset
        };

        struct BorderSide
        {
            float fWidth;
            BorderStyle enStyle;
            std::wstring sColor; //HEX color

            BorderSide() : fWidth(0), enStyle(BorderStyle::none), sColor(L""){};
            BorderSide(const BorderSide& oBorderSide) : fWidth(oBorderSide.fWidth), enStyle(oBorderSide.enStyle),
                                                        sColor(oBorderSide.sColor){};
        };

        struct Border
        {
            BorderSide stTop;
            BorderSide stRight;
            BorderSide stBottom;
            BorderSide stLeft;

            Border(){};
            Border(const Border &oBorder) : stTop(oBorder.stTop), stRight(oBorder.stRight),
                                            stBottom(oBorder.stBottom), stLeft(oBorder.stLeft){};
        };
    }
}

#endif // CSSPROPERTIES_H
