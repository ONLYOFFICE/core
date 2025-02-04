#include "ConstValues.h"

namespace NSCSS
{
	namespace NSConstValues
	{
		const std::map<std::wstring, std::wstring> COLORS
		{
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
			{L"midnightblue",       L"191970"}, {L"navyblue",              L"A0B0E0"},
			/* White tones */
			{L"white",              L"FFFFFF"}, {L"snow",                  L"FFFAFA"}, {L"honeydew",      L"F0FFF0"},
			{L"mintcream",          L"F5FFFA"}, {L"azure",                 L"F0FFFF"}, {L"aliceblue",     L"F0F8FF"},
			{L"ghostwhite",         L"F8F8FF"}, {L"whitesmoke",            L"F5F5F5"}, {L"seashell",      L"FFF5EE"},
			{L"beige",              L"F5F5DC"}, {L"oldlace",               L"FDF5E6"}, {L"floralwhite",   L"FFFAF0"},
			{L"ivory",              L"FFFFF0"}, {L"antiquewhite",          L"FAEBD7"}, {L"linen",         L"FAF0E6"},
			{L"lavenderblush",      L"FFF0F5"}, {L"mistyrose",             L"FFE4E1"},
			/* Gray tones */
			{L"gainsboro",          L"DCDCDC"}, {L"lightgray",             L"D3D3D3"}, {L"silver",        L"C0C0C0"},
			{L"darkgray",           L"A9A9A9"}, {L"gray",                  L"808080"}, {L"dimgray",       L"696969"},
			{L"lightslategray",     L"778899"}, {L"slategray",             L"708090"}, {L"darkslategray", L"2F4F4F"},
			{L"black",              L"000000"}, {L"grey",                  L"808080"},
			/* Outdated */
			{L"windowtext",         L"000000"},  {L"transparent",           L"000000"}
		};

		const std::vector<std::wstring> DISPLAY_VALUES
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

		#ifdef CSS_CALCULATOR_WITH_XHTML
		const std::wstring PSEUDO_CLASSES[33]
		{
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
			L"visited"
		};
		#endif
	}
}
