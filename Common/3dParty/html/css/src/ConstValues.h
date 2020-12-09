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
                R_Shd = 6,
                R_SmallCaps = 7
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
            const float fNoneValue = -99999999999.0f;

            enum class FontStretch
            {
                none = 0,
                normal,
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
                none = 0,
                normal,
                italic,
                oblique,
            };

            enum class FontVariant
            {
                none = 0,
                normal,
                smallCaps,
            };

            enum class FontWeight
            {
                none = 0,
                normal,
                bold
            };

            class Font
            {
                float fSize;
                FontStretch enStretch;
                FontStyle enStyle;
                FontVariant enVariant;
                FontWeight enWeight;
                float fLineHeight;
                std::wstring sFamily;

            public:

                Font() : fSize      (fNoneValue),
                         enStretch  (FontStretch::none),
                         enStyle    (FontStyle::none),
                         enVariant  (FontVariant::none),
                         enWeight   (FontWeight::none),
                         fLineHeight(fNoneValue){}

                Font operator+=(const Font& oFont)
                {
                    if (oFont.fSize != fNoneValue)
                        fSize = oFont.fSize;
                    if (oFont.enStretch != FontStretch::none)
                        enStretch = oFont.enStretch;
                    if (oFont.enStyle != FontStyle::none)
                        enStyle = oFont.enStyle;
                    if (oFont.enVariant != FontVariant::none)
                        enVariant = oFont.enVariant;
                    if (oFont.enWeight != FontWeight::none)
                        enWeight = oFont.enWeight;
                    if (oFont.fLineHeight != fNoneValue)
                        fLineHeight = oFont.fLineHeight;
                    if (!oFont.sFamily.empty())
                        sFamily = oFont.sFamily;

                    return *this;
                }

                bool operator==(const Font& oFont) const
                {
                    return  fSize       == oFont.fSize       &&
                            fLineHeight == oFont.fLineHeight &&
                            sFamily     == oFont.sFamily     &&
                            enStretch   == oFont.enStretch   &&
                            enStyle     == oFont.enStyle     &&
                            enVariant   == oFont.enVariant   &&
                            enWeight    == oFont.enWeight;
                }

                bool Empty() const
                {
                    return fSize == 22.0f && enStretch == FontStretch::none &&
                           enStyle == FontStyle::none && enVariant == FontVariant::none &&
                           enWeight == FontWeight::none && fLineHeight < fSize && sFamily.empty();
                }

                void Clear()
                {
                    fSize = fNoneValue;
                    enStretch = FontStretch::none;
                    enStyle = FontStyle::none;
                    enVariant = FontVariant::none;
                    enWeight = FontWeight::none;
                    fLineHeight = fNoneValue;
                    sFamily.clear();
                }

                void SetFont(const std::wstring &sFont)
                {
                    if (sFont.empty())
                        return;

                    const std::vector<std::wstring> arValues = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(sFont, L" ,/");

                    bool bIsBrackets = false;
                    unsigned short nPosition = 0;

                    for (const std::wstring &sWord : arValues)
                    {
                        if (nPosition < 4 && enStyle == FontStyle::none)
                        {
                            if (sWord == L"normal")
                            {
                                enStyle = FontStyle::normal;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"italic")
                            {
                                enStyle = FontStyle::italic;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"oblique")
                            {
                                enStyle = FontStyle::oblique;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"inherit")
                            {
                                ++nPosition;
                                continue;
                            }
                        }

                        if (nPosition < 4 && enVariant == FontVariant::none)
                        {
                            if (sWord == L"normal")
                            {
                                enVariant = FontVariant::normal;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"small-caps")
                            {
                                enVariant = FontVariant::smallCaps;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"inherit")
                            {
                                ++nPosition;
                                continue;
                            }
                        }

                        if (nPosition < 4 && enWeight == FontWeight::none)
                        {
                            if (sWord == L"bold" || sWord == L"bolder" || sWord[0] == L'6' || sWord[0] == L'7' || sWord[0] == L'8' ||sWord[0] == L'9')
                            {
                                enWeight = FontWeight::bold;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"normal" || sWord[0] == L'3' || sWord[0] == L'4' ||sWord[0] == L'5')
                            {
                                enWeight = FontWeight::normal;
                                ++nPosition;
                                continue;
                            }
                        }

                        if (nPosition < 4 && enStretch == FontStretch::none)
                        {
                            if (sWord == L"ultra-condensed")
                            {
                                enStretch = FontStretch::ultraCondensed;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"extra-condensed")
                            {
                                enStretch = FontStretch::extraCondensed;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"condensed")
                            {
                                enStretch = FontStretch::condensed;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"semi-condensed")
                            {
                                enStretch = FontStretch::semiCondensed;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"normal")
                            {
                                enStretch = FontStretch::normal;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"semi-expanded")
                            {
                                enStretch = FontStretch::semiExpanded;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"extra-expanded")
                            {
                                enStretch = FontStretch::extraExpanded;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"ultra-expanded")
                            {
                                enStretch = FontStretch::ultraExpanded;
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"inherit")
                            {
                                ++nPosition;
                                continue;
                            }
                        }

                        if (nPosition < 5 && iswdigit(sWord[0]))
                        {
                            nPosition = 5;
                            fSize = wcstof(sWord.c_str(), NULL);
                            continue;
                        }

                        if (nPosition == 5 && iswdigit(sWord[0]))
                        {
                            fLineHeight = wcstof(sWord.c_str(), NULL);
                            ++nPosition;
                            continue;
                        }

                        if (nPosition >= 5)
                        {
                            if (bIsBrackets)
                            {
                                sFamily += L' ' + sWord;
                                if (sWord.back() == L'"' || sWord.back() == L'\'')
                                    bIsBrackets = false;
                                continue;
                            }

                            if (sWord[0] == L'"' || sWord[0] == L'\'')
                            {
                                if (bIsBrackets)
                                    bIsBrackets = false;
                                else
                                {
                                    bIsBrackets = true;
                                    sFamily = sWord;
                                }

                                if (sWord.back() == L'"' || sWord.back() == L'\'')
                                    bIsBrackets = false;

                                continue;
                            }
                        }
                    }
                }

                void SetSize(const std::wstring &sSize)
                {
                    if (sSize.empty())
                        return;

                    const float fValue = wcstof(sSize.c_str(), NULL);
                    if (fValue > 0.0f)
                        fSize = fValue;
                }

                void SetStretch(const std::wstring &sStretch)
                {
                    if (sStretch.empty())
                        return;

                    if (sStretch == L"ultra-condensed")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::ultraCondensed;
                    else if (sStretch == L"extra-condensed")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::extraCondensed;
                    else if (sStretch == L"condensed")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::condensed;
                    else if (sStretch == L"semi-condensed")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::semiCondensed;
                    else if (sStretch == L"normal")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::normal;
                    else if (sStretch == L"semi-expanded")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::semiExpanded;
                    else if (sStretch == L"expanded")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::expanded;
                    else if (sStretch == L"extra-expanded")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::extraExpanded;
                    else if (sStretch == L"ultra-expanded")
                        enStretch = NSConstValues::NSCssProperties::FontStretch::ultraExpanded;
                }

                void SetStyle(const std::wstring &sStyle)
                {
                    if (sStyle.empty())
                        return;

                    if (sStyle == L"italic")
                        enStyle = NSConstValues::NSCssProperties::FontStyle::italic;
                    else if (sStyle == L"oblique")
                        enStyle = NSConstValues::NSCssProperties::FontStyle::oblique;
                    else if (sStyle == L"normal")
                        enStyle = NSConstValues::NSCssProperties::FontStyle::normal;
                }

                void SetVariant(const std::wstring &sVariant)
                {
                    if (sVariant.empty())
                        return;

                    if (sVariant == L"small-caps")
                        enVariant = NSConstValues::NSCssProperties::FontVariant::smallCaps;
                    else if (sVariant == L"normal")
                        enVariant = NSConstValues::NSCssProperties::FontVariant::normal;
                }

                void SetWeight(const std::wstring &sWeight)
                {
                    if (sWeight.empty())
                        return;

                    if (sWeight == L"bold" || sWeight == L"bolder" ||
                        sWeight[0] == L'6' || sWeight[0] == L'7' ||
                        sWeight[0] == L'8' || sWeight[0] == L'9')
                        enWeight = NSConstValues::NSCssProperties::FontWeight::bold;
                    else if (sWeight == L"normal" || sWeight[0] == L'3' ||
                             sWeight[0] == L'4'   || sWeight[0] == L'5')
                        enWeight = NSConstValues::NSCssProperties::FontWeight::normal;
                }

                void SetLineHeight(const std::wstring &sLineHeight)
                {
                    if (sLineHeight.empty())
                        return;

                    const float fValue = wcstof(sLineHeight.c_str(), NULL);
                    if (fValue > 0.0f && fValue >= fSize)
                        fLineHeight = fValue;
                }

                float GetSize() const
                {
                    return (fSize == fNoneValue) ? 22.0f : fSize;
                }

                std::wstring GetSizeW() const
                {
                    if (fSize <= 0.0f)
                        return std::wstring();

                    const int nValue = static_cast<int>(fSize);
                    const bool bLowerLimit = (0.75 <= nValue - fSize);
                    const bool bUpperLimit = (nValue - fSize < 0.25);

                    return std::to_wstring(bUpperLimit ? nValue : bLowerLimit ? nValue - 1 : nValue - 0.5);
                }

                std::wstring GetFamily() const
                {
                    return sFamily;
                }

                std::wstring GetStyle() const
                {
                    if (enStyle == FontStyle::italic)
                        return L"italic";

                    if (enStyle == FontStyle::normal)
                        return L"normal";

                    return std::wstring();
                }

                std::wstring GetWeight() const
                {
                    if (enWeight == FontWeight::bold)
                        return L"bold";

                    if (enWeight == FontWeight::normal)
                        return L"normal";

                    return std::wstring();
                }

                std::wstring GetLineHeight() const
                {
                    if (fLineHeight < 0.0f || fLineHeight < fSize)
                        return std::wstring();

                    const int nValue = static_cast<int>(fLineHeight);
                    const bool bLowerLimit = (0.75 <= nValue - fLineHeight);
                    const bool bUpperLimit = (nValue - fLineHeight < 0.25);

                    return std::to_wstring(bUpperLimit ? nValue : bLowerLimit ? nValue - 1 : nValue - 0.5);
                }

                std::wstring GetVariant() const
                {
                    if (enVariant == FontVariant::smallCaps)
                        return L"smallCaps";

                    if (enVariant == FontVariant::normal)
                        return L"normal";

                    return std::wstring();
                }
            };

            class Margin
            {
                float fTopSide;
                float fRightSide;
                float fBottomSide;
                float fLeftSide;

            public:
                Margin() : fTopSide     (fNoneValue),
                           fRightSide   (fNoneValue),
                           fBottomSide  (fNoneValue),
                           fLeftSide    (fNoneValue){}

                Margin operator+=(const Margin& oMargin)
                {
                    if (oMargin.fTopSide != fNoneValue)
                        fTopSide = oMargin.fTopSide;
                    if (oMargin.fRightSide != fNoneValue)
                        fRightSide = oMargin.fRightSide;
                    if (oMargin.fBottomSide != fNoneValue)
                        fBottomSide = oMargin.fBottomSide;
                    if (oMargin.fLeftSide != fNoneValue)
                        fLeftSide = oMargin.fLeftSide;

                    return *this;;
                }

                bool operator==(const Margin& oMargin) const
                {
                    return  fTopSide    == oMargin.fTopSide     &&
                            fRightSide  == oMargin.fRightSide   &&
                            fBottomSide == oMargin.fBottomSide  &&
                            fLeftSide   == oMargin.fLeftSide;
                }

                bool Empty() const
                {
                    return fTopSide == fRightSide == fBottomSide == fLeftSide == fNoneValue;
                };

                void AddMargin(const std::wstring& sMargin)
                {
                    if (sMargin.empty())
                        return;

                    if (fTopSide == fNoneValue)
                        fTopSide = 0;
                    if (fRightSide == fNoneValue)
                        fRightSide = 0;
                    if (fBottomSide == fNoneValue)
                        fBottomSide = 0;
                    if (fLeftSide == fNoneValue)
                        fLeftSide = 0;

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

                void AddTopMargin(const std::wstring& sTopMargin)
                {
                    if (sTopMargin.empty())
                        return;

                    if (fTopSide == fNoneValue)
                        fTopSide = 0;

                    fTopSide += wcstof(sTopMargin.c_str(), NULL);
                }

                void AddRightMargin(const std::wstring& sRightMargin)
                {
                    if (sRightMargin.empty())
                        return;

                    if (fRightSide == fNoneValue)
                        fRightSide = 0;

                    fRightSide += wcstof(sRightMargin.c_str(), NULL);
                }

                void AddBottomMargin(const std::wstring& sBottomMargin)
                {
                    if (sBottomMargin.empty())
                        return;

                    if (fBottomSide == fNoneValue)
                        fBottomSide = 0;

                    fBottomSide += wcstof(sBottomMargin.c_str(), NULL);
                }

                void AddLeftMargin(const std::wstring& sLeftMargin)
                {
                    if (sLeftMargin.empty())
                        return;

                    if (fLeftSide == fNoneValue)
                        fLeftSide = 0;

                    fLeftSide += wcstof(sLeftMargin.c_str(), NULL);
                }

                std::wstring GetTopSide() const
                {
                    if (fTopSide == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fTopSide  * 10.0f + 0.5f));
                }

                std::wstring GetRightSide() const
                {
                    if (fTopSide == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fRightSide  * 10.0f + 0.5f));
                }

                std::wstring GetBottomSide() const
                {
                    if (fBottomSide == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fBottomSide  * 10.0f + 0.5f));
                }

                std::wstring GetLeftSide() const
                {
                    if (fLeftSide == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fLeftSide  * 10.0f + 0.5f));
                }
            };

            enum class TextDecoration
            {
                none = 0,
                underline,
                normal
            };

            enum class TextAlign
            {
                none = 0,
                left,
                right,
                center,
                justify
            };

            class Text
            {
                float fIndent;
                TextAlign enAlign;
                TextDecoration enDecoration;
                std::wstring sColor; //HEX color
            public:

                Text() : fIndent        (fNoneValue),
                         enAlign        (TextAlign::none),
                         enDecoration   (TextDecoration::none){}

                Text operator+=(const Text& oText)
                {
                    if (oText.fIndent != fNoneValue)
                        fIndent = oText.fIndent;
                    if (oText.enAlign != TextAlign::none)
                        enAlign = oText.enAlign;
                    if (oText.enDecoration != TextDecoration::none)
                        enDecoration = oText.enDecoration;
                    if (!oText.sColor.empty())
                        sColor = oText.sColor;

                    return *this;
                }

                bool operator==(const Text& oText) const
                {
                    return  fIndent      == oText.fIndent       &&
                            enAlign      == oText.enAlign       &&
                            enDecoration == oText.enDecoration  &&
                            sColor       == oText.sColor;
                }

                bool Empty() const
                {
                    return fIndent == 0 && enAlign == TextAlign::none &&
                           enDecoration == TextDecoration::none && sColor.empty();
                }

                void SetAlign(const std::wstring& sAlign)
                {
                    if (sAlign.empty())
                        return;

                    if (sAlign == L"center")
                       enAlign = NSConstValues::NSCssProperties::TextAlign::center;
                    else if(sAlign == L"justify")
                        enAlign = NSConstValues::NSCssProperties::TextAlign::justify;
                    else if(sAlign == L"left" || sAlign == L"start")
                        enAlign = NSConstValues::NSCssProperties::TextAlign::left;
                    else if(sAlign == L"right" || sAlign == L"end")
                        enAlign = NSConstValues::NSCssProperties::TextAlign::right;
                }

                void SetDecoration(const std::wstring sDecoration)
                {
                    if (sDecoration.empty())
                        return;

                    if (sDecoration == L"underline")
                        enDecoration = NSConstValues::NSCssProperties::TextDecoration::underline;
                    else if (sDecoration == L"none")
                        enDecoration = NSConstValues::NSCssProperties::TextDecoration::normal;
                }

                void SetIndent(const std::wstring& sIndent)
                {
                    if (sIndent.empty())
                        return;

                    if (sIndent.find_first_not_of(L" 0") != std::wstring::npos)
                        fIndent = wcstof(sIndent.c_str(), NULL);
                }

                void SetColor(const std::wstring& sValue)
                {
                    if (sValue.empty())
                        return;

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
                        std::wstring sNewColor = sValue;
                        std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), tolower);

                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sColor = oHEX->second;
                    }
                }

                std::wstring GetIndentW() const
                {
                    if (fIndent == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fIndent + ((fIndent > 0.0f) ?  0.5f : -0.5f)));
                }

                std::wstring GetAlign() const
                {
                    if (enAlign == TextAlign::left)
                        return L"left";
                    if (enAlign == TextAlign::center)
                        return L"center";
                    if (enAlign == TextAlign::right)
                        return L"right";
                    if (enAlign == TextAlign::justify)
                        return L"both";

                    return std::wstring();
                }

                std::wstring GetDecoration() const
                {
                    if (enDecoration == TextDecoration::underline)
                        return L"single";
                    else if (enDecoration == TextDecoration::normal)
                        return L"none";

                    return std::wstring();
                }

                std::wstring GetColor() const
                {
                    return sColor;
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

            class BorderSide
            {
            public:
                float fWidth;
                std::wstring sStyle;
                std::wstring sColor; //HEX color

                BorderSide() : fWidth(fNoneValue),
                               sStyle(L"single"),
                               sColor(L"auto"){}

                BorderSide operator+=(const BorderSide& oBorderSide)
                {
                    if (oBorderSide.Empty())
                        return *this;

                    fWidth = oBorderSide.fWidth;
                    sStyle = oBorderSide.sStyle;
                    sColor = oBorderSide.sColor;

                    return *this;
                }

                bool operator==(const BorderSide& oBorderSide) const
                {
                    return  fWidth  == oBorderSide.fWidth &&
                            sStyle  == oBorderSide.sStyle &&
                            sColor  == oBorderSide.sColor;
                }

                bool Empty() const
                {
                    return fWidth  == fNoneValue &&
                           sStyle  == L"single"  &&
                           sColor  == L"auto";
                }

                void SetStyle(const std::wstring& sValue)
                {
                    if (sValue.empty())
                        return;

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
                    if (sValue.empty())
                        return;

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
                        const std::wstring sNewColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                        if (!sNewColor.empty())
                            sColor = sNewColor;
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
                    if (sValue.empty())
                        return BorderSide();

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
                            const float fWidth = wcstof(sValue.c_str(), NULL);
                            if (fWidth > 0.0f)
                                oBorderSide.fWidth = fWidth;
                        }
                        else if (sValue.substr(0, 3) == L"rgb")
                        {
                            const std::wstring sColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                            if (!sColor.empty())
                                 oBorderSide.sColor = sColor;
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

            class Border
            {
            public:
                BorderSide stTop;
                BorderSide stRight;
                BorderSide stBottom;
                BorderSide stLeft;

                Border operator+=(const Border& oBorder)
                {
                    stTop    += oBorder.stTop;
                    stRight  += oBorder.stRight;
                    stBottom += oBorder.stBottom;
                    stLeft   += oBorder.stLeft;

                    return *this;;
                }

                bool operator==(const Border& oBorder) const
                {
                    return stTop    == oBorder.stTop    &&
                           stRight  == oBorder.stRight  &&
                           stBottom == oBorder.stBottom &&
                           stLeft   == oBorder.stLeft;
                }

                bool EqualSides() const
                {
                    return  stTop    == stRight     &&
                            stRight  == stBottom    &&
                            stBottom == stLeft;
                }

                bool Empty() const
                {
                    return stTop.Empty() && stRight.Empty() && stBottom.Empty() && stLeft.Empty();
                }

                void SetWidth(const float& fValue)
                {
                    if (fValue <= 0.0f)
                        return;

                    stTop.fWidth    = fValue;
                    stRight.fWidth  = fValue;
                    stBottom.fWidth = fValue;
                    stLeft.fWidth   = fValue;
                }

                void SetStyle(const std::wstring& sValue)
                {
                    if (!sValue.empty())
                        return;

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

                void SetColor(const std::wstring& sValue)
                {
                    if (sValue.empty())
                        return;

                    std::wstring sNewColor;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sNewColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sNewColor.clear();
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

            class Background
            {
                std::wstring sColor;
            public:

                Background(){}

                Background operator+=(const Background& oBackground)
                {
                    if (oBackground.sColor.empty())
                        sColor = oBackground.sColor;

                    return *this;
                }

                bool operator==(const Background& oBackground) const
                {
                    return sColor == oBackground.sColor;
                }

                bool Empty() const
                {
                    return sColor.empty();
                }

                void SetBackground(const std::wstring &sBackground)
                {

                }

                void SetColor(const std::wstring &sValue)
                {
                    if (sValue.empty())
                        return;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                            sColor = sValue.substr(1, 7);
                        else if (sValue.length() == 4)
                        {
                            sColor.clear();
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
                        const std::wstring sNewColor = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(sValue);
                        if (!sNewColor.empty())
                            sColor = sNewColor;
                    }
                    else
                    {
                        std::wstring sNewColor = sValue;
                        std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), tolower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                            sColor = oHEX->second;
                    }
                }

                std::wstring GetColor() const
                {
                    return sColor;
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
