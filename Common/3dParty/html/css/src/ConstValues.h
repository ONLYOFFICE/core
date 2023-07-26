#ifndef CONSTVALUES_H
#define CONSTVALUES_H

#include "CNode.h"

namespace NSCSS
{
	#ifndef RGB_TO_INT
	#define RGB_TO_INT(r, g, b) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) ) )
	#endif

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

		CSizeWindow();
		CSizeWindow(unsigned short unWidth, unsigned short unHeight);

		bool Empty() const;
		void Clear();

		bool operator==(const CSizeWindow& oSizeWindow) const;
		bool operator!=(const CSizeWindow& oSizeWindow) const;
	};

	struct StatistickElement
	{
		enum TypeElement
		{
			IsStyle = 0,
			IsId
		} m_enType;
		std::wstring sValue;

		bool operator<(const StatistickElement& oStatistickElement) const;
	};

	struct CTree
	{
		NSCSS::CNode m_oNode;
		std::vector<CTree> m_arrChild;

		static void CountingNumberRepetitions(const CTree &oTree, std::map<StatistickElement, unsigned int> &mStatictics);
	};

	namespace NSConstValues
	{
		extern const std::map<std::wstring, std::wstring> COLORS;
		extern const std::vector<std::wstring> DISPLAY_VALUES;

		#ifdef CSS_CALCULATOR_WITH_XHTML
		namespace NSProperties
		{
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
		extern const std::wstring PSEUDO_CLASSES[33];
		#endif
	}
=======
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
}

#endif // CONSTVALUES_H
