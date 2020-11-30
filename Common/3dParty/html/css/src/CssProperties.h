#ifndef CSSPROPERTIES_H
#define CSSPROPERTIES_H

#include <string>
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
        };

        struct Margin
        {
            float fTopSide;
            float fRightSide;
            float fBottomSide;
            float fLeftSide;

            void AddMargin(const std::wstring &sMargin)
            {
                const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(sMargin, L" ");
                switch (arValues.size())
                {
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
            std::wstring *m_sColor; //HEX color
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
        };

        struct Border
        {
            BorderSide stTop;
            BorderSide stRight;
            BorderSide stBottom;
            BorderSide stLeft;
        };
    }
}

#endif // CSSPROPERTIES_H
