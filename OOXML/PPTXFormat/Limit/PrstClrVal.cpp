/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "PrstClrVal.h"

namespace PPTX
{
	namespace Limit
	{		
		PrstClrVal::PrstClrVal()
		{
			m_strValue = L"black";
		}
		void PrstClrVal::set(const std::wstring& strValue)
		{
			if ((L"aliceBlue" == strValue) || // (Alice Blue Preset Color)  Specifies a color with RGB value (240,248,255)
				(L"antiqueWhite" == strValue) || // (Antique White Preset Color)  Specifies a color with RGB value (250,235,215)
				(L"aqua" == strValue) || // (Aqua Preset Color)  Specifies a color with RGB value (0,255,255)
				(L"aquamarine" == strValue) || // (Aquamarine Preset Color)  Specifies a color with RGB value (127,255,212)
				(L"azure" == strValue) || // (Azure Preset Color)  Specifies a color with RGB value (240,255,255)
				(L"beige" == strValue) || // (Beige Preset Color)  Specifies a color with RGB value (245,245,220)
				(L"bisque" == strValue) || // (Bisque Preset Color)  Specifies a color with RGB value (255,228,196)
				(L"black" == strValue) || // (Black Preset Color)  Specifies a color with RGB value (0,0,0)
				(L"blanchedAlmond" == strValue) || // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205)
				(L"blue" == strValue) || // (Blue Preset Color)  Specifies a color with RGB value (0,0,255)
				(L"blueViolet" == strValue) || // (Blue Violet Preset Color)  Specifies a color with RGB value (138,43,226)
				(L"brown" == strValue) || // (Brown Preset Color)  Specifies a color with RGB value (165,42,42)
				(L"burlyWood" == strValue) || // (Burly Wood Preset Color)  Specifies a color with RGB value (222,184,135)
				(L"cadetBlue" == strValue) || // (Cadet Blue Preset Color)  Specifies a color with RGB value (95,158,160)
				(L"chartreuse" == strValue) || // (Chartreuse Preset Color)  Specifies a color with RGB value (127,255,0)
				(L"chocolate" == strValue) || // (Chocolate Preset Color)  Specifies a color with RGB value (210,105,30)
				(L"coral" == strValue) || // (Coral Preset Color)  Specifies a color with RGB value (255,127,80)
				(L"cornflowerBlue" == strValue) || // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237)
				(L"cornsilk" == strValue) || // (Cornsilk Preset Color)  Specifies a color with RGB value (255,248,220)
				(L"crimson" == strValue) || // (Crimson Preset Color)  Specifies a color with RGB value (220,20,60)
				(L"cyan" == strValue) || // (Cyan Preset Color)  Specifies a color with RGB value (0,255,255)
				(L"darkBlue" == strValue) || // (Dark Blue Preset Color)  Specifies a color with RGB value (0,0,139)
				(L"darkCyan" == strValue) || // (Dark Cyan Preset Color)  Specifies a color with RGB value (0,139,139)
				(L"darkGoldenrod" == strValue) || // (Dark Goldenrod Preset Color)  Specifies a color with RGB value (184,134,11)
				(L"darkGray" == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(L"darkGreen" == strValue) || // (Dark Green Preset Color)  Specifies a color with RGB value (0,100,0)
				(L"darkGrey" == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(L"darkKhaki" == strValue) || // (Dark Khaki Preset Color)  Specifies a color with RGB value (189,183,107)
				(L"darkMagenta" == strValue) || // (Dark Magenta Preset Color)  Specifies a color with RGB value (139,0,139)
				(L"darkOliveGreen" == strValue) || // (Dark Olive Green Preset Color)  Specifies a color with RGB value (85,107,47)
				(L"darkOrange" == strValue) || // (Dark Orange Preset Color)  Specifies a color with RGB value (255,140,0)
				(L"darkOrchid" == strValue) || // (Dark Orchid Preset Color)  Specifies a color with RGB value (153,50,204)
				(L"darkRed" == strValue) || // (Dark Red Preset Color)  Specifies a color with RGB value (139,0,0)
				(L"darkSalmon" == strValue) || // (Dark Salmon Preset Color)  Specifies a color with RGB value (233,150,122)
				(L"darkSeaGreen" == strValue) || // (Dark Sea Green Preset Color)  Specifies a color with RGB value (143,188,143)
				(L"darkSlateBlue" == strValue) || // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
				(L"darkSlateGray" == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(L"darkSlateGrey" == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(L"darkTurquoise" == strValue) || // (Dark Turquoise Preset Color)  Specifies a color with RGB value (0,206,209)
				(L"darkViolet" == strValue) || // (Dark Violet Preset Color)  Specifies a color with RGB value (148,0,211)
				(L"deepPink" == strValue) || // (Deep Pink Preset Color)  Specifies a color with RGB value (255,20,147)
				(L"deepSkyBlue" == strValue) || // (Deep Sky Blue Preset Color)  Specifies a color with RGB value (0,191,255)
				(L"dimGray" == strValue) || // (Dim Gray Preset Color)  Specifies a color with RGB value (105,105,105)
				(L"dimGrey" == strValue) || // (Dim Gray Preset Color)  Specifies a color with RGB value (105,105,105)
				(L"dkBlue" == strValue) || // (Dark Blue Preset Color)  Specifies a color with RGB value (0,0,139)
				(L"dkCyan" == strValue) || // (Dark Cyan Preset Color)  Specifies a color with RGB value (0,139,139)
				(L"dkGoldenrod" == strValue) || // (Dark Goldenrod Preset Color)  Specifies a color with RGB value (184,134,11)
				(L"dkGray" == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(L"dkGreen" == strValue) || // (Dark Green Preset Color)  Specifies a color with RGB value (0,100,0)
				(L"dkGrey" == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(L"dkKhaki" == strValue) || // (Dark Khaki Preset Color)  Specifies a color with RGB value (189,183,107)
				(L"dkMagenta" == strValue) || // (Dark Magenta Preset Color)  Specifies a color with RGB value (139,0,139)
				(L"dkOliveGreen" == strValue) || // (Dark Olive Green Preset Color)  Specifies a color with RGB value (85,107,47)
				(L"dkOrange" == strValue) || // (Dark Orange Preset Color)  Specifies a color with RGB value (255,140,0)
				(L"dkOrchid" == strValue) || // (Dark Orchid Preset Color)  Specifies a color with RGB value (153,50,204)
				(L"dkRed" == strValue) || // (Dark Red Preset Color)  Specifies a color with RGB value (139,0,0)
				(L"dkSalmon" == strValue) || // (Dark Salmon Preset Color)  Specifies a color with RGB value (233,150,122)
				(L"dkSeaGreen" == strValue) || // (Dark Sea Green Preset Color)  Specifies a color with RGB value (143,188,139)
				(L"dkSlateBlue" == strValue) || // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
				(L"dkSlateGray" == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(L"dkSlateGrey" == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(L"dkTurquoise" == strValue) || // (Dark Turquoise Preset Color)  Specifies a color with RGB value (0,206,209)
				(L"dkViolet" == strValue) || // (Dark Violet Preset Color)  Specifies a color with RGB value (148,0,211)
				(L"dodgerBlue" == strValue) || // (Dodger Blue Preset Color)  Specifies a color with RGB value (30,144,255)
				(L"firebrick" == strValue) || // (Firebrick Preset Color)  Specifies a color with RGB value (178,34,34)
				(L"floralWhite" == strValue) || // (Floral White Preset Color)  Specifies a color with RGB value (255,250,240)
				(L"forestGreen" == strValue) || // (Forest Green Preset Color)  Specifies a color with RGB value (34,139,34)
				(L"fuchsia" == strValue) || // (Fuchsia Preset Color)  Specifies a color with RGB value (255,0,255)
				(L"gainsboro" == strValue) || // (Gainsboro Preset Color)  Specifies a color with RGB value (220,220,220)
				(L"ghostWhite" == strValue) || // (Ghost White Preset Color)  Specifies a color with RGB value (248,248,255)
				(L"gold" == strValue) || // (Gold Preset Color)  Specifies a color with RGB value (255,215,0)
				(L"goldenrod" == strValue) || // (Goldenrod Preset Color)  Specifies a color with RGB value (218,165,32)
				(L"gray" == strValue) || // (Gray Preset Color)  Specifies a color with RGB value (128,128,128)
				(L"green" == strValue) || // (Green Preset Color)  Specifies a color with RGB value (0,128,0)
				(L"greenYellow" == strValue) || // (Green Yellow Preset Color)  Specifies a color with RGB value (173,255,47)
				(L"grey" == strValue) || // (Gray Preset Color)  Specifies a color with RGB value (128,128,128)
				(L"honeydew" == strValue) || // (Honeydew Preset Color)  Specifies a color with RGB value (240,255,240)
				(L"hotPink" == strValue) || // (Hot Pink Preset Color)  Specifies a color with RGB value (255,105,180)
				(L"indianRed" == strValue) || // (Indian Red Preset Color)  Specifies a color with RGB value (205,92,92)
				(L"indigo" == strValue) || // (Indigo Preset Color)  Specifies a color with RGB value (75,0,130)
				(L"ivory" == strValue) || // (Ivory Preset Color)  Specifies a color with RGB value (255,255,240)
				(L"khaki" == strValue) || // (Khaki Preset Color)  Specifies a color with RGB value (240,230,140)
				(L"lavender" == strValue) || // (Lavender Preset Color)  Specifies a color with RGB value (230,230,250)
				(L"lavenderBlush" == strValue) || // (Lavender Blush Preset Color)  Specifies a color with RGB value (255,240,245)
				(L"lawnGreen" == strValue) || // (Lawn Green Preset Color)  Specifies a color with RGB value (124,252,0)
				(L"lemonChiffon" == strValue) || // (Lemon Chiffon Preset Color)  Specifies a color with RGB value (255,250,205)
				(L"lightBlue" == strValue) || // (Light Blue Preset Color)  Specifies a color with RGB value (173,216,230)
				(L"lightCoral" == strValue) || // (Light Coral Preset Color)  Specifies a color with RGB value (240,128,128)
				(L"lightCyan" == strValue) || // (Light Cyan Preset Color)  Specifies a color with RGB value (224,255,255)
				(L"lightGoldenrodYellow" == strValue) || // (Light Goldenrod Color) Yellow Preset  Specifies a color with RGB value (250,250,210)
				(L"lightGray" == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(L"lightGreen" == strValue) || // (Light Green Preset Color)  Specifies a color with RGB value (144,238,144)
				(L"lightGrey" == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(L"lightPink" == strValue) || // (Light Pink Preset Color)  Specifies a color with RGB value (255,182,193)
				(L"lightSalmon" == strValue) || // (Light Salmon Preset Color)  Specifies a color with RGB value (255,160,122)
				(L"lightSeaGreen" == strValue) || // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
				(L"lightSkyBlue" == strValue) || // (Light Sky Blue Preset Color)  Specifies a color with RGB value (135,206,250)
				(L"lightSlateGray" == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(L"lightSlateGrey" == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(L"lightSteelBlue" == strValue) || // (Light Steel Blue Preset Color)  Specifies a color with RGB value (176,196,222)
				(L"lightYellow" == strValue) || // (Light Yellow Preset Color)  Specifies a color with RGB value (255,255,224)
				(L"lime" == strValue) || // (Lime Preset Color)  Specifies a color with RGB value (0,255,0)
				(L"limeGreen" == strValue) || // (Lime Green Preset Color)  Specifies a color with RGB value (50,205,50)
				(L"linen" == strValue) || // (Linen Preset Color)  Specifies a color with RGB value (250,240,230)
				(L"ltBlue" == strValue) || // (Light Blue Preset Color)  Specifies a color with RGB value (173,216,230)
				(L"ltCoral" == strValue) || // (Light Coral Preset Color)  Specifies a color with RGB value (240,128,128)
				(L"ltCyan" == strValue) || // (Light Cyan Preset Color)  Specifies a color with RGB value (224,255,255)
				(L"ltGoldenrodYellow" == strValue) || // (Light Goldenrod Color) Yellow Preset  Specifies a color with RGB value (250,250,120)
				(L"ltGray" == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(L"ltGreen" == strValue) || // (Light Green Preset Color)  Specifies a color with RGB value (144,238,144)
				(L"ltGrey" == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(L"ltPink" == strValue) || // (Light Pink Preset Color)  Specifies a color with RGB value (255,182,193)
				(L"ltSalmon" == strValue) || // (Light Salmon Preset Color)  Specifies a color with RGB value (255,160,122)
				(L"ltSeaGreen" == strValue) || // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
				(L"ltSkyBlue" == strValue) || // (Light Sky Blue Preset Color)  Specifies a color with RGB value (135,206,250)
				(L"ltSlateGray" == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(L"ltSlateGrey" == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(L"ltSteelBlue" == strValue) || // (Light Steel Blue Preset Color)  Specifies a color with RGB value (176,196,222)
				(L"ltYellow" == strValue) || // (Light Yellow Preset Color)  Specifies a color with RGB value (255,255,224)
				(L"magenta" == strValue) || // (Magenta Preset Color)  Specifies a color with RGB value (255,0,255)
				(L"maroon" == strValue) || // (Maroon Preset Color)  Specifies a color with RGB value (128,0,0)
				(L"medAquamarine" == strValue) || // (Medium Aquamarine Preset Color)  Specifies a color with RGB value (102,205,170)
				(L"medBlue" == strValue) || // (Medium Blue Preset Color)  Specifies a color with RGB value (0,0,205)
				(L"mediumAquamarine" == strValue) || // (Medium Aquamarine Color) Preset  Specifies a color with RGB value (102,205,170)
				(L"mediumBlue" == strValue) || // (Medium Blue Preset Color)  Specifies a color with RGB value (0,0,205)
				(L"mediumOrchid" == strValue) || // (Medium Orchid Preset Color)  Specifies a color with RGB value (186,85,211)
				(L"mediumPurple" == strValue) || // (Medium Purple Preset Color)  Specifies a color with RGB value (147,112,219)
				(L"mediumSeaGreen" == strValue) || // (Medium Sea Green Preset Color)  Specifies a color with RGB value (60,179,113)
				(L"mediumSlateBlue" == strValue) || // (Medium Slate Blue Preset Color)  Specifies a color with RGB value (123,104,238)
				(L"mediumSpringGreen" == strValue) || // (Medium Spring Color) Green Preset  Specifies a color with RGB value (0,250,154)
				(L"mediumTurquoise" == strValue) || // (Medium Turquoise Preset Color)  Specifies a color with RGB value (72,209,204)
				(L"mediumVioletRed" == strValue) || // (Medium Violet Red Preset Color)  Specifies a color with RGB value (199,21,133)
				(L"medOrchid" == strValue) || // (Medium Orchid Preset Color)  Specifies a color with RGB value (186,85,211)
				(L"medPurple" == strValue) || // (Medium Purple Preset Color)  Specifies a color with RGB value (147,112,219)
				(L"medSeaGreen" == strValue) || // (Medium Sea Green Preset Color)  Specifies a color with RGB value (60,179,113)
				(L"medSlateBlue" == strValue) || // (Medium Slate Blue Preset Color)  Specifies a color with RGB value (123,104,238)
				(L"medSpringGreen" == strValue) || // (Medium Spring Green Preset Color)  Specifies a color with RGB value (0,250,154)
				(L"medTurquoise" == strValue) || // (Medium Turquoise Preset Color)  Specifies a color with RGB value (72,209,204)
				(L"medVioletRed" == strValue) || // (Medium Violet Red Preset Color)  Specifies a color with RGB value (199,21,133)
				(L"midnightBlue" == strValue) || // (Midnight Blue Preset Color)  Specifies a color with RGB value (25,25,112)
				(L"mintCream" == strValue) || // (Mint Cream Preset Color)  Specifies a color with RGB value (245,255,250)
				(L"mistyRose" == strValue) || // (Misty Rose Preset Color)  Specifies a color with RGB value (255,228,225)
				(L"moccasin" == strValue) || // (Moccasin Preset Color)  Specifies a color with RGB value (255,228,181)
				(L"navajoWhite" == strValue) || // (Navajo White Preset Color)  Specifies a color with RGB value (255,222,173)
				(L"navy" == strValue) || // (Navy Preset Color)  Specifies a color with RGB value (0,0,128)
				(L"oldLace" == strValue) || // (Old Lace Preset Color)  Specifies a color with RGB value (253,245,230)
				(L"olive" == strValue) || // (Olive Preset Color)  Specifies a color with RGB value (128,128,0)
				(L"oliveDrab" == strValue) || // (Olive Drab Preset Color)  Specifies a color with RGB value (107,142,35)
				(L"orange" == strValue) || // (Orange Preset Color)  Specifies a color with RGB value (255,165,0)
				(L"orangeRed" == strValue) || // (Orange Red Preset Color)  Specifies a color with RGB value (255,69,0)
				(L"orchid" == strValue) || // (Orchid Preset Color)  Specifies a color with RGB value (218,112,214)
				(L"paleGoldenrod" == strValue) || // (Pale Goldenrod Preset Color)  Specifies a color with RGB value (238,232,170)
				(L"paleGreen" == strValue) || // (Pale Green Preset Color)  Specifies a color with RGB value (152,251,152)
				(L"paleTurquoise" == strValue) || // (Pale Turquoise Preset Color)  Specifies a color with RGB value (175,238,238)
				(L"paleVioletRed" == strValue) || // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147)
				(L"papayaWhip" == strValue) || // (Papaya Whip Preset Color)  Specifies a color with RGB value (255,239,213)
				(L"peachPuff" == strValue) || // (Peach Puff Preset Color)  Specifies a color with RGB value (255,218,185)
				(L"peru" == strValue) || // (Peru Preset Color)  Specifies a color with RGB value (205,133,63)
				(L"pink" == strValue) || // (Pink Preset Color)  Specifies a color with RGB value (255,192,203)
				(L"plum" == strValue) || // (Plum Preset Color)  Specifies a color with RGB value (221,160,221)
				(L"powderBlue" == strValue) || // (Powder Blue Preset Color)  Specifies a color with RGB value (176,224,230)
				(L"purple" == strValue) || // (Purple Preset Color)  Specifies a color with RGB value (128,0,128)
				(L"red" == strValue) || // (Red Preset Color)  Specifies a color with RGB value (255,0,0)
				(L"rosyBrown" == strValue) || // (Rosy Brown Preset Color)  Specifies a color with RGB value (188,143,143)
				(L"royalBlue" == strValue) || // (Royal Blue Preset Color)  Specifies a color with RGB value (65,105,225)
				(L"saddleBrown" == strValue) || // (Saddle Brown Preset Color)  Specifies a color with RGB value (139,69,19)
				(L"salmon" == strValue) || // (Salmon Preset Color)  Specifies a color with RGB value (250,128,114)
				(L"sandyBrown" == strValue) || // (Sandy Brown Preset Color)  Specifies a color with RGB value (244,164,96)
				(L"seaGreen" == strValue) || // (Sea Green Preset Color)  Specifies a color with RGB value (46,139,87)
				(L"seaShell" == strValue) || // (Sea Shell Preset Color)  Specifies a color with RGB value (255,245,238)
				(L"sienna" == strValue) || // (Sienna Preset Color)  Specifies a color with RGB value (160,82,45)
				(L"silver" == strValue) || // (Silver Preset Color)  Specifies a color with RGB value (192,192,192)
				(L"skyBlue" == strValue) || // (Sky Blue Preset Color)  Specifies a color with RGB value (135,206,235)
				(L"slateBlue" == strValue) || // (Slate Blue Preset Color)  Specifies a color with RGB value (106,90,205)
				(L"slateGray" == strValue) || // (Slate Gray Preset Color)  Specifies a color with RGB value (112,128,144)
				(L"slateGrey" == strValue) || // (Slate Gray Preset Color)  Specifies a color with RGB value (112,128,144)
				(L"snow" == strValue) || // (Snow Preset Color)  Specifies a color with RGB value (255,250,250)
				(L"springGreen" == strValue) || // (Spring Green Preset Color)  Specifies a color with RGB value (0,255,127)
				(L"steelBlue" == strValue) || // (Steel Blue Preset Color)  Specifies a color with RGB value (70,130,180)
				(L"tan" == strValue) || // (Tan Preset Color)  Specifies a color with RGB value (210,180,140)
				(L"teal" == strValue) || // (Teal Preset Color)  Specifies a color with RGB value (0,128,128)
				(L"thistle" == strValue) || // (Thistle Preset Color)  Specifies a color with RGB value (216,191,216)
				(L"tomato" == strValue) || // (Tomato Preset Color)  Specifies a color with RGB value (255,99,71)
				(L"turquoise" == strValue) || // (Turquoise Preset Color)  Specifies a color with RGB value (64,224,208)
				(L"violet" == strValue) || // (Violet Preset Color)  Specifies a color with RGB value (238,130,238)
				(L"wheat" == strValue) || // (Wheat Preset Color)  Specifies a color with RGB value (245,222,179)
				(L"white" == strValue) || // (White Preset Color)  Specifies a color with RGB value (255,255,255)
				(L"whiteSmoke" == strValue) || // (White Smoke Preset Color)  Specifies a color with RGB value (245,245,245)
				(L"yellow" == strValue) || // (Yellow Preset Color)  Specifies a color with RGB value (255,255,0)
				(L"yellowGreen" == strValue)) // (Yellow Green Preset Color)  Specifies a color with RGB value (154,205,50)
			{
				m_strValue = strValue;
			}
		}
		unsigned char PrstClrVal::GetBYTECode() const
		{
			//not using
			return 0;
		}
		void PrstClrVal::SetBYTECode(const unsigned char& src)
		{
			//not using
		}
	} // namespace Limit
} // namespace PPTX
