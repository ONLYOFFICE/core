#ifndef CONSTVALUES_H
#define CONSTVALUES_H

#include "CssCalculator_global.h"
#include "StaticFunctions.h"
#include "CNode.h"
#include <vector>
#include <iostream>

namespace NSCSS
{
    typedef enum {
        Default = 0,
        Pixel,
        Point,
        Cantimeter,
        Millimeter,
        Inch,
        Peak
    } UnitMeasure;

    namespace NSConstValues
    {
        namespace NSMaps {
            const std::map<std::wstring, std::wstring> mColors {
                /* Red tones */
                {L"indianeed",          L"CD5C5C"}, {L"lightcoral",            L"F08080"}, {L"salmon",        L"FA8072"},
                {L"darksalmon",         L"E9967A"}, {L"lightsalmon",           L"FFA07A"}, {L"crimson",       L"DC143C"},
                {L"red",                L"FF0000"}, {L"firebrick",             L"B22222"}, {L"darkeed",       L"8B0000"},
                /* Pink tones */
                {L"pink",               L"FFC0CB"}, {L"lightpink",             L"FFB6C1"}, {L"hotpink",       L"FF69B4"},
                {L"deeppink",           L"FF1493"}, {L"mediumvioletred",       L"C71585"}, {L"palevioleteed", L"DB7093"},
                /* Orange tones */
                {L"lightsalmon",        L"FFA07A"}, {L"coral",                 L"FF7F50"}, {L"tomato",         L"FF6347"},
                {L"orangered",          L"FF4500"}, {L"darkorange",            L"FF8C00"}, {L"orange",         L"FFA500"},
                /* Yellow tones */
                {L"gold",               L"FFD700"}, {L"yellow",                L"FFFF00"}, {L"lightyellow",   L"FFFFE0"},
                {L"lemonchiffon",       L"FFFACD"}, {L"lightgoldenrodyellow",  L"FAFAD2"}, {L"papayawhip",    L"FFEFD5"},
                {L"moccasin",           L"FFE4B5"}, {L"peachpuff",             L"FFDAB9"}, {L"palegoldenrod", L"EEE8AA"},
                {L"khaki",              L"F0E68C"}, {L"darkkhaki",             L"BDB76B"},
                /* Purple tones */
                {L"lavender",           L"E6E6FA"}, {L"thistle",               L"D8BFD8"}, {L"plum",          L"DDA0DD"},
                {L"violet",             L"EE82EE"}, {L"orchid",                L"DA70D6"}, {L"fuchsia",       L"FF00FF"},
                {L"magenta",            L"FF00FF"}, {L"mediumorchid",          L"BA55D3"}, {L"mediumpurple",  L"9370DB"},
                {L"blueviolet",         L"8A2BE2"}, {L"darkviolet",            L"9400D3"}, {L"darkorchid",    L"9932CC"},
                {L"darkmagenta",        L"8B008B"}, {L"purple",                L"800080"}, {L"indigo",        L"4B0082"},
                {L"slateblue",          L"6A5ACD"}, {L"darkslateblue",         L"483D8B"},
                /* Brown tones */
                {L"cornsilk",           L"FFF8DC"}, {L"blanchedalmond",        L"FFEBCD"}, {L"bisque",        L"FFE4C4"},
                {L"navajowhite",        L"FFDEAD"}, {L"wheat",                 L"F5DEB3"}, {L"burlywood",     L"DEB887"},
                {L"tan",                L"D2B48C"}, {L"rosybrown",             L"BC8F8F"}, {L"sandybrown",    L"F4A460"},
                {L"goldenrod",          L"DAA520"}, {L"darkgoldenrod",         L"B8860B"}, {L"peru",          L"CD853F"},
                {L"chocolate",          L"D2691E"}, {L"saddlebrown",           L"8B4513"}, {L"sienna",        L"A0522D"},
                {L"brown",              L"A52A2A"}, {L"maroon",                L"800000"},
                /* Green tones */
                {L"greenyellow",        L"ADFF2F"}, {L"chartreuse",            L"7FFF00"}, {L"lawngreen",     L"7CFC00"},
                {L"lime",               L"00FF00"}, {L"limegreen",             L"32CD32"}, {L"palegreen",     L"98FB98"},
                {L"lightgreen",         L"90EE90"}, {L"mediumspringgreen",     L"00FA9A"}, {L"springgreen",   L"00FF7F"},
                {L"mediumseagreen",     L"3CB371"}, {L"seagreen",              L"2E8B57"}, {L"forestgreen",   L"228B22"},
                {L"green",              L"008000"}, {L"darkgreen",             L"006400"}, {L"yellowgreen",   L"9ACD32"},
                {L"olivedrab",          L"6B8E23"}, {L"olive",                 L"808000"}, {L"darkolivegreen",L"556B2F"},                                                                   {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"},
                {L"mediumaquamarine",   L"66CDAA"}, {L"darkseagreen",          L"8FBC8F"}, {L"lightseagreen", L"20B2AA"},                                                                   {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"},
                {L"darkcyan",           L"008B8B"}, {L"teal",                  L"008080"},
                /* Blue tones */
                {L"aqua",               L"00FFFF"}, {L"cyan",                  L"00FFFF"}, {L"lightcyan",     L"E0FFFF"},
                {L"paleturquoise",      L"AFEEEE"}, {L"aquamarine",            L"7FFFD4"}, {L"turquoise",     L"40E0D0"},                                                                   {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"},
                {L"mediumturquoise",    L"48D1CC"}, {L"darkturquoise",         L"00CED1"}, {L"cadetblue",     L"5F9EA0"},
                {L"steelblue",          L"4682B4"}, {L"lightsteelblue",        L"B0C4DE"}, {L"powderblue",    L"B0E0E6"},
                {L"lightblue",          L"ADD8E6"}, {L"skyblue",               L"87CEEB"}, {L"lightskyblue",  L"87CEFA"},
                {L"deepskyblue",        L"00BFFF"}, {L"dodgerblue",            L"1E90FF"}, {L"cornflowerblue",L"6495ED"},
                {L"mediumdlateblue",    L"7B68EE"}, {L"royalblue",             L"4169E1"}, {L"blue",          L"0000FF"},                                                                   {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"}, {L"LightCoral", L"#F08080"},
                {L"mediumblue",         L"0000CD"}, {L"darkblue",              L"00008B"}, {L"navy",          L"000080"},
                {L"midnightblue",       L"191970"},
                /* White tones */
                {L"white",              L"FFFFFF"}, {L"snow",                  L"FFFAFA"}, {L"honeydew",      L"F0FFF0"},
                {L"mintcream",          L"F5FFFA"}, {L"azure",                 L"F0FFFF"}, {L"aliceblue",     L"F0F8FF"},
                {L"ghostwhite",         L"F8F8FF"}, {L"whitesmoke",            L"F5F5F5"}, {L"seashell",      L"FFF5EE"},
                {L"beige",              L"F5F5DC"}, {L"oldlace",               L"FDF5E6"}, {L"floralwhite",   L"FFFAF0"},
                {L"ivory",              L"FFFFF0"}, {L"antiquewhite",          L"FAEBD7"}, {L"linen",         L"FAF0E6"},
                {L"lavenderblush",      L"FFF0F5"}, {L"mistyrose",             L"FFE4E1"},
                /* Gray tones */
                {L"gainsboro",          L"DCDCDC"}, {L"lightgrey",             L"D3D3D3"}, {L"silver",        L"C0C0C0"},
                {L"darkgray",           L"A9A9A9"}, {L"gray",                  L"808080"}, {L"dimgray",       L"696969"},
                {L"lightslategray",     L"778899"}, {L"slategray",             L"708090"}, {L"darkslategray", L"2F4F4F"},
                {L"black",              L"000000"},
                /* Outdated */
                {L"windowtext",         L"000000"}
            };
        }

        namespace NSProperties {
            typedef enum
            {
                B_CustomStyle = 0,
                B_StyleId = 1,
                B_Type = 2,
                B_Default = 3,

                B_Name = 4,
                B_BasedOn = 5,
                B_QFormat = 6,
                B_Link = 7,
                B_UnhideWhenUsed = 8,
                B_UiPriority = 9,
            } BasicProperties;

            typedef enum
            {
                P_Jc = 0,
                P_Spacing = 1,
                P_ContextualSpacing = 2,
                P_Ind = 3,
                P_OutlineLvl = 4,
                P_Shd = 5,
                // <pBdr>
                    P_TopBorder = 6,
                    P_LeftBorder = 7,
                    P_BottomBorder = 8,
                    P_RightBorder = 9,
                // </pBdr>
                P_KeepLines = 10,
                P_KeepNext = 11,
            } ParagraphProperties;

            typedef enum
            {
                R_RFonts = 0,
                R_Sz = 1,
                R_B = 2,
                R_I = 3,
                R_Color = 4,
                R_U = 5,
                R_Shd = 6
            } RunnerProperties;

            typedef enum
            {
                T_TblInd = 0,
                // <tblCellMar>
                    T_CellTop = 1,
                    T_CellLeft = 2,
                    T_CellBottom = 3,
                    T_CellRight = 4,
                // <tblCellMar>

                // <tblBorders>
                    T_BorderTop = 5,
                    T_BorderLeft = 6,
                    T_BorderBottom = 7,
                    T_BorderRight = 8,
                    T_BorderInsideH = 9,
                    T_BorderInsideV = 10
                // </tblBorders>
            } TableProperties;
        }

        namespace NSCssProperties
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

            class Margin
            {
                public:
                Margin(){};
                ~Margin(){};

                float fTopSide = 0;
                float fRightSide = 0;
                float fBottomSide = 0;
                float fLeftSide = 0;

                bool Empty() const
                {
                    if (fTopSide == fRightSide == fBottomSide == fLeftSide == 0)
                        return true;
                    return false;
                };

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
                };
            };


            enum class TextDecoration
            {
                none = 0,
                lineThrough,
                overline,
                underline
            };

            enum class TextAlign
            {
                left = 0,
                right,
                center,
                justify
            };

            struct Text
            {
                float fIndent;
                TextAlign enAlign;
                TextDecoration enDecoration;
                std::wstring sColor; //HEX color

                void SetColor(const std::wstring& sValue)
                {
                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sColor += sValue[1];
                            sColor += sValue[1];
                            sColor += sValue[2];
                            sColor += sValue[2];
                            sColor += sValue[3];
                            sColor += sValue[3];
                        }
                    }
                    else if (sValue.substr(0, 3) == L"rgb")
                    {
                        std::wcout << L"{" << sValue << L"}" << std::endl;
                        sColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                        std::wcout << L"|" << sColor << L"|" << std::endl;
                    }
                    else
                    {
                        std::wstring sNewColor;
                        std::transform(sValue.begin(), sValue.end(), sNewColor.begin(), tolower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sColor = oHEX->second;
                    }
                }
            };

            enum class BorderStyle
            {
                none = 0,
                single,
                dotted,
                dashed,
                dual,
                threeDEmboss,
                threeDEngrave,
                thinThickMediumGap,
                thickThinMediumGap
            };

            struct BorderSide
            {
                float fWidth = 0;
                std::wstring sStyle = L"";
                std::wstring sColor = L"auto"; //HEX color

                bool operator==(const BorderSide& oBorderSide) const
                {
                    return  fWidth  == oBorderSide.fWidth &&
                            sStyle == oBorderSide.sStyle &&
                            sColor  == oBorderSide.sColor;
                }

                bool Empty() const
                {
                    return fWidth <= 0 &&
                            sStyle.empty() &&
                            sColor == L"auto";
                }

                void SetStyle(const std::wstring& sValue)
                {
                    std::wstring sNewValue = sValue;
                    if (!sNewValue.empty())
                    {
                        std::transform(sNewValue.begin(), sNewValue.end(), sNewValue.begin(), tolower);

                        if (sNewValue == L"dotted")
                            sStyle = sNewValue;
                        else if (sNewValue == L"dashed")
                            sStyle = sNewValue;
                        else if (sNewValue == L"solid")
                            sStyle = L"single";
                        else if (sNewValue == L"double")
                            sStyle = L"double";
                        else if (sNewValue == L"groove")
                            sStyle = L"threeDEmboss";
                        else if (sNewValue == L"ridge")
                            sStyle = L"threeDEngrave";
                        else if (sNewValue == L"inset")
                            sStyle = L"thinThickMediumGap";
                        else if (sNewValue == L"outset")
                            sStyle = L"thickThinMediumGap";
                    }
                }

                void SetColor(const std::wstring& sValue)
                {
                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sColor += sValue[1];
                            sColor += sValue[1];
                            sColor += sValue[2];
                            sColor += sValue[2];
                            sColor += sValue[3];
                            sColor += sValue[3];
                        }
                    }
                    else if (sValue.substr(0, 3) == L"rgb")
                    {
                        sColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                    }
                    else
                    {
                        std::wstring sNewColor;
                        std::transform(sValue.begin(), sValue.end(), sNewColor.begin(), tolower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sColor = oHEX->second;
                    }
                }

                static BorderSide GetCorrectSide(const std::wstring& sValue)
                {
                    const std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsW(sValue, L" ");
                    BorderSide oBorderSide;
                    for (std::wstring sValue : arValues)
                    {
                        if (sValue[0] == L'#')
                        {
                            if (sValue.length() == 7)
                                oBorderSide.sColor = sValue.substr(1, 7);
                            else if (sValue.length() == 4)
                            {
                                oBorderSide.sColor += sValue[1];
                                oBorderSide.sColor += sValue[1];
                                oBorderSide.sColor += sValue[2];
                                oBorderSide.sColor += sValue[2];
                                oBorderSide.sColor += sValue[3];
                                oBorderSide.sColor += sValue[3];
                            }
                        }
                        else if (iswdigit(sValue[0]))
                        {
                            oBorderSide.fWidth = wcstof(sValue.c_str(), NULL);
                        }
                        else if (sValue.substr(0, 3) == L"rgb")
                        {
                            oBorderSide.sColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                        }
                        else
                        {
                            std::transform(sValue.begin(), sValue.end(), sValue.begin(), tolower);

                            if (sValue == L"dotted")
                                oBorderSide.sStyle = sValue;
                            else if (sValue == L"dashed")
                                oBorderSide.sStyle = sValue;
                            else if (sValue == L"solid")
                                oBorderSide.sStyle = L"single";
                            else if (sValue == L"double")
                                oBorderSide.sStyle = L"double";
                            else if (sValue == L"groove")
                                oBorderSide.sStyle = L"threeDEmboss";
                            else if (sValue == L"ridge")
                                oBorderSide.sStyle = L"threeDEngrave";
                            else if (sValue == L"inset")
                                oBorderSide.sStyle = L"thinThickMediumGap";
                            else if (sValue == L"outset")
                                oBorderSide.sStyle = L"thickThinMediumGap";
                            else
                            {
                                const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sValue);
                                if (oHEX != NSMaps::mColors.end())
                                    oBorderSide.sColor = oHEX->second;
                            }
                        }
                    }
                    return oBorderSide;
                };
            };

