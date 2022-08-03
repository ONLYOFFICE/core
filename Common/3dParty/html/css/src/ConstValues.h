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

    typedef enum
    {
        ScalingDirectionNone = 0,
        ScalingDirectionX = 1,
        ScalingDirectionY = 2
    } ScalingDirection;

    struct CSizeWindow
    {
            unsigned short m_ushWidth;
            unsigned short m_ushHeight;

            CSizeWindow()
                    : m_ushWidth(0), m_ushHeight(0) {};
            CSizeWindow(unsigned short unWidth, unsigned short unHeight)
                    : m_ushWidth(unWidth), m_ushHeight(unHeight) {};

            bool Empty() const
            {
                    return ((0 == m_ushWidth) && (0 == m_ushHeight));
            }

            void Clear()
            {
                    m_ushWidth = m_ushHeight = 0;
            }

            bool operator==(const CSizeWindow& oSizeWindow) const
            {
                    return ((m_ushWidth == oSizeWindow.m_ushWidth) && (m_ushHeight == oSizeWindow.m_ushHeight));
            }
            bool operator!=(const CSizeWindow& oSizeWindow) const
            {
                    return ((m_ushWidth != oSizeWindow.m_ushWidth) || (m_ushHeight != oSizeWindow.m_ushHeight));
            }
    };

    struct StatistickElement
    {
        enum TypeElement
        {
            IsStyle = 0,
            IsId
        } m_enType;
        std::wstring sValue;

        bool operator<(const StatistickElement& oStatistickElement) const
        {
            return sValue < oStatistickElement.sValue;
        }
    };

    struct CTree
    {
        NSCSS::CNode m_oNode;
        std::vector<CTree> m_arrChild;

        inline static void CountingNumberRepetitions(const CTree &oTree, std::map<StatistickElement, unsigned int> &mStatictics)
        {
            if (!oTree.m_oNode.m_sId.empty())
                ++mStatictics[StatistickElement{StatistickElement::IsId, L'#' + oTree.m_oNode.m_sId}];
            if (!oTree.m_oNode.m_sStyle.empty())
                ++mStatictics[StatistickElement{StatistickElement::IsStyle, oTree.m_oNode.m_sStyle}];

            if (!oTree.m_arrChild.empty())
                for (const CTree& oChildren : oTree.m_arrChild)
                    CountingNumberRepetitions(oChildren, mStatictics);
        }
    };

    namespace NSConstValues
    {
        namespace NSMaps {
            const std::map<std::wstring, std::wstring> mColors {
                /* Red tones */
                {L"indianeed",          L"CD5C5C"}, {L"lightcoral",            L"F08080"}, {L"salmon",        L"FA8072"},
                {L"darksalmon",         L"E9967A"}, {L"lightsalmon",           L"FFA07A"}, {L"crimson",       L"DC143C"},
                {L"red",                L"FF0000"}, {L"firebrick",             L"B22222"}, {L"darkred",       L"8B0000"},
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
                {L"windowtext",         L"000000"},  {L"transparent",           L"000000"}
            };
        }

        static const std::vector<std::wstring> arDisplayValues =
        {
                /* <display-outside> values */
                L"block", L"inline", L"run-in",
                /* <display-inside> values */
                L"flow", L"flow-root", L"table", L"flex", L"grid", L"ruby",
                /* <display-outside> plus <display-inside> values */
                L"block flow", L"inline table", L"flex run-in",
                /* <display-listitem> values */
                L"list-item", L"list-item block", L"list-item inline", L"list-item flow",
                L"list-item flow-root", L"list-item block flow",
                L"list-item block flow-root", L"flow list-item block",
                /* <display-internal> values */
                L"table-row-group", L"table-header-group", L"table-footer-group",
                L"table-row", L"table-cell", L"table-column-group", L"table-column",
                L"table-caption", L"ruby-base", L"ruby-text", L"ruby-base-container",
                L"ruby-text-container",
                /* <display-box> values */
                L"contents", L"none",
                /* <display-legacy> values */
                L"inline-block", L"inline-table", L"inline-flex", L"inline-grid",
                /* Global values */
                L"inherit", L"initial", L"unset"
        };

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
                R_Highlight = 6,
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

                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;

            public:

                Font() : fSize      (24.0f),
                         enStretch  (FontStretch::none),
                         enStyle    (FontStyle::none),
                         enVariant  (FontVariant::none),
                         enWeight   (FontWeight::none),
                         fLineHeight(fNoneValue),
                         bImportants({false, false, false, false, false, false, false}),
                         arLevels   ({0, 0, 0, 0, 0, 0, 0}){};

                void ClearImportants()
                {
                    bImportants = {false, false, false, false, false, false, false};
                }

                Font operator+=(const Font& oFont)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(bImportants))
                        return *this;

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

                    bImportants = oFont.bImportants;
                    arLevels    = oFont.arLevels;

                    return *this;
                }

                static void FontEquation(Font &oFirstFont, Font &oSecondFont)
                {
                    if (oFirstFont.bImportants[0] && !oSecondFont.bImportants[0] && oFirstFont.fSize != fNoneValue)
                        oSecondFont.fSize = fNoneValue;
                    else if (oSecondFont.bImportants[0] && !oFirstFont.bImportants[0] && oSecondFont.fSize != fNoneValue)
                        oFirstFont.fSize = fNoneValue;
                    else if (oFirstFont.fSize != fNoneValue && oSecondFont.fSize != fNoneValue)
                    {
                        if (oFirstFont.arLevels[0] < oSecondFont.arLevels[0])
                            oFirstFont.fSize = fNoneValue;
                        else
                            oSecondFont.fSize = fNoneValue;
                    }

                    if (oFirstFont.bImportants[1] && !oSecondFont.bImportants[1] && oFirstFont.enStretch != FontStretch::none)
                        oSecondFont.enStretch = FontStretch::none;
                    else if (oSecondFont.bImportants[1] && !oFirstFont.bImportants[1] && oSecondFont.enStretch != FontStretch::none)
                        oFirstFont.enStretch = FontStretch::none;
                    else if (oFirstFont.enStretch != FontStretch::none && oSecondFont.enStretch != FontStretch::none)
                    {
                        if (oFirstFont.arLevels[1] < oSecondFont.arLevels[1])
                            oFirstFont.enStretch = FontStretch::none;
                        else
                            oSecondFont.enStretch = FontStretch::none;
                    }

                    if (oFirstFont.bImportants[2] && !oSecondFont.bImportants[2] && oFirstFont.enStyle != FontStyle::none)
                        oSecondFont.enStyle = FontStyle::none;
                    else if (oSecondFont.bImportants[2] && !oFirstFont.bImportants[2] && oSecondFont.enStyle != FontStyle::none)
                        oFirstFont.enStyle = FontStyle::none;
                    else if (oFirstFont.enStyle != FontStyle::none && oSecondFont.enStyle != FontStyle::none)
                    {
                        if (oFirstFont.arLevels[2] < oSecondFont.arLevels[2])
                            oFirstFont.enStyle = FontStyle::none;
                        else
                            oSecondFont.enStyle = FontStyle::none;
                    }

                    if (oFirstFont.bImportants[3] && !oSecondFont.bImportants[3] && oFirstFont.enVariant != FontVariant::none)
                        oSecondFont.enVariant = FontVariant::none;
                    else if (oSecondFont.bImportants[3] && !oFirstFont.bImportants[3] && oSecondFont.enVariant != FontVariant::none)
                        oFirstFont.enVariant = FontVariant::none;
                    else if (oFirstFont.enVariant != FontVariant::none && oSecondFont.enVariant != FontVariant::none)
                    {
                        if (oFirstFont.arLevels[3] < oSecondFont.arLevels[3])
                            oFirstFont.enVariant = FontVariant::none;
                        else
                            oSecondFont.enVariant = FontVariant::none;
                    }

                    if (oFirstFont.bImportants[4] && !oSecondFont.bImportants[4] && oFirstFont.enWeight != FontWeight::none)
                        oSecondFont.enWeight = FontWeight::none;
                    else if (oSecondFont.bImportants[4] && !oFirstFont.bImportants[4] && oSecondFont.enWeight != FontWeight::none)
                        oFirstFont.enWeight = FontWeight::none;
                    else if (oFirstFont.enWeight != FontWeight::none && oSecondFont.enWeight != FontWeight::none)
                    {
                        if (oFirstFont.arLevels[4] < oSecondFont.arLevels[4])
                            oFirstFont.enWeight = FontWeight::none;
                        else
                            oSecondFont.enWeight = FontWeight::none;
                    }

                    if (oFirstFont.bImportants[5] && !oSecondFont.bImportants[5] && oFirstFont.fLineHeight != fNoneValue)
                        oSecondFont.fLineHeight = fNoneValue;
                    else if (oSecondFont.bImportants[5] && !oFirstFont.bImportants[5] && oSecondFont.fLineHeight != fNoneValue)
                        oFirstFont.fLineHeight = fNoneValue;
                    else if (oFirstFont.fLineHeight != fNoneValue && oSecondFont.fLineHeight != fNoneValue)
                    {
                        if (oFirstFont.arLevels[5] < oSecondFont.arLevels[5])
                            oFirstFont.fLineHeight = fNoneValue;
                        else
                            oSecondFont.fLineHeight = fNoneValue;
                    }

                    if (oFirstFont.bImportants[6] && !oSecondFont.bImportants[6] && !oFirstFont.sFamily.empty())
                        oSecondFont.sFamily.clear();
                    else if (oSecondFont.bImportants[6] && !oFirstFont.bImportants[6] && !oSecondFont.sFamily.empty())
                        oFirstFont.sFamily.clear();
                    else if (!oFirstFont.sFamily.empty() && !oSecondFont.sFamily.empty())
                    {
                        if (oFirstFont.arLevels[6] < oSecondFont.arLevels[6])
                            oFirstFont.sFamily.clear();
                        else
                            oSecondFont.sFamily.clear();
                    }
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
                    return fSize == fNoneValue && enStretch == FontStretch::none &&
                           enStyle == FontStyle::none && enVariant == FontVariant::none &&
                           enWeight == FontWeight::none && fLineHeight == fNoneValue && sFamily.empty();
                }

                void Clear()
                {
                    fSize       = fNoneValue;
                    enStretch   = FontStretch::none;
                    enStyle     = FontStyle::none;
                    enVariant   = FontVariant::none;
                    enWeight    = FontWeight::none;
                    fLineHeight = fNoneValue;
                    sFamily     .clear();
                    bImportants  = {false, false, false, false, false, false, false};

                }

                void SetFont(const std::wstring &sFont, const unsigned int unLevel, const bool& bHardMode = false)
                {
                    if (sFont.empty() || (NS_STATIC_FUNCTIONS::IsTrue(bImportants) && !bHardMode))
                        return;

                    const std::vector<std::wstring> arValues = NSCSS::NS_STATIC_FUNCTIONS::GetWordsW(sFont, L" ,/");

                    bool bIsBrackets = false;
                    unsigned short nPosition = 0;

                    for (const std::wstring &sWord : arValues)
                    {
                        if (nPosition < 4)
                        {
                            if (sWord == L"normal")
                            {
                                if (!bImportants[2] || bHardMode)
                                {
                                    enStyle = FontStyle::normal;
                                    arLevels[2] = unLevel;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"italic")
                            {
                                if (!bImportants[2] || bHardMode)
                                {
                                    enStyle = FontStyle::italic;
                                    arLevels[2] = unLevel;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"oblique")
                            {
                                if (!bImportants[2] || bHardMode)
                                {
                                    enStyle = FontStyle::oblique;
                                    arLevels[2] = unLevel;
                                }
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
                                if (!bImportants[3] || bHardMode)
                                {
                                    enVariant = FontVariant::normal;
                                    arLevels[3] = unLevel;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"small-caps")
                            {
                                if (!bImportants[3] || bHardMode)
                                {
                                    enVariant = FontVariant::smallCaps;
                                    arLevels[3] = unLevel;
                                }
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
                            if (sWord == L"bold" || sWord == L"bolder" || ((sWord[0] == L'6' || sWord[0] == L'7' || sWord[0] == L'8' ||sWord[0] == L'9') && sWord.length() == 3))
                            {
                                if (!bImportants[4] || bHardMode)
                                {
                                    arLevels[4] = unLevel;
                                    enWeight = FontWeight::bold;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"normal" || ((sWord[0] == L'3' || sWord[0] == L'4' ||sWord[0] == L'5') && sWord.length() == 3))
                            {
                                if (!bImportants[4] || bHardMode)
                                {
                                    arLevels[4] = unLevel;
                                    enWeight = FontWeight::normal;
                                }
                                ++nPosition;
                                continue;
                            }
                        }

                        if (nPosition < 4 && enStretch == FontStretch::none)
                        {
                            if (sWord == L"ultra-condensed")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::ultraCondensed;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"extra-condensed")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::extraCondensed;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"condensed")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::condensed;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"semi-condensed")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::semiCondensed;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"normal")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::normal;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"semi-expanded")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::semiExpanded;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"extra-expanded")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::extraExpanded;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"ultra-expanded")
                            {
                                if (!bImportants[1] || bHardMode)
                                {
                                    arLevels[1] = unLevel;
                                    enStretch = FontStretch::ultraExpanded;
                                }
                                ++nPosition;
                                continue;
                            }
                            else if (sWord == L"inherit")
                            {
                                ++nPosition;
                                continue;
                            }
                        }

                        if ((nPosition < 5 && iswdigit(sWord[0])) || iswdigit(sWord[0]))
                        {
                            nPosition = 5;
                            if (!bImportants[0] || bHardMode)
                            {
                                arLevels[0] = unLevel;
                                fSize = wcstof(sWord.c_str(), NULL);
                            }
                            continue;
                        }

                        if (nPosition == 5 && iswdigit(sWord[0]))
                        {
                            if (!bImportants[5] || bHardMode)
                            {
                                arLevels[5] = unLevel;
                                fLineHeight = wcstof(sWord.c_str(), NULL);
                            }
                            ++nPosition;
                            continue;
                        }

                        if (nPosition >= 5)
                        {
                            if (bIsBrackets)
                            {
                                arLevels[6] = unLevel;
                                if (!bImportants[6] || bHardMode)
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
                                    if (!bImportants[6] || bHardMode)
                                        sFamily = sWord;
                                }

                                if (sWord.back() == L'"' || sWord.back() == L'\'')
                                    bIsBrackets = false;

                                continue;
                            }
                        }
                    }
                }

                void SetFontFamily(const std::wstring &sFamily, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sFamily.empty() || (bImportants[6] && !bHardMode) || NS_STATIC_FUNCTIONS::NumberInWString(sFamily))
                        return;

                    std::wstring sNewFamily(sFamily);

                    sNewFamily.erase(std::remove(sNewFamily.begin(), sNewFamily.end(), '\''), sNewFamily.end());
                    sNewFamily.erase(std::remove(sNewFamily.begin(), sNewFamily.end(), '"'), sNewFamily.end());

                    std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sNewFamily, L",");

                    for (std::vector<std::wstring>::iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
                    {
                        if ((*iWord).empty())
                            continue;

                        arLevels[6] = unLevel;
                        this->sFamily = L'"' + *iWord + L'"';
                        return;
                    }

//                    std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sFamily, L",");

//                    for (std::vector<std::wstring>::reverse_iterator iWord = arWords.rbegin() + ((arWords.size() == 1) ? 0 : 1); iWord != arWords.rend(); ++iWord)
//                    {
//                        if ((*iWord).empty())
//                            continue;

//                        const std::wstring sValue = NS_STATIC_FUNCTIONS::StripSymbols(*iWord, L" ");

//                        if (sValue[0] == L'"' || sValue[0] == L'\'')
//                        {
//                            arLevels[6] = unLevel;
//                            this->sFamily = sValue;
//                            return;
//                        }
//                        else if (sValue != L"serif"      && sValue != L"sans-serif"  && sValue != L"monospace" &&
//                                 sValue != L"cursive"    && sValue != L"fantasy"     && sValue != L"system-ui" &&
//                                 sValue != L"emoji"      && sValue != L"math"        && sValue != L"fangsong" &&
//                                 sValue != L"inherit"    && sValue != L"initial"     && sValue != L"unset")
//                        {
//                            arLevels[6] = unLevel;
//                            this->sFamily = L'\'' + sValue + L'\'';
//                            return;
//                        }
//                    }
                }

                void SetSize(const std::wstring &sSize, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sSize.empty() || (bImportants[0] && !bHardMode))
                        return;

                    const float fValue = wcstof(sSize.c_str(), NULL);
                    if (fValue > 0.0f)
                    {
                        arLevels[0] = unLevel;
                        fSize = fValue;
                    }
                }

                void SetStretch(const std::wstring &sStretch, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sStretch.empty() || (bImportants[1] && !bHardMode))
                        return;

                    if (sStretch == L"ultra-condensed")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::ultraCondensed;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"extra-condensed")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::extraCondensed;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"condensed")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::condensed;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"semi-condensed")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::semiCondensed;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"normal")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::normal;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"semi-expanded")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::semiExpanded;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"expanded")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::expanded;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"extra-expanded")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::extraExpanded;
                        arLevels[1] = unLevel;
                    }
                    else if (sStretch == L"ultra-expanded")
                    {
                        enStretch = NSConstValues::NSCssProperties::FontStretch::ultraExpanded;
                        arLevels[1] = unLevel;
                    }
                }

                void SetStyle(const std::wstring &sStyle, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sStyle.empty() || (bImportants[2] && !bHardMode))
                        return;

                    if (sStyle == L"italic")
                    {
                        arLevels[2] = unLevel;
                        enStyle = NSConstValues::NSCssProperties::FontStyle::italic;
                    }
                    else if (sStyle == L"oblique")
                    {
                        arLevels[2] = unLevel;
                        enStyle = NSConstValues::NSCssProperties::FontStyle::oblique;
                    }
                    else if (sStyle == L"normal")
                    {
                        arLevels[2] = unLevel;
                        enStyle = NSConstValues::NSCssProperties::FontStyle::normal;
                    }
                }

                void SetVariant(const std::wstring &sVariant, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sVariant.empty() || (bImportants[3] && !bHardMode))
                        return;

                    if (sVariant == L"small-caps")
                    {
                        arLevels[3] = unLevel;
                        enVariant = NSConstValues::NSCssProperties::FontVariant::smallCaps;
                    }
                    else if (sVariant == L"normal")
                    {
                        arLevels[3] = unLevel;
                        enVariant = NSConstValues::NSCssProperties::FontVariant::normal;
                    }
                }

                void SetWeight(const std::wstring &sWeight, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sWeight.empty() || (bImportants[4] && !bHardMode))
                        return;

                    if (sWeight == L"bold" || sWeight == L"bolder" ||
                        sWeight[0] == L'6' || sWeight[0] == L'7' ||
                        sWeight[0] == L'8' || sWeight[0] == L'9')
                    {
                        arLevels[4] = unLevel;
                        enWeight = NSConstValues::NSCssProperties::FontWeight::bold;
                    }
                    else if (sWeight == L"normal" || sWeight[0] == L'3' ||
                             sWeight[0] == L'4'   || sWeight[0] == L'5')
                    {
                        arLevels[4] = unLevel;
                        enWeight = NSConstValues::NSCssProperties::FontWeight::normal;
                    }
                }

                void SetLineHeight(const std::wstring &sLineHeight, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sLineHeight.empty() || (bImportants[5] && !bHardMode) || unLevel < arLevels[5])
                        return;

                    if (sLineHeight == L"normal")
                    {
                        arLevels[5] = unLevel;
                        fLineHeight = 24;
                    }
                    else
                    {
                        const float fValue = wcstof(sLineHeight.c_str(), NULL);
                        if (fValue > 0.0f)
                        {
                            arLevels[5] = unLevel;
                            fLineHeight = fValue;
                        }
                    }
                }

                void SetImportantAll(const bool &bImportant)
                {
                    if (bImportant)
                        bImportants = {true, true, true, true, true, true, true};
                    else
                        bImportants = {false, false, false, false, false, false, false};
                }

                void SetImportantSize(const bool &bImportant)
                {
                    bImportants[0] = bImportant;
                }

                void SetImportantenStretch(const bool &bImportant)
                {
                    bImportants[1] = bImportant;
                }

                void SetImportantenStyle(const bool &bImportant)
                {
                    bImportants[2] = bImportant;
                }

                void SetImportantenVariant(const bool &bImportant)
                {
                    bImportants[3] = bImportant;
                }

                void SetImportantenWeight(const bool &bImportant)
                {
                    bImportants[4] = bImportant;
                }

                void SetImportantenLineHeight(const bool &bImportant)
                {
                    bImportants[5] = bImportant;
                }

                void SetImportantenFamily(const bool &bImportant)
                {
                    bImportants[6] = bImportant;
                }

                float GetSize() const
                {
                    return (fSize == fNoneValue) ? 22.0f : fSize;
                }

                std::wstring GetSizeW() const
                {
                    if (fSize == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<unsigned short>(fSize + 0.5f));
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

                    return std::to_wstring(static_cast<unsigned short>(fLineHeight * 10.0f + 0.5f));
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

                bool bPermission;
                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;

            public:
                Margin() : fTopSide     (fNoneValue),
                           fRightSide   (fNoneValue),
                           fBottomSide  (fNoneValue),
                           fLeftSide    (fNoneValue),
                           bPermission  (true),
                           bImportants  ({false, false, false, false}),
                           arLevels     ({0, 0, 0, 0}){}

                void ClearImportants()
                {
                    bImportants = {false, false, false, false};
                }

                Margin operator+=(const Margin& oMargin)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(bImportants))
                        return *this;

                    if (oMargin.fTopSide != fNoneValue)
                        fTopSide = oMargin.fTopSide;
                    if (oMargin.fRightSide != fNoneValue)
                        fRightSide = oMargin.fRightSide;
                    if (oMargin.fBottomSide != fNoneValue)
                        fBottomSide = oMargin.fBottomSide;
                    if (oMargin.fLeftSide != fNoneValue)
                        fLeftSide = oMargin.fLeftSide;

                    bImportants = oMargin.bImportants;
                    arLevels    = oMargin.arLevels;

                    return *this;;
                }

                static void MarginEquation(Margin &oFirstMargin, Margin &oSecondMargin)
                {
                    if (oFirstMargin.bImportants[0] && !oSecondMargin.bImportants[0] && oFirstMargin.fTopSide != fNoneValue)
                        oSecondMargin.fTopSide = fNoneValue;
                    else if (oSecondMargin.bImportants[0] && !oFirstMargin.bImportants[0] && oSecondMargin.fTopSide != fNoneValue)
                        oFirstMargin.fTopSide = fNoneValue;
                    else if (oFirstMargin.fTopSide != fNoneValue && oSecondMargin.fTopSide != fNoneValue)
                    {
                        if (oFirstMargin.arLevels[0] < oSecondMargin.arLevels[0])
                            oFirstMargin.fTopSide = fNoneValue;
                        else
                            oSecondMargin.fTopSide += oFirstMargin.fTopSide;
                    }

                    if (oFirstMargin.bImportants[1] && !oSecondMargin.bImportants[1] && oFirstMargin.fRightSide != fNoneValue)
                        oSecondMargin.fRightSide = fNoneValue;
                    else if (oSecondMargin.bImportants[1] && !oFirstMargin.bImportants[1] && oSecondMargin.fRightSide != fNoneValue)
                        oFirstMargin.fRightSide = fNoneValue;
                    else if (oFirstMargin.fRightSide != fNoneValue && oSecondMargin.fRightSide != fNoneValue)
                    {
                        if (oFirstMargin.arLevels[1] < oSecondMargin.arLevels[1])
                            oFirstMargin.fRightSide = fNoneValue;
                        else
                            oSecondMargin.fRightSide += oFirstMargin.fRightSide;
                    }

                    if (oFirstMargin.bImportants[2] && !oSecondMargin.bImportants[2] && oFirstMargin.fBottomSide != fNoneValue)
                        oSecondMargin.fBottomSide = fNoneValue;
                    else if (oSecondMargin.bImportants[2] && !oFirstMargin.bImportants[2] && oSecondMargin.fBottomSide != fNoneValue)
                        oFirstMargin.fBottomSide = fNoneValue;
                    else if (oFirstMargin.fBottomSide != fNoneValue && oSecondMargin.fBottomSide != fNoneValue)
                    {
                        if (oFirstMargin.arLevels[2] < oSecondMargin.arLevels[2])
                            oFirstMargin.fBottomSide = fNoneValue;
                        else
                            oSecondMargin.fBottomSide += oFirstMargin.fBottomSide;
                    }

                    if (oFirstMargin.bImportants[3] && !oSecondMargin.bImportants[3] && oFirstMargin.fLeftSide != fNoneValue)
                        oSecondMargin.fLeftSide = fNoneValue;
                    else if (oSecondMargin.bImportants[3] && !oFirstMargin.bImportants[3] && oSecondMargin.fLeftSide != fNoneValue)
                        oFirstMargin.fLeftSide = fNoneValue;
                    else if (oFirstMargin.fLeftSide != fNoneValue && oSecondMargin.fLeftSide != fNoneValue)
                    {
                        if (oFirstMargin.arLevels[3] < oSecondMargin.arLevels[3])
                            oFirstMargin.fLeftSide = fNoneValue;
                        else
                            oSecondMargin.fLeftSide += oFirstMargin.fLeftSide;
                    }
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
                    return fTopSide == fNoneValue && fRightSide == fNoneValue && fBottomSide == fNoneValue && fLeftSide == fNoneValue;
                };

                void SetPermission(const bool &bPermission)
                {
                    this->bPermission = bPermission;
                }

                void SetImportantAll(const bool &bImportant)
                {
                    if (bImportant)
                        bImportants = {true, true, true, true};
                    else
                        bImportants = {false, false, false, false};
                }

                void SetImportantTopSide(const bool &bImportant)
                {
                    bImportants[0] = bImportant;
                }

                void SetImportantRightSide(const bool &bImportant)
                {
                    bImportants[1] = bImportant;
                }

                void SetImportantBottomSide(const bool &bImportant)
                {
                    bImportants[2] = bImportant;
                }

                void SetImportantLeftSide(const bool &bImportant)
                {
                    bImportants[3] = bImportant;
                }

                void AddMargin(const std::wstring& sMargin, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sMargin.empty() || !bPermission || (NS_STATIC_FUNCTIONS::IsTrue(bImportants) && !bHardMode))
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

                            if (!bImportants[0] || bHardMode)
                            {
                                arLevels[0] = unLevel;
                                fTopSide    += fValue;
                            }
                            if (!bImportants[1] || bHardMode)
                            {
                                arLevels[1] = unLevel;
                                fRightSide  += fValue;
                            }
                            if (!bImportants[2] || bHardMode)
                            {
                                arLevels[2] = unLevel;
                                fBottomSide += fValue;
                            }
                            if (!bImportants[3] || bHardMode)
                            {
                                arLevels[3] = unLevel;
                                fLeftSide   += fValue;
                            }
                            break;
                        }
                        case 2:
                        {
                            const float fValue1 = wcstof(arValues[0].c_str(), NULL);
                            const float fValue2 = wcstof(arValues[1].c_str(), NULL);

                            if (!bImportants[0] || bHardMode)
                            {
                                arLevels[0] = unLevel;
                                fTopSide    += fValue1;
                            }
                            if (!bImportants[1] || bHardMode)
                            {
                                arLevels[1] = unLevel;
                                fRightSide  += fValue2;
                            }
                            if (!bImportants[2] || bHardMode)
                            {
                                arLevels[2] = unLevel;
                                fBottomSide += fValue1;
                            }
                            if (!bImportants[3] || bHardMode)
                            {
                                arLevels[3] = unLevel;
                                fLeftSide   += fValue2;
                            }
                            break;
                        }
                        case 3:
                        {
                            const float fValue = wcstof(arValues[1].c_str(), NULL);
                            if (!bImportants[0] || bHardMode)
                            {
                                arLevels[0] = unLevel;
                                fTopSide    += wcstof(arValues[0].c_str(), NULL);
                            }
                            if (!bImportants[1] || bHardMode)
                            {
                                arLevels[1] = unLevel;
                                fRightSide  += fValue;
                            }
                            if (!bImportants[2] || bHardMode)
                            {
                                arLevels[2] = unLevel;
                                fBottomSide += wcstof(arValues[2].c_str(), NULL);
                            }
                            if (!bImportants[3] || bHardMode)
                            {
                                arLevels[3] = unLevel;
                                fLeftSide   += fValue;
                            }
                            break;
                        }
                        case 4:
                        {
                            if (!bImportants[0] || bHardMode)
                            {
                                arLevels[0] = unLevel;
                                fTopSide    += wcstof(arValues[0].c_str(), NULL);
                            }
                            if (!bImportants[1] || bHardMode)
                            {
                                arLevels[1] = unLevel;
                                fRightSide  += wcstof(arValues[1].c_str(), NULL);
                            }
                            if (!bImportants[2] || bHardMode)
                            {
                                arLevels[2] = unLevel;
                                fBottomSide += wcstof(arValues[2].c_str(), NULL);
                            }
                            if (!bImportants[3] || bHardMode)
                            {
                                arLevels[3] = unLevel;
                                fLeftSide   += wcstof(arValues[3].c_str(), NULL);
                            }
                            break;
                        }
                    }
                };

                void AddTopMargin(const std::wstring& sTopMargin, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sTopMargin.empty() || !bPermission || (bImportants[0] && !bHardMode))
                        return;

                    if (fTopSide == fNoneValue)
                        fTopSide = 0;

                    arLevels[0] = unLevel;
                    fTopSide += wcstof(sTopMargin.c_str(), NULL);
                }

                void AddRightMargin(const std::wstring& sRightMargin, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sRightMargin.empty() || !bPermission || (bImportants[1] && !bHardMode))
                        return;

                    if (fRightSide == fNoneValue)
                        fRightSide = 0;

                    arLevels[1] = unLevel;
                    fRightSide += wcstof(sRightMargin.c_str(), NULL);
                }

                void AddBottomMargin(const std::wstring& sBottomMargin, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sBottomMargin.empty() || !bPermission || (bImportants[2] && !bHardMode))
                        return;

                    if (fBottomSide == fNoneValue)
                        fBottomSide = 0;

                    arLevels[2] = unLevel;
                    fBottomSide += wcstof(sBottomMargin.c_str(), NULL);
                }

                void AddLeftMargin(const std::wstring& sLeftMargin, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sLeftMargin.empty() || !bPermission || (bImportants[3] && !bHardMode))
                        return;

                    if (fLeftSide == fNoneValue)
                        fLeftSide = 0;

                    arLevels[3] = unLevel;
                    fLeftSide += wcstof(sLeftMargin.c_str(), NULL);
                }

                bool GetPermission() const
                {
                        return bPermission;
                }

                double GetTopSide() const
                {
                        if (fTopSide < 0)
                                return 0;

                        return fTopSide;
                }

                std::wstring GetTopSideW() const
                {
                    if (fTopSide < 0)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fTopSide  * 10.0f + 0.5f));
                }

                double GetRightSide() const
                {
                        if (fRightSide < 0)
                                return 0;

                        return fRightSide;
                }

                std::wstring GetRightSideW() const
                {
                    if (fRightSide < 0)
                        return std::wstring();

                    return (fRightSide >= -128.0f) ? std::to_wstring(static_cast<short int>(fRightSide  * 10.0f + ((fLeftSide > 0) ? 0.5f : -0.5f))) : L"-1280";
                }

                double GetBottomSide() const
                {
                        if (fBottomSide < 0)
                                return 0;

                        return fBottomSide;
                }

                std::wstring GetBottomSideW() const
                {
                    if (fBottomSide < 0)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fBottomSide  * 10.0f + 0.5f));
                }

                double GetLeftSide() const
                {
                        if (fLeftSide < 0)
                                return 0;

                        return fLeftSide;
                }

                std::wstring GetLeftSideW() const
                {
                    if (fLeftSide < 0)
                        return std::wstring();

                    return (fLeftSide >= -128.0f) ? std::to_wstring(static_cast<short int>(fLeftSide  * 10.0f + ((fLeftSide > 0) ? 0.5f : -0.5f))) : L"-1280";
                }
            };

            enum class TextDecoration
            {
                none = 0,
                underline,
                lineThrough,
                normal
            };

            enum class Align
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
                Align enAlign;
                TextDecoration enDecoration;
                std::wstring sColor; //HEX color

                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;

            public:

                Text() : fIndent        (fNoneValue),
                         enAlign        (Align::none),
                         enDecoration   (TextDecoration::none),
                         bImportants    ({false, false, false, false}),
                         arLevels       ({0, 0, 0, 0}){}

                void ClearImportants()
                {
                    bImportants = {false, false, false, false};
                }

                Text operator+=(const Text& oText)
                {
                    if (oText.fIndent != fNoneValue)
                        fIndent = oText.fIndent;
                    if (oText.enAlign != Align::none)
                        enAlign = oText.enAlign;
                    if (oText.enDecoration != TextDecoration::none)
                        enDecoration = oText.enDecoration;
                    if (!oText.sColor.empty())
                        sColor = oText.sColor;

                    bImportants = oText.bImportants;

                    return *this;
                }

                static void TextEquation(Text &oFirstText, Text &oSecondText)
                {
                    if (oFirstText.bImportants[0] && !oSecondText.bImportants[0] && oFirstText.fIndent != fNoneValue)
                        oSecondText.fIndent = fNoneValue;
                    else if (oSecondText.bImportants[0] && !oFirstText.bImportants[0] && oSecondText.fIndent != fNoneValue)
                        oFirstText.fIndent = fNoneValue;
                    else if (oFirstText.fIndent != fNoneValue && oSecondText.fIndent != fNoneValue)
                    {
                        if (oFirstText.arLevels[0] < oSecondText.arLevels[0])
                            oFirstText.fIndent = fNoneValue;
                        else
                            oSecondText.fIndent = fNoneValue;
                    }

                    if (oFirstText.bImportants[1] && !oSecondText.bImportants[1] && oFirstText.enAlign != Align::none)
                        oSecondText.enAlign = Align::none;
                    else if (oSecondText.bImportants[1] && !oFirstText.bImportants[1] && oSecondText.enAlign != Align::none)
                        oFirstText.enAlign = Align::none;
                    else if (oFirstText.enAlign != Align::none && oSecondText.enAlign != Align::none)
                    {
                        if (oFirstText.arLevels[1] < oSecondText.arLevels[1])
                            oFirstText.enAlign = Align::none;
                        else
                            oSecondText.enAlign = Align::none;
                    }

                    if (oFirstText.bImportants[2] && !oSecondText.bImportants[2] && oFirstText.enDecoration != TextDecoration::none)
                        oSecondText.enDecoration = TextDecoration::none;
                    else if (oSecondText.bImportants[2] && !oFirstText.bImportants[2] && oSecondText.enDecoration != TextDecoration::none)
                        oFirstText.enDecoration = TextDecoration::none;
                    else if (oFirstText.enDecoration != TextDecoration::none && oSecondText.enDecoration != TextDecoration::none)
                    {
                        if (oFirstText.arLevels[2] < oSecondText.arLevels[2])
                            oFirstText.enDecoration = TextDecoration::none;
                        else
                            oSecondText.enDecoration = TextDecoration::none;
                    }

                    if (oFirstText.bImportants[3] && !oSecondText.bImportants[3] && !oFirstText.sColor.empty())
                        oSecondText.sColor.clear();
                    else if(oSecondText.bImportants[3] && !oFirstText.bImportants[3] && !oSecondText.sColor.empty())
                        oFirstText.sColor.clear();
                    else if (!oFirstText.sColor.empty() && !oSecondText.sColor.empty())
                    {
                        if (oFirstText.arLevels[3] < oSecondText.arLevels[3])
                             oFirstText.sColor.clear();
                        else
                             oSecondText.sColor.clear();
                    }
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
                    return fIndent == fNoneValue && enAlign == Align::none &&
                           enDecoration == TextDecoration::none && sColor.empty();
                }

                void SetAlign(const std::wstring& sAlign, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sAlign.empty() || (bImportants[1] && !bHardMode) || unLevel < arLevels[1])
                        return;

                    if (sAlign == L"center")
                    {
                        arLevels[1] = unLevel;
                        enAlign = NSConstValues::NSCssProperties::Align::center;
                    }
                    else if(sAlign == L"justify")
                    {
                        arLevels[1] = unLevel;
                        enAlign = NSConstValues::NSCssProperties::Align::justify;
                    }
                    else if(sAlign == L"left" || sAlign == L"start")
                    {
                        arLevels[1] = unLevel;
                        enAlign = NSConstValues::NSCssProperties::Align::left;
                    }
                    else if(sAlign == L"right" || sAlign == L"end")
                    {
                        arLevels[1] = unLevel;
                        enAlign = NSConstValues::NSCssProperties::Align::right;
                    }
                }

                void SetDecoration(const std::wstring sDecoration, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sDecoration.empty() || (bImportants[2] && !bHardMode))
                        return;

                    if (sDecoration == L"underline")
                    {
                        arLevels[2] = unLevel;
                        enDecoration = NSConstValues::NSCssProperties::TextDecoration::underline;
                    }
                    else if (sDecoration == L"line-through")
                    {
                            arLevels[2] = unLevel;
                            enDecoration = NSConstValues::NSCssProperties::TextDecoration::lineThrough;
                    }
                    else if (sDecoration == L"none")
                    {
                        arLevels[2] = unLevel;
                        enDecoration = NSConstValues::NSCssProperties::TextDecoration::normal;
                    }
                }

                void SetIndent(const std::wstring& sIndent, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sIndent.empty() || (bImportants[0] && !bHardMode))
                        return;

                    if (sIndent.find_first_not_of(L" 0") != std::wstring::npos)
                    {
                        fIndent = wcstof(sIndent.c_str(), NULL) * 10.0f;
                        arLevels[0] = unLevel;
                    }
                }

                void SetColor(const std::wstring& sValue, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sValue.empty() || (bImportants[3] && !bHardMode))
                        return;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                        {
                            arLevels[3] = unLevel;
                            sColor = sValue.substr(1, 7);
                        }
                        else if (sValue.length() == 4)
                        {
                            sColor.clear();
                            arLevels[3] = unLevel;
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
                        {
                            arLevels[3] = unLevel;
                            sColor = sNewColor;
                        }
                    }
                    else
                    {
                        std::wstring sNewColor = sValue;
                        std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), towlower);

                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                        {
                            arLevels[3] = unLevel;
                            sColor = oHEX->second;
                        }
                    }
                }

                void SetImportantAll(const bool& bImportant)
                {
                    if (bImportant)
                        bImportants = {true, true, true, true};
                    else
                        bImportants = {false, false, false, false};
                }

                void SetImportantIndent(const bool& bImportant)
                {
                    bImportants[0] = bImportant;
                }

                void SetImportantAlign(const bool& bImportant)
                {
                    bImportants[1] = bImportant;
                }

                void SetImportantDecoration(const bool& bImportant)
                {
                    bImportants[2] = bImportant;
                }

                void SetImportantColor(const bool& bImportant)
                {
                    bImportants[3] = bImportant;
                }

                std::wstring GetIndentW() const
                {
                    if (fIndent == fNoneValue)
                        return std::wstring();

                    return std::to_wstring(static_cast<short int>(fIndent + ((fIndent > 0.0f) ?  0.5f : -0.5f)));
                }

                std::wstring GetAlign() const
                {
                    if (enAlign == Align::left)
                        return L"left";
                    if (enAlign == Align::center)
                        return L"center";
                    if (enAlign == Align::right)
                        return L"right";
                    if (enAlign == Align::justify)
                        return L"both";

                    return std::wstring();
                }

                std::wstring GetDecoration() const
                {
                    if (enDecoration == TextDecoration::underline)
                        return L"single";
                    else if (enDecoration == TextDecoration::lineThrough)
                        return L"line-through";
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
                float fWidth;
                std::wstring sStyle;
                std::wstring sColor; //HEX color

                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;

                bool bBlock;

            public:

                BorderSide() : fWidth(fNoneValue),
                               sStyle(L"auto"),
                               sColor(L"auto"),
                               bImportants({false, false, false}),
                               arLevels({0, 0, 0}),
                               bBlock(false){}

                void ClearImportants()
                {
                    bImportants = {false, false, false};
                }

                void Block()
                {
                    bBlock = true;
                }

                void Unlock()
                {
                    bBlock = false;
                }

                BorderSide operator=(const BorderSide& oBorderSide)
                {
                    if (oBorderSide.Empty())
                        return *this;

                    fWidth = oBorderSide.fWidth;
                    sStyle = oBorderSide.sStyle;
                    sColor = oBorderSide.sColor;

                    bImportants = oBorderSide.bImportants;
                    arLevels    = oBorderSide.arLevels;

                    return *this;
                }

                static void BorderSideEquation(BorderSide &oFirstBorderSide, BorderSide &oSecondBorderSide)
                {
                    if (oFirstBorderSide.bImportants[0] && !oSecondBorderSide.bImportants[0] && oFirstBorderSide.fWidth != fNoneValue)
                        oSecondBorderSide.fWidth = fNoneValue;
                    else if (oSecondBorderSide.bImportants[0] && !oFirstBorderSide.bImportants[0] && oSecondBorderSide.fWidth != fNoneValue)
                        oFirstBorderSide.fWidth = fNoneValue;
                    else if (oFirstBorderSide.fWidth != fNoneValue && oSecondBorderSide.fWidth != fNoneValue)
                    {
                        if (oFirstBorderSide.arLevels[0] < oSecondBorderSide.arLevels[0])
                            oFirstBorderSide.fWidth = fNoneValue;
                        else
                            oSecondBorderSide.fWidth = fNoneValue;
                    }

                    if (oFirstBorderSide.bImportants[1] && !oSecondBorderSide.bImportants[1] && !oFirstBorderSide.sStyle.empty())
                        oSecondBorderSide.sStyle = L"auto";
                    else if (oSecondBorderSide.bImportants[1] && !oFirstBorderSide.bImportants[1] && !oSecondBorderSide.sStyle.empty())
                        oFirstBorderSide.sStyle = L"auto";
                    else if (!oFirstBorderSide.sStyle.empty() && !oSecondBorderSide.sStyle.empty())
                    {
                        if (oFirstBorderSide.arLevels[1] < oSecondBorderSide.arLevels[1])
                            oFirstBorderSide.sStyle = L"auto";
                        else
                            oSecondBorderSide.sStyle = L"auto";
                    }

                    if (oFirstBorderSide.bImportants[2] && !oSecondBorderSide.bImportants[2] && !oFirstBorderSide.sColor.empty())
                        oSecondBorderSide.sColor = L"auto";
                    else if (oSecondBorderSide.bImportants[2] && !oFirstBorderSide.bImportants[2] && !oSecondBorderSide.sColor.empty())
                        oFirstBorderSide.sColor = L"auto";
                    else if (!oFirstBorderSide.sColor.empty() && !oSecondBorderSide.sColor.empty())
                    {
                        if (oFirstBorderSide.arLevels[2] < oSecondBorderSide.arLevels[2])
                            oFirstBorderSide.sColor = L"auto";
                        else
                            oSecondBorderSide.sColor = L"auto";
                    }
                }

                bool operator==(const BorderSide& oBorderSide) const
                {
                    return  fWidth  == oBorderSide.fWidth &&
                            sStyle  == oBorderSide.sStyle &&
                            sColor  == oBorderSide.sColor;
                }

                bool Empty() const
                {
                    return fWidth < 0;
                }

                void SetWidthWithoutChecking(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (bImportants[0] && !bHardMode)
                    {
                        arLevels[0] = unLevel;
                        this->fWidth = fWidth;
                    }
                }

                void SetStyleWithoutChecking(const std::wstring& sStyle, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (bImportants[1] && !bHardMode)
                    {
                        arLevels[1] = unLevel;
                        this->sStyle = sStyle;
                    }
                }

                void SetColorWithoutChecking(const std::wstring& sColor, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (bImportants[2] && !bHardMode)
                    {
                        arLevels[2] = unLevel;
                        this->sColor = sColor;
                    }
                }

                void SetWidth(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (fWidth >= 0.0f && bImportants[0] && !bHardMode)
                    {
                        arLevels[0] = unLevel;
                        this->fWidth = fWidth;
                    }
                }

                void SetStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (sValue.empty() || (bImportants[1] && !bHardMode))
                        return;

                    std::wstring sNewValue = sValue;
                    if (!sNewValue.empty())
                    {
                        std::transform(sNewValue.begin(), sNewValue.end(), sNewValue.begin(), towlower);

                        if (sNewValue == L"dotted")
                        {
                            arLevels[1] = unLevel;
                            sStyle = sNewValue;
                        }
                        else if (sNewValue == L"dashed")
                        {
                            arLevels[1] = unLevel;
                            sStyle = sNewValue;
                        }
                        else if (sNewValue == L"solid")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"single";
                        }
                        else if (sNewValue == L"double")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"double";
                        }
                        else if (sNewValue == L"groove")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"threeDEmboss";
                        }
                        else if (sNewValue == L"ridge")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"threeDEngrave";
                        }
                        else if (sNewValue == L"inset")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"thinThickMediumGap";
                        }
                        else if (sNewValue == L"outset")
                        {
                            arLevels[1] = unLevel;
                            sStyle = L"thickThinMediumGap";
                        }
                    }
                }

                void SetColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (sValue.empty() || (bImportants[2] && !bHardMode))
                        return;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 6)
                        {
                            arLevels[2] = unLevel;
                            sColor = sValue;
                        }
                        else if (sValue.length() == 7)
                        {
                            arLevels[2] = unLevel;
                            sColor = sValue.substr(1, 7);
                        }
                        else if (sValue.length() == 4)
                        {
                            sColor.clear();
                            arLevels[2] = unLevel;
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
                        {
                            arLevels[2] = unLevel;
                            sColor = sNewColor;
                        }
                    }
                    else
                    {
                        std::wstring sNewColor = sValue;
                        std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), towlower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.cend())
                        {
                            arLevels[2] = unLevel;
                            sColor = oHEX->second;
                        }
                    }
                }

                void SetImportantAll(const bool &bImportant)
                {
                    if (bImportant)
                        bImportants = {true, true, true};
                    else
                        bImportants = {false, false, false};
                }

                void SetImportantWidth(const bool &bImportant)
                {
                    bImportants[0] = bImportant;
                }

                void SetImportantStyle(const bool &bImportant)
                {
                    bImportants[1] = bImportant;
                }

                void SetImportantColor(const bool &bImportant)
                {
                    bImportants[2] = bImportant;
                }

                bool GetImportantWidth() const
                {
                    return bImportants[0];
                }

                bool GetImportantStyle() const
                {
                    return bImportants[1];
                }

                bool GetImportantColor() const
                {
                    return bImportants[2];
                }

                std::vector<bool> GetImportants() const
                {
                    return bImportants;
                }

                std::wstring GetStyle() const
                {
                    if (bBlock) return L"auto";
                    if (0 == fWidth) return L"none";
                    else if (L"auto" == sStyle) return L"single";
                    else return sStyle;
                }

                std::wstring GetColor() const
                {
                    return (bBlock) ? L"auto" : sColor;
                }

                std::wstring GetWidthW() const
                {
                    if (fWidth < 0)
                        return L"0";

                    return (bBlock) ? L"0" : std::to_wstring(static_cast<unsigned short>(fWidth + 0.5f));
                }

                float GetWidth() const
                {
                    return fWidth;
                }

                void SetAllLevels(const unsigned int& unLevel)
                {
                    for (size_t i = 0; i < arLevels.size(); ++i)
                        arLevels[i] = unLevel;
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
                            if (fWidth >= 0.0f)
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
                            std::transform(sValue.begin(), sValue.end(), sValue.begin(), towlower);

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
                            else if (sValue == L"none")
                                oBorderSide.sStyle = L"none";
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
                BorderSide oTop;
                BorderSide oRight;
                BorderSide oBottom;
                BorderSide oLeft;

            public:

                void ClearImportants()
                {
                    oTop.ClearImportants();
                    oRight.ClearImportants();
                    oBottom.ClearImportants();
                    oLeft.ClearImportants();
                }

                Border operator=(const Border& oBorder)
                { 
                    oTop    = oBorder.oTop;
                    oRight  = oBorder.oRight;
                    oBottom = oBorder.oBottom;
                    oLeft   = oBorder.oLeft;

                    return *this;
                }

                static void BorderEquation(Border &oFirstBorder, Border &oSecondBorder)
                {
                    BorderSide::BorderSideEquation(oFirstBorder.oTop, oSecondBorder.oTop);
                    BorderSide::BorderSideEquation(oFirstBorder.oRight, oSecondBorder.oRight);
                    BorderSide::BorderSideEquation(oFirstBorder.oBottom, oSecondBorder.oBottom);
                    BorderSide::BorderSideEquation(oFirstBorder.oLeft, oSecondBorder.oLeft);
                }

                void Block()
                {
                    oTop.Block();
                    oRight.Block();
                    oBottom.Block();
                    oLeft.Block();
                }

                void Unlock()
                {
                    oTop.Unlock();
                    oRight.Unlock();
                    oBottom.Unlock();
                    oLeft.Unlock();
                }

                bool operator==(const Border& oBorder) const
                {
                    return oTop    == oBorder.oTop    &&
                           oRight  == oBorder.oRight  &&
                           oBottom == oBorder.oBottom &&
                           oLeft   == oBorder.oLeft;
                }

                bool EqualSides() const
                {
                    return  oTop    == oRight     &&
                            oRight  == oBottom    &&
                            oBottom == oLeft;
                }

                bool Empty() const
                {
                    return oTop.Empty() && oRight.Empty() && oBottom.Empty() && oLeft.Empty();
                }

                void SetTopSide(const BorderSide &oTopSide, const bool& bHardMode = false)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(oTop.GetImportants()) && !bHardMode)
                        return;

                    oTop = oTopSide;
                }

                void SetRightSide(const BorderSide &oRightSide, const bool& bHardMode = false)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(oRight.GetImportants()) && !bHardMode)
                        return;

                    oRight = oRightSide;
                }

                void SetBottomSide(const BorderSide &oBottomSide, const bool& bHardMode = false)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(oBottom.GetImportants()) && !bHardMode)
                        return;

                    oBottom = oBottomSide;
                }

                void SetLeftSide(const BorderSide &oLeftSide, const bool& bHardMode = false)
                {
                    if (NS_STATIC_FUNCTIONS::IsTrue(oLeft.GetImportants()) && !bHardMode)
                        return;

                    oLeft = oLeftSide;
                }

                void SetWidth(const float& fValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (fValue < 0.0f)
                        return;

                    if ((!oTop.GetImportantWidth() || bHardMode))
                        oTop   .SetWidthWithoutChecking(fValue, unLevel);
                    if ((!oRight.GetImportantWidth() || bHardMode))
                        oRight .SetWidthWithoutChecking(fValue, unLevel);
                    if ((!oBottom.GetImportantWidth() || bHardMode))
                        oBottom.SetWidthWithoutChecking(fValue, unLevel);
                    if ((!oLeft.GetImportantWidth() || bHardMode))
                        oLeft  .SetWidthWithoutChecking(fValue, unLevel);
                }

                void SetStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (!sValue.empty())
                        return;

                    std::wstring sNewValue = sValue;
                    std::transform(sNewValue.begin(), sNewValue.end(), sNewValue.begin(), towlower);
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
                    else
                        return;

                    if ((!oTop.GetImportantStyle() || bHardMode))
                        oTop   .SetStyleWithoutChecking(sStyle, unLevel);
                    if ((!oRight.GetImportantStyle() || bHardMode))
                        oRight .SetStyleWithoutChecking(sStyle, unLevel);
                    if ((!oBottom.GetImportantStyle() || bHardMode))
                        oBottom.SetStyleWithoutChecking(sStyle, unLevel);
                    if ((!oLeft.GetImportantStyle() || bHardMode))
                        oLeft  .SetStyleWithoutChecking(sStyle, unLevel);
                }

                void SetColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
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
                        std::wstring sTempvalue = sValue;
                        std::transform(sTempvalue.begin(), sTempvalue.end(), sTempvalue.begin(), towlower);
                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sTempvalue);
                        if (oHEX != NSMaps::mColors.end())
                            sNewColor = oHEX->second;
                    }

                    if (!sNewColor.empty())
                    {
                        if ((!oTop.GetImportantColor() || bHardMode))
                            oTop   .SetColorWithoutChecking(sNewColor, unLevel);
                        if ((!oRight.GetImportantColor() || bHardMode))
                            oRight .SetColorWithoutChecking(sNewColor, unLevel);
                        if ((!oBottom.GetImportantColor() || bHardMode))
                            oBottom.SetColorWithoutChecking(sNewColor, unLevel);
                        if ((!oLeft.GetImportantColor() || bHardMode))
                            oLeft  .SetColorWithoutChecking(sNewColor, unLevel);
                    }
                }

                void SetImportantWidth(const bool& bImportant)
                {
                    oTop.SetImportantWidth(bImportant);
                    oRight.SetImportantWidth(bImportant);
                    oBottom.SetImportantWidth(bImportant);
                    oLeft.SetImportantWidth(bImportant);
                }

                void SetImportantWidthTopSide(const bool& bImportant)
                {
                    oTop.SetImportantWidth(bImportant);
                }

                void SetImportantWidthRightSide(const bool& bImportant)
                {
                    oRight.SetImportantWidth(bImportant);
                }

                void SetImportantWidthBottomSide(const bool& bImportant)
                {
                    oBottom.SetImportantWidth(bImportant);
                }

                void SetImportantWidthLeftSide(const bool& bImportant)
                {
                    oLeft.SetImportantWidth(bImportant);
                }

                void SetImportantStyle(const bool& bImportant)
                {
                    oTop.SetImportantStyle(bImportant);
                    oRight.SetImportantStyle(bImportant);
                    oBottom.SetImportantStyle(bImportant);
                    oLeft.SetImportantStyle(bImportant);
                }

                void SetImportantStyleTopSide(const bool& bImportant)
                {
                    oTop.SetImportantStyle(bImportant);
                }

                void SetImportantStyleRightSide(const bool& bImportant)
                {
                    oRight.SetImportantStyle(bImportant);
                }

                void SetImportantStyleBottomSide(const bool& bImportant)
                {
                    oBottom.SetImportantStyle(bImportant);
                }

                void SetImportantStyleLeftSide(const bool& bImportant)
                {
                    oLeft.SetImportantStyle(bImportant);
                }

                void SetImportantColor(const bool &bImportant)
                {
                    oTop.SetImportantColor(bImportant);
                    oRight.SetImportantColor(bImportant);
                    oBottom.SetImportantColor(bImportant);
                    oLeft.SetImportantColor(bImportant);
                }

                void SetImportantColorTopSide(const bool& bImportant)
                {
                    oTop.SetImportantColor(bImportant);
                }

                void SetImportantColorRightSide(const bool& bImportant)
                {
                    oRight.SetImportantColor(bImportant);
                }

                void SetImportantColorBottomSide(const bool& bImportant)
                {
                    oBottom.SetImportantColor(bImportant);
                }

                void SetImportantColorLeftSide(const bool& bImportant)
                {
                    oLeft.SetImportantColor(bImportant);
                }

                void SetTopSideWidth(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oTop.SetWidth(fWidth, unLevel, bHardMode);
                }

                void SetRightSideWidth(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oRight.SetWidth(fWidth, unLevel, bHardMode);
                }

                void SetBottomSideWidth(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oBottom.SetWidth(fWidth, unLevel, bHardMode);
                }

                void SetLeftSideWidth(const float& fWidth, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oLeft.SetWidth(fWidth, unLevel, bHardMode);
                }

                void SetTopSideStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oTop.SetStyle(sValue, unLevel, bHardMode);
                }

                void SetRightSideStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oRight.SetStyle(sValue, unLevel, bHardMode);
                }

                void SetBottomSideStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oBottom.SetStyle(sValue, unLevel, bHardMode);
                }

                void SetLeftSideStyle(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oLeft.SetStyle(sValue, unLevel, bHardMode);
                }

                void SetTopSideColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oTop.SetColor(sValue, unLevel, bHardMode);
                }

                void SetRightSideColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oRight.SetColor(sValue, unLevel, bHardMode);
                }

                void SetBottomSideColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oBottom.SetColor(sValue, unLevel, bHardMode);
                }

                void SetLeftSideColor(const std::wstring& sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    oLeft.SetColor(sValue, unLevel, bHardMode);
                }


                std::wstring GetWidthTopSideW() const
                {
                    return oTop.GetWidthW();
                }

                std::wstring GetStyleTopSide() const
                {
                    return oTop.GetStyle();
                }

                std::wstring GetColorTopSide() const
                {
                    return oTop.GetColor();
                }

                std::wstring GetWidthRightSideW() const
                {
                    return oRight.GetWidthW();
                }

                std::wstring GetStyleRightSide() const
                {
                    return oRight.GetStyle();
                }

                std::wstring GetColorRightSide() const
                {
                    return oRight.GetColor();
                }

                std::wstring GetWidthBottomSideW() const
                {
                    return oBottom.GetWidthW();
                }

                std::wstring GetStyleBottomSide() const
                {
                    return oBottom.GetStyle();
                }

                std::wstring GetColorBottomSide() const
                {
                    return oBottom.GetColor();
                }

                std::wstring GetWidthLeftSideW() const
                {
                    return oLeft.GetWidthW();
                }

                std::wstring GetStyleLeftSide() const
                {
                    return oLeft.GetStyle();
                }

                std::wstring GetColorLeftSide() const
                {
                    return oLeft.GetColor();
                }

                bool EmptyTopSide() const
                {
                    return oTop.Empty();
                }

                bool EmptyRightSide() const
                {
                    return oRight.Empty();
                }

                bool EmptyBottomSide() const
                {
                    return oBottom.Empty();
                }

                bool EmptyLeftSide() const
                {
                    return oLeft.Empty();
                }

            };

            class Background
            {
                std::wstring sColor;
                bool bInBorder;

                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;
            public:

                Background() : bInBorder(false), bImportants({false}), arLevels({0}){}

                void ClearImportants()
                {
                    bImportants = {false};
                }

                void InBorder()
                {
                    bInBorder = true;
                }

                bool IsInBorder() const
                {
                    return bInBorder;
                }

                Background operator+=(const Background& oBackground)
                {
                    if (oBackground.sColor.empty() || (bImportants[0] && !oBackground.bImportants[0]))
                        return *this;

                    sColor = oBackground.sColor;

                    return *this;
                }

                static void BackgroundEquation(Background &oFirstBackground, Background &oSecondBackground)
                {
                    if (oFirstBackground.bImportants[0] && !oSecondBackground.bImportants[0] && !oFirstBackground.sColor.empty())
                        oSecondBackground.sColor.clear();
                    else if (oSecondBackground.bImportants[0] && !oFirstBackground.bImportants[0] && !oSecondBackground.sColor.empty())
                        oFirstBackground.sColor.clear();
                    else if (!oSecondBackground.sColor.empty())
                    {
                        if (oFirstBackground.arLevels[0] < oSecondBackground.arLevels[0])
                            oFirstBackground.sColor.clear();
                        else
                            oSecondBackground.sColor.clear();
                    }
                }

                bool operator==(const Background& oBackground) const
                {
                    return sColor == oBackground.sColor;
                }

                bool Empty() const
                {
                    return sColor.empty();
                }

                void SetColor(const std::wstring &sValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (sValue.empty() || (bImportants[0] && !bHardMode))
                        return;

                    if (sValue[0] == L'#')
                    {
                        if (sValue.length() == 7)
                        {
                            arLevels[0] = unLevel;
                            sColor = sValue.substr(1, 7);
                        }
                        else if (sValue.length() == 4)
                        {
                            arLevels[0] = unLevel;
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
                        {
                            arLevels[0] = unLevel;
                            sColor = sNewColor;
                        }
                    }
                    else
                    {
                        std::wstring sNewColor = sValue;
                        std::transform(sNewColor.begin(), sNewColor.end(), sNewColor.begin(), towlower);

                        if (sNewColor == L"transparent")
                            return;

                        const std::map<std::wstring, std::wstring>::const_iterator oHEX = NSMaps::mColors.find(sNewColor);
                        if (oHEX != NSMaps::mColors.end())
                        {
                            arLevels[0] = unLevel;
                            sColor = oHEX->second;
                        }
                    }
                }

                void SetBackground(const std::wstring& sBackground, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (sBackground.empty() || (bImportants[0] && !bHardMode))
                        return;

                    const std::vector<std::wstring> &arValues = NS_STATIC_FUNCTIONS::GetWordsW(sBackground, L" ");

                    for (const std::wstring& sValue : arValues)
                    {
                        SetColor(sValue, unLevel);
                        if (!sColor.empty())
                            return;
                    }
                }

                void SetImportantAll(const bool &bImportant)
                {
                    if (bImportant)
                        bImportants = {true};
                    else
                        bImportants = {false};
                }

                void SetImportantBackground(const bool &bImportant)
                {
                    bImportants[0] = bImportant;
                }

                std::wstring GetColorHex() const
                {
                    return sColor;
                }

                std::wstring GetColor() const
                {
                    std::wstring wsUpperColor = sColor;
                    std::transform(wsUpperColor.begin(), wsUpperColor.end(), wsUpperColor.begin(), ::toupper);

                    std::map<std::wstring, std::wstring>::const_iterator oIter = std::find_if(NSMaps::mColors.begin(), NSMaps::mColors.end(),
                                                                                              [&wsUpperColor](const std::pair<std::wstring, std::wstring> oValue)
                                                                                              { return oValue.second == wsUpperColor;});

                    if (oIter != NSMaps::mColors.end() &&
                       (oIter->first == L"yellow" ||
                        oIter->first == L"green" ||
                        oIter->first == L"cyan" ||
                        oIter->first == L"magenta" ||
                        oIter->first == L"blue" ||
                        oIter->first == L"red" ||
                        oIter->first == L"darkBlue" ||
                        oIter->first == L"darkCyan" ||
                        oIter->first == L"darkGreen" ||
                        oIter->first == L"darkMagenta" ||
                        oIter->first == L"darkRed" ||
                        oIter->first == L"darkYellow" ||
                        oIter->first == L"darkGray" ||
                        oIter->first == L"lightGray"))
                            return oIter->first;

                    return L"";
                }
            };

            class Display
            {
                std::wstring wsDisplay;
                int nWidth;
                int nHeight;
                std::wstring wsAlign;
                std::wstring wsVerticalAlign;

                std::vector<bool> bImportants;
                std::vector<unsigned int> arLevels;

            public:
                Display() : wsDisplay(L"inline"), nWidth(-1), nHeight(-1), wsAlign(), bImportants{false, false, false, false, false}, arLevels{0, 0, 0, 0, 0} {};

                void ClearImportants()
                {
                    bImportants = {false, false, false, false, false};
                }

                Display operator+=(const Display& oDisplay)
                {
                    if (oDisplay.Empty())
                        return *this;

                    wsDisplay   = oDisplay.wsDisplay;
                    nWidth      = oDisplay.nWidth;
                    nHeight     = oDisplay.nHeight;
                    wsAlign     = oDisplay.wsAlign;

                    return *this;
                }

                static void DisplayEquation(Display &oFirstDisplay, Display &oSecondDisplay)
                {
                    if (oFirstDisplay.bImportants[0] && !oSecondDisplay.bImportants[0] && !oFirstDisplay.wsDisplay.empty())
                        oSecondDisplay.wsDisplay.clear();
                    else if (oSecondDisplay.bImportants[0] && !oFirstDisplay.bImportants[0] && !oSecondDisplay.wsDisplay.empty())
                        oFirstDisplay.wsDisplay.clear();
                    else if (!oSecondDisplay.wsDisplay.empty())
                    {
                        if (oFirstDisplay.arLevels[0] < oSecondDisplay.arLevels[0])
                            oFirstDisplay.wsDisplay.clear();
                        else
                            oSecondDisplay.wsDisplay.clear();
                    }

                    if (oFirstDisplay.bImportants[1] && !oSecondDisplay.bImportants[1] && oFirstDisplay.nWidth > 0)
                        oSecondDisplay.nWidth = -1;
                    else if (oSecondDisplay.bImportants[1] && !oFirstDisplay.bImportants[1] && oSecondDisplay.nWidth > 0)
                        oFirstDisplay.nWidth = -1;
                    else if (oSecondDisplay.nWidth > 0)
                    {
                        if (oFirstDisplay.arLevels[1] < oSecondDisplay.arLevels[1])
                            oFirstDisplay.nWidth = -1;
                        else
                            oSecondDisplay.nWidth = -1;
                    }

                    if (oFirstDisplay.bImportants[2] && !oSecondDisplay.bImportants[2] && oFirstDisplay.nHeight > 0)
                        oSecondDisplay.nHeight = -1;
                    else if (oSecondDisplay.bImportants[2] && !oFirstDisplay.bImportants[2] && oSecondDisplay.nHeight > 0)
                        oFirstDisplay.nHeight = -1;
                    else if (oSecondDisplay.nHeight > 0)
                    {
                        if (oFirstDisplay.arLevels[2] < oSecondDisplay.arLevels[2])
                            oFirstDisplay.nHeight = -1;
                        else
                            oSecondDisplay.nHeight = -1;
                    }

                    if (oFirstDisplay.bImportants[3] && !oSecondDisplay.bImportants[3] && !oFirstDisplay.wsAlign.empty())
                        oSecondDisplay.wsAlign.clear();
                    else if (oSecondDisplay.bImportants[3] && !oFirstDisplay.bImportants[3] && !oSecondDisplay.wsAlign.empty())
                        oFirstDisplay.wsAlign.clear();
                    else if (!oSecondDisplay.wsAlign.empty())
                    {
                        if (oFirstDisplay.arLevels[3] < oSecondDisplay.arLevels[3])
                            oFirstDisplay.wsAlign.clear();
                        else
                            oSecondDisplay.wsAlign.clear();
                    }

                    if (oFirstDisplay.bImportants[4] && !oSecondDisplay.bImportants[4] && !oFirstDisplay.wsVerticalAlign.empty())
                        oSecondDisplay.wsVerticalAlign.clear();
                    else if (oSecondDisplay.bImportants[4] && !oFirstDisplay.bImportants[4] && !oSecondDisplay.wsVerticalAlign.empty())
                        oFirstDisplay.wsVerticalAlign.clear();
                    else if (!oSecondDisplay.wsVerticalAlign.empty())
                    {
                        if (oFirstDisplay.arLevels[4] < oSecondDisplay.arLevels[4])
                            oFirstDisplay.wsVerticalAlign.clear();
                        else
                            oSecondDisplay.wsVerticalAlign.clear();
                    }
                }

                bool operator==(const Display& oDisplay) const
                {
                    return wsDisplay == oDisplay.wsDisplay && nWidth == oDisplay.nWidth && nHeight == oDisplay.nHeight &&
                           wsAlign == oDisplay.wsAlign && wsVerticalAlign == oDisplay.wsVerticalAlign;
                }

                bool Empty() const
                {
                    return (L"inline" == wsDisplay || wsDisplay.empty()) && (nWidth < 0) && (nHeight < 0) &&
                           (wsAlign.empty()) && (wsVerticalAlign.empty());
                }

                void SetImportantAll(const bool &bImportant)
                {
                    bImportants = {bImportant, bImportant, bImportant, bImportant, bImportant};
                }

                void SetImportantDisplay(const bool &bImportant)
                {
                    bImportants[0] = bImportant;
                }

                void SetImportantWidth(const bool &bImportant)
                {
                    bImportants[1] = bImportant;
                }

                void SetImportantHeight(const bool &bImportant)
                {
                    bImportants[2] = bImportant;
                }

                void SetImportantAlign(const bool& bImportant)
                {
                    bImportants[3] = bImportant;
                }

                void SetImportantVerticalAlign(const bool& bImportant)
                {
                    bImportants[4] = bImportant;
                }

                void SetDisplay(const std::wstring& wsNewDisplay, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (wsNewDisplay.empty() || (bImportants[0] && !bHardMode))
                        return;

                    if (arDisplayValues.end() != std::find(arDisplayValues.begin(), arDisplayValues.end(), wsNewDisplay))
                    {
                        wsDisplay = wsNewDisplay;
                        arLevels[0] = unLevel;
                    }
                }

                void SetWidth(const std::wstring& wsNewValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (wsNewValue.empty() || !iswdigit(wsNewValue[0]) || (bImportants[1] && !bHardMode))
                        return;

                    arLevels[1] = unLevel;
                    nWidth = std::stoi(wsNewValue);
                }

                void SetWidth(int nValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (bImportants[1] && !bHardMode)
                        return;

                    arLevels[1] = unLevel;
                    nWidth = nValue;
                }

                void SetHeight(const std::wstring& wsNewValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (wsNewValue.empty() || !iswdigit(wsNewValue[0]) || (bImportants[2] && !bHardMode))
                        return;

                    arLevels[2] = unLevel;
                    nHeight = std::stoi(wsNewValue);
                }

                void SetHeight(int nValue, const unsigned int& unLevel, const bool &bHardMode = false)
                {
                    if (bImportants[2] && !bHardMode)
                        return;

                    arLevels[2] = unLevel;
                    nHeight = nValue;
                }

                void SetAlign(const std::wstring& sAlign, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sAlign.empty() || (bImportants[3] && !bHardMode))
                        return;

                    if (L"center" == sAlign || L"left" == sAlign || L"start" == sAlign ||
                        L"right" == sAlign || L"end" == sAlign)
                    {
                         arLevels[3] = unLevel;
                         wsAlign = sAlign;
                    }
                }

                void SetVerticalAlign(const std::wstring& sVerticalAlign, const unsigned int& unLevel, const bool& bHardMode = false)
                {
                    if (sVerticalAlign.empty() || (bImportants[4] && !bHardMode))
                        return;

                    if (L"top" == sVerticalAlign || L"baseline" == sVerticalAlign || L"text-top" == sVerticalAlign)
                    {
                        wsVerticalAlign = L"top";
                        arLevels[4] = unLevel;
                    }
                        else if (L"bottom" == sVerticalAlign || L"text-bottom" == sVerticalAlign)
                    {
                        wsVerticalAlign = L"bottom";
                        arLevels[4] = unLevel;
                    }
                    else if (L"middle" == sVerticalAlign)
                    {
                        wsVerticalAlign = sVerticalAlign;
                        arLevels[4] = unLevel;
                    }
                }

                std::wstring GetDisplay() const
                {
                    return wsDisplay;
                }

                std::wstring GetWidthW() const
                {
                    if (nWidth < 0)
                            return std::wstring();

                    return std::to_wstring(nWidth);
                }

                int GetWidth() const
                {
                    return nWidth;
                }

                std::wstring GetHeightW() const
                {
                    if (nHeight < 0)
                            return std::wstring();

                    return std::to_wstring(nHeight);
                }

                std::wstring GetAlign() const
                {
                    return wsAlign;
                }

                std::wstring GetVerticalAlign() const
                {
                    return wsVerticalAlign;
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
