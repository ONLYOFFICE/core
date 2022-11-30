/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
			m_strValue = _T("black");
		}
		void PrstClrVal::set(const std::wstring& strValue)
		{
			if ((_T("aliceBlue") == strValue) || // (Alice Blue Preset Color)  Specifies a color with RGB value (240,248,255)
				(_T("antiqueWhite") == strValue) || // (Antique White Preset Color)  Specifies a color with RGB value (250,235,215)
				(_T("aqua") == strValue) || // (Aqua Preset Color)  Specifies a color with RGB value (0,255,255)
				(_T("aquamarine") == strValue) || // (Aquamarine Preset Color)  Specifies a color with RGB value (127,255,212)
				(_T("azure") == strValue) || // (Azure Preset Color)  Specifies a color with RGB value (240,255,255)
				(_T("beige") == strValue) || // (Beige Preset Color)  Specifies a color with RGB value (245,245,220)
				(_T("bisque") == strValue) || // (Bisque Preset Color)  Specifies a color with RGB value (255,228,196)
				(_T("black") == strValue) || // (Black Preset Color)  Specifies a color with RGB value (0,0,0)
				(_T("blanchedAlmond") == strValue) || // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205)
				(_T("blue") == strValue) || // (Blue Preset Color)  Specifies a color with RGB value (0,0,255)
				(_T("blueViolet") == strValue) || // (Blue Violet Preset Color)  Specifies a color with RGB value (138,43,226)
				(_T("brown") == strValue) || // (Brown Preset Color)  Specifies a color with RGB value (165,42,42)
				(_T("burlyWood") == strValue) || // (Burly Wood Preset Color)  Specifies a color with RGB value (222,184,135)
				(_T("cadetBlue") == strValue) || // (Cadet Blue Preset Color)  Specifies a color with RGB value (95,158,160)
				(_T("chartreuse") == strValue) || // (Chartreuse Preset Color)  Specifies a color with RGB value (127,255,0)
				(_T("chocolate") == strValue) || // (Chocolate Preset Color)  Specifies a color with RGB value (210,105,30)
				(_T("coral") == strValue) || // (Coral Preset Color)  Specifies a color with RGB value (255,127,80)
				(_T("cornflowerBlue") == strValue) || // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237)
				(_T("cornsilk") == strValue) || // (Cornsilk Preset Color)  Specifies a color with RGB value (255,248,220)
				(_T("crimson") == strValue) || // (Crimson Preset Color)  Specifies a color with RGB value (220,20,60)
				(_T("cyan") == strValue) || // (Cyan Preset Color)  Specifies a color with RGB value (0,255,255)
				(_T("darkBlue") == strValue) || // (Dark Blue Preset Color)  Specifies a color with RGB value (0,0,139)
				(_T("darkCyan") == strValue) || // (Dark Cyan Preset Color)  Specifies a color with RGB value (0,139,139)
				(_T("darkGoldenrod") == strValue) || // (Dark Goldenrod Preset Color)  Specifies a color with RGB value (184,134,11)
				(_T("darkGray") == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(_T("darkGreen") == strValue) || // (Dark Green Preset Color)  Specifies a color with RGB value (0,100,0)
				(_T("darkGrey") == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(_T("darkKhaki") == strValue) || // (Dark Khaki Preset Color)  Specifies a color with RGB value (189,183,107)
				(_T("darkMagenta") == strValue) || // (Dark Magenta Preset Color)  Specifies a color with RGB value (139,0,139)
				(_T("darkOliveGreen") == strValue) || // (Dark Olive Green Preset Color)  Specifies a color with RGB value (85,107,47)
				(_T("darkOrange") == strValue) || // (Dark Orange Preset Color)  Specifies a color with RGB value (255,140,0)
				(_T("darkOrchid") == strValue) || // (Dark Orchid Preset Color)  Specifies a color with RGB value (153,50,204)
				(_T("darkRed") == strValue) || // (Dark Red Preset Color)  Specifies a color with RGB value (139,0,0)
				(_T("darkSalmon") == strValue) || // (Dark Salmon Preset Color)  Specifies a color with RGB value (233,150,122)
				(_T("darkSeaGreen") == strValue) || // (Dark Sea Green Preset Color)  Specifies a color with RGB value (143,188,143)
				(_T("darkSlateBlue") == strValue) || // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
				(_T("darkSlateGray") == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(_T("darkSlateGrey") == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(_T("darkTurquoise") == strValue) || // (Dark Turquoise Preset Color)  Specifies a color with RGB value (0,206,209)
				(_T("darkViolet") == strValue) || // (Dark Violet Preset Color)  Specifies a color with RGB value (148,0,211)
				(_T("deepPink") == strValue) || // (Deep Pink Preset Color)  Specifies a color with RGB value (255,20,147)
				(_T("deepSkyBlue") == strValue) || // (Deep Sky Blue Preset Color)  Specifies a color with RGB value (0,191,255)
				(_T("dimGray") == strValue) || // (Dim Gray Preset Color)  Specifies a color with RGB value (105,105,105)
				(_T("dimGrey") == strValue) || // (Dim Gray Preset Color)  Specifies a color with RGB value (105,105,105)
				(_T("dkBlue") == strValue) || // (Dark Blue Preset Color)  Specifies a color with RGB value (0,0,139)
				(_T("dkCyan") == strValue) || // (Dark Cyan Preset Color)  Specifies a color with RGB value (0,139,139)
				(_T("dkGoldenrod") == strValue) || // (Dark Goldenrod Preset Color)  Specifies a color with RGB value (184,134,11)
				(_T("dkGray") == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(_T("dkGreen") == strValue) || // (Dark Green Preset Color)  Specifies a color with RGB value (0,100,0)
				(_T("dkGrey") == strValue) || // (Dark Gray Preset Color)  Specifies a color with RGB value (169,169,169)
				(_T("dkKhaki") == strValue) || // (Dark Khaki Preset Color)  Specifies a color with RGB value (189,183,107)
				(_T("dkMagenta") == strValue) || // (Dark Magenta Preset Color)  Specifies a color with RGB value (139,0,139)
				(_T("dkOliveGreen") == strValue) || // (Dark Olive Green Preset Color)  Specifies a color with RGB value (85,107,47)
				(_T("dkOrange") == strValue) || // (Dark Orange Preset Color)  Specifies a color with RGB value (255,140,0)
				(_T("dkOrchid") == strValue) || // (Dark Orchid Preset Color)  Specifies a color with RGB value (153,50,204)
				(_T("dkRed") == strValue) || // (Dark Red Preset Color)  Specifies a color with RGB value (139,0,0)
				(_T("dkSalmon") == strValue) || // (Dark Salmon Preset Color)  Specifies a color with RGB value (233,150,122)
				(_T("dkSeaGreen") == strValue) || // (Dark Sea Green Preset Color)  Specifies a color with RGB value (143,188,139)
				(_T("dkSlateBlue") == strValue) || // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
				(_T("dkSlateGray") == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(_T("dkSlateGrey") == strValue) || // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
				(_T("dkTurquoise") == strValue) || // (Dark Turquoise Preset Color)  Specifies a color with RGB value (0,206,209)
				(_T("dkViolet") == strValue) || // (Dark Violet Preset Color)  Specifies a color with RGB value (148,0,211)
				(_T("dodgerBlue") == strValue) || // (Dodger Blue Preset Color)  Specifies a color with RGB value (30,144,255)
				(_T("firebrick") == strValue) || // (Firebrick Preset Color)  Specifies a color with RGB value (178,34,34)
				(_T("floralWhite") == strValue) || // (Floral White Preset Color)  Specifies a color with RGB value (255,250,240)
				(_T("forestGreen") == strValue) || // (Forest Green Preset Color)  Specifies a color with RGB value (34,139,34)
				(_T("fuchsia") == strValue) || // (Fuchsia Preset Color)  Specifies a color with RGB value (255,0,255)
				(_T("gainsboro") == strValue) || // (Gainsboro Preset Color)  Specifies a color with RGB value (220,220,220)
				(_T("ghostWhite") == strValue) || // (Ghost White Preset Color)  Specifies a color with RGB value (248,248,255)
				(_T("gold") == strValue) || // (Gold Preset Color)  Specifies a color with RGB value (255,215,0)
				(_T("goldenrod") == strValue) || // (Goldenrod Preset Color)  Specifies a color with RGB value (218,165,32)
				(_T("gray") == strValue) || // (Gray Preset Color)  Specifies a color with RGB value (128,128,128)
				(_T("green") == strValue) || // (Green Preset Color)  Specifies a color with RGB value (0,128,0)
				(_T("greenYellow") == strValue) || // (Green Yellow Preset Color)  Specifies a color with RGB value (173,255,47)
				(_T("grey") == strValue) || // (Gray Preset Color)  Specifies a color with RGB value (128,128,128)
				(_T("honeydew") == strValue) || // (Honeydew Preset Color)  Specifies a color with RGB value (240,255,240)
				(_T("hotPink") == strValue) || // (Hot Pink Preset Color)  Specifies a color with RGB value (255,105,180)
				(_T("indianRed") == strValue) || // (Indian Red Preset Color)  Specifies a color with RGB value (205,92,92)
				(_T("indigo") == strValue) || // (Indigo Preset Color)  Specifies a color with RGB value (75,0,130)
				(_T("ivory") == strValue) || // (Ivory Preset Color)  Specifies a color with RGB value (255,255,240)
				(_T("khaki") == strValue) || // (Khaki Preset Color)  Specifies a color with RGB value (240,230,140)
				(_T("lavender") == strValue) || // (Lavender Preset Color)  Specifies a color with RGB value (230,230,250)
				(_T("lavenderBlush") == strValue) || // (Lavender Blush Preset Color)  Specifies a color with RGB value (255,240,245)
				(_T("lawnGreen") == strValue) || // (Lawn Green Preset Color)  Specifies a color with RGB value (124,252,0)
				(_T("lemonChiffon") == strValue) || // (Lemon Chiffon Preset Color)  Specifies a color with RGB value (255,250,205)
				(_T("lightBlue") == strValue) || // (Light Blue Preset Color)  Specifies a color with RGB value (173,216,230)
				(_T("lightCoral") == strValue) || // (Light Coral Preset Color)  Specifies a color with RGB value (240,128,128)
				(_T("lightCyan") == strValue) || // (Light Cyan Preset Color)  Specifies a color with RGB value (224,255,255)
				(_T("lightGoldenrodYellow") == strValue) || // (Light Goldenrod Color) Yellow Preset  Specifies a color with RGB value (250,250,210)
				(_T("lightGray") == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(_T("lightGreen") == strValue) || // (Light Green Preset Color)  Specifies a color with RGB value (144,238,144)
				(_T("lightGrey") == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(_T("lightPink") == strValue) || // (Light Pink Preset Color)  Specifies a color with RGB value (255,182,193)
				(_T("lightSalmon") == strValue) || // (Light Salmon Preset Color)  Specifies a color with RGB value (255,160,122)
				(_T("lightSeaGreen") == strValue) || // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
				(_T("lightSkyBlue") == strValue) || // (Light Sky Blue Preset Color)  Specifies a color with RGB value (135,206,250)
				(_T("lightSlateGray") == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(_T("lightSlateGrey") == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(_T("lightSteelBlue") == strValue) || // (Light Steel Blue Preset Color)  Specifies a color with RGB value (176,196,222)
				(_T("lightYellow") == strValue) || // (Light Yellow Preset Color)  Specifies a color with RGB value (255,255,224)
				(_T("lime") == strValue) || // (Lime Preset Color)  Specifies a color with RGB value (0,255,0)
				(_T("limeGreen") == strValue) || // (Lime Green Preset Color)  Specifies a color with RGB value (50,205,50)
				(_T("linen") == strValue) || // (Linen Preset Color)  Specifies a color with RGB value (250,240,230)
				(_T("ltBlue") == strValue) || // (Light Blue Preset Color)  Specifies a color with RGB value (173,216,230)
				(_T("ltCoral") == strValue) || // (Light Coral Preset Color)  Specifies a color with RGB value (240,128,128)
				(_T("ltCyan") == strValue) || // (Light Cyan Preset Color)  Specifies a color with RGB value (224,255,255)
				(_T("ltGoldenrodYellow") == strValue) || // (Light Goldenrod Color) Yellow Preset  Specifies a color with RGB value (250,250,120)
				(_T("ltGray") == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(_T("ltGreen") == strValue) || // (Light Green Preset Color)  Specifies a color with RGB value (144,238,144)
				(_T("ltGrey") == strValue) || // (Light Gray Preset Color)  Specifies a color with RGB value (211,211,211)
				(_T("ltPink") == strValue) || // (Light Pink Preset Color)  Specifies a color with RGB value (255,182,193)
				(_T("ltSalmon") == strValue) || // (Light Salmon Preset Color)  Specifies a color with RGB value (255,160,122)
				(_T("ltSeaGreen") == strValue) || // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
				(_T("ltSkyBlue") == strValue) || // (Light Sky Blue Preset Color)  Specifies a color with RGB value (135,206,250)
				(_T("ltSlateGray") == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(_T("ltSlateGrey") == strValue) || // (Light Slate Gray Preset Color)  Specifies a color with RGB value (119,136,153)
				(_T("ltSteelBlue") == strValue) || // (Light Steel Blue Preset Color)  Specifies a color with RGB value (176,196,222)
				(_T("ltYellow") == strValue) || // (Light Yellow Preset Color)  Specifies a color with RGB value (255,255,224)
				(_T("magenta") == strValue) || // (Magenta Preset Color)  Specifies a color with RGB value (255,0,255)
				(_T("maroon") == strValue) || // (Maroon Preset Color)  Specifies a color with RGB value (128,0,0)
				(_T("medAquamarine") == strValue) || // (Medium Aquamarine Preset Color)  Specifies a color with RGB value (102,205,170)
				(_T("medBlue") == strValue) || // (Medium Blue Preset Color)  Specifies a color with RGB value (0,0,205)
				(_T("mediumAquamarine") == strValue) || // (Medium Aquamarine Color) Preset  Specifies a color with RGB value (102,205,170)
				(_T("mediumBlue") == strValue) || // (Medium Blue Preset Color)  Specifies a color with RGB value (0,0,205)
				(_T("mediumOrchid") == strValue) || // (Medium Orchid Preset Color)  Specifies a color with RGB value (186,85,211)
				(_T("mediumPurple") == strValue) || // (Medium Purple Preset Color)  Specifies a color with RGB value (147,112,219)
				(_T("mediumSeaGreen") == strValue) || // (Medium Sea Green Preset Color)  Specifies a color with RGB value (60,179,113)
				(_T("mediumSlateBlue") == strValue) || // (Medium Slate Blue Preset Color)  Specifies a color with RGB value (123,104,238)
				(_T("mediumSpringGreen") == strValue) || // (Medium Spring Color) Green Preset  Specifies a color with RGB value (0,250,154)
				(_T("mediumTurquoise") == strValue) || // (Medium Turquoise Preset Color)  Specifies a color with RGB value (72,209,204)
				(_T("mediumVioletRed") == strValue) || // (Medium Violet Red Preset Color)  Specifies a color with RGB value (199,21,133)
				(_T("medOrchid") == strValue) || // (Medium Orchid Preset Color)  Specifies a color with RGB value (186,85,211)
				(_T("medPurple") == strValue) || // (Medium Purple Preset Color)  Specifies a color with RGB value (147,112,219)
				(_T("medSeaGreen") == strValue) || // (Medium Sea Green Preset Color)  Specifies a color with RGB value (60,179,113)
				(_T("medSlateBlue") == strValue) || // (Medium Slate Blue Preset Color)  Specifies a color with RGB value (123,104,238)
				(_T("medSpringGreen") == strValue) || // (Medium Spring Green Preset Color)  Specifies a color with RGB value (0,250,154)
				(_T("medTurquoise") == strValue) || // (Medium Turquoise Preset Color)  Specifies a color with RGB value (72,209,204)
				(_T("medVioletRed") == strValue) || // (Medium Violet Red Preset Color)  Specifies a color with RGB value (199,21,133)
				(_T("midnightBlue") == strValue) || // (Midnight Blue Preset Color)  Specifies a color with RGB value (25,25,112)
				(_T("mintCream") == strValue) || // (Mint Cream Preset Color)  Specifies a color with RGB value (245,255,250)
				(_T("mistyRose") == strValue) || // (Misty Rose Preset Color)  Specifies a color with RGB value (255,228,225)
				(_T("moccasin") == strValue) || // (Moccasin Preset Color)  Specifies a color with RGB value (255,228,181)
				(_T("navajoWhite") == strValue) || // (Navajo White Preset Color)  Specifies a color with RGB value (255,222,173)
				(_T("navy") == strValue) || // (Navy Preset Color)  Specifies a color with RGB value (0,0,128)
				(_T("oldLace") == strValue) || // (Old Lace Preset Color)  Specifies a color with RGB value (253,245,230)
				(_T("olive") == strValue) || // (Olive Preset Color)  Specifies a color with RGB value (128,128,0)
				(_T("oliveDrab") == strValue) || // (Olive Drab Preset Color)  Specifies a color with RGB value (107,142,35)
				(_T("orange") == strValue) || // (Orange Preset Color)  Specifies a color with RGB value (255,165,0)
				(_T("orangeRed") == strValue) || // (Orange Red Preset Color)  Specifies a color with RGB value (255,69,0)
				(_T("orchid") == strValue) || // (Orchid Preset Color)  Specifies a color with RGB value (218,112,214)
				(_T("paleGoldenrod") == strValue) || // (Pale Goldenrod Preset Color)  Specifies a color with RGB value (238,232,170)
				(_T("paleGreen") == strValue) || // (Pale Green Preset Color)  Specifies a color with RGB value (152,251,152)
				(_T("paleTurquoise") == strValue) || // (Pale Turquoise Preset Color)  Specifies a color with RGB value (175,238,238)
				(_T("paleVioletRed") == strValue) || // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147)
				(_T("papayaWhip") == strValue) || // (Papaya Whip Preset Color)  Specifies a color with RGB value (255,239,213)
				(_T("peachPuff") == strValue) || // (Peach Puff Preset Color)  Specifies a color with RGB value (255,218,185)
				(_T("peru") == strValue) || // (Peru Preset Color)  Specifies a color with RGB value (205,133,63)
				(_T("pink") == strValue) || // (Pink Preset Color)  Specifies a color with RGB value (255,192,203)
				(_T("plum") == strValue) || // (Plum Preset Color)  Specifies a color with RGB value (221,160,221)
				(_T("powderBlue") == strValue) || // (Powder Blue Preset Color)  Specifies a color with RGB value (176,224,230)
				(_T("purple") == strValue) || // (Purple Preset Color)  Specifies a color with RGB value (128,0,128)
				(_T("red") == strValue) || // (Red Preset Color)  Specifies a color with RGB value (255,0,0)
				(_T("rosyBrown") == strValue) || // (Rosy Brown Preset Color)  Specifies a color with RGB value (188,143,143)
				(_T("royalBlue") == strValue) || // (Royal Blue Preset Color)  Specifies a color with RGB value (65,105,225)
				(_T("saddleBrown") == strValue) || // (Saddle Brown Preset Color)  Specifies a color with RGB value (139,69,19)
				(_T("salmon") == strValue) || // (Salmon Preset Color)  Specifies a color with RGB value (250,128,114)
				(_T("sandyBrown") == strValue) || // (Sandy Brown Preset Color)  Specifies a color with RGB value (244,164,96)
				(_T("seaGreen") == strValue) || // (Sea Green Preset Color)  Specifies a color with RGB value (46,139,87)
				(_T("seaShell") == strValue) || // (Sea Shell Preset Color)  Specifies a color with RGB value (255,245,238)
				(_T("sienna") == strValue) || // (Sienna Preset Color)  Specifies a color with RGB value (160,82,45)
				(_T("silver") == strValue) || // (Silver Preset Color)  Specifies a color with RGB value (192,192,192)
				(_T("skyBlue") == strValue) || // (Sky Blue Preset Color)  Specifies a color with RGB value (135,206,235)
				(_T("slateBlue") == strValue) || // (Slate Blue Preset Color)  Specifies a color with RGB value (106,90,205)
				(_T("slateGray") == strValue) || // (Slate Gray Preset Color)  Specifies a color with RGB value (112,128,144)
				(_T("slateGrey") == strValue) || // (Slate Gray Preset Color)  Specifies a color with RGB value (112,128,144)
				(_T("snow") == strValue) || // (Snow Preset Color)  Specifies a color with RGB value (255,250,250)
				(_T("springGreen") == strValue) || // (Spring Green Preset Color)  Specifies a color with RGB value (0,255,127)
				(_T("steelBlue") == strValue) || // (Steel Blue Preset Color)  Specifies a color with RGB value (70,130,180)
				(_T("tan") == strValue) || // (Tan Preset Color)  Specifies a color with RGB value (210,180,140)
				(_T("teal") == strValue) || // (Teal Preset Color)  Specifies a color with RGB value (0,128,128)
				(_T("thistle") == strValue) || // (Thistle Preset Color)  Specifies a color with RGB value (216,191,216)
				(_T("tomato") == strValue) || // (Tomato Preset Color)  Specifies a color with RGB value (255,99,71)
				(_T("turquoise") == strValue) || // (Turquoise Preset Color)  Specifies a color with RGB value (64,224,208)
				(_T("violet") == strValue) || // (Violet Preset Color)  Specifies a color with RGB value (238,130,238)
				(_T("wheat") == strValue) || // (Wheat Preset Color)  Specifies a color with RGB value (245,222,179)
				(_T("white") == strValue) || // (White Preset Color)  Specifies a color with RGB value (255,255,255)
				(_T("whiteSmoke") == strValue) || // (White Smoke Preset Color)  Specifies a color with RGB value (245,245,245)
				(_T("yellow") == strValue) || // (Yellow Preset Color)  Specifies a color with RGB value (255,255,0)
				(_T("yellowGreen") == strValue)) // (Yellow Green Preset Color)  Specifies a color with RGB value (154,205,50)
			{
				m_strValue = strValue;
			}
		}
		BYTE PrstClrVal::GetBYTECode() const
		{
			//not using
			return 0;
		}
		void PrstClrVal::SetBYTECode(const BYTE& src)
		{
			//not using
		}
	} // namespace Limit
} // namespace PPTX