            struct Border
            {
                BorderSide stTop;
                BorderSide stRight;
                BorderSide stBottom;
                BorderSide stLeft;

                bool EqualSides() const
                {
                    return  stTop    == stRight &&
                            stRight  == stBottom &&
                            stBottom == stLeft;
                }

                bool Empty() const
                {
                    return stTop.Empty() && stRight.Empty() && stBottom.Empty() && stLeft.Empty();
                }

                void SetWidth(const float& fValue)
                {
                    if (fValue >= 0)
                    {
                        stTop.fWidth    = fValue;
                        stRight.fWidth  = fValue;
                        stBottom.fWidth = fValue;
                        stLeft.fWidth   = fValue;
                    }
                }

                void SetStyle(const std::wstring& sValue)
                {
                    if (!sValue.empty())
                    {
                        std::wstring sNewValue = sValue;
                        std::transform(sNewValue.begin(), sNewValue.end(), sNewValue.begin(), tolower);
                        std::wstring sStyle;

                        if (sNewValue == L"dotted")
                            sStyle = sValue;
                        else if (sNewValue == L"dashed")
                            sStyle = sValue;
                        else if (sNewValue == L"solid")
                            sStyle = L"single";
                        else if (sNewValue == L"double")
                            sStyle = L"double";
                        else if (sNewValue == L"groove")
                            sStyle = L"threeDEmboss";
                        else if (sNewValue == L"ridge")
                            sStyle = L"threeDEngrave";
                        else if (sNewValue == L"inset")
                            sStyle = L"thinThickMediumGap";
                        else if (sNewValue == L"outset")
                            sStyle = L"thickThinMediumGap";

                        stTop.sStyle    = sStyle;
                        stRight.sStyle  = sStyle;
                        stBottom.sStyle = sStyle;
                        stLeft.sStyle   = sStyle;
                    }
                }

                void SetColor(const std::wstring& sValue)
                {
                    std::wstring sNewColor;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sNewColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sNewColor += sValue[1];
                            sNewColor += sValue[1];
                            sNewColor += sValue[2];
                            sNewColor += sValue[2];
                            sNewColor += sValue[3];
                            sNewColor += sValue[3];
                        }
                    }
                    else if (sValue.substr(0, 3) == L"rgb")
                    {
                        sNewColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                    }
                    else
                    {
                        std::transform(sValue.begin(), sValue.end(), sNewColor.begin(), tolower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sNewColor = oHEX->second;
                    }

                    if (!sNewColor.empty())
                    {
                        stTop.sColor    = sNewColor;
                        stRight.sColor  = sNewColor;
                        stBottom.sColor = sNewColor;
                        stLeft.sColor   = sNewColor;
                    }
                }

            };

            struct Background
            {
                std::wstring sColor;

                void SetColor(const std::wstring &sValue)
                {
                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sColor += sValue[1];
                            sColor += sValue[1];
                            sColor += sValue[2];
                            sColor += sValue[2];
                            sColor += sValue[3];
                            sColor += sValue[3];
                        }
                    }
                    else if (sValue.substr(0, 3) == L"rgb")
                    {
                        sColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                    }
                    else
                    {
                        std::wstring sNewColor;
                        std::transform(sValue.begin(), sValue.end(), sNewColor.begin(), tolower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sColor = oHEX->second;
                    }
                }
            };
        }

        const std::vector<std::wstring> arPseudoClasses {
                                                        L"invalid",
                                                        L"read-only",
                                                        L"-moz-placeholder",
                                                        L"-webkit-input-placeholder",
                                                        L"active",
                                                        L"checked",
                                                        L"default",
                                                        L"disabled",
                                                        L"empty",
                                                        L"enabled",
                                                        L"first-child",
                                                        L"first-of-type",
                                                        L"focus",
                                                        L"hover",
                                                        L"indeterminate",
                                                        L"lang",
                                                        L"last-child",
                                                        L"last-of-type",
                                                        L"link",
                                                        L"not",
                                                        L"nth-child",
                                                        L"nth-last-child",
                                                        L"nth-last-of-type",
                                                        L"nth-of-type",
                                                        L"only-child",
                                                        L"only-of-type",
                                                        L"optional",
                                                        L"read-write",
                                                        L"required",
                                                        L"root",
                                                        L"target",
                                                        L"valid",
                                                        L"visited"};
}
}

#endif // CONSTVALUES_H
