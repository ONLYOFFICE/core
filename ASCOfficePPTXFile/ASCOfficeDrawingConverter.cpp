/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "ASCOfficeDrawingConverter.h"

#include "./PPTXFormat/Logic/SpTreeElem.h"
#include "./PPTXFormat/Logic/Geometry.h"
#include "./PPTXFormat/Logic/SpTreeElem.h"
#include "./PPTXFormat/Logic/Shape.h"
#include "./PPTXFormat/Logic/Pic.h"
#include "./PPTXFormat/Logic/CxnSp.h"
#include "./PPTXFormat/Logic/SpTree.h"
#include "./PPTXFormat/Logic/GraphicFrame.h"
#include "./PPTXFormat/Logic/Colors/SrgbClr.h"
#include "./PPTXFormat/Logic/Colors/PrstClr.h"
#include "./PPTXFormat/Logic/Colors/SchemeClr.h"
#include "./PPTXFormat/Logic/Colors/SysClr.h"
#include "./PPTXFormat/DocxFormat/Media/Image.h"
#include "./PPTXFormat/PPTX.h"
#include "./PPTXFormat/DocxFormat/Drawing/LegacyDiagramText.h"

#include "./Editor/Drawing/Elements.h"
#include "./Editor/Drawing/Shapes/BaseShape/PPTXShape/pptx2pptshapeconverter.h"

#include "../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../Common/DocxFormat/Source/Common/SimpleTypes_Base.h"

const double g_emu_koef	= 25.4 * 36000 / 72.0;

//#define AVS_OFFICE_DRAWING_DUMP_XML_TEST
//#define AVS_OFFICE_DRAWING_DUMP_PPTX_TO_PPT_TEST

using namespace NSGuidesVML;

void DUMP_MESSAGE_TO_FILE(const char* strMessage)
{
	FILE* file = fopen("c:\\1.txt", "a+");
	fprintf(file, strMessage);
	fclose(file);
}

static void GetColorWithEffect(const std::wstring& sColor, const int& R, const int& G, const int& B,
                               int& resR, int& resG, int& resB)
{
    resR = R;
    resG = G;
    resB = B;

    double param = 0;
    std::wstring::size_type pos1 = sColor.find('(');
    std::wstring::size_type pos2 = sColor.find(')');
    if (pos1 == std::wstring::npos || pos2 == std::wstring::npos)
        return;
    if (pos2 < (pos1 + 2))
        return;

    param = std::stod(sColor.substr(pos1 + 1, pos2 - pos1 - 1));
    bool isEffect = false;

    if (0 == sColor.find(L"darken"))
    {
        resR = (int)(R * param / 255);
        resG = (int)(G * param / 255);
        resB = (int)(B * param / 255);
        isEffect = true;
    }
    else if (0 == sColor.find(L"lighten"))
    {
        resR = 255 - (int)((255 - R) * param / 255);
        resG = 255 - (int)((255 - G) * param / 255);
        resB = 255 - (int)((255 - B) * param / 255);
        isEffect = true;
    }
    else if (0 == sColor.find(L"add"))
    {
        resR = R + (int)param;
        resG = G + (int)param;
        resB = B + (int)param;
        isEffect = true;
    }
    else if (0 == sColor.find(L"subtract"))
    {
        resR = R - (int)param;
        resG = G - (int)param;
        resB = B - (int)param;
        isEffect = true;
    }
    else if (0 == sColor.find(L"reversesubtract"))
    {
        resR = (int)param - R;
        resG = (int)param - G;
        resB = (int)param - B;
        isEffect = true;
    }
    else if (0 == sColor.find(L"blackwhite"))
    {
        int nparam = (int)nparam;
        resR = (R < nparam) ? 0 : 255;
        resG = (G < nparam) ? 0 : 255;
        resB = (B < nparam) ? 0 : 255;
        isEffect = true;
    }

    if (isEffect)
    {
        resR = (resR < 0) ? 0 : resR;
        resR = (resR > 255) ? 255 : resR;

        resG = (resG < 0) ? 0 : resG;
        resG = (resG > 255) ? 255 : resG;

        resB = (resB < 0) ? 0 : resB;
        resB = (resB > 255) ? 255 : resB;
    }
}

namespace NS_DWC_Common
{
	void CorrentCropString(CString& s)
	{
		int nLen = s.GetLength();
		if (nLen > 0 && (s[nLen - 1] == ((TCHAR)'f')))
		{
			s.Delete(nLen - 1);
			int nVal = XmlUtils::GetInteger(s);
			double dKoef = 100000.0 / 65536;
			nVal = (int)(dKoef * nVal);
			s = _T("");
			s.Format(_T("%d"), nVal);
		}
	}

	BYTE getOpacityFromString(const CString opacityStr)
	{
		BYTE alpha;
		if (opacityStr.Find(_T("f")) != -1)
			alpha = (BYTE) (XmlUtils::GetDouble(opacityStr) / 65536 * 256);
		else
		{
			if (0 == opacityStr.Find(_T(".")))
			{
				CString str = _T("0") + opacityStr;
				alpha = (BYTE)(XmlUtils::GetDouble(str) * 256);
			}
			else
				alpha = (BYTE)XmlUtils::GetDouble(opacityStr) * 256;
		}
		return alpha;
	}

	long getRealFromString( const CString& str )
	{
		long val = 0;

		if (str.Find(_T("f")) != -1)
			val = XmlUtils::GetInteger(str);
		else
			val = (long)(XmlUtils::GetDouble(str) * 65536);

		return val;
	}

	int getRotateAngle(const CString& str, const nullable_bool& flipX, const nullable_bool& flipY)
	{
		bool bIsInvertAngle = false;
		
		int nCheckInvert = 0;

		if (flipX.is_init() && flipX.get() == true)
			nCheckInvert += 1;
		if (flipY.is_init() && flipY.get() == true)
			nCheckInvert += 1;

		int nRot = XmlUtils::GetInteger(str);
		if (str.ReverseFind(TCHAR('f')) != -1)
		{
			double dVal = (double)nRot;
			dVal /= 65536;

			if (nCheckInvert == 1)
			{
				dVal = -dVal;
			}
			
			if (dVal > 360)
			{
				int nPart = (int)(dVal / 360);
				dVal = dVal - nPart * 360;
			}
			else if (dVal < 0)
			{
				int nPart = (int)(dVal / 360);
				nPart = 1 - nPart;
				dVal = dVal + nPart * 360;
			}

			nRot = (int)(dVal * 60000);
		}
		else
		{
			if (nCheckInvert == 1)
			{
				nRot = -nRot;
			}

			if (nRot > 360)
			{
				int nPart = (int)(nRot / 360);
				nRot = nRot - nPart * 360;
			}
			else if (nRot < 0)
			{
				int nPart = (int)(nRot / 360);
				nPart = 1 - nPart;
				nRot = nRot + nPart * 360;
			}

			nRot *= 60000;
		}	

		return nRot;
	}

	NSPresentationEditor::CColor getColorFromString(const CString& colorStr)
	{
		NSPresentationEditor::CColor color;
		if (colorStr.Find(_T("#")) != -1)
		{
			if (colorStr.GetLength() == 4)
			{
				int lColor = XmlUtils::GetColor(colorStr.Mid(1, 3));
				BYTE lB = ((lColor >> 16) & 0x0F);
				BYTE lG = ((lColor >> 20) & 0x0F);
				BYTE lR = ((lColor >> 8) & 0x0F);

				color.R = ((lR << 4) + lR);
				color.G = ((lG << 4) + lG);
				color.B = ((lB << 4) + lB);
				color.A = 0;
			}
			else
			{
				int lColor = XmlUtils::GetColor(colorStr.Mid(1, 6));
				color.R = (BYTE)(lColor >> 0);
				color.G = (BYTE)(lColor >> 8);
				color.B = (BYTE)(lColor >> 16);
				color.A = 0;
			}
		}
		else
		{
			CString str;

			int pos = colorStr.Find(' ');
			if( pos < 0 )
				str = colorStr;
			else
				str = colorStr.Left( pos );

			int RGB = 0;

			switch(str[0])
			{
			case 'a':
                if(str == _T("aliceBlue"))			{RGB = 0xF0F8FF; break;} // (Alice Blue Preset Color)		Specifies a color with RGB value (240,248,255)
                if(str == _T("antiqueWhite"))		{RGB = 0xFAEBD7; break;} // (Antique White Preset Color)	Specifies a color with RGB value (250,235,215)
                if(str == _T("aqua"))				{RGB = 0x00FFFF; break;} // (Aqua Preset Color)				Specifies a color with RGB value (0,255,255)
                if(str == _T("aquamarine"))			{RGB = 0x7FFFD4; break;} // (Aquamarine Preset Color)		Specifies a color with RGB value (127,255,212)
                if(str == _T("azure"))				{RGB = 0xF0FFFF; break;} // (Azure Preset Color)			Specifies a color with RGB value (240,255,255)
				break;
			case 'b':
                if(str == _T("beige"))				{RGB = 0xF5F5DC; break;} // (Beige Preset Color)			Specifies a color with RGB value (245,245,220)
                if(str == _T("bisque"))				{RGB = 0xFFE4C4; break;} // (Bisque Preset Color)			Specifies a color with RGB value (255,228,196)
                if(str == _T("black"))				{RGB = 0x000000; break;} // (Black Preset Color)			Specifies a color with RGB value (0,0,0)
                if(str == _T("blanchedAlmond"))		{RGB = 0xFFEBCD; break;} // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205)
                if(str == _T("blue"))				{RGB = 0x0000FF; break;} // (Blue Preset Color)				Specifies a color with RGB value (0,0,255)
                if(str == _T("blueViolet"))			{RGB = 0x8A2BE2; break;} // (Blue Violet Preset Color)		Specifies a color with RGB value (138,43,226)
                if(str == _T("brown"))				{RGB = 0xA52A2A; break;} // (Brown Preset Color)			Specifies a color with RGB value (165,42,42)
                if(str == _T("burlyWood"))			{RGB = 0xDEB887; break;} // (Burly Wood Preset Color)		Specifies a color with RGB value (222,184,135)
				break;
			case 'c':
                if(str == _T("cadetBlue"))			{RGB = 0x5F9EA0; break;} // (Cadet Blue Preset Color)		Specifies a color with RGB value (95,158,160)
                if(str == _T("chartreuse"))			{RGB = 0x7FFF00; break;} // (Chartreuse Preset Color)		Specifies a color with RGB value (127,255,0)
                if(str == _T("chocolate"))			{RGB = 0xD2691E; break;} // (Chocolate Preset Color)		Specifies a color with RGB value (210,105,30)
                if(str == _T("coral"))				{RGB = 0xFF7F50; break;} // (Coral Preset Color)			Specifies a color with RGB value (255,127,80)
                if(str == _T("cornflowerBlue"))		{RGB = 0x6495ED; break;} // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237)
                if(str == _T("cornsilk"))			{RGB = 0xFFF8DC; break;} // (Cornsilk Preset Color)			Specifies a color with RGB value (255,248,220)
                if(str == _T("crimson"))			{RGB = 0xDC143C; break;} // (Crimson Preset Color)			Specifies a color with RGB value (220,20,60)
                if(str == _T("cyan"))				{RGB = 0x00FFFF; break;} // (Cyan Preset Color)				Specifies a color with RGB value (0,255,255)
				break;
			case 'd':
                if(str == _T("darkBlue"))			{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
                if(str == _T("darkCyan"))			{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
                if(str == _T("darkGoldenrod"))		{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
                if(str == _T("darkGray"))			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
                if(str == _T("darkGreen"))			{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
                if(str == _T("darkGrey"))			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
                if(str == _T("darkKhaki"))			{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
                if(str == _T("darkMagenta")	)	{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
                if(str == _T("darkOliveGreen"))		{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
                if(str == _T("darkOrange"))			{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
                if(str == _T("darkOrchid"))			{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
                if(str == _T("darkRed"))			{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
                if(str == _T("darkSalmon"))			{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
                if(str == _T("darkSeaGreen"))		{RGB = 0x8FBC8F; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,143)
                if(str == _T("darkSlateBlue"))		{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
                if(str == _T("darkSlateGray"))		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
                if(str == _T("darkSlateGrey"))		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
                if(str == _T("darkTurquoise"))		{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
                if(str == _T("darkViolet"))			{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
                if(str == _T("deepPink"))			{RGB = 0xFF1493; break;} // (Deep Pink Preset Color)		Specifies a color with RGB value (255,20,147)
                if(str == _T("deepSkyBlue"))		{RGB = 0x00BFFF; break;} // (Deep Sky Blue Preset Color)	Specifies a color with RGB value (0,191,255)
                if(str == _T("dimGray"))			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
                if(str == _T("dimGrey"))			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
                if(str == _T("dkBlue"))				{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
                if(str == _T("dkCyan"))				{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
                if(str == _T("dkGoldenrod"))		{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
                if(str == _T("dkGray"))				{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
                if(str == _T("dkGreen"))			{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
                if(str == _T("dkGrey"))				{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
                if(str == _T("dkKhaki"))			{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
                if(str == _T("dkMagenta"))			{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
                if(str == _T("dkOliveGreen"))		{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
                if(str == _T("dkOrange"))			{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
                if(str == _T("dkOrchid"))			{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
                if(str == _T("dkRed"))				{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
                if(str == _T("dkSalmon"))			{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
                if(str == _T("dkSeaGreen"))			{RGB = 0x8FBC8B; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,139)
                if(str == _T("dkSlateBlue"))		{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
                if(str == _T("dkSlateGray"))		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
                if(str == _T("dkSlateGrey")	)	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
                if(str == _T("dkTurquoise"))		{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
                if(str == _T("dkViolet"))			{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
                if(str == _T("dodgerBlue"))			{RGB = 0x1E90FF; break;} // (Dodger Blue Preset Color)		Specifies a color with RGB value (30,144,255)
				break;
			case 'f':
                if(str == _T("firebrick"))			{RGB = 0xB22222; break;} // (Firebrick Preset Color)		Specifies a color with RGB value (178,34,34)
                if(str == _T("floralWhite"))		{RGB = 0xFFFAF0; break;} // (Floral White Preset Color)		Specifies a color with RGB value (255,250,240)
                if(str == _T("forestGreen"))		{RGB = 0x228B22; break;} // (Forest Green Preset Color)		Specifies a color with RGB value (34,139,34)
                if(str == _T("fuchsia"))			{RGB = 0xFF00FF; break;} // (Fuchsia Preset Color)			Specifies a color with RGB value (255,0,255)
				break;
			case 'g':
                if(str == _T("gainsboro"))			{RGB = 0xDCDCDC; break;} // (Gainsboro Preset Color)		Specifies a color with RGB value (220,220,220)
                if(str == _T("ghostWhite"))			{RGB = 0xF8F8FF; break;} // (Ghost White Preset Color)		Specifies a color with RGB value (248,248,255)
                if(str == _T("gold"))				{RGB = 0xFFD700; break;} // (Gold Preset Color)				Specifies a color with RGB value (255,215,0)
                if(str == _T("goldenrod"))			{RGB = 0xDAA520; break;} // (Goldenrod Preset Color)		Specifies a color with RGB value (218,165,32)
                if(str == _T("gray"))				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
                if(str == _T("green"))				{RGB = 0x008000; break;} // (Green Preset Color)				Specifies a color with RGB value (0,128,0)
                if(str == _T("greenYellow"))		{RGB = 0xADFF2F; break;} // (Green Yellow Preset Color)		Specifies a color with RGB value (173,255,47)
                if(str == _T("grey"))				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
				break;
			case 'h':
                if(str == _T("honeydew"))			{RGB = 0xF0FFF0; break;} // (Honeydew Preset Color)			Specifies a color with RGB value (240,255,240)
                if(str == _T("hotPink"))			{RGB = 0xFF69B4; break;} // (Hot Pink Preset Color)			Specifies a color with RGB value (255,105,180)
				break;
			case 'i':
                if(str == _T("indianRed"))			{RGB = 0xCD5C5C; break;} // (Indian Red Preset Color)		Specifies a color with RGB value (205,92,92)
                if(str == _T("indigo"))				{RGB = 0x4B0082; break;} // (Indigo Preset Color)			Specifies a color with RGB value (75,0,130)
                if(str == _T("ivory"))				{RGB = 0xFFFFF0; break;} // (Ivory Preset Color)			Specifies a color with RGB value (255,255,240)
				break;
			case 'k':
                if(str == _T("khaki"))				{RGB = 0xF0E68C; break;} // (Khaki Preset Color)			Specifies a color with RGB value (240,230,140)
				break;
			case 'l':
                if(str == _T("lavender"))			{RGB = 0xE6E6FA; break;} // (Lavender Preset Color)			Specifies a color with RGB value (230,230,250)
                if(str == _T("lavenderBlush"))		{RGB = 0xFFF0F5; break;} // (Lavender Blush Preset Color)	Specifies a color with RGB value (255,240,245)
                if(str == _T("lawnGreen"))			{RGB = 0x7CFC00; break;} // (Lawn Green Preset Color)		Specifies a color with RGB value (124,252,0)
                if(str == _T("lemonChiffon"))		{RGB = 0xFFFACD; break;} // (Lemon Chiffon Preset Color)	Specifies a color with RGB value (255,250,205)
                if(str == _T("lightBlue"))			{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
                if(str == _T("lightCoral"))			{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
                if(str == _T("lightCyan"))			{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
                if(str == _T("lightGoldenrodYellow"))	{RGB = 0xFAFAD2;break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,210)
                if(str == _T("lightGray"))			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
                if(str == _T("lightGreen"))			{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
                if(str == _T("lightGrey"))			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
                if(str == _T("lightPink"))			{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
                if(str == _T("lightSalmon"))		{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
                if(str == _T("lightSeaGreen"))		{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
                if(str == _T("lightSkyBlue"))		{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
                if(str == _T("lightSlateGray"))		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
                if(str == _T("lightSlateGrey"))		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
                if(str == _T("lightSteelBlue"))		{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
                if(str == _T("lightYellow"))		{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
                if(str == _T("lime"))				{RGB = 0x00FF00; break;} // (Lime Preset Color)				Specifies a color with RGB value (0,255,0)
                if(str == _T("limeGreen"))			{RGB = 0x32CD32; break;} // (Lime Green Preset Color)		Specifies a color with RGB value (50,205,50)
                if(str == _T("linen"))				{RGB = 0xFAF0E6; break;} // (Linen Preset Color)			Specifies a color with RGB value (250,240,230)
                if(str == _T("ltBlue"))				{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
                if(str == _T("ltCoral"))			{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
                if(str == _T("ltCyan"))				{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
                if(str == _T("ltGoldenrodYellow"))	{RGB = 0xFAFA78; break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,120)
                if(str == _T("ltGray"))				{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
                if(str == _T("ltGreen"))			{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
                if(str == _T("ltGrey"))				{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
                if(str == _T("ltPink"))				{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
                if(str == _T("ltSalmon"))			{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
                if(str == _T("ltSeaGreen"))			{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
                if(str == _T("ltSkyBlue"))			{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
                if(str == _T("ltSlateGray")	)	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
                if(str == _T("ltSlateGrey"))		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
                if(str == _T("ltSteelBlue"))		{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
                if(str == _T("ltYellow"))			{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
				break;
			case 'm':
                if(str == _T("magenta"))			{RGB = 0xFF00FF; break;} // (Magenta Preset Color)			Specifies a color with RGB value (255,0,255)
                if(str == _T("maroon"))				{RGB = 0x800000; break;} // (Maroon Preset Color)			Specifies a color with RGB value (128,0,0)
                if(str == _T("medAquamarine"))		{RGB = 0x66CDAA; break;} // (Medium Aquamarine Preset Color)Specifies a color with RGB value (102,205,170)
                if(str == _T("medBlue"))			{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
                if(str == _T("mediumAquamarine"))	{RGB = 0x66CDAA; break;} // (Medium Aquamarine Color)		Specifies a color with RGB value (102,205,170)
                if(str == _T("mediumBlue"))			{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
                if(str == _T("mediumOrchid"))		{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
                if(str == _T("mediumPurple"))		{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
                if(str == _T("mediumSeaGreen"))		{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
                if(str == _T("mediumSlateBlue")	)   {RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
                if(str == _T("mediumSpringGreen"))	{RGB = 0x00FA9A; break;} // (Medium Spring Color)			Specifies a color with RGB value (0,250,154)
                if(str == _T("mediumTurquoise"))	{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
                if(str == _T("mediumVioletRed"))	{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
                if(str == _T("medOrchid"))			{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
                if(str == _T("medPurple"))			{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
                if(str == _T("medSeaGreen"))		{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
                if(str == _T("medSlateBlue"))		{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
                if(str == _T("medSpringGreen"))		{RGB = 0x00FA9A; break;} // (Medium Spring Preset Color)	Specifies a color with RGB value (0,250,154)
                if(str == _T("medTurquoise"))		{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
                if(str == _T("medVioletRed"))		{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
                if(str == _T("midnightBlue"))		{RGB = 0x191970; break;} // (Midnight Blue Preset Color)	Specifies a color with RGB value (25,25,112)
                if(str == _T("mintCream"))			{RGB = 0xF5FFFA; break;} // (Mint Cream Preset Color)		Specifies a color with RGB value (245,255,250)
                if(str == _T("mistyRose"))			{RGB = 0xFFE4FF; break;} // (Misty Rose Preset Color)		Specifies a color with RGB value (255,228,225)
                if(str == _T("moccasin"))			{RGB = 0xFFE4B5; break;} // (Moccasin Preset Color)			Specifies a color with RGB value (255,228,181)
				break;
			case 'n':
                if(str == _T("navajoWhite"))		{RGB = 0xFFDEAD; break;} // (Navajo White Preset Color)		Specifies a color with RGB value (255,222,173)
                if(str == _T("navy"))				{RGB = 0x000080; break;} // (Navy Preset Color)				Specifies a color with RGB value (0,0,128)
				break;
			case 'o':
                if(str == _T("oldLace")	)		{RGB = 0xFDF5E6; break;} // (Old Lace Preset Color)			Specifies a color with RGB value (253,245,230)
                if(str == _T("olive"))				{RGB = 0x808000; break;} // (Olive Preset Color)			Specifies a color with RGB value (128,128,0)
                if(str == _T("oliveDrab"))			{RGB = 0x6B8E23; break;} // (Olive Drab Preset Color)		Specifies a color with RGB value (107,142,35)
                if(str == _T("orange"))				{RGB = 0xFFA500; break;} // (Orange Preset Color)			Specifies a color with RGB value (255,165,0)
                if(str == _T("orangeRed"))			{RGB = 0xFF4500; break;} // (Orange Red Preset Color)		Specifies a color with RGB value (255,69,0)
                if(str == _T("orchid"))				{RGB = 0xDA70D6; break;} // (Orchid Preset Color)			Specifies a color with RGB value (218,112,214)
				break;
			case 'p':
                if(str == _T("paleGoldenrod"))		{RGB = 0xEEE8AA; break;} // (Pale Goldenrod Preset Color)	Specifies a color with RGB value (238,232,170)
                if(str == _T("paleGreen"))			{RGB = 0x98FB98; break;} // (Pale Green Preset Color)		Specifies a color with RGB value (152,251,152)
                if(str == _T("paleTurquoise"))		{RGB = 0xAFEEEE; break;} // (Pale Turquoise Preset Color)	Specifies a color with RGB value (175,238,238)
                if(str == _T("paleVioletRed"))		{RGB = 0xDB7093; break;} // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147)
                if(str == _T("papayaWhip"))			{RGB = 0xFFEFD5; break;} // (Papaya Whip Preset Color)		Specifies a color with RGB value (255,239,213)
                if(str == _T("peachPuff"))			{RGB = 0xFFDAB9; break;} // (Peach Puff Preset Color)		Specifies a color with RGB value (255,218,185)
                if(str == _T("peru")	)			{RGB = 0xCD853F; break;} // (Peru Preset Color)				Specifies a color with RGB value (205,133,63)
                if(str == _T("pink"))				{RGB = 0xFFC0CB; break;} // (Pink Preset Color)				Specifies a color with RGB value (255,192,203)
                if(str == _T("plum"))				{RGB = 0xD3A0D3; break;} // (Plum Preset Color)				Specifies a color with RGB value (221,160,221)
                if(str == _T("powderBlue"))			{RGB = 0xB0E0E6; break;} // (Powder Blue Preset Color)		Specifies a color with RGB value (176,224,230)
                if(str == _T("purple"))				{RGB = 0x800080; break;} // (Purple Preset Color)			Specifies a color with RGB value (128,0,128)
				break;
			case 'r':
                if(str == _T("red"))				{RGB = 0xFF0000; break;} // (Red Preset Color)				Specifies a color with RGB value (255,0,0)
                if(str == _T("rosyBrown"))			{RGB = 0xBC8F8F; break;} // (Rosy Brown Preset Color)		Specifies a color with RGB value (188,143,143)
                if(str == _T("royalBlue"))			{RGB = 0x4169E1; break;} // (Royal Blue Preset Color)		Specifies a color with RGB value (65,105,225)
				break;
			case 's':
                if(str == _T("saddleBrown"))		{RGB = 0x8B4513; break;} // (Saddle Brown Preset Color)		Specifies a color with RGB value (139,69,19)
                if(str == _T("salmon"))				{RGB = 0xFA8072; break;} // (Salmon Preset Color)			Specifies a color with RGB value (250,128,114)
                if(str == _T("sandyBrown"))			{RGB = 0xF4A460; break;} // (Sandy Brown Preset Color)		Specifies a color with RGB value (244,164,96)
                if(str == _T("seaGreen"))			{RGB = 0x2E8B57; break;} // (Sea Green Preset Color)		Specifies a color with RGB value (46,139,87)
                if(str == _T("seaShell"))			{RGB = 0xFFF5EE; break;} // (Sea Shell Preset Color)		Specifies a color with RGB value (255,245,238)
                if(str == _T("sienna"))				{RGB = 0xA0522D; break;} // (Sienna Preset Color)			Specifies a color with RGB value (160,82,45)
                if(str == _T("silver"))				{RGB = 0xC0C0C0; break;} // (Silver Preset Color)			Specifies a color with RGB value (192,192,192)
                if(str == _T("skyBlue"))			{RGB = 0x87CEEB; break;} // (Sky Blue Preset Color)			Specifies a color with RGB value (135,206,235)
                if(str == _T("slateBlue"))			{RGB = 0x6A5AEB; break;} // (Slate Blue Preset Color)		Specifies a color with RGB value (106,90,205)
                if(str == _T("slateGray"))			{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
                if(str == _T("slateGrey"))			{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
                if(str == _T("snow"))				{RGB = 0xFFFAFA; break;} // (Snow Preset Color)				Specifies a color with RGB value (255,250,250)
                if(str == _T("springGreen"))		{RGB = 0x00FF7F; break;} // (Spring Green Preset Color)		Specifies a color with RGB value (0,255,127)
                if(str == _T("steelBlue"))			{RGB = 0x4682B4; break;} // (Steel Blue Preset Color)		Specifies a color with RGB value (70,130,180)
				break;
			case 't':
                if(str == _T("tan"))				{RGB = 0xD2B48C; break;} // (Tan Preset Color)				Specifies a color with RGB value (210,180,140)
                if(str == _T("teal"))				{RGB = 0x008080; break;} // (Teal Preset Color)				Specifies a color with RGB value (0,128,128)
                if(str == _T("thistle")	)		{RGB = 0xD8BFD8; break;} // (Thistle Preset Color)			Specifies a color with RGB value (216,191,216)
                if(str == _T("tomato"))				{RGB = 0xFF7347; break;} // (Tomato Preset Color)			Specifies a color with RGB value (255,99,71)
                if(str == _T("turquoise"))			{RGB = 0x40E0D0; break;} // (Turquoise Preset Color)		Specifies a color with RGB value (64,224,208)
				break;
			case 'v':
                if(str == _T("violet"))				{RGB = 0xEE82EE; break;} // (Violet Preset Color)			Specifies a color with RGB value (238,130,238)
				break;
			case 'w':
                if(str == _T("wheat"))				{RGB = 0xF5DEB3; break;} // (Wheat Preset Color)			Specifies a color with RGB value (245,222,179)
                if(str == _T("white"))				{RGB = 0xFFFFFF; break;} // (White Preset Color)			Specifies a color with RGB value (255,255,255)
                if(str == _T("whiteSmoke"))			{RGB = 0xF5F5F5; break;} // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245)
				break;
			case 'y':
                if(str == _T("yellow"))				{RGB = 0xFFFF00; break;} // (Yellow Preset Color)			Specifies a color with RGB value (255,255,0)
                if(str == _T("yellowGreen"))		{RGB = 0x9ACD32; break;} // (Yellow Green Preset Color)		Specifies a color with RGB value (154,205,50)
				break;
			}

			color.R = (BYTE)(RGB >>16);
			color.G = (BYTE)(RGB >> 8);
			color.B = (BYTE)(RGB);
			color.A = 0;		
		}
		return color;
	}
}

namespace PPTX
{
	CStringTrimmer::CStringTrimmer()
	{
		m_Separator = (TCHAR)' ';
	}
	CStringTrimmer::~CStringTrimmer()
	{
	}
	void CStringTrimmer::LoadFromString(CString& strParams)
	{
		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	= 0;
		TCHAR* pDataMem = pData;

		int nCurPosition = 0;
		while (nPosition <= nCount)
		{
			if (nPosition == nCount || (pData[nPosition] == m_Separator))
			{
				int nLen = nPosition - nCurPosition;
				if (nLen == 0)
				{
					m_arParams.push_back(_T(""));
				}
				else
				{
					m_arParams.push_back(strParams.Mid(nCurPosition, nLen));
				}
				nCurPosition = nPosition + 1;
			}
			++nPosition;
		}
	}
	double CStringTrimmer::GetParameter(int nIndex, double dDefault)
	{
		if (nIndex < 0 || nIndex >= (int)m_arParams.size())
			return dDefault;

		if (m_arParams[nIndex].GetLength() == 0)
			return dDefault;

		SimpleTypes::CPoint parserPoint;
		return parserPoint.FromString(m_arParams[nIndex]);
	}

	CCSS::CCSS()
	{
		Clear();
	}
	CCSS::~CCSS()
	{
	}
	AVSINLINE void CCSS::Clear()
	{
		m_strClassName = _T("");
		m_mapSettings.clear();
	}
	void CCSS::LoadFromString(CString& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	= 0;
		TCHAR* pDataMem = pData;

		while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('\n') == *pDataMem) || (TCHAR('.') == *pDataMem)))
		{
			++nPosition;
			++pDataMem;
		}

		int nPosNameStart = nPosition;
		while ((nPosition < nCount) && (TCHAR(' ') != *pDataMem) && (TCHAR('{') != *pDataMem))
		{
			++nPosition;
			++pDataMem;
		}
		m_strClassName = strParams.Mid(nPosNameStart, nPosition - nPosNameStart);

		while (true)
		{
			++nPosition;
			++pDataMem;

			// пропускаем пробелы
			while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('{') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
			while ((nPosition < nCount) && (TCHAR(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			CString strName = strParams.Mid(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(';') != *pDataMem) && (TCHAR('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			CString strValue = strParams.Mid(nPosOld, nPosition - nPosOld);

			m_mapSettings.insert(std::pair<CString, CString>(strName, strValue));
		}
	}

	void CCSS::LoadFromString2(CString& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	= 0;
		TCHAR* pDataMem = pData;

		m_strClassName = _T("");

		while (true)
		{
			// пропускаем пробелы
			while ((nPosition < nCount) && ((TCHAR(' ') == *pDataMem) || (TCHAR('{') == *pDataMem) || (TCHAR(';') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
			while ((nPosition < nCount) && (TCHAR(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			CString strName = strParams.Mid(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
			while ((nPosition < nCount) && (TCHAR(';') != *pDataMem) && (TCHAR('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			CString strValue = strParams.Mid(nPosOld, nPosition - nPosOld);

			if (pData[nPosOld] == WCHAR('.'))
				strValue = (_T("0") + strValue);

			//добавляем через [], а не insert, потому что ключи могут дублироваться(а в предыдущей реализации использовалось последнее значение)
			m_mapSettings[strName] = strValue;
		}
	}

	CStylesCSS::CStylesCSS() : m_arStyles()
	{
	}
	CStylesCSS::~CStylesCSS()
	{
	}
	AVSINLINE void CStylesCSS::Clear()
	{
		m_arStyles.clear();
	}
	void CStylesCSS::LoadStyles(CString& strParams)
	{
		Clear();

		TCHAR* pData	= strParams.GetBuffer();
		int nCount		= strParams.GetLength();

		int nPosition	 = 0;
		int nPositionOld = 0;
		TCHAR* pDataMem = pData;

		while (nPosition < nCount)
		{
			if (*pDataMem == TCHAR('}'))
			{
				CString strTemp = strParams.Mid(nPositionOld, nPosition - nPositionOld + 1);
				
				CCSS elm;
				m_arStyles.push_back(elm);
				m_arStyles.back().LoadFromString(strTemp);

				nPositionOld = nPosition + 1;
			}

			++nPosition;
			++pDataMem;
		}
	}
}

CSpTreeElemProps::CSpTreeElemProps()
{
	X = 0;
	Y = 0;
	Width = 0;
	Height = 0;

	IsTop = true;
}
CElementProps::CElementProps() : m_Properties()
{
}
CElementProps::~CElementProps()
{
	FinalRelease();
}
void CElementProps::FinalRelease()
{
#if defined(_WIN32) || defined (_WIN64) // linux: BSTR = CString
	for (std::map<LONG, VARIANT>::iterator pPair = m_Properties.begin(); pPair != m_Properties.end(); ++pPair)
	{
		if (pPair->second.vt == VT_BSTR)
			SysFreeString(pPair->second.bstrVal);
	}
#endif
	m_Properties.clear();
}
HRESULT CElementProps::GetProperty(LONG lId, VARIANT* pProp)
{
	if (NULL == pProp)
		return S_FALSE;

	std::map<LONG, VARIANT>::iterator pPair = m_Properties.find(lId);
	if (m_Properties.end() == pPair)
		return S_FALSE;

	bool bIsSupportProp = CopyProperty(*pProp, pPair->second);

	if (!bIsSupportProp)
	{
		return S_FALSE;
	}

	return S_OK;
}
HRESULT CElementProps::SetProperty(LONG lId, VARIANT prop)
{
	VARIANT var;
	bool bIsSupportProp = CopyProperty(var, prop);
	if (!bIsSupportProp)
		return S_FALSE;

#if defined(_WIN32) || defined (_WIN64)
	std::map<LONG, VARIANT>::iterator pPair = m_Properties.find(lId);
	if (m_Properties.end() != pPair)
	{
		if (pPair->second.vt == VT_BSTR)
			SysFreeString(pPair->second.bstrVal);
	}
#endif

	m_Properties.insert(std::pair<LONG, VARIANT>(lId, var));
	return S_OK;
}
bool CElementProps::CopyProperty(VARIANT& oDst, const VARIANT& oSrc)
{
	oDst.vt = oSrc.vt;
	switch (oDst.vt)
	{
	case VT_I4:
		{
			oDst.lVal = oSrc.lVal;
			break;
		}
	case VT_R8:
		{
			oDst.dblVal = oSrc.dblVal;
			break;
		}
	case VT_BSTR:
		{
#if defined(_WIN32) || defined (_WIN64)
			oDst.bstrVal = SysAllocString(oSrc.bstrVal);
#else
            oDst.bstrVal = oSrc.bstrVal;
#endif
			break;
		}
	default:
		return false;			
	}
	return true;
}
CDrawingConverter::CElement::CElement()
{
	m_pElement	= NULL;
	m_pProps	= NULL;
}
CDrawingConverter::CElement::~CElement()
{
	RELEASEOBJECT(m_pElement);
	RELEASEOBJECT(m_pProps);
}

CDrawingConverter::CElement& CDrawingConverter::CElement::operator=(const CDrawingConverter::CElement& oSrc)
{
	m_pElement	= oSrc.m_pElement;
	m_pProps	= oSrc.m_pProps;
	return *this;
}
CDrawingConverter::CElement::CElement(const CElement& oSrc)
{
	*this = oSrc;
}
CDrawingConverter::CDrawingConverter()
{
	m_strCurrentRelsPath = _T("");
	m_nCurrentIndexObject = 0;
	m_strFontDirectory = _T("");
	m_lNextId = 1;
	m_lCurrentObjectTop = 0;
	m_pOOXToVMLRenderer = NULL;
    m_bIsUseConvertion2007 = true;
	m_pBinaryWriter = new NSBinPptxRW::CBinaryFileWriter();
	m_pReader = new NSBinPptxRW::CBinaryFileReader();
	m_pImageManager = new NSBinPptxRW::CImageManager2();
	m_pXmlWriter = new NSBinPptxRW::CXmlWriter();
	m_pTheme = new NSCommon::smart_ptr<PPTX::WrapperFile>();
	m_pClrMap = new NSCommon::smart_ptr<PPTX::WrapperWritingElement>;
}
CDrawingConverter::~CDrawingConverter()
{
	Clear();
	RELEASEOBJECT(m_pOOXToVMLRenderer);
	RELEASEOBJECT(m_pBinaryWriter);
	RELEASEOBJECT(m_pReader);
	RELEASEOBJECT(m_pImageManager);
	RELEASEOBJECT(m_pXmlWriter);
	RELEASEOBJECT(m_pTheme);
	RELEASEOBJECT(m_pClrMap);
}
HRESULT CDrawingConverter::SetMainDocument(BinDocxRW::CDocxSerializer* pDocument)
{
	m_pBinaryWriter->ClearNoAttack();
	m_pBinaryWriter->m_pCommon->m_pImageManager->NewDocument();
	m_pBinaryWriter->SetMainDocument(pDocument);
	m_pReader->SetMainDocument(pDocument);
	m_lNextId = 1;
	return S_OK;
}
HRESULT CDrawingConverter::SetRelsPath(const CString& bsRelsPath)
{
	// чтобы не переоткрывать рельсы - посмотрим - может у нас уже есть такой??
	m_strCurrentRelsPath = bsRelsPath;
	return SetCurrentRelsPath();
}
CString CDrawingConverter::GetRelsPath()
{
	return m_strCurrentRelsPath;
}
HRESULT CDrawingConverter::SetMediaDstPath(const CString& bsMediaPath)
{
	m_pBinaryWriter->m_pCommon->m_pImageManager->m_strDstMedia = (CString)bsMediaPath;
	m_pImageManager->SetDstMedia(m_pBinaryWriter->m_pCommon->m_pImageManager->m_strDstMedia);

#if defined(_WIN32) || defined (_WIN64)
	CreateDirectory(bsMediaPath, NULL);
#else
    FileSystem::Directory::CreateDirectory(bsMediaPath);
#endif
	return S_OK;
}
HRESULT CDrawingConverter::SetEmbedDstPath(const CString& bsEmbedPath)
{
	m_pImageManager->SetDstEmbed(bsEmbedPath);

#if defined(_WIN32) || defined (_WIN64)
	CreateDirectory(bsEmbedPath, NULL);
#else
    FileSystem::Directory::CreateDirectory(bsEmbedPath);
#endif
	return S_OK;
}
HRESULT CDrawingConverter::AddShapeType(const CString& bsXml)
{
	CString strXml = _T("<main ");

	strXml += _T("\
				 xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
				 xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
				 xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
				 xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
				 xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
				 xmlns:v=\"urn:schemas-microsoft-com:vml\" \
				 xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
				 xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
				 xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
				 xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
				 xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
				 xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
				 xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
				 xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
				 xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
				 xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
				 xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
				 xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
				 xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
				 xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"");

	strXml += _T(">");

	strXml += (CString)bsXml;

	strXml += _T("</main>");

	XmlUtils::CXmlNode oNode;
	oNode.FromXmlString(strXml);

	if (oNode.IsValid())
	{
#ifdef PPT_DEF
        CPPTShape* pShape = new CPPTShape();
		pShape->m_bIsShapeType = true;

		XmlUtils::CXmlNode oNodeST = oNode.ReadNodeNoNS(_T("shapetype"));

		CString strId = oNodeST.GetAttribute(_T("id"));
		pShape->LoadFromXMLShapeType(oNodeST);

		CShape* pS = new CShape(NSBaseShape::unknown, 0);
		pS->m_pShape = pShape;
		LoadCoordSize(oNodeST, pS);

		m_mapShapeTypes.insert(std::pair<CString, CShape*>(strId, pS));			
#endif
    }

	return S_OK;
}
HRESULT CDrawingConverter::AddObject(const CString& bsXml, CString** pMainProps)
{
    CString sBegin(_T("<main xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\">"));
    
    CString sEnd(_T("</main>"));
    CString strXml = sBegin + bsXml + sEnd;
    
    m_pBinaryWriter->m_pCommon->CheckFontPicker();
    
    ++m_lCurrentObjectTop;
    bool bResult = ParceObject(strXml, pMainProps);
    --m_lCurrentObjectTop;

	//if (0 == m_lCurrentObjectTop)
	//{
	//	m_pBinaryWriter->ClearNoAttack();
	//}

	return bResult ? S_OK : S_FALSE;
}

bool CDrawingConverter::ParceObject(const CString& strXml, CString** pMainProps)
{
	XmlUtils::CXmlNode oMainNode;
	if (!oMainNode.FromXmlString(strXml))
		return NULL;

	XmlUtils::CXmlNodes oNodes;
	if (!oMainNode.GetNodes(_T("*"), oNodes))
		return NULL;

	//ULONG lCurrentPosition = m_pBinaryWriter->GetPosition();
	m_pBinaryWriter->StartRecord(0);

	m_pBinaryWriter->ClearShapeCurSizes();

	LONG lCount = oNodes.GetCount();
	for (LONG i = 0; i < lCount; ++i)
	{
		XmlUtils::CXmlNode oParseNode;
		oNodes.GetAt(i, oParseNode);

		CString strFullName = oParseNode.GetName();
		CString strNS = XmlUtils::GetNamespace(strFullName);
		CString strName = XmlUtils::GetNameNoNS(strFullName);

		while (true) // analog goto
		{
			if (strName == _T("drawing"))
			{
				XmlUtils::CXmlNode oNodeAnchorInline = oParseNode.ReadNodeNoNS(_T("anchor"));
				if (!oNodeAnchorInline.IsValid())
				{
					oNodeAnchorInline = oParseNode.ReadNodeNoNS(_T("inline"));
				}

				if (oNodeAnchorInline.IsValid())
				{
					XmlUtils::CXmlNode oNodeExt;
					if (oNodeAnchorInline.GetNode(_T("wp:extent"), oNodeExt))
					{
						m_pBinaryWriter->m_lWidthCurShape = oNodeExt.ReadAttributeInt(_T("cx"));
						m_pBinaryWriter->m_lHeightCurShape = oNodeExt.ReadAttributeInt(_T("cy"));
					}

					SendMainProps(oNodeAnchorInline.GetXml(), pMainProps);

					XmlUtils::CXmlNode oNodeGraphic = oNodeAnchorInline.ReadNodeNoNS(_T("graphic"));
					XmlUtils::CXmlNode oNodeGraphicData = oNodeGraphic.ReadNodeNoNS(_T("graphicData"));

					if (oNodeGraphicData.IsValid())
					{
						XmlUtils::CXmlNodes oChilds;
						oNodeGraphicData.GetNodes(_T("*"), oChilds);

						if (1 == oChilds.GetCount())
						{
							XmlUtils::CXmlNode oNodeContent;
							oChilds.GetAt(0, oNodeContent);

							PPTX::Logic::SpTreeElem oElem;

							CString strCurrentRelsPath = m_strCurrentRelsPath;

							if (_T("dgm:relIds") == oNodeContent.GetName() && m_pBinaryWriter->m_pCommonRels->is_init())
							{
								nullable<PPTX::RId> id_data;
								oNodeContent.ReadAttributeBase(L"r:dm", id_data);

								if (id_data.is_init())
								{
									smart_ptr<PPTX::Image> pDiagData = (*m_pBinaryWriter->m_pCommonRels)->image(*id_data);
									
									if (pDiagData.is_init())
									{
										CString strDiagDataPath = pDiagData->filename().m_strFilename;
										
										XmlUtils::CXmlNode oNodeDiagData;
										if (oNodeDiagData.FromXmlFile2(strDiagDataPath))
										{
											nullable<PPTX::RId> id_drawing;

											XmlUtils::CXmlNode oNode2 = oNodeDiagData.ReadNode(_T("dgm:extLst"));
											if (oNode2.IsValid())
											{
												XmlUtils::CXmlNode oNode3 = oNode2.ReadNode(_T("a:ext"));
												if (oNode3.IsValid())
												{
													XmlUtils::CXmlNode oNode4 = oNode3.ReadNode(_T("dsp:dataModelExt"));
													if (oNode4.IsValid())
													{
														oNode4.ReadAttributeBase(L"relId", id_drawing);
													}
												}
											}

											if (id_drawing.is_init())
											{
												smart_ptr<PPTX::Image> pDiagDW = (*m_pBinaryWriter->m_pCommonRels)->image(*id_drawing);

												if (pDiagDW.is_init())
												{
													CString strPathDiagDW = pDiagDW->filename().m_strFilename;

													XmlUtils::CXmlNode oNodeDW;
													oNodeDW.FromXmlFile2(strPathDiagDW);

													XmlUtils::CXmlNode oNodeS = oNodeDW.ReadNodeNoNS(_T("spTree"));
													oElem = oNodeS;

													if (oElem.is<PPTX::Logic::SpTree>())
													{
														PPTX::Logic::SpTree& _pElem = oElem.as<PPTX::Logic::SpTree>();
														if (!_pElem.grpSpPr.xfrm.is_init())
														{
															_pElem.grpSpPr.xfrm = new PPTX::Logic::Xfrm();

															_pElem.grpSpPr.xfrm->offX = (int)0;
															_pElem.grpSpPr.xfrm->offY = (int)0;
															_pElem.grpSpPr.xfrm->extX = m_pBinaryWriter->m_lWidthCurShape;
															_pElem.grpSpPr.xfrm->extY = m_pBinaryWriter->m_lHeightCurShape;
															_pElem.grpSpPr.xfrm->chOffX = (int)0;
															_pElem.grpSpPr.xfrm->chOffY = (int)0;
															_pElem.grpSpPr.xfrm->chExtX = m_pBinaryWriter->m_lWidthCurShape;
															_pElem.grpSpPr.xfrm->chExtY = m_pBinaryWriter->m_lHeightCurShape;
														}
														else
														{
															if (!_pElem.grpSpPr.xfrm->offX.is_init())
																_pElem.grpSpPr.xfrm->offX = (int)0;
															if (!_pElem.grpSpPr.xfrm->offY.is_init())
																_pElem.grpSpPr.xfrm->offY = (int)0;
															if (!_pElem.grpSpPr.xfrm->extX.is_init())
																_pElem.grpSpPr.xfrm->extX = m_pBinaryWriter->m_lWidthCurShape;
															if (!_pElem.grpSpPr.xfrm->extY.is_init())
																_pElem.grpSpPr.xfrm->extY = m_pBinaryWriter->m_lHeightCurShape;
															if (!_pElem.grpSpPr.xfrm->chOffX.is_init())
																_pElem.grpSpPr.xfrm->chOffX = (int)0;
															if (!_pElem.grpSpPr.xfrm->chOffY.is_init())
																_pElem.grpSpPr.xfrm->chOffY = (int)0;
															if (!_pElem.grpSpPr.xfrm->chExtX.is_init())
																_pElem.grpSpPr.xfrm->chExtX = m_pBinaryWriter->m_lWidthCurShape;
															if (!_pElem.grpSpPr.xfrm->chExtY.is_init())
																_pElem.grpSpPr.xfrm->chExtY = m_pBinaryWriter->m_lHeightCurShape;
														}
													}
											
													m_strCurrentRelsPath = strPathDiagDW;
													SetCurrentRelsPath();
												}
											}
										}
									}
								}								
							}
							else if (_T("wpc:wpc") == oNodeContent.GetName())
							{
								PPTX::Logic::SpTree* pTree = new PPTX::Logic::SpTree();
								pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
								pTree->grpSpPr.xfrm->offX = 0;
								pTree->grpSpPr.xfrm->offY = 0;
								pTree->grpSpPr.xfrm->extX = m_pBinaryWriter->m_lWidthCurShape;
								pTree->grpSpPr.xfrm->extY = m_pBinaryWriter->m_lHeightCurShape;

								pTree->fromXML(oNodeContent);
								oElem.InitElem(pTree);
							}
							else
							{
								oElem = oNodeContent;

							#ifdef AVS_OFFICE_DRAWING_DUMP_PPTX_TO_PPT_TEST
								CString strVMLShapeXml = GetVMLShapeXml(oElem);
							#endif
							}

							if (!oElem.is_init())
							{
								CString strXFRM = _T("");
								strXFRM.Format(_T("<a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"%d\" cy=\"%d\"/></a:xfrm>"), 
									m_pBinaryWriter->m_lWidthCurShape, m_pBinaryWriter->m_lHeightCurShape);

								CString strUnsupported = _T("<wps:wsp \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\"><wps:cNvSpPr/><wps:spPr>");
								strUnsupported += strXFRM;
								strUnsupported += _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom><a:noFill/>\
<a:ln><a:solidFill><a:srgbClr val=\"0070C0\"/></a:solidFill></a:ln></wps:spPr>\
<wps:bodyPr rot=\"0\" spcFirstLastPara=\"0\" vertOverflow=\"overflow\" horzOverflow=\"overflow\" vert=\"horz\" wrap=\"square\" lIns=\"91440\" tIns=\"45720\" \
rIns=\"91440\" bIns=\"45720\" numCol=\"1\" spcCol=\"0\" rtlCol=\"0\" fromWordArt=\"0\" anchor=\"ctr\" anchorCtr=\"0\" forceAA=\"0\" compatLnSpc=\"1\">\
<a:prstTxWarp prst=\"textNoShape\"><a:avLst/></a:prstTxWarp><a:noAutofit/></wps:bodyPr></wps:wsp>");

								XmlUtils::CXmlNode oNodeUnsupported;
								oNodeUnsupported.FromXmlString(strUnsupported);

								oElem = oNodeUnsupported;
							}

							m_pBinaryWriter->WriteRecord1(1, oElem);

							if (strCurrentRelsPath != m_strCurrentRelsPath)
							{
								m_strCurrentRelsPath = strCurrentRelsPath;
								SetCurrentRelsPath();
							}
						}
					}
				}

				/*
				XmlUtils::CXmlNode oNodeInline = oParseNode.ReadNodeNoNS(_T("inline"));
				if (oNodeInline.IsValid())
				{
					SendMainProps(oNodeInline.GetXml(), pMainProps);

					XmlUtils::CXmlNode oNodeGraphic = oNodeInline.ReadNodeNoNS(_T("graphic"));
					XmlUtils::CXmlNode oNodeGraphicData = oNodeGraphic.ReadNodeNoNS(_T("graphicData"));

					if (oNodeGraphicData.IsValid())
					{
						XmlUtils::CXmlNodes oChilds;
						oNodeGraphicData.GetNodes(_T("*"), oChilds);

						if (1 == oChilds.GetCount())
						{
							XmlUtils::CXmlNode oNodeContent;
							oChilds.GetAt(0, oNodeContent);

							PPTX::Logic::SpTreeElem oElem;
							oElem = oNodeContent;

						#ifdef _DEBUG
							CString strVMLShapeXml = GetVMLShapeXml(oElem);
						#endif

							m_pBinaryWriter->WriteRecord1(1, oElem);
						}
					}
				}
				*/

				break;
			}
			else if (strName == _T("pict") || strName == _T("object"))
			{
				XmlUtils::CXmlNodes oChilds;
				if (oParseNode.GetNodes(_T("*"), oChilds))
				{
					LONG lChildsCount = oChilds.GetCount();
                    bool bIsFound = false;
					PPTX::Logic::SpTreeElem* pElem = NULL;
					PPTX::Logic::COLEObject* pOle = NULL;
					for (LONG k = 0; k < lChildsCount; k++)
					{
						XmlUtils::CXmlNode oNodeP;
						oChilds.GetAt(k, oNodeP);

						CString strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
						if (_T("shape") == strNameP ||
							_T("rect") == strNameP ||
							_T("oval") == strNameP ||
							_T("line") == strNameP ||
							_T("roundrect") == strNameP ||
							_T("polyline") == strNameP)
						{
							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								*pElem = doc_LoadShape(oNodeP, pMainProps, true);

#ifdef AVS_OFFICE_DRAWING_DUMP_XML_TEST
								NSBinPptxRW::CXmlWriter oXmlW;
								pElem->toXmlWriter(&oXmlW);
								oXmlW.m_lDocType = XMLWRITER_DOC_TYPE_DOCX;
								CString strXmlTemp = oXmlW.GetXmlString();
#endif
							}
						}
						else if (_T("OLEObject") == strNameP)
						{
							pOle = new PPTX::Logic::COLEObject();
							pOle->fromXML(oNodeP);
						}
						else if (_T("group") == strNameP)
						{
							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								*pElem = doc_LoadGroup(oNodeP, pMainProps, true);

#ifdef AVS_OFFICE_DRAWING_DUMP_XML_TEST
								NSBinPptxRW::CXmlWriter oXmlW;
								oXmlW.m_lDocType = XMLWRITER_DOC_TYPE_DOCX;
								pElem->toXmlWriter(&oXmlW);
								CString strXmlTemp = oXmlW.GetXmlString();
#endif
							}
						}
						else if (_T("shapetype") == strNameP)
						{
							AddShapeType(oNodeP.GetXml());
						}
						else
						{
							continue;
						}

						if (bIsFound)
							break;
					}
					if(NULL != pElem)
					{
						if(NULL != pOle && pOle->m_sProgId.IsInit() && (pOle->m_oId.IsInit() || pOle->m_sFilepathBin.IsInit()))
						{
							PPTX::Logic::Shape* pShape = dynamic_cast<PPTX::Logic::Shape*>(pElem->GetElem().operator ->());
							if(NULL != pShape && pShape->spPr.Fill.Fill.IsInit())
							{
								const PPTX::Logic::BlipFill& oBlipFill = pShape->spPr.Fill.Fill.as<PPTX::Logic::BlipFill>();
								if(oBlipFill.blip.IsInit())
								{
									if (pOle->m_sFilepathBin.IsInit())
									{
										oBlipFill.blip->oleFilepathBin = pOle->m_sFilepathBin.get();
									}
									else if (pOle->m_oId.IsInit())
									{
										oBlipFill.blip->oleRid = pOle->m_oId.get().ToString();
									}
									if(strName == _T("object"))
									{
										int nDxaOrig = oParseNode.ReadAttributeInt(_T("w:dxaOrig"));
										int nDyaOrig = oParseNode.ReadAttributeInt(_T("w:dyaOrig"));
										if (nDxaOrig > 0 && nDyaOrig > 0)
										{
											pOle->m_oDxaOrig = nDxaOrig;
											pOle->m_oDyaOrig = nDyaOrig;
										}
									}

									PPTX::Logic::Pic *newElem = new PPTX::Logic::Pic();

									newElem->blipFill	= oBlipFill;
									newElem->spPr		= pShape->spPr;
									newElem->style		= pShape->style;
									newElem->oleObject.reset(pOle);
									pOle = NULL;

									pElem->InitElem(newElem);
								}
							}
						}
						m_pBinaryWriter->WriteRecord1(1, *pElem);
					}
					RELEASEOBJECT(pElem)
					RELEASEOBJECT(pOle)
				}

				break;
			}
			else if (strName == _T("AlternateContent"))
			{
				XmlUtils::CXmlNode oNodeDr;
				if (oParseNode.GetNode(_T("w:drawing"), oNodeDr))
				{
					strName = _T("drawing");
					oParseNode = oNodeDr;
					continue;
				}

				if (oParseNode.GetNode(_T("mc:Choice"), oNodeDr))
				{
					oParseNode = oNodeDr;
					continue;
				}

				if (oParseNode.GetNode(_T("w:pict"), oNodeDr))
				{
					strName = _T("pict");
					oParseNode = oNodeDr;
					continue;
				}

				if (oParseNode.GetNode(_T("w:object"), oNodeDr))
				{
					strName = _T("object");
					oParseNode = oNodeDr;
					continue;
				}

				if (oParseNode.GetNode(_T("xdr:sp"), oNodeDr))
				{
					strName = _T("sp");
					oParseNode = oNodeDr;
					continue;
				}

				if (oParseNode.GetNode(_T("mc:Fallback"), oNodeDr))
				{
					oParseNode = oNodeDr;
					continue;
				}				

				break;
			}
			else
			{
				PPTX::Logic::SpTreeElem oElem;
				oElem = oParseNode;

				if (oElem.is_init())
				{
					m_pBinaryWriter->WriteRecord1(1, oElem);
				}
				break;
			}
		}
	}

	m_pBinaryWriter->EndRecord();
	return true;
}

PPTX::Logic::SpTreeElem CDrawingConverter::doc_LoadShape(XmlUtils::CXmlNode& oNodeShape, CString**& pMainProps,bool bIsTop)
{
	PPTX::Logic::SpTreeElem elem;

	CString strNameNode = oNodeShape.GetName();
	bool bIsNeedCoordSizes = true;

	CString strStyleAdvenced = _T("");

	NSPresentationEditor::CShapeElement oShapeElem;
	CPPTShape* pPPTShape = NULL;

	if (_T("v:rect") == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
		pPPTShape->ReCalculate();
	}
	else if (_T("v:roundrect") == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)2);
		pPPTShape->ReCalculate();
	}
	else if (_T("v:oval") == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)3);
		pPPTShape->ReCalculate();
	}
	else if (_T("v:line") == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)20);
		pPPTShape->ReCalculate();

		CString strCoord1 = oNodeShape.GetAttributeOrValue(_T("from"));
		CString strCoord2 = oNodeShape.GetAttributeOrValue(_T("to"));
		if (strCoord1 != _T("") && strCoord2 != _T(""))
		{
			std::vector<CString> oArray1;
			NSStringUtils::ParseString(_T(","), strCoord1, &oArray1);

			std::vector<CString> oArray2;
			NSStringUtils::ParseString(_T(","), strCoord2, &oArray2);

			if (oArray1.size() >= 2 && oArray2.size() >= 2)
			{
				SimpleTypes::CPoint parserPoint;
				double x1 = parserPoint.FromString(oArray1[0]);
				double y1 = parserPoint.FromString(oArray1[1]);
				double x2 = parserPoint.FromString(oArray2[0]);
				double y2 = parserPoint.FromString(oArray2[1]);

				if (x1 > x2)
				{
					double tmp = x1;
					x1 = x2;
					x2 = tmp;
				}
				if (y1 > y2)
				{
					double tmp = y1;
					y1 = y2;
					y2 = tmp;
				}

				strStyleAdvenced.Format(_T(";left:%.2lf;top:%.2lf;width:%.2lf;height:%.2lf;"), x1, y1, x2 - x1, y2 - y1);				
			}
		}
	}
	else if (_T("v:polyline") == strNameNode)
	{
		CString strPoints = oNodeShape.GetAttributeOrValue(_T("points"));
		if (_T("") != strPoints)
		{
			std::vector<CString> oArray;
			NSStringUtils::ParseString(_T(","), strPoints, &oArray);

			int nSize = oArray.size();
			if ((nSize % 2 == 0) && nSize > 3)
			{
				int* _POINTS = new int[nSize];
				double dKoef = 25.4 * 36000 / 72.0;

				for (int k = 0; k < nSize; ++k)
				{
					if (_T("") == oArray[k])
					{
						_POINTS[k] = 0;
					}
					else
					{
						SimpleTypes::CPoint parserPoint;
						_POINTS[k] = (int)(dKoef * parserPoint.FromString(oArray[k]));
					}
				}

				// теперь определяем bounds
				int _x = INT_MAX;
				int _y = INT_MAX;
				int _r = INT_MIN;
				int _b = INT_MIN;

				for (int k = 0; k < nSize; k += 2)
				{
					int tmpx = _POINTS[k];
					int tmpy = _POINTS[k + 1];
					if (tmpx < _x)
						_x = tmpx;
					if (tmpx > _r)
						_r = tmpx;
					if (tmpy < _y)
						_y = tmpy;
					if (tmpy > _b)
						_b = tmpy;
				}

				int nOffsetX = _POINTS[0] - _x;
				int nOffsetY = _POINTS[1] - _y;

				strStyleAdvenced.Format(_T(";margin-left:%d;margin-top:%d;width:%d;height:%d;polyline_correct:true;"), _x, _y, _r - _x, _b - _y);

                double dKoefX = 21600.0 / (std::max)((_r - _x), 1);
                double dKoefY = 21600.0 / (std::max)((_b - _y), 1);
				CString strPath = _T("");
				for (int k = 0; k < nSize; k += 2)
				{
					if (k == 0)
					{
						CString _s = _T("");
						_s.Format(_T("m%d,%d"), (int)(dKoefX * (_POINTS[k] - _x)), (int)(dKoefY * (_POINTS[k + 1] - _y)));
						strPath += _s;
					}
					else
					{
						CString _s = _T("");
						_s.Format(_T("l%d,%d"), (int)(dKoefX * (_POINTS[k] - _x)), (int)(dKoefY * (_POINTS[k + 1] - _y)));
						strPath += _s;
					}					
				}

				strPath += _T("e");

				RELEASEARRAYOBJECTS(_POINTS);

				pPPTShape = new CPPTShape();
				pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
				// иначе сохранится рект
				pPPTShape->m_eType = PPTShapes::sptCustom;

				pPPTShape->LoadPathList(strPath);
				pPPTShape->ReCalculate();
				bIsNeedCoordSizes = false;
			}
		}		
	}
	else if (_T("v:shape") == strNameNode)
	{
		CString strType = oNodeShape.GetAttribute(_T("type"));
		if (strType.GetLength() > 2 && strType[0] == (TCHAR)('#'))
		{
			strType = strType.Mid(1);

			std::map<CString, CShape*>::iterator pPair = m_mapShapeTypes.find(strType);
			if (m_mapShapeTypes.end() != pPair)
			{
				pPPTShape = new CPPTShape();
				pPair->second->m_pShape->SetToDublicate(pPPTShape);
				pPPTShape->m_eType = ((CPPTShape*)(pPair->second->m_pShape))->m_eType;
			}
		}
		
		if (NULL == pPPTShape)
		{
			CString strType = oNodeShape.GetAttribute(_T("o:connectortype"));
			if (strType.GetLength() > 0)
			{
				int type = 0;
				if (strType == _T("elbow"))			type = PPTShapes::sptCBentConnector2;
				else if (strType == _T("straight"))	type = PPTShapes::sptCStraightConnector1;
				else if (strType == _T("curved"))	type = PPTShapes::sptCCurvedConnector2;

				if (type > 0)
				{
					pPPTShape = new CPPTShape();
					pPPTShape->SetShapeType((PPTShapes::ShapeType)type);
					pPPTShape->m_bIsStroked = true;
					pPPTShape->ReCalculate();
				}
			}
			else
			{
				pPPTShape = new CPPTShape();
				pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
				// иначе сохранится рект
				pPPTShape->m_eType = PPTShapes::sptCustom;
			}			
		}

		pPPTShape->LoadFromXMLShapeType(oNodeShape);
	}

	if (pPPTShape != NULL)
	{		
		oShapeElem.m_oShape.m_pShape = pPPTShape;

		if (bIsNeedCoordSizes)
			LoadCoordSize(oNodeShape, &oShapeElem.m_oShape);
		else
		{
			oShapeElem.m_oShape.m_dWidthLogic  = 21600;
			oShapeElem.m_oShape.m_dHeightLogic = 21600;

			oShapeElem.m_oShape.m_pShape->m_oPath.SetCoordsize(21600, 21600);			
		}

        CString strXmlPPTX;

		PPTX::Logic::Shape* pShape = new PPTX::Logic::Shape();
		
		

		/*
#ifdef _DEBUG
		const PPTX::Logic::CustGeom lpGeom = pShape->spPr.Geometry.as<PPTX::Logic::CustGeom>();
		CString strShapeWWW = lpGeom.GetODString();
		NSPresentationEditor::CShapeElement* lpShapeElement = new CShapeElement(strShapeWWW);

		lpShapeElement->m_oShape.ReCalculate();

		std::map<CString, long>* mapGuides = &((CPPTXShape*)(lpShapeElement->m_oShape.m_pShape))->FManager.mapGuides;
		std::vector<double>* Guides = ((CPPTXShape*)(lpShapeElement->m_oShape.m_pShape))->FManager.Guides;
		double formulas[1905];
		for (int u = 0; u < 1905; u++)
		{
			CString strF = _T("");
			strF.Format(_T("gd%d"), u);

			long num = mapGuides->FindKey(strF);
			if (num >= 0)
			{
				formulas[u] = (*Guides)[mapGuides->GetValueAt(num)];
			}
		}
#endif
		*/

		if (!pShape->TextBoxBodyPr.is_init())
			pShape->TextBoxBodyPr = new PPTX::Logic::BodyPr();

        bool bIsTrimTextPath = false;

		if (pPPTShape->IsWordArt())
		{
			enum EFilltype
			{
				etBlipFill = 0,
				etGradFill = 1,
				etNoFill = 2,
				etPattFill = 3,
				etSolidFill = 4
			};

			PPTShapes::ShapeType eShapeType = pPPTShape->m_eType;
			SimpleTypes::ETextShapeType eTextShapeType;
			switch (eShapeType)
			{
				case PPTShapes::ShapeType::sptCTextPlain:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextPlain; break;
				case PPTShapes::ShapeType::sptCTextArchUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchUp; break;
				case PPTShapes::ShapeType::sptCTextArchDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchDown; break;
				case PPTShapes::ShapeType::sptCTextButton:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextButton; break;
				case PPTShapes::ShapeType::sptCTextCurveUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCurveUp; break;
				case PPTShapes::ShapeType::sptCTextCurveDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCurveDown; break;
				case PPTShapes::ShapeType::sptCTextCanUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCanUp; break;
				case PPTShapes::ShapeType::sptCTextCanDown:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCanDown; break;
				case PPTShapes::ShapeType::sptCTextWave1:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave1; break;
				case PPTShapes::ShapeType::sptCTextWave2:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave2; break;
				case PPTShapes::ShapeType::sptCTextWave3:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDoubleWave1; break;
				case PPTShapes::ShapeType::sptCTextWave4:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave4; break;
				case PPTShapes::ShapeType::sptCTextInflate:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflate; break;
				case PPTShapes::ShapeType::sptCTextDeflate:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflate; break;
				case PPTShapes::ShapeType::sptCTextInflateBottom:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflateBottom; break;
				case PPTShapes::ShapeType::sptCTextDeflateBottom:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateBottom; break;
				case PPTShapes::ShapeType::sptCTextInflateTop:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflateTop; break;
				case PPTShapes::ShapeType::sptCTextDeflateTop:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateTop; break;
				case PPTShapes::ShapeType::sptCTextDeflateInflate:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateInflate; break;
				case PPTShapes::ShapeType::sptCTextDeflateInflateDeflate:	eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateInflateDeflate; break;
				case PPTShapes::ShapeType::sptCTextFadeRight:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeRight; break;
				case PPTShapes::ShapeType::sptCTextFadeLeft:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeLeft; break;
				case PPTShapes::ShapeType::sptCTextFadeUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeUp; break;
				case PPTShapes::ShapeType::sptCTextFadeDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeDown; break;
				case PPTShapes::ShapeType::sptCTextSlantUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextSlantUp; break;
				case PPTShapes::ShapeType::sptCTextSlantDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextSlantDown; break;
				case PPTShapes::ShapeType::sptCTextCascadeUp:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCascadeUp; break;
				case PPTShapes::ShapeType::sptCTextCascadeDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCascadeDown; break;
				case PPTShapes::ShapeType::sptCTextButtonPour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextButtonPour; break;
				case PPTShapes::ShapeType::sptCTextStop:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextStop; break;
				case PPTShapes::ShapeType::sptCTextTriangle:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextTriangle; break;
				case PPTShapes::ShapeType::sptCTextTriangleInverted:		eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextTriangleInverted; break;
				case PPTShapes::ShapeType::sptCTextChevron:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextChevron; break;
				case PPTShapes::ShapeType::sptCTextChevronInverted:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextChevronInverted; break;
				case PPTShapes::ShapeType::sptCTextRingInside:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextRingInside; break;
				case PPTShapes::ShapeType::sptCTextRingOutside:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextRingOutside; break;
				case PPTShapes::ShapeType::sptCTextCirclePour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCirclePour; break;
				case PPTShapes::ShapeType::sptCTextArchUpPour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchUpPour; break;
				case PPTShapes::ShapeType::sptCTextArchDownPour:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchDownPour; break;
				default:													eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextNoShape; break;
			}
			SimpleTypes::CTextShapeType<> oTextShapeType;
			oTextShapeType.SetValue(eTextShapeType);
			CString strPrstTxWarp = _T("<a:prstTxWarp prst=\"") + oTextShapeType.ToString() + _T("\"><a:avLst/></a:prstTxWarp>");

			XmlUtils::CXmlNode oPrstTxWarpNode;
			oPrstTxWarpNode.FromXmlString(strPrstTxWarp);

			pShape->TextBoxBodyPr->prstTxWarp = oPrstTxWarpNode;
			//pShape->TextBoxBodyPr->wrap = new PPTX::Limit::TextWrap();
			//pShape->TextBoxBodyPr->wrap->set(_T("none"));

            // TODO:
            // тут утечки памяти. И вообще надо переписать. все намешано
            // нужно с комментами прояснить всю конвертацию
			XmlUtils::CXmlNodes oChilds;
			if (oNodeShape.GetNodes(_T("*"), oChilds))
			{
				EFilltype eFillType = etSolidFill;
				CString sTxbxContent = _T("<w:txbxContent><w:p>");
				CString sParaRun = _T("<w:r>");
				CString srPr;
				CString sFont = (_T("Arial Black"));
				//CString sDashStyle;
				int nFontSize = 36;
				LONG lChildsCount = oChilds.GetCount();
				CString strString = _T("");
				BYTE lAlpha;
				bool bOpacity = false;
				bool bOpacity2 = false;
				PPTX::Logic::ColorModifier oMod;
				PPTX::Logic::ColorModifier oMod2;
				std::vector<PPTX::Logic::UniColor*> arColors;
				std::vector<PPTX::Logic::UniColor*> arColorsNew;
				std::vector<int> arPos;
				std::vector<int> arPosNew;
				std::map<PPTX::Logic::UniColor*, int> arGradMap;
				double nFocus = 0;
				int nAngle = 90;
				bool bColors = false;

                int R = 255;
                int G = 255;
                int B = 255;

				nullable_string sFillColor;
				oNodeShape.ReadAttributeBase(L"fillcolor", sFillColor);
				if (sFillColor.is_init())
				{
					eFillType = etSolidFill;
					NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sFillColor);
					PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
					pSolid->m_namespace = _T("a");
					pSolid->Color.Color = new PPTX::Logic::SrgbClr();
					pSolid->Color.Color->SetRGB(color.R, color.G, color.B);					
					arColors.push_back(&pSolid->Color);
					arPos.push_back(0);

                    R = color.R;
                    G = color.G;
                    B = color.B;
				}
				else
					eFillType = etNoFill;

                if (eFillType == etNoFill)
                {
                    // default color for vml = white
                    nullable_string sFilled;
                    oNodeShape.ReadAttributeBase(L"filled", sFilled);
                    if (!sFilled.is_init() || (*sFilled != _T("false") && *sFilled != _T("f")))
                    {
                        eFillType = etSolidFill;
                        PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
                        pSolid->m_namespace = _T("a");
                        pSolid->Color.Color = new PPTX::Logic::SrgbClr();
                        pSolid->Color.Color->SetRGB(R, G, B);
                        arColors.push_back(&pSolid->Color);
                        arPos.push_back(0);
                    }
                }

				for (LONG k = 0; k < lChildsCount; k++)
				{
					XmlUtils::CXmlNode oNodeP;
					oChilds.GetAt(k, oNodeP);

					CString strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
					if (_T("textpath") == strNameP)
					{
						strString = oNodeP.GetAttribute(_T("string"));
                        // мы используем его в хмл
                        CorrectXmlString(strString);

						CString strStyle = oNodeP.GetAttribute(_T("style"));
						PPTX::CCSS oCSSParser;
						oCSSParser.LoadFromString2(strStyle);
						std::map<CString, CString>::iterator pPair = oCSSParser.m_mapSettings.find(_T("font-family"));
						if (pPair != oCSSParser.m_mapSettings.end())
						{
							sFont = pPair->second;
							sFont.Replace(_T("\""), _T(""));
						}
						pPair = oCSSParser.m_mapSettings.find(_T("font-size"));
						if (pPair != oCSSParser.m_mapSettings.end())
						{
							nFontSize = _wtoi(pPair->second.GetBuffer()) * 2;
                        }

                        if (true)
                        {
                            nullable_string sFitPath;
                            oNodeP.ReadAttributeBase(L"fitpath", sFitPath);
                            if (sFitPath.is_init() && (*sFitPath == _T("true") || *sFitPath == _T("t")))
                            {
                                nFontSize = 2;
                            }

                            nullable_string sFitShape;
                            oNodeP.ReadAttributeBase(L"fitshape", sFitShape);
                            if (sFitShape.is_init() && (*sFitShape == _T("true") || *sFitShape == _T("t")))
                            {
                                nFontSize = 2;
                            }

                            nullable_string sTrim;
                            oNodeP.ReadAttributeBase(L"trim", sTrim);
                            if (sTrim.is_init() && (*sTrim == _T("true") || *sTrim == _T("t")))
                            {
                                bIsTrimTextPath = true;
                            }
                        }

					}
					else if (_T("fill") == strNameP)
					{						
						nullable_string sOpacity;
						nullable_string sOpacity2;
						nullable_string sColor2;
						nullable_string sType;
						nullable_string sFocus;
						nullable_string sAngle;
						nullable_string sColors;

						oNodeP.ReadAttributeBase(L"opacity", sOpacity);
						oNodeP.ReadAttributeBase(L"opacity2", sOpacity2);
						oNodeP.ReadAttributeBase(L"color2", sColor2);
						oNodeP.ReadAttributeBase(L"type", sType);
						oNodeP.ReadAttributeBase(L"focus", sFocus);
						oNodeP.ReadAttributeBase(L"angle", sAngle);
						oNodeP.ReadAttributeBase(L"colors", sColors);

						if (sFocus.is_init())
						{
							nFocus = _wtoi(sFocus->GetBuffer())/100.0;
						}
						if (sOpacity.is_init())
						{
							bOpacity = true;
							lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);
							oMod.name = _T("alpha");
							int nA = (int)(lAlpha * 100000.0 / 255.0);
							oMod.val = nA;
							if (arColors.at(0)->is_init())
								arColors.at(0)->Color->Modifiers.push_back(oMod);
						}
						if (sOpacity2.is_init())
						{
							bOpacity2 = true;
							lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity2);
							oMod.name = _T("alpha");
							int nA = (int)(lAlpha * 100000.0 / 255.0);
							oMod2.val = nA;
							if (arColors.at(1)->is_init())
								arColors.at(1)->Color->Modifiers.push_back(oMod2);
						}
						if (sColor2.is_init())
						{
							if (etSolidFill == eFillType)
								eFillType = etGradFill;

                            NSPresentationEditor::CColor color;
                            if (sColor2->Find(L"fill") != -1)
                            {
                                std::wstring sColorEffect = string2std_string(*sColor2);
                                if (sColorEffect.length() > 5)
                                    sColorEffect = sColorEffect.substr(5);

                                int resR, resG, resB;
                                GetColorWithEffect(sColorEffect, R, G, B, resR, resG, resB);

                                color.R = resR;
                                color.G = resG;
                                color.B = resB;
                            }
                            else
                            {
                                color = NS_DWC_Common::getColorFromString(*sColor2);
                            }

							PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
							oColor->Color = new PPTX::Logic::SrgbClr();
							oColor->Color->SetRGB(color.R, color.G, color.B);

							if (bOpacity2)
								oColor->Color->Modifiers.push_back(oMod2);

							if (arColors.size() > 0)
								arColors.push_back(oColor);
							else
							{
								PPTX::Logic::UniColor *oColor1 = new PPTX::Logic::UniColor();
								oColor1->Color = new PPTX::Logic::SrgbClr();
                                oColor1->Color->SetRGB(color.R, color.G, color.B);

								arColors.push_back(oColor1);
								arPos.push_back(0);
								arColors.push_back(oColor);
							}
							arPos.push_back(100000);
						}
						if (sType.is_init())
						{
							if (*sType == _T("gradient"))
							{
								eFillType = etGradFill;
							}
                            else if (*sType == _T("gradientradial") || *sType == _T("gradientRadial"))
							{
                                // TODO: дописать радиальный!!!
                                eFillType = etGradFill;
							}
						}
						if (sAngle.is_init())
						{
							nAngle =  _wtoi(sAngle->GetBuffer());
							nAngle =  (-1)*nAngle + 90;
						}
						if (sColors.is_init())
						{
							bColors = true;
							CString strColors = sColors.get();
							CString resToken;
							int curPos = 0;

							arColors.clear();
							arPos.clear();

							resToken = strColors.Tokenize(_T(";"), curPos);
							while (resToken != _T(""))
							{
								
								CString strPos = resToken.Left(resToken.Find(_T(" ")));
								CString strColor = resToken.Right(resToken.GetLength() - resToken.Find(_T(" ")) - 1);
								double pos;
								pos = _wtof(strPos.GetBuffer());
								NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(strColor);
								PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
								oColor->Color = new PPTX::Logic::SrgbClr();
								oColor->Color->SetRGB(color.R, color.G, color.B);

								if (pos <= 1)
									pos = 100000 * pos;
								else
									pos = pos / 65536 * 100000;
								arColors.push_back(oColor);
								arPos.push_back((int)pos);

								arGradMap.insert(std::pair<PPTX::Logic::UniColor*, int>(oColor, (int)pos) );

								resToken = strColors.Tokenize(_T(";"), curPos);
							};
						}

					}
					/*else if (_T("dashstyle") == strNameP)
					{
						nullable_string sStrokeDashStyle;
						oNodeP.ReadAttributeBase(L"dashstyle", sStrokeDashStyle);
						if (sStrokeDashStyle.is_init())
						{
							if (*sStrokeDashStyle == _T("solid"))
								sDashStyle = _T("solid");
							else if (*sStrokeDashStyle == _T("shortdash"))
								sDashStyle = _T("sysDash");
							else if (*sStrokeDashStyle == _T("shortdot"))
								sDashStyle = _T("sysDot");
							else if (*sStrokeDashStyle == _T("shortdashdot"))
								sDashStyle = _T("sysDashDot");
							else if (*sStrokeDashStyle == _T("shortdashdotdot"))
								sDashStyle = _T("sysDashDotDot");
							else if (*sStrokeDashStyle == _T("dot"))
								sDashStyle = _T("dot");
							else if (*sStrokeDashStyle == _T("dash"))
								sDashStyle = _T("dash");
							else if (*sStrokeDashStyle == _T("longdash"))
								sDashStyle = _T("lgDash");
							else if (*sStrokeDashStyle == _T("dashdot"))
								sDashStyle = _T("dashDot");
							else if (*sStrokeDashStyle == _T("longdashdot"))
								sDashStyle = _T("lgDashDot");
							else if (*sStrokeDashStyle == _T("longdashdotdot"))
								sDashStyle = _T("lgDashDotDot");
							else
								sDashStyle = _T("solid");
						}
					}*/
				}

				//srPr += _T("<w:rPr>");
				srPr += _T("<w:rFonts w:ascii=\"") + sFont + _T("\" w:hAnsi=\"") + sFont + _T("\"/>");
				CString strSize;
				strSize.Format(_T("%d"), nFontSize);
				srPr += _T("<w:sz w:val=\"") + strSize + _T("\"/><w:szCs w:val=\"") + strSize + _T("\"/>");

				 
				nullable_string sStrokeColor;
				nullable_string sStrokeWeight;
				nullable_string sStroked;				
				oNodeShape.ReadAttributeBase(L"strokecolor", sStrokeColor);
				oNodeShape.ReadAttributeBase(L"strokeweight", sStrokeWeight);
				oNodeShape.ReadAttributeBase(L"stroked", sStroked);

				//textFill
				srPr += _T("<w14:textFill>");			
				
					
				if (eFillType == etSolidFill)
				{
					srPr += _T("<w14:solidFill>");
					srPr += arColors.at(0)->toXML();
					srPr += _T("</w14:solidFill>");
				}
				else if (eFillType == etGradFill)
				{					
					srPr += _T("<w14:gradFill><w14:gsLst>");
					int nSize = arColors.size();
					bool bRevert = false;
					int nColorsLen = arColors.size();

					int nDiff = nSize - 1;
					if (nFocus != 1 && nFocus != 0)
						nSize = nSize + nDiff;

					double nShift = 100000 / nSize;
					double dNewZero = 100000 * nFocus;

					//(0 < nFocus &&  nFocus < 1)
					if (((nAngle == 90) && (-1 < nFocus &&  nFocus < 0)) || ((nAngle != 90) && (0 < nFocus &&  nFocus < 1)))
					{
						if (nAngle == 90)
							dNewZero *= -1;

						arColorsNew.push_back(arColors.at(nColorsLen - 1));
						arPosNew.push_back(0);

						for (int i = nColorsLen - 2; i > 0; i--)
						{
							arColorsNew.push_back(arColors.at(i));

							double dPosNew = dNewZero * arPos.at(i) / 100000;
							arPosNew.push_back((int)dPosNew);
						}

						for (int i = 0; i < nColorsLen; i++)
						{
							arColorsNew.push_back(arColors.at(i));

							double dPosNew = dNewZero * arPos.at(i) / 100000 + dNewZero;
							arPosNew.push_back((int)dPosNew);
						}
					}
					//else if (-1 < nFocus &&  nFocus < 0)
					else if (((nAngle != 90) && (-1 < nFocus &&  nFocus < 0)) || ((nAngle == 90) && (0 < nFocus &&  nFocus < 1)))
					{
						
						if (nAngle != 90)
							dNewZero *= -1;

						for (int i = 0; i < nColorsLen-1; i++)
						{
							arColorsNew.push_back(arColors.at(i));

							double dPosNew = dNewZero * arPos.at(i) / 100000 ;
							arPosNew.push_back((int)dPosNew);
						}

						arColorsNew.push_back(arColors.at(nColorsLen - 1));
						arPosNew.push_back(dNewZero);

						for (int i = nColorsLen-2; i >= 0; i--)
						{
							arColorsNew.push_back(arColors.at(i));

							double n1 = 1 - (double)arPos.at(i) / 100000;
							double dPosNew = dNewZero * n1 + dNewZero;
							arPosNew.push_back((int)dPosNew);
						}							
					}
					//nFocus == 0
					else if ((nAngle != 90 && nFocus == 0) || (nAngle == 90 && nFocus == 1))
					{
						for (int i = 0; i < nColorsLen; i++)
						{
							arColorsNew.push_back(arColors.at(i));
							arPosNew.push_back(arPos.at(i));
						}
					}
					//nFocus == 1
					else if ((nAngle != 90 && nFocus == 1) || (nAngle == 90 && nFocus == 0))
					{
						for (int i = nColorsLen - 1; i >= 0; i--)
						{
							arColorsNew.push_back(arColors.at(i));
							arPosNew.push_back(arPos.at(nColorsLen - i - 1));
						}
					}

					for (int i = 0; i < arColorsNew.size(); i++)
					{
						int pos = arPosNew.at(i);
						CString color = arColorsNew.at(i)->toXML();

						CString strPos;
						strPos.Format(_T("%d"), pos);
						srPr += _T("<w14:gs w14:pos = \"") + strPos + _T("\">");
						srPr += color;
						srPr += _T("</w14:gs>");
					}
					
					CString strAngle;
					strAngle.Format(_T("%d"), nAngle * 60000);
					srPr += _T("</w14:gsLst><w14:lin w14:ang=\"") + strAngle + _T("\" w14:scaled=\"0\"/></w14:gradFill>");
				}
				else if (eFillType == etNoFill)
					srPr += _T("<w14:noFill/>");

				srPr += _T("</w14:textFill>");

				//textOutline
				double m_dValue;
				CString strStrokeW;
				if (sStrokeWeight.is_init())
				{
					CString strW(*sStrokeWeight);
					strW.Remove('pt');
					m_dValue = _wtof(strW);
				}
				else
					m_dValue = 1;
				strStrokeW.Format(_T("%d"), (int)Pt_To_Emu(m_dValue));
				srPr += _T("<w14:textOutline w14:w=\"") + strStrokeW + _T("\">");

                smart_ptr<PPTX::Logic::SolidFill> pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = _T("a");
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				NSPresentationEditor::CColor color;
				
				bool bStroked = true;
				if (sStroked.is_init())
				{
					if (*sStroked == _T("false") || *sStroked == _T("f"))
					{
						srPr += _T("<w14:noFill/>");
						bStroked = false;
					}
				}				
				
				if (sStrokeColor.is_init())
				{
					color = NS_DWC_Common::getColorFromString(*sStrokeColor);
					pSolid->Color.Color->SetRGB(color.R, color.G, color.B);
				}
				else
					pSolid->Color.Color->SetRGB(0x00, 0x00, 0x00);

				if (bStroked)
					srPr += pSolid->toXML();

				srPr += _T("</w14:textOutline>");			

				//srPr += _T("</w:rPr>");
                sParaRun += _T("<w:rPr>") + srPr + _T("</w:rPr>") + _T("<w:t>") + strString + _T("</w:t></w:r>");
				sTxbxContent += _T("<w:pPr><w:rPr>") + srPr + _T("</w:rPr></w:pPr>") + sParaRun + _T("</w:p></w:txbxContent>");
				pShape->TextBoxShape = sTxbxContent;
			}
			strXmlPPTX = _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");

            // у старого wordArt никаких отступов
            pShape->TextBoxBodyPr->lIns = 0;
            pShape->TextBoxBodyPr->tIns = 0;
            pShape->TextBoxBodyPr->rIns = 0;
            pShape->TextBoxBodyPr->bIns = 0;

            if (!bIsTrimTextPath)
            {
                // нужно для данного размера шейпа выставить отступы сверху и снизу
                // top: Ascent - CapHeight
                // bottom: Descent
            }
		}
		else
		{
			strXmlPPTX = oShapeElem.ConvertPPTShapeToPPTX(true);
		}

		XmlUtils::CXmlNode oNodeG;
		oNodeG.FromXmlString(strXmlPPTX);
		pShape->spPr.Geometry = oNodeG;

		XmlUtils::CXmlNode oNodeTextBox;
		
		CString sTextboxStyle;
		if (oNodeShape.GetNode(_T("v:textbox"), oNodeTextBox))
		{
			XmlUtils::CXmlNode oNodeContent;
			if (oNodeTextBox.GetNode(_T("w:txbxContent"), oNodeContent))
			{
				pShape->TextBoxShape = oNodeContent.GetXml();
			}
			else if (oNodeTextBox.GetNode(_T("div"), oNodeContent))
			{//The <div> tag was used to create an area in which the rotation would take place. 
				ConvertTextVML(oNodeTextBox, pShape);
			}

			CString sTextInset		= oNodeTextBox.GetAttribute(_T("inset"));
			CString sTextInsetMode	= oNodeTextBox.GetAttribute(_T("o:insetmode"));
					sTextboxStyle	= oNodeTextBox.GetAttribute(_T("style"));

			if (_T("") != sTextInset && ((_T("") == sTextInsetMode) || (_T("custom") == sTextInsetMode)))
			{
				PPTX::CStringTrimmer oTrimmer;
				oTrimmer.m_Separator = (TCHAR)',';
				oTrimmer.LoadFromString(sTextInset);

				double dTextMarginLeft = oTrimmer.GetParameter(0, 0.1);
				double dTextMarginTop = oTrimmer.GetParameter(1, 0.05);
				double dTextMarginRight = oTrimmer.GetParameter(2, 0.1);
				double dTextMarginBottom = oTrimmer.GetParameter(3, 0.05);

				pShape->TextBoxBodyPr->lIns = (int)(12700 * dTextMarginLeft);
				pShape->TextBoxBodyPr->tIns = (int)(12700 * dTextMarginTop);
				pShape->TextBoxBodyPr->rIns = (int)(12700 * dTextMarginRight);
				pShape->TextBoxBodyPr->bIns = (int)(12700 * dTextMarginBottom);
			}

            if (!sTextboxStyle.IsEmpty())
            {//todooo прописать все остальное 
                PPTX::CCSS oCSSParser;
                oCSSParser.LoadFromString2(sTextboxStyle);

                if (oCSSParser.m_mapSettings.size() > 0)
                {
                    std::map<CString, CString>::iterator pPair = oCSSParser.m_mapSettings.find(_T("layout-flow"));

                    if (pPair != oCSSParser.m_mapSettings.end())
                    {
                        if (pPair->second == _T("vertical"))
                        {
                            pShape->TextBoxBodyPr->vert = new PPTX::Limit::TextVerticalType();
                            pShape->TextBoxBodyPr->vert->set(L"vert");
                        }
                    }

                    pPair = oCSSParser.m_mapSettings.find(_T("mso-layout-flow-alt"));
                    if (pPair != oCSSParser.m_mapSettings.end())
                    {
                        if (pPair->second == _T("bottom-to-top"))
                        {
                            if (pShape->TextBoxBodyPr->vert.IsInit() == false)
                                pShape->TextBoxBodyPr->vert = new PPTX::Limit::TextVerticalType();
                            pShape->TextBoxBodyPr->vert->set(L"vert270");
                       }
                    }
					if (pShape->TextBoxBodyPr->vert.IsInit())
					{
						if (pShape->txBody.IsInit() == false)
                            pShape->txBody = new PPTX::Logic::TxBody();
						pShape->txBody->bodyPr.vert = pShape->TextBoxBodyPr->vert;		
					}

                    pPair = oCSSParser.m_mapSettings.find(_T("mso-rotate"));
                    if (pPair != oCSSParser.m_mapSettings.end())
                    {
						try
						{
							pShape->TextBoxBodyPr->rot = _wtoi(pPair->second) * 60000; //для docx, xlsx
							if (pShape->txBody.IsInit() == false)	//для pptx
                                pShape->txBody = new PPTX::Logic::TxBody();
							pShape->txBody->bodyPr.rot = pShape->TextBoxBodyPr->rot;
						}catch(...){}
                    }

                }
            }
		}

		XmlUtils::CXmlNode oNodeTextData;
		if (oNodeShape.GetNode(_T("p:textdata"), oNodeTextData))
		{
			CString sId = oNodeTextData.GetAttribute(_T("id"));

			if (sId.GetLength() > 0 && m_pBinaryWriter->m_pCommonRels->IsInit())
			{
				PPTX::RId rId(sId);
				smart_ptr<PPTX::LegacyDiagramText> pExt = (*m_pBinaryWriter->m_pCommonRels)->legacyDiagramText(rId);

				if (pExt.IsInit())
				{
					if (pShape->txBody.IsInit() == false)
						pShape->txBody = new PPTX::Logic::TxBody();
					if (pShape->txBody.IsInit())
					{
						PPTX::Logic::Paragraph p;
						PPTX::Logic::Run  *r = new PPTX::Logic::Run();
						r->SetText(pExt->m_Text);

						if (pExt->m_arrRPr.size() > 0)
						{
							r->rPr = pExt->m_arrRPr[0];
						}
						
						PPTX::Logic::RunElem elm;
						p.RunElems.push_back(elm);

						if (pExt->m_arrPPr.size() > 0)
						{
							p.pPr = pExt->m_arrPPr[0];
						}
						p.RunElems.back().InitRun(r);

						pShape->txBody->Paragrs.push_back(p);
					}
				}
			}
		}

		CString strStyle = oNodeShape.GetAttribute(_T("style"));
		if (_T("") != strStyleAdvenced)
			strStyle += strStyleAdvenced;

		PPTX::CCSS oCSSParser;
		oCSSParser.LoadFromString2(strStyle);

		CSpTreeElemProps oProps;
		oProps.IsTop = bIsTop;
		CString strMainPos = GetDrawingMainProps(oNodeShape, oCSSParser, oProps);
/////////////////////////////////////////text options

		if (oCSSParser.m_mapSettings.size() > 0)
		{
			std::map<CString, CString>::iterator pPair = oCSSParser.m_mapSettings.find(_T("v-text-anchor"));
			
			if (pPair != oCSSParser.m_mapSettings.end())
			{
				if (pPair->second == _T("middle"))					pShape->TextBoxBodyPr->anchor = _T("ctr");
				if (pPair->second == _T("bottom"))					pShape->TextBoxBodyPr->anchor = _T("b");
				if (pPair->second == _T("top-center"))				pShape->TextBoxBodyPr->anchor = _T("t");
				if (pPair->second == _T("middle-center"))			pShape->TextBoxBodyPr->anchor = _T("ctr");
				if (pPair->second == _T("bottom-center"))			pShape->TextBoxBodyPr->anchor = _T("b");
				if (pPair->second == _T("top-baseline"))			pShape->TextBoxBodyPr->anchor = _T("t");
				if (pPair->second == _T("bottom-baseline"))			pShape->TextBoxBodyPr->anchor = _T("b");
				if (pPair->second == _T("top-center-baseline"))		pShape->TextBoxBodyPr->anchor = _T("t");
				if (pPair->second == _T("bottom-center-baseline"))	pShape->TextBoxBodyPr->anchor = _T("b");
			}
				
			pPair = oCSSParser.m_mapSettings.find(_T("mso-wrap-style"));
			if (pPair != oCSSParser.m_mapSettings.end() )
			{
				if (pPair->second == _T("none"))
					pShape->TextBoxBodyPr->wrap = _T("none");
				else
					pShape->TextBoxBodyPr->wrap = _T("wrap");
			}
			else if (!pPPTShape->IsWordArt())
				pShape->TextBoxBodyPr->wrap = _T("wrap");
		}
////////////////////////////////////////////////////////////////////////////////////
		if (bIsTop)
		{			
			SendMainProps(strMainPos, pMainProps);

			if (m_oxfrm_override.IsInit())
			{
				pShape->spPr.xfrm = m_oxfrm_override;
			}
			else	
			{
				//oProps.X выставлять не надо, они учтены в pMainProps
				pShape->spPr.xfrm = new PPTX::Logic::Xfrm();
				pShape->spPr.xfrm->offX = 0;
				pShape->spPr.xfrm->offY = 0;
				pShape->spPr.xfrm->extX = oProps.Width;
				pShape->spPr.xfrm->extY = oProps.Height;
			}

			std::map<CString, CString>::iterator pPair;

			pPair = oCSSParser.m_mapSettings.find(_T("flip"));
			if (oCSSParser.m_mapSettings.end() != pPair)
			{
				if (pPair->second == _T("x"))
					pShape->spPr.xfrm->flipH = true;
				else if (pPair->second == _T("y"))
					pShape->spPr.xfrm->flipV = true;
				else if ((pPair->second == _T("xy")) || (pPair->second == _T("yx")) || (pPair->second == _T("x y")) || (pPair->second == _T("y x")))
				{
					pShape->spPr.xfrm->flipH = true;
					pShape->spPr.xfrm->flipV = true;
				}
			}

			pPair = oCSSParser.m_mapSettings.find(_T("rotation"));
			if (oCSSParser.m_mapSettings.end() != pPair)
			{
				pShape->spPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pPair->second, pShape->spPr.xfrm->flipH, pShape->spPr.xfrm->flipV);
			}
		}
		else
		{
			m_pBinaryWriter->m_lWidthCurShape = 0;
			m_pBinaryWriter->m_lHeightCurShape = 0;

			pShape->spPr.xfrm = new PPTX::Logic::Xfrm();
			pShape->spPr.xfrm->offX = oProps.X;
			pShape->spPr.xfrm->offY = oProps.Y;
			pShape->spPr.xfrm->extX = oProps.Width;
			pShape->spPr.xfrm->extY = oProps.Height;

			std::map<CString, CString>::iterator pPair;

			pPair = oCSSParser.m_mapSettings.find(_T("flip"));
			if (oCSSParser.m_mapSettings.end() != pPair)
			{
				if (pPair->second == _T("x"))
					pShape->spPr.xfrm->flipH = true;
				else if (pPair->second == _T("y"))
					pShape->spPr.xfrm->flipV = true;
				else if ((pPair->second == _T("xy")) || (pPair->second == _T("yx")) || (pPair->second == _T("x y")) || (pPair->second == _T("y x")))
				{
					pShape->spPr.xfrm->flipH = true;
					pShape->spPr.xfrm->flipV = true;
				}
			}

			pPair = oCSSParser.m_mapSettings.find(_T("rotation"));
			if (oCSSParser.m_mapSettings.end() != pPair)
			{
				pShape->spPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pPair->second, pShape->spPr.xfrm->flipH, pShape->spPr.xfrm->flipV);
			}
		}

		if (!bIsTop)
		{
			pShape->nvSpPr.cNvPr.id = m_lNextId;
			m_lNextId++;
		}
		else
		{
			pShape->nvSpPr.cNvPr.id = -1;
		}

		elem.InitElem(pShape);

		if (!pPPTShape->IsWordArt())
			CheckPenShape(elem, oNodeShape, pPPTShape->m_eType, pPPTShape);		
		CheckBrushShape(elem, oNodeShape, pPPTShape->m_eType, pPPTShape);
	}

	return elem;
}

PPTX::Logic::SpTreeElem CDrawingConverter::doc_LoadGroup(XmlUtils::CXmlNode& oNode, CString**& pMainProps, bool bIsTop)
{
	PPTX::Logic::SpTreeElem elem;

	PPTX::Logic::SpTree* pTree = new PPTX::Logic::SpTree();

	if (bIsTop)
		pTree->m_name = _T("wpg:wgp");
	else
		pTree->m_name = _T("wpg:grpSp");

	XmlUtils::CXmlNodes oNodes;
	if (oNode.GetNodes(_T("*"), oNodes))
	{
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oNodeT;
			oNodes.GetAt(i, oNodeT);

			CString strNameP = XmlUtils::GetNameNoNS(oNodeT.GetName());

			if (_T("shapetype") == strNameP)
			{
				//AddShapeType(oNodeT.GetXml()); 
				CString strId = oNodeT.GetAttribute(_T("id"));

				if (strId.GetLength() > 0)
				{
					if (m_mapShapeTypes.find(strId) == m_mapShapeTypes.end())
					{
						CPPTShape* pShape = new CPPTShape();
						pShape->m_bIsShapeType = true;

						pShape->LoadFromXMLShapeType(oNodeT);

						CShape* pS = new CShape(NSBaseShape::unknown, 0);
						pS->m_pShape = pShape;
						LoadCoordSize(oNodeT, pS);

						m_mapShapeTypes.insert(std::pair<CString, CShape*>(strId, pS));	
					}
				}
			}
			else if (_T("shape") == strNameP ||
					_T("rect") == strNameP ||
					_T("oval") == strNameP ||
					_T("line") == strNameP ||
					_T("polyline") == strNameP ||
					_T("roundrect") == strNameP)
			{
				PPTX::Logic::SpTreeElem _el = doc_LoadShape(oNodeT, pMainProps, false);
				
				if (_el.is_init())
					pTree->SpTreeElems.push_back(_el);
			}
			else if (_T("group") == strNameP)
			{
				PPTX::Logic::SpTreeElem _el = doc_LoadGroup(oNodeT, pMainProps, false);

				if (_el.is_init())
					pTree->SpTreeElems.push_back(_el);
			}
		}
	}

	CString strStyle = oNode.GetAttribute(_T("style"));

	PPTX::CCSS oCSSParser;
	oCSSParser.LoadFromString2(strStyle);

	CSpTreeElemProps oProps;
	oProps.IsTop = bIsTop;
	CString strMainPos = GetDrawingMainProps(oNode, oCSSParser, oProps);

	LONG lCoordOriginX = 0;
	LONG lCoordOriginY = 0;
	LONG lCoordSizeW = oProps.Width;
	LONG lCoordSizeH = oProps.Height;
	
	CString strCoordSize = oNode.GetAttributeOrValue(_T("coordsize"));
	if (strCoordSize != _T(""))
	{
		std::vector<CString> oArray;
		NSStringUtils::ParseString(_T(","), strCoordSize, &oArray);

		if (oArray.size() >= 2)
		{
			lCoordSizeW = XmlUtils::GetInteger(oArray[0]);
			lCoordSizeH = XmlUtils::GetInteger(oArray[1]);
		}
	}

	CString strCoordOrigin = oNode.GetAttributeOrValue(_T("coordorigin"));
	if (strCoordOrigin != _T(""))
	{
		std::vector<CString> oArray;
		NSStringUtils::ParseString(_T(","), strCoordOrigin, &oArray);

		if (oArray.size() >= 2)
		{
			lCoordOriginX = XmlUtils::GetInteger(oArray[0]);
			lCoordOriginY = XmlUtils::GetInteger(oArray[1]);
		}
	}

	if (bIsTop)
	{
		SendMainProps(strMainPos, pMainProps);

		if (m_oxfrm_override.IsInit())
		{
			pTree->grpSpPr.xfrm = m_oxfrm_override;
			
			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;		}
		else
		{
			pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
			pTree->grpSpPr.xfrm->offX = 0;
			pTree->grpSpPr.xfrm->offY = 0;
			pTree->grpSpPr.xfrm->extX = oProps.Width;
			pTree->grpSpPr.xfrm->extY = oProps.Height;

			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;
		}

		std::map<CString, CString>::iterator pPair;

		pPair = oCSSParser.m_mapSettings.find(_T("flip"));
		if (oCSSParser.m_mapSettings.end() != pPair)
		{
			if (pPair->second == _T("x"))
				pTree->grpSpPr.xfrm->flipH = true;
			else if (pPair->second == _T("y"))
				pTree->grpSpPr.xfrm->flipV = true;
			else if ((pPair->second == _T("xy")) || (pPair->second == _T("yx")) || (pPair->second == _T("x y")) || (pPair->second == _T("y x")))
			{
				pTree->grpSpPr.xfrm->flipH = true;
				pTree->grpSpPr.xfrm->flipV = true;
			}
		}

		pPair = oCSSParser.m_mapSettings.find(_T("rotation"));
		if (oCSSParser.m_mapSettings.end() != pPair)
		{
			pTree->grpSpPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pPair->second, pTree->grpSpPr.xfrm->flipH, pTree->grpSpPr.xfrm->flipV);
		}
	}
	else
	{
		if (m_oxfrm_override.IsInit())
		{
			pTree->grpSpPr.xfrm = m_oxfrm_override;
		}
		else
		{
			pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
			pTree->grpSpPr.xfrm->offX = oProps.X;
			pTree->grpSpPr.xfrm->offY = oProps.Y;
			pTree->grpSpPr.xfrm->extX = oProps.Width;
			pTree->grpSpPr.xfrm->extY = oProps.Height;

			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;
		}

		std::map<CString, CString>::iterator pPair;

		pPair = oCSSParser.m_mapSettings.find(_T("flip"));
		if (oCSSParser.m_mapSettings.end() != pPair)
		{
			if (pPair->second == _T("x"))
				pTree->grpSpPr.xfrm->flipH = true;
			else if (pPair->second == _T("y"))
				pTree->grpSpPr.xfrm->flipV = true;
			else if ((pPair->second == _T("xy")) || (pPair->second == _T("yx")) || (pPair->second == _T("x y")) || (pPair->second == _T("y x")))
			{
				pTree->grpSpPr.xfrm->flipH = true;
				pTree->grpSpPr.xfrm->flipV = true;
			}
		}

		pPair = oCSSParser.m_mapSettings.find(_T("rotation"));
		if (oCSSParser.m_mapSettings.end() != pPair)
		{
			pTree->grpSpPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pPair->second, pTree->grpSpPr.xfrm->flipH, pTree->grpSpPr.xfrm->flipV);			
		}
	}

	if (!bIsTop)
	{
		pTree->nvGrpSpPr.cNvPr.id = m_lNextId;
		m_lNextId++;
	}
	else
	{
		pTree->nvGrpSpPr.cNvPr.id = -1;
	}
	
	elem.InitElem(pTree);
	return elem;
}

void CDrawingConverter::LoadCoordSize(XmlUtils::CXmlNode& oNode, CShape* pShape)
{
	pShape->m_dWidthLogic = ShapeSizeVML;
	pShape->m_dHeightLogic = ShapeSizeVML;
	XmlUtils::CXmlNode oNodeTemplate;
	if (oNode.GetNode(_T("coordsize"), oNodeTemplate))
	{
		CString strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("val"));
		if (strCoordSize != _T(""))
		{
			std::vector<CString> oArray;
			NSStringUtils::ParseString(_T(","), strCoordSize, &oArray);

			if (oArray.size() >= 2)
			{
                pShape->m_dWidthLogic  = (std::max)(XmlUtils::GetInteger(oArray[0]), 1);
                pShape->m_dHeightLogic = (std::max)(XmlUtils::GetInteger(oArray[1]), 1);
			}
		}
	}
	else
	{
		CString strCoordSize = oNode.GetAttributeOrValue(_T("coordsize"));
		if (strCoordSize != _T(""))
		{
			std::vector<CString> oArray;
			NSStringUtils::ParseString(_T(","), strCoordSize, &oArray);

			if (oArray.size() >= 2)
			{
                pShape->m_dWidthLogic  = (std::max)(XmlUtils::GetInteger(oArray[0]), 1);
                pShape->m_dHeightLogic = (std::max)(XmlUtils::GetInteger(oArray[1]), 1);
			}
		}
	}

	pShape->m_pShape->m_oPath.SetCoordsize((LONG)pShape->m_dWidthLogic, (LONG)pShape->m_dHeightLogic);
}

CString CDrawingConverter::GetDrawingMainProps(XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps)
{
	std::map<CString, CString>::iterator pPair;

	bool bIsInline = false;

	if ((oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(_T("left")))			&&
		(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(_T("margin-left")))	&&
		(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(_T("top")))			&& 
		(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(_T("margin-top"))))
	{
		bIsInline = true;
	}

	if (!bIsInline)
	{
		pPair = oCssStyles.m_mapSettings.find(_T("position"));
		if (oCssStyles.m_mapSettings.end() != pPair && pPair->second == _T("static"))
		{
			bIsInline = true;
		}
	}

	SimpleTypes::CPoint parserPoint;
	double dKoef = 25.4 * 36000 / 72.0;
	double dKoefSize = oProps.IsTop ? dKoef : 1;

	LONG left	= 0;
	LONG top	= 0;
	LONG width	= 0;
	LONG height = 0;

	pPair = oCssStyles.m_mapSettings.find(_T("polyline_correct"));
	bool bIsPolyCorrect = (oCssStyles.m_mapSettings.end() != pPair) ? true : false;
	if (bIsPolyCorrect)
		dKoefSize = 1;

	if (!bIsInline)
	{
		pPair = oCssStyles.m_mapSettings.find(_T("margin-left"));
		if (oCssStyles.m_mapSettings.end() == pPair)
			pPair = oCssStyles.m_mapSettings.find(_T("left"));

		if (oCssStyles.m_mapSettings.end() != pPair)
		{
			 left = (LONG)(dKoefSize * parserPoint.FromString(pPair->second));
		}

		pPair = oCssStyles.m_mapSettings.find(_T("margin-top"));
		
		if (oCssStyles.m_mapSettings.end() == pPair)
			pPair = oCssStyles.m_mapSettings.find(_T("top"));

		if (oCssStyles.m_mapSettings.end() != pPair)
		{
			 top = (LONG)(dKoefSize * parserPoint.FromString(pPair->second));
		}
	}

	pPair = oCssStyles.m_mapSettings.find(_T("width"));
	if (oCssStyles.m_mapSettings.end() != pPair)
	{
		width = (LONG)(dKoefSize * parserPoint.FromString(pPair->second));
	}
	else
	{
		pPair = oCssStyles.m_mapSettings.find(_T("margin-right"));
		if (oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.end())
			width = (LONG)(dKoefSize * parserPoint.FromString(pPair->second)) - left;
	}

	pPair = oCssStyles.m_mapSettings.find(_T("height"));
	if (oCssStyles.m_mapSettings.end() != pPair)
	{
		height = (LONG)(dKoefSize * parserPoint.FromString(pPair->second));
	}
	else
	{
		pPair = oCssStyles.m_mapSettings.find(_T("margin-bottom"));
		if (oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.end())
			height = (LONG)(dKoefSize * parserPoint.FromString(pPair->second)) - top;
	}

	unsigned long margL = (unsigned long)(9 * dKoef);
	unsigned long margT = 0;
	unsigned long margR = (unsigned long)(9 * dKoef);
	unsigned long margB = 0;

	pPair = oCssStyles.m_mapSettings.find(_T("mso-wrap-distance-left"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		margL = (unsigned long)(dKoef * parserPoint.FromString(pPair->second));

	pPair = oCssStyles.m_mapSettings.find(_T("mso-wrap-distance-top"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		margT = (unsigned long)(dKoef * parserPoint.FromString(pPair->second));

	pPair = oCssStyles.m_mapSettings.find(_T("mso-wrap-distance-right"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		margR = (unsigned long)(dKoef * parserPoint.FromString(pPair->second));

	pPair = oCssStyles.m_mapSettings.find(_T("mso-wrap-distance-bottom"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		margB = (unsigned long)(dKoef * parserPoint.FromString(pPair->second));

	oProps.X = left;
	oProps.Y = top;
	oProps.Width = width;
	oProps.Height = height;

	m_pBinaryWriter->m_lWidthCurShape = width;
	m_pBinaryWriter->m_lHeightCurShape = height;

	if (bIsInline)
	{
		NSBinPptxRW::CXmlWriter oWriter;
		oWriter.StartNode(_T("wp:inline"));

		oWriter.StartAttributes();
		oWriter.WriteAttribute(_T("xmlns:wp"), (CString)_T("http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"));
		oWriter.WriteAttribute(_T("distT"), margT);
		oWriter.WriteAttribute(_T("distB"), margB);
		oWriter.WriteAttribute(_T("distL"), margL);
		oWriter.WriteAttribute(_T("distR"), margR);
		oWriter.EndAttributes();

		oWriter.StartNode(_T("wp:extent"));
		oWriter.StartAttributes();
		oWriter.WriteAttribute(_T("cx"), width);
		oWriter.WriteAttribute(_T("cy"), height);
		oWriter.EndAttributes();
		oWriter.EndNode(_T("wp:extent"));

		CString strId = _T("");
		strId.Format(_T("<wp:docPr id=\"%d\" name=\"\"/>"), m_lNextId);
		m_lNextId++;

		oWriter.WriteString(strId);

		oWriter.EndNode(_T("wp:inline"));

		return oWriter.GetXmlString();
	}

	NSBinPptxRW::CXmlWriter oWriter;
	oWriter.StartNode(_T("wp:anchor"));

	oWriter.StartAttributes();

	oWriter.WriteAttribute(_T("xmlns:wp"), (CString)_T("http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"));
	oWriter.WriteAttribute(_T("distT"), margT);
	oWriter.WriteAttribute(_T("distB"), margB);
	oWriter.WriteAttribute(_T("distL"), margL);
	oWriter.WriteAttribute(_T("distR"), margR);

	pPair = oCssStyles.m_mapSettings.find(_T("z-index"));
	nullable_int zIndex;
	if (oCssStyles.m_mapSettings.end() != pPair)
	{
		zIndex = (int)parserPoint.FromString(pPair->second);

		if (*zIndex >= 0)
		{
			oWriter.WriteAttribute(_T("relativeHeight"), *zIndex);
		}
		else
        {
           // DWORD dwIndex = (DWORD)(*zIndex);
            oWriter.WriteAttribute(_T("relativeHeight"), -(*zIndex));
		}		
	}

	XmlUtils::CXmlNode oNodeWrap = oNode.ReadNode(_T("w10:wrap"));
	CString strWType = _T("");
	if (oNodeWrap.IsValid())
	{
		strWType = oNodeWrap.GetAttribute(_T("type"));

		/*
		nullable_string sAnchorX;
		oNodeWrap.ReadAttributeBase(L"anchorx", sAnchorX);
		nullable_string sAnchorY;
		oNodeWrap.ReadAttributeBase(L"anchory", sAnchorY);

		if (sAnchorX.is_init())
			oCssStyles.m_mapSettings.insert(std::pair<>(_T("mso-position-horizontal-relative"), *sAnchorX);
		if (sAnchorY.is_init())
			oCssStyles.m_mapSettings.insert(std::pair<>(_T("mso-position-vertical-relative"), *sAnchorY);
		*/
	}

	CString strWrapPoints = oNode.GetAttribute(_T("wrapcoords"));
	CString strWrapPointsResult = _T("");
	if (_T("") != strWrapPoints)
	{
		std::vector<CString> arPoints;
		NSStringUtils::ParseString(_T(" "), strWrapPoints, &arPoints);

		int nCountP = arPoints.size();
		if (nCountP > 1 && ((nCountP % 2) == 0))
		{
			strWrapPointsResult = _T("<wp:wrapPolygon edited=\"1\">");

			for (int i = 0; i < nCountP; i += 2)
			{
				if (i == 0)
				{
					strWrapPointsResult += (_T("<wp:start x=\"") + arPoints[i] + _T("\" y=\"") + arPoints[i + 1] + _T("\"/>"));
				}
				else
				{
					strWrapPointsResult += (_T("<wp:lineTo x=\"") + arPoints[i] + _T("\" y=\"") + arPoints[i + 1] + _T("\"/>"));
				}
			}

            strWrapPointsResult += _T("</wp:wrapPolygon>");
		}
	}

	if ((!oNodeWrap.IsValid() || strWType == _T("")) && zIndex.is_init())
	{
		if (*zIndex > 0)
		{
			oWriter.WriteAttribute(_T("allowOverlap"), (CString)_T("1"));
		}
		else if (*zIndex < 0)
		{
			oWriter.WriteAttribute(_T("behindDoc"), (CString)_T("1"));
		}
	}

	oWriter.EndAttributes();

	oWriter.StartNode(_T("wp:positionH"));

	oWriter.StartAttributes();

	pPair = oCssStyles.m_mapSettings.find(_T("mso-position-horizontal-relative"));
	if (pPair != oCssStyles.m_mapSettings.end())
	{
		if (_T("char") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("character"));
		else if (_T("page") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("page"));
		else if (_T("margin") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("margin"));
		else if (_T("left-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("leftMargin"));
		else if (_T("right-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("rightMargin"));
		else if (_T("inner-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("insideMargin"));
		else if (_T("outer-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("outsideMargin"));
		else
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("column"));
	}
	else
	{
		oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("column"));
	}

	oWriter.EndAttributes();

	CString strPosH = _T("absolute");
	pPair = oCssStyles.m_mapSettings.find(_T("mso-position-horizontal"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		strPosH = pPair->second;

	if (strPosH == _T("absolute"))
	{
		oWriter.WriteString(_T("<wp:posOffset>"));
		oWriter.WriteLONG(left);
		oWriter.WriteString(_T("</wp:posOffset>"));		
	}
	else
	{
		oWriter.WriteString(_T("<wp:align>"));
		oWriter.WriteString(strPosH);
		oWriter.WriteString(_T("</wp:align>"));		
	}

	oWriter.EndNode(_T("wp:positionH"));

	oWriter.StartNode(_T("wp:positionV"));

	oWriter.StartAttributes();

	pPair = oCssStyles.m_mapSettings.find(_T("mso-position-vertical-relative"));
	if (pPair != oCssStyles.m_mapSettings.end())
	{
		if (_T("margin") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("margin"));
		else if (_T("text") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("paragraph"));
		else if (_T("page") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("page"));
		else if (_T("top-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("topMargin"));
		else if (_T("bottom-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("bottomMargin"));
		else if (_T("inner-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("insideMargin"));
		else if (_T("outer-margin-area") == pPair->second)
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("outsideMargin"));
		else
			oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("line"));
	}
	else
	{
		oWriter.WriteAttribute(_T("relativeFrom"), (CString)_T("paragraph"));
	}

	oWriter.EndAttributes();

	CString strPosV = _T("absolute");
	pPair = oCssStyles.m_mapSettings.find(_T("mso-position-vertical"));
	if (oCssStyles.m_mapSettings.end() != pPair)
		strPosV = pPair->second;

	if (strPosV == _T("absolute"))
	{
		oWriter.WriteString(_T("<wp:posOffset>"));
		oWriter.WriteLONG(top);
		oWriter.WriteString(_T("</wp:posOffset>"));		
	}
	else
	{
		oWriter.WriteString(_T("<wp:align>"));
		oWriter.WriteString(strPosV);
		oWriter.WriteString(_T("</wp:align>"));
	}

	oWriter.EndNode(_T("wp:positionV"));

	oWriter.StartNode(_T("wp:extent"));
	oWriter.StartAttributes();
	oWriter.WriteAttribute(_T("cx"), width);
	oWriter.WriteAttribute(_T("cy"), height);
	oWriter.EndAttributes();
	oWriter.EndNode(_T("wp:extent"));

	if (oNodeWrap.IsValid())
	{		
		if (strWType == _T("none") || strWType == _T(""))
			oWriter.WriteString(_T("<wp:wrapNone/>"));
		else if (strWType == _T("square"))
			oWriter.WriteString(_T("<wp:wrapSquare wrapText=\"bothSides\"/>"));
		else if (strWType == _T("topAndBottom"))
			oWriter.WriteString(_T("<wp:wrapTopAndBottom/>"));
		else if (strWType == _T("tight"))
		{
			if (_T("") == strWrapPointsResult)
			{
				oWriter.WriteString(_T("<wp:wrapTight wrapText=\"bothSides\"/>"));
			}
			else
			{
				oWriter.WriteString(_T("<wp:wrapTight wrapText=\"bothSides\">"));
				oWriter.WriteString(strWrapPointsResult);
				oWriter.WriteString(_T("</wp:wrapTight>"));
			}			
		}
		else if (strWType == _T("through"))
		{
			if (_T("") == strWrapPointsResult)
			{
				oWriter.WriteString(_T("<wp:wrapThrough wrapText=\"bothSides\"/>"));
			}
			else
			{
				oWriter.WriteString(_T("<wp:wrapThrough wrapText=\"bothSides\">"));
				oWriter.WriteString(strWrapPointsResult);
				oWriter.WriteString(_T("</wp:wrapThrough>"));
			}						
		}		
		else 
			oWriter.WriteString(_T("<wp:wrapSquare wrapText=\"bothSides\"/>"));
	}	
	else
	{
		oWriter.WriteString(_T("<wp:wrapNone/>"));
	}	

	CString strId = _T("");
	strId.Format(_T("<wp:docPr id=\"%d\" name=\"\"/>"), m_lNextId);
	m_lNextId++;

	oWriter.WriteString(strId);

	oWriter.EndNode(_T("wp:anchor"));
	
	return oWriter.GetXmlString();
}


CString CDrawingConverter::GetVMLShapeXml(PPTX::Logic::SpTreeElem& oElem)
{
	/*
	// TEST Serializer
	CPPTShape* pShape = CPPTShape::CreateByType(PPTShapes::sptCCloudCallout);
	CString sDumpXml = GetVMLShapeXml(pShape);
	RELEASEOBJECT(pShape);
	*/

	CPPTXShape* pShapePPTX = NULL;
	if (oElem.is<PPTX::Logic::Shape>())
	{
		const PPTX::Logic::Shape& lpOriginShape = oElem.as<PPTX::Logic::Shape>();

		if (lpOriginShape.spPr.Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = lpOriginShape.spPr.Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = PPTX2EditorAdvanced::GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt != OOXMLShapes::sptNil) 
			{
				pShapePPTX = new CPPTXShape();
				pShapePPTX->SetType(NSBaseShape::pptx, _lspt);

				CString strAdjustValues = lpGeom.GetODString();
				pShapePPTX->LoadAdjustValuesList(strAdjustValues);
			}
		}
		else if(lpOriginShape.spPr.Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = lpOriginShape.spPr.Geometry.as<PPTX::Logic::CustGeom>();
			CString strShape = lpGeom.GetODString();
			pShapePPTX = new CPPTXShape();
			pShapePPTX->LoadFromXML(strShape);
		}
		else
		{
			pShapePPTX = new CPPTXShape();
			pShapePPTX->SetType(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);
		}								
	}

	if (NULL != pShapePPTX)
	{
		NSGuidesVML::CConverterPPTXPPT oConverterPPTX_2_PPT;
		oConverterPPTX_2_PPT.Convert(pShapePPTX);

		CString sDumpXml = GetVMLShapeXml(oConverterPPTX_2_PPT.GetConvertedShape());

		CFile oFile;
		oFile.CreateFile(_T("C:\\PPTMemory\\vml_output.xml"));
		oFile.WriteStringUTF8(sDumpXml);
		oFile.CloseFile();
		
		return sDumpXml;
	}

	return _T("");
}

CString CDrawingConverter::GetVMLShapeXml(CPPTShape* pPPTShape)
{
	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.StartNode(_T("v:shape"));
	oXmlWriter.StartAttributes();

	CString strCoordSize = _T("");
	LONG lCoordW = 21600;
	LONG lCoordH = 21600;
	if (0 < pPPTShape->m_oPath.m_arParts.size())
	{
		lCoordW = pPPTShape->m_oPath.m_arParts[0].width;
		lCoordH = pPPTShape->m_oPath.m_arParts[0].height;
	}
	strCoordSize.Format(_T("%d,%d"), lCoordW, lCoordH);
	oXmlWriter.WriteAttribute(_T("coordsize"), strCoordSize);

	int nAdjCount = pPPTShape->m_arAdjustments.size();
	if (nAdjCount > 0)
	{
		oXmlWriter.WriteString(_T(" adj=\""));

		for (int i = 0; i < nAdjCount; ++i)
		{
			if (0 != i)
			{
				CString s = _T("");
				s.Format(_T(",%d"), pPPTShape->m_arAdjustments[i]);
				oXmlWriter.WriteString(s);
			}
			else
			{
				CString s = _T("");
				s.Format(_T("%d"), pPPTShape->m_arAdjustments[i]);
				oXmlWriter.WriteString(s);
			}
		}

		oXmlWriter.WriteString(_T("\""));
	}

	oXmlWriter.WriteAttribute(_T("path"), pPPTShape->m_strPath);
	oXmlWriter.EndAttributes();

	std::vector<CFormula>& arGuides = pPPTShape->m_oManager.m_arFormulas;
	int nGuides = arGuides.size();
	if (nGuides != 0)
	{
		oXmlWriter.StartNode(_T("v:formulas"));
		oXmlWriter.StartAttributes();
		oXmlWriter.EndAttributes();

		for (int i = 0; i < nGuides; ++i)
		{
			CFormula& oGuide = arGuides[i];
			if ((int)oGuide.m_eFormulaType >= VML_GUIDE_COUNT)
				break;

			oXmlWriter.WriteString(_T("<v:f eqn=\""));
			
			oXmlWriter.WriteString((CString)VML_GUIDE_TYPE[(int)oGuide.m_eFormulaType]);
			
			BYTE nParams = VML_GUIDE_PARAM_COUNT[(int)oGuide.m_eFormulaType];
			if (nParams > 0)
			{
				CString str = _T("");
				if (oGuide.m_eType1 == ptAdjust)
				{
					str.Format(_T(" #%d"), oGuide.m_lParam1);
				}
				else if (oGuide.m_eType1 == ptFormula)
				{
					str.Format(_T(" @%d"), oGuide.m_lParam1);
				}
				else
				{
					str.Format(_T(" %d"), oGuide.m_lParam1);
				}
				oXmlWriter.WriteString(str);
			}
			if (nParams > 1)
			{
				CString str = _T("");
				if (oGuide.m_eType2 == ptAdjust)
				{
					str.Format(_T(" #%d"), oGuide.m_lParam2);
				}
				else if (oGuide.m_eType2 == ptFormula)
				{
					str.Format(_T(" @%d"), oGuide.m_lParam2);
				}
				else
				{
					str.Format(_T(" %d"), oGuide.m_lParam2);
				}
				oXmlWriter.WriteString(str);
			}
			if (nParams > 2)
			{
				CString str = _T("");
				if (oGuide.m_eType3 == ptAdjust)
				{
					str.Format(_T(" #%d"), oGuide.m_lParam3);
				}
				else if (oGuide.m_eType3 == ptFormula)
				{
					str.Format(_T(" @%d"), oGuide.m_lParam3);
				}
				else
				{
					str.Format(_T(" %d"), oGuide.m_lParam3);
				}
				oXmlWriter.WriteString(str);
			}

			oXmlWriter.WriteString(_T("\"/>"));
		}

		oXmlWriter.EndNode(_T("v:formulas"));

		size_t nTextRectCount = pPPTShape->m_arStringTextRects.size();
		if (0 < nTextRectCount)
		{
			oXmlWriter.WriteString(_T("<v:path textboxrect=\""));

			for (size_t i = 0; i < nTextRectCount; ++i)
			{
				if (0 != i)
					oXmlWriter.WriteString(_T(","));

				oXmlWriter.WriteString(pPPTShape->m_arStringTextRects[i]);
			}

			oXmlWriter.WriteString(_T("\"/>"));
		}

		int nHandles = pPPTShape->m_arHandles.size();
		if (0 < nHandles)
		{
			oXmlWriter.StartNode(_T("v:handles"));
			oXmlWriter.StartAttributes();
			oXmlWriter.EndAttributes();

			for (int i = 0; i < nHandles; ++i)
			{
				oXmlWriter.StartNode(_T("v:h"));
				
				CHandle_& oH = pPPTShape->m_arHandles[i];

				if (oH.position != _T(""))
					oXmlWriter.WriteAttribute(_T("position"), oH.position);

				if (oH.xrange != _T(""))
					oXmlWriter.WriteAttribute(_T("xrange"), oH.xrange);

				if (oH.yrange != _T(""))
					oXmlWriter.WriteAttribute(_T("yrange"), oH.yrange);

				if (oH.polar != _T(""))
					oXmlWriter.WriteAttribute(_T("polar"), oH.polar);

				if (oH.radiusrange != _T(""))
					oXmlWriter.WriteAttribute(_T("radiusrange"), oH.radiusrange);

				if (oH.switchHandle != _T(""))
					oXmlWriter.WriteAttribute(_T("switch"), oH.switchHandle);

				oXmlWriter.WriteString(_T("/>"));
			}

			oXmlWriter.EndNode(_T("v:handles"));
		}
	}

	oXmlWriter.EndNode(_T("v:shape"));
	return oXmlWriter.GetXmlString();
}

void CDrawingConverter::SendMainProps(const CString& strMainProps, CString**& pMainProps)
{
	*pMainProps = new CString();
	**pMainProps = strMainProps;
}

void CDrawingConverter::CheckBrushShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape)
{
	PPTX::Logic::Shape* pShape = (PPTX::Logic::Shape*)dynamic_cast<PPTX::Logic::Shape*>(oElem.GetElem().operator ->());

    int R = 255;
    int G = 255;
    int B = 255;

	nullable_string sFillColor;
	oNode.ReadAttributeBase(L"fillcolor", sFillColor);
	if (sFillColor.is_init() && !pPPTShape->IsWordArt())
	{
		NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sFillColor);

		PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
		pSolid->m_namespace = _T("a");
		pSolid->Color.Color = new PPTX::Logic::SrgbClr();
		pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

        pShape->spPr.Fill.Fill = pSolid;
	}
    else if (!pPPTShape->IsWordArt())
    {
        // default fillcolor in vml = white
        PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
        pSolid->m_namespace = _T("a");
        pSolid->Color.Color = new PPTX::Logic::SrgbClr();
        pSolid->Color.Color->SetRGB(R, G, B);

        pShape->spPr.Fill.Fill = pSolid;
    }

	nullable_string sFilled;
	oNode.ReadAttributeBase(L"filled", sFilled);
	if (sFilled.is_init())
	{
		if (*sFilled == _T("false") || *sFilled == _T("f"))
		{
			PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
			pNoFill->m_namespace = _T("a");
			pShape->spPr.Fill.Fill = pNoFill;
		}
	}
	else if (!pPPTShape->m_bIsFilled)
	{
		PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
		pNoFill->m_namespace = _T("a");
		pShape->spPr.Fill.Fill = pNoFill;
	}

	nullable_string sOpacity;
	oNode.ReadAttributeBase(_T("opacity"), sOpacity);
	if (sOpacity.is_init())
	{
		BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);

		if (pShape->spPr.Fill.is<PPTX::Logic::SolidFill>())
		{
			PPTX::Logic::ColorModifier oMod;
			oMod.name = _T("alpha");
			int nA = (int)(lAlpha * 100000.0 / 255.0);
			oMod.val = nA;
			pShape->spPr.Fill.as<PPTX::Logic::SolidFill>().Color.Color->Modifiers.push_back(oMod);
		}
	}

	XmlUtils::CXmlNode oNodeFill = oNode.ReadNode(_T("v:fill"));
	if (oNodeFill.IsValid() && !pPPTShape->IsWordArt())
	{
		nullable_string sType;
		oNodeFill.ReadAttributeBase(L"type", sType);

		sOpacity.reset();
		oNodeFill.ReadAttributeBase(_T("opacity"), sOpacity);

		nullable_string sColor;
		oNodeFill.ReadAttributeBase(L"color", sColor);
		if (sColor.is_init())
		{
			NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sColor);

			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = _T("a");
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

			pShape->spPr.Fill.Fill = pSolid;

			if (!sFillColor.is_init()) 
				sFillColor = sColor;
		}
		if (!sColor.is_init()) sColor = sFillColor;

		nullable_string sRid;
		oNodeFill.ReadAttributeBase(L"r:id", sRid);
		if (sRid.is_init())
		{			
			PPTX::Logic::BlipFill* pBlipFill = new PPTX::Logic::BlipFill();
			pBlipFill->m_namespace = _T("a");
			pBlipFill->blip = new PPTX::Logic::Blip();
			pBlipFill->blip->embed = new PPTX::RId(*sRid);

			if (sType.is_init() && ((*sType == _T("tile")) || (*sType == _T("pattern"))))
			{
				pBlipFill->tile = new PPTX::Logic::Tile();				
			}

			pShape->spPr.Fill.Fill = pBlipFill;
		}		
		nullable_string sRotate;
		oNodeFill.ReadAttributeBase(L"rotate", sRotate);

		nullable_string sMethod;
		oNodeFill.ReadAttributeBase(L"method", sMethod);
		
		nullable_string sColor2;
		oNodeFill.ReadAttributeBase(L"color2", sColor2);
		
		nullable_string sFocus;
		oNodeFill.ReadAttributeBase(L"focus", sFocus);
		//
        if (sType.is_init() && (*sType == _T("gradient") || *sType == _T("gradientradial") || *sType == _T("gradientRadial")))
		{
			PPTX::Logic::GradFill* pGradFill = new PPTX::Logic::GradFill();
			pGradFill->m_namespace = _T("a");
		
			if (sColor.is_init())
			{
				NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sColor);

				PPTX::Logic::Gs Gs_;
				Gs_.color.Color = new PPTX::Logic::SrgbClr();
				Gs_.color.Color->SetRGB(color.R, color.G, color.B);

				Gs_.pos = 0;
				pGradFill->GsLst.push_back( Gs_ );

                R = color.R;
                G = color.G;
                B = color.B;
			}
			if (sColor2.is_init())
			{
				PPTX::Logic::Gs Gs_;
				Gs_.color.Color = new PPTX::Logic::SrgbClr();
                if (sColor2->Find(L"fill") != -1)
				{
                    std::wstring sColorEffect = string2std_string(*sColor2);
                    if (sColorEffect.length() > 5)
                        sColorEffect = sColorEffect.substr(5);

                    int resR, resG, resB;
                    GetColorWithEffect(sColorEffect, R, G, B, resR, resG, resB);

                    Gs_.color.Color->SetRGB(resR, resG, resB);
				}
				else
				{
					NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sColor2);
					Gs_.color.Color->SetRGB(color.R, color.G, color.B);
				}

				Gs_.pos = 100 * 1000;
				pGradFill->GsLst.push_back( Gs_ );
			}
			if (pGradFill->GsLst.size() < 2)	//Sindicatum.docx
			{
				PPTX::Logic::Gs Gs_; 
				Gs_.pos = 0;
				Gs_.color.Color = new PPTX::Logic::SrgbClr(); Gs_.color.Color->SetRGB(0xff, 0xff, 0xff);
				
				if (pGradFill->GsLst[0].pos == 0)
					Gs_.pos = 100 * 1000;
				
				pGradFill->GsLst.push_back( Gs_ );
			}
			//todooo method
			if (sRotate.is_init())
			{
				pGradFill->lin = new PPTX::Logic::Lin();
				pGradFill->lin->scaled = 1;

				if (*sRotate == L"l") pGradFill->lin->ang = 0   * 60000;
				if (*sRotate == L"t") pGradFill->lin->ang = 90  * 60000;
				if (*sRotate == L"b") pGradFill->lin->ang = 270 * 60000;
				if (*sRotate == L"r") pGradFill->lin->ang = 180 * 60000;
			}

			pShape->spPr.Fill.Fill = pGradFill;
		}	

		if (sOpacity.is_init())
		{
			BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);

			if (pShape->spPr.Fill.is<PPTX::Logic::SolidFill>())
			{
				PPTX::Logic::ColorModifier oMod;
				oMod.name = _T("alpha");
				int nA = (int)(lAlpha * 100000.0 / 255.0);
				oMod.val = nA;
				pShape->spPr.Fill.as<PPTX::Logic::SolidFill>().Color.Color->Modifiers.push_back(oMod);
			}
			else if (pShape->spPr.Fill.is<PPTX::Logic::BlipFill>())
			{
				PPTX::Logic::AlphaModFix* pAlphaMod = new PPTX::Logic::AlphaModFix();
				int nA = (int)(lAlpha * 100000.0 / 255.0);
				pAlphaMod->amt = nA;

				PPTX::Logic::UniEffect oEff;
				oEff.InitPointer(pAlphaMod);

				pShape->spPr.Fill.as<PPTX::Logic::BlipFill>().blip->Effects.push_back(oEff);
			}
		}
	}
	if (true)
	{
		XmlUtils::CXmlNode oNodeFillID = oNode.ReadNode(_T("v:imagedata"));

		if (oNodeFillID.IsValid())
		{
			nullable_string sRid;
			oNodeFillID.ReadAttributeBase(L"r:id", sRid);
			
			nullable_string sRelid;
			oNodeFillID.ReadAttributeBase(L"o:relid", sRelid);

			if (sRid.is_init() || sRelid.is_init())
			{			
				nullable_string sType;
				oNodeFillID.ReadAttributeBase(L"type", sType);

				PPTX::Logic::BlipFill* pBlipFill = new PPTX::Logic::BlipFill();
				pBlipFill->m_namespace = _T("a");
				pBlipFill->blip = new PPTX::Logic::Blip();

				CString sId = sRid.IsInit() ? *sRid : (sRelid.IsInit() ? *sRelid : _T(""));
				pBlipFill->blip->embed = new PPTX::RId(sId);

				if (sType.is_init() && *sType == _T("tile"))
				{
					pBlipFill->tile = new PPTX::Logic::Tile();				
				}else
				{
					//stretch ??? bug 28238
					pBlipFill->stretch = new PPTX::Logic::Stretch();

				}

				pShape->spPr.Fill.Fill = pBlipFill;

				CString strCropT = oNodeFillID.GetAttribute(_T("croptop"));
				CString strCropL = oNodeFillID.GetAttribute(_T("cropleft"));
				CString strCropR = oNodeFillID.GetAttribute(_T("cropright"));
				CString strCropB = oNodeFillID.GetAttribute(_T("cropbottom"));

				NS_DWC_Common::CorrentCropString(strCropL);
				NS_DWC_Common::CorrentCropString(strCropT);
				NS_DWC_Common::CorrentCropString(strCropR);
				NS_DWC_Common::CorrentCropString(strCropB);

				if (_T("") != strCropL || _T("") != strCropT || _T("") != strCropR || _T("") != strCropB)
				{
					pBlipFill->srcRect = new PPTX::Logic::Rect();

					CString str0 = _T("0");
					if (_T("") != strCropL)
						pBlipFill->srcRect->l = strCropL;
					else
						pBlipFill->srcRect->l = str0;

					if (_T("") != strCropT)
						pBlipFill->srcRect->t = strCropT;
					else
						pBlipFill->srcRect->t = str0;

					if (_T("") != strCropR)
						pBlipFill->srcRect->r = strCropR;
					else
						pBlipFill->srcRect->r = str0;

					if (_T("") != strCropB)
						pBlipFill->srcRect->b = strCropB;
					else
						pBlipFill->srcRect->b = str0;
				}
			}
		}
	}

	// default params
	if (!pShape->spPr.Fill.Fill.is_init())
	{
		if (pPPTShape->IsWordArt())
		{
			PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
			pNoFill->m_namespace = _T("a");
			pShape->spPr.Fill.Fill = pNoFill;
		}
		else
		{
			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = _T("a");
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(0xFF, 0xFF, 0xFF);
			pShape->spPr.Fill.Fill = pSolid;

			if (sOpacity.is_init())
			{
				BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);
				PPTX::Logic::ColorModifier oMod;
				oMod.name = _T("alpha");
				int nA = (int)(lAlpha * 100000.0 / 255.0);
				oMod.val = nA;
				pSolid->Color.Color->Modifiers.push_back(oMod);
			}
		}
	}
}
void CDrawingConverter::CheckPenShape(PPTX::Logic::SpTreeElem& oElem, XmlUtils::CXmlNode& oNode, PPTShapes::ShapeType eType, CPPTShape* pPPTShape)
{
	PPTX::Logic::Shape* pShape = (PPTX::Logic::Shape*)dynamic_cast<PPTX::Logic::Shape*>(oElem.GetElem().operator ->());

	// attributes
	nullable_string sStrokeColor;
	oNode.ReadAttributeBase(L"strokecolor", sStrokeColor);
	if (sStrokeColor.is_init())
	{
		NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sStrokeColor);

		if (!pShape->spPr.ln.is_init())
			pShape->spPr.ln = new PPTX::Logic::Ln();

		PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
		pSolid->m_namespace = _T("a");
		pSolid->Color.Color = new PPTX::Logic::SrgbClr();
		pSolid->Color.Color->SetRGB(color.R, color.G, color.B);
		pShape->spPr.ln->Fill.Fill = pSolid;
	}

	nullable_string sStrokeWeight;
	oNode.ReadAttributeBase(L"strokeweight", sStrokeWeight);
	if (sStrokeWeight.is_init())
	{
		if (!pShape->spPr.ln.is_init())
			pShape->spPr.ln = new PPTX::Logic::Ln();

		if (sStrokeWeight->GetLength() > 0 && sStrokeWeight->GetAt(0) == TCHAR('.'))
		{
			sStrokeWeight = (_T("0") + *sStrokeWeight);
		}

		SimpleTypes::CPoint oPoint;
		int size = (int)(g_emu_koef * oPoint.FromString(*sStrokeWeight));		

		pShape->spPr.ln->w = size;
		pPPTShape->m_bIsStroked = true;
	}

	nullable_string sStroked;
	oNode.ReadAttributeBase(L"stroked", sStroked);
	if (sStroked.is_init())
	{
		if (*sStroked == _T("false") || *sStroked == _T("f"))
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			pShape->spPr.ln->Fill.Fill = new PPTX::Logic::NoFill();
		}
	}
	else if (!pPPTShape->m_bIsStroked)
	{
		if (!pShape->spPr.ln.is_init())
			pShape->spPr.ln = new PPTX::Logic::Ln();

		pShape->spPr.ln->Fill.Fill = new PPTX::Logic::NoFill();
	}

	XmlUtils::CXmlNode oNodeStroke = oNode.ReadNode(_T("v:stroke"));
	if (oNodeStroke.IsValid())
	{
		sStrokeColor.reset();
		oNodeStroke.ReadAttributeBase(L"strokecolor", sStrokeColor);
		if (sStrokeColor.is_init())
		{
			NSPresentationEditor::CColor color = NS_DWC_Common::getColorFromString(*sStrokeColor);

			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = _T("a");
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

			pShape->spPr.ln->Fill.Fill = pSolid;
		}

		nullable_string sStrokeDashStyle;
		oNodeStroke.ReadAttributeBase(L"dashstyle", sStrokeDashStyle);
		if (sStrokeDashStyle.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			pShape->spPr.ln->prstDash = new PPTX::Logic::PrstDash();

			if (*sStrokeDashStyle == _T("solid"))
				pShape->spPr.ln->prstDash->val = _T("solid");
			else if (*sStrokeDashStyle == _T("shortdash"))
				pShape->spPr.ln->prstDash->val = _T("sysDash");
			else if (*sStrokeDashStyle == _T("shortdot"))
				pShape->spPr.ln->prstDash->val = _T("sysDot");
			else if (*sStrokeDashStyle == _T("shortdashdot"))
				pShape->spPr.ln->prstDash->val = _T("sysDashDot");
			else if (*sStrokeDashStyle == _T("shortdashdotdot"))
				pShape->spPr.ln->prstDash->val = _T("sysDashDotDot");
			else if (*sStrokeDashStyle == _T("dot"))
				pShape->spPr.ln->prstDash->val = _T("dot");
			else if (*sStrokeDashStyle == _T("dash"))
				pShape->spPr.ln->prstDash->val = _T("dash");
			else if (*sStrokeDashStyle == _T("longdash"))
				pShape->spPr.ln->prstDash->val = _T("lgDash");
			else if (*sStrokeDashStyle == _T("dashdot"))
				pShape->spPr.ln->prstDash->val = _T("dashDot");
			else if (*sStrokeDashStyle == _T("longdashdot"))
				pShape->spPr.ln->prstDash->val = _T("lgDashDot");
			else if (*sStrokeDashStyle == _T("longdashdotdot"))
				pShape->spPr.ln->prstDash->val = _T("lgDashDotDot");
			else
				pShape->spPr.ln->prstDash->val = _T("solid");
		}

		nullable_string sEndArraw;
		oNodeStroke.ReadAttributeBase(_T("endarrow"), sEndArraw);
		if (sEndArraw.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			pShape->spPr.ln->tailEnd = new PPTX::Logic::LineEnd();

			if (*sEndArraw == _T("none"))
				pShape->spPr.ln->tailEnd->type = _T("none");
			else if (*sEndArraw == _T("block"))
				pShape->spPr.ln->tailEnd->type = _T("triangle");
			else if (*sEndArraw == _T("classic"))
				pShape->spPr.ln->tailEnd->type = _T("stealth");
			else if (*sEndArraw == _T("diamond"))
				pShape->spPr.ln->tailEnd->type = _T("diamond");
			else if (*sEndArraw == _T("oval"))
				pShape->spPr.ln->tailEnd->type = _T("oval");
			else if (*sEndArraw == _T("open"))
				pShape->spPr.ln->tailEnd->type = _T("arrow");
			else
				pShape->spPr.ln->tailEnd->type = _T("none");
		}

		nullable_string sEndArrawLen;
		oNodeStroke.ReadAttributeBase(_T("endarrowlength"), sEndArrawLen);
		if (sEndArrawLen.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (!pShape->spPr.ln->tailEnd.is_init())
				pShape->spPr.ln->tailEnd = new PPTX::Logic::LineEnd();

			if (*sEndArrawLen == _T("short"))
				pShape->spPr.ln->tailEnd->len = _T("sm");
			else if (*sEndArrawLen == _T("medium"))
				pShape->spPr.ln->tailEnd->len = _T("med");
			else if (*sEndArrawLen == _T("long"))
				pShape->spPr.ln->tailEnd->len = _T("lg");
			else
				pShape->spPr.ln->tailEnd->len = _T("med");
		}

		nullable_string sEndArrawWidth;
		oNodeStroke.ReadAttributeBase(_T("endarrowwidth"), sEndArrawWidth);
		if (sEndArrawWidth.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (!pShape->spPr.ln->tailEnd.is_init())
				pShape->spPr.ln->tailEnd = new PPTX::Logic::LineEnd();

			if (*sEndArrawWidth == _T("narrow"))
				pShape->spPr.ln->tailEnd->w = _T("sm");
			else if (*sEndArrawWidth == _T("medium"))
				pShape->spPr.ln->tailEnd->w = _T("med");
			else if (*sEndArrawWidth == _T("wide"))
				pShape->spPr.ln->tailEnd->w = _T("lg");
			else
				pShape->spPr.ln->tailEnd->w = _T("med");
		}

		nullable_string sStartArraw;
		oNodeStroke.ReadAttributeBase(_T("startarrow"), sStartArraw);
		if (sStartArraw.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			pShape->spPr.ln->headEnd = new PPTX::Logic::LineEnd();

			if (*sStartArraw == _T("none"))
				pShape->spPr.ln->headEnd->type = _T("none");
			else if (*sStartArraw == _T("block"))
				pShape->spPr.ln->headEnd->type = _T("triangle");
			else if (*sStartArraw == _T("classic"))
				pShape->spPr.ln->headEnd->type = _T("stealth");
			else if (*sStartArraw == _T("diamond"))
				pShape->spPr.ln->headEnd->type = _T("diamond");
			else if (*sStartArraw == _T("oval"))
				pShape->spPr.ln->headEnd->type = _T("oval");
			else if (*sStartArraw == _T("open"))
				pShape->spPr.ln->headEnd->type = _T("arrow");
			else
				pShape->spPr.ln->headEnd->type = _T("none");
		}

		nullable_string sStartArrawLen;
		oNodeStroke.ReadAttributeBase(_T("startarrowlength"), sStartArrawLen);
		if (sStartArrawLen.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (!pShape->spPr.ln->headEnd.is_init())
				pShape->spPr.ln->headEnd = new PPTX::Logic::LineEnd();

			if (*sStartArrawLen == _T("short"))
				pShape->spPr.ln->headEnd->len = _T("sm");
			else if (*sStartArrawLen == _T("medium"))
				pShape->spPr.ln->headEnd->len = _T("med");
			else if (*sStartArrawLen == _T("long"))
				pShape->spPr.ln->headEnd->len = _T("lg");
			else
				pShape->spPr.ln->headEnd->len = _T("med");
		}

		nullable_string sStartArrawWidth;
		oNodeStroke.ReadAttributeBase(_T("startarrowwidth"), sStartArrawWidth);
		if (sStartArrawWidth.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (!pShape->spPr.ln->headEnd.is_init())
				pShape->spPr.ln->headEnd = new PPTX::Logic::LineEnd();

			if (*sStartArrawWidth == _T("narrow"))
				pShape->spPr.ln->headEnd->w = _T("sm");
			else if (*sStartArrawWidth == _T("medium"))
				pShape->spPr.ln->headEnd->w = _T("med");
			else if (*sStartArrawWidth == _T("wide"))
				pShape->spPr.ln->headEnd->w = _T("lg");
			else
				pShape->spPr.ln->headEnd->w = _T("med");
		}

		nullable_string sEndCap;
		oNodeStroke.ReadAttributeBase(_T("endcap"), sEndCap);
		if (sEndCap.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (*sEndCap == _T("flat"))
				pShape->spPr.ln->cap = _T("flat");
			else if (*sEndCap == _T("round"))
				pShape->spPr.ln->cap = _T("rnd");
			else if (*sEndCap == _T("square"))
				pShape->spPr.ln->cap = _T("sq");
		}

		nullable_string sLineJoin;
		oNodeStroke.ReadAttributeBase(_T("joinstyle"), sLineJoin);
		if (sLineJoin.is_init())
		{
			if (!pShape->spPr.ln.is_init())
				pShape->spPr.ln = new PPTX::Logic::Ln();

			if (*sLineJoin == _T("bevel"))
				pShape->spPr.ln->Join.type = PPTX::Logic::JoinBevel;
			else if (*sLineJoin == _T("miter"))
				pShape->spPr.ln->Join.type = PPTX::Logic::JoinMiter;
			else if (*sLineJoin == _T("round"))
				pShape->spPr.ln->Join.type = PPTX::Logic::JoinRound;
		}		
	}

	// default params
	if (eType != PPTShapes::sptCFrame)
	{
		if (!pShape->spPr.ln.is_init())
			pShape->spPr.ln = new PPTX::Logic::Ln();

		if (!pShape->spPr.ln->Fill.Fill.is_init())
		{
			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = _T("a");
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(0, 0, 0);
			pShape->spPr.ln->Fill.Fill = pSolid;
		}
	}
}

HRESULT CDrawingConverter::GetThemeBinary(BYTE** ppBinary, long& lBinarySize, const CString& bsThemeFilePath)
{
	if(NULL == ppBinary)
		return S_FALSE;
	CString strOldRels = m_strCurrentRelsPath;

	m_strCurrentRelsPath = bsThemeFilePath;
	SetCurrentRelsPath();

	PPTX::FileMap oFileMap;
	OOX::CPath oPath = m_strCurrentRelsPath;
	smart_ptr<PPTX::Theme> pTheme = new PPTX::Theme(oPath, oFileMap);

	//m_pBinaryWriter->ClearNoAttack();
	ULONG lOldPos = m_pBinaryWriter->GetPosition();
	m_pBinaryWriter->m_pCommon->CheckFontPicker();
	pTheme->toPPTY(m_pBinaryWriter);

	lBinarySize = m_pBinaryWriter->GetPosition() - lOldPos;
	*ppBinary = new BYTE[lBinarySize];
	
	BYTE* pDataD = *ppBinary;
	BYTE* pDataS = m_pBinaryWriter->GetBuffer() + lOldPos;
	memcpy(pDataD, pDataS, lBinarySize);

	m_pBinaryWriter->SetPosition(lOldPos);

	*m_pBinaryWriter->ThemeDoc = pTheme.smart_dynamic_cast<PPTX::FileContainer>();
	//m_pBinaryWriter->ThemeDoc.reset();

	m_strCurrentRelsPath = strOldRels;
	SetCurrentRelsPath();

	return S_OK;
}

HRESULT CDrawingConverter::LoadClrMap(const CString& bsXml)
{
	smart_ptr<PPTX::Logic::ClrMap> pClrMap = new PPTX::Logic::ClrMap();
	
	CString strXml = _T("<main xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">") + (CString)bsXml + _T("</main>");
	XmlUtils::CXmlNode oNode;
	oNode.FromXmlString(strXml);

	if (oNode.IsValid())
	{
		pClrMap->fromXMLW(oNode.ReadNode(_T("w:clrSchemeMapping")));
	}
	
	*m_pClrMap = pClrMap.smart_dynamic_cast<PPTX::WrapperWritingElement>();
	return S_OK;
}

HRESULT CDrawingConverter::SaveThemeXml(LONG lStart, LONG lLength, const CString& bsThemePath)
{
	if (bsThemePath.IsEmpty())
		return S_FALSE;

	m_pReader->Seek(lStart);
	// reset rels
	SetDstContentRels();

	//m_pReader->m_pRels->Clear();
	//m_pReader->m_pRels->StartTheme();

	smart_ptr<PPTX::Theme> pTheme = new PPTX::Theme();
	pTheme->fromPPTY(m_pReader);
	m_pXmlWriter->ClearNoAttack();
	
	CString strThemePath = (CString)bsThemePath;

	pTheme->toXmlWriter(m_pXmlWriter);
	m_pXmlWriter->SaveToFile(strThemePath);

	OOX::CPath filename = strThemePath;
    CString strTemp = filename.GetDirectory()  + FILE_SEPARATOR_STR + _T("_rels") + FILE_SEPARATOR_STR;

	CString strFileName = filename.GetFilename();
	
	if (strFileName == _T(""))	strTemp += _T(".rels");
	else						strTemp += (strFileName + _T(".rels"));
	
    filename = strTemp;

    SaveDstContentRels(filename.GetPath());

	m_pXmlWriter->ClearNoAttack();

	*m_pTheme = pTheme.smart_dynamic_cast<PPTX::WrapperFile>();
	return S_OK;
}

HRESULT CDrawingConverter::SaveObject(LONG lStart, LONG lLength, const CString& bsMainProps, CString** bsXml)
{
	if (bsXml == NULL)
		return S_OK;

	bool bIsInline = false;
	CString strMainProps = (CString)bsMainProps;
	CString strMainPropsTail;
	int nIndexF = strMainProps.Find(_T("</wp:inline>"));
	if (-1 != nIndexF)
	{
		bIsInline = true;
		strMainProps = strMainProps.Mid(0, nIndexF);
	}
	else
	{
		nIndexF = strMainProps.Find(_T("</wp:anchor>"));
		strMainProps = strMainProps.Mid(0, nIndexF);
	}

	if (-1 == nIndexF)
		return S_FALSE;

	int nIndexTail = (std::min)(strMainProps.Find(_T("<wp14:sizeRelH")), strMainProps.Find(_T("<wp14:sizeRelV")));
	if(-1 != nIndexTail)
	{
		strMainPropsTail = strMainProps.Right(strMainProps.GetLength() - nIndexTail);
		strMainProps = strMainProps.Mid(0, nIndexTail);
	}

	/*
	CString strId = _T("");
	strId.Format(_T("<wp:docPr id=\"%d\" name=\"\"/>"), m_pReader->GenerateNextId());
	strMainProps += strId;
	*/

	//strMainProps += _T("<wp:cNvGraphicFramePr/>");

	m_pReader->Seek(lStart);
	
	++m_nCurrentIndexObject;

	BYTE typeRec1 = m_pReader->GetUChar(); // must be 0;
	LONG _e = m_pReader->GetPos() + m_pReader->GetLong() + 4;

	m_pReader->Skip(5); // type record (must be 1) + 4 byte - len record
	PPTX::Logic::SpTreeElem oElem;

	//oElem.fromPPTY(&m_oReader);
	m_pReader->m_lDocumentType = XMLWRITER_DOC_TYPE_DOCX;

	oElem.fromPPTY(m_pReader);
	bool bOle = false;
	if (oElem.is<PPTX::Logic::Pic>())
	{
		PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
		if(oPic.oleObject.IsInit())
		{
			bOle = oPic.oleObject->isValid();
		}
	}
	
	m_pReader->m_lDocumentType = XMLWRITER_DOC_TYPE_PPTX;


	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.m_lObjectIdVML = m_pXmlWriter->m_lObjectIdVML;
	oXmlWriter.m_lObjectIdOle = m_pXmlWriter->m_lObjectIdOle;
	oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_DOCX;
	oXmlWriter.m_bIsUseOffice2007 = m_bIsUseConvertion2007;

	oXmlWriter.m_bIsTop = (1 == m_nCurrentIndexObject) ? true : false;

#if defined(BUILD_CONFIG_FULL_VERSION) && defined(AVS_USE_CONVERT_PPTX_TOCUSTOM_VML)
	if (NULL == m_pOOXToVMLRenderer)
		m_pOOXToVMLRenderer = new COOXToVMLGeometry();
	oXmlWriter.m_pOOXToVMLRenderer = m_pOOXToVMLRenderer;
#endif
	if(bOle)
	{
		ConvertPicVML(oElem, bsMainProps, oXmlWriter);
	}
	else
	{
	
		bool bIsNeedConvert2007 = false;
	#ifdef BUILD_CONFIG_FULL_VERSION
		if (m_bIsUseConvertion2007)
		{
			if (oElem.is<PPTX::Logic::SpTree>())
			{
				oXmlWriter.WriteString(_T("<mc:AlternateContent><mc:Choice Requires=\"wpg\">"));
				bIsNeedConvert2007 = true;
			}
			else if (oElem.is<PPTX::Logic::Shape>())
			{
				oXmlWriter.WriteString(_T("<mc:AlternateContent><mc:Choice Requires=\"wps\">"));
				bIsNeedConvert2007 = true;
			}
		}
	#endif

		oXmlWriter.WriteString(_T("<w:drawing>"));
		oXmlWriter.WriteString(strMainProps);

		if (oElem.is<PPTX::Logic::SpTree>())
		{
			oXmlWriter.WriteString(_T("<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
	<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\">"));
		}
		else if (oElem.is<PPTX::Logic::Pic>())
		{
			oXmlWriter.WriteString(_T("<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
	<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"));
		}
		else
		{
			oXmlWriter.WriteString(_T("<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
	<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
		}
		oElem.toXmlWriter(&oXmlWriter);
		oXmlWriter.WriteString(_T("</a:graphicData>\
	</a:graphic>"));	

		oXmlWriter.WriteString(strMainPropsTail);
		oXmlWriter.WriteString(bIsInline ? _T("</wp:inline>") : _T("</wp:anchor>"));
		oXmlWriter.WriteString(_T("</w:drawing>"));

		if (bIsNeedConvert2007)
		{
			oXmlWriter.WriteString(_T("</mc:Choice><mc:Fallback><w:pict>"));

			if (oElem.is<PPTX::Logic::SpTree>())
			{
				ConvertGroupVML(oElem, bsMainProps, oXmlWriter);
			}
			else if (oElem.is<PPTX::Logic::Shape>())
			{
				ConvertShapeVML(oElem, bsMainProps, oXmlWriter);
			}

			oXmlWriter.WriteString(_T("</w:pict></mc:Fallback></mc:AlternateContent>"));
		}
	}
	--m_nCurrentIndexObject;

	m_pXmlWriter->m_lObjectIdVML = oXmlWriter.m_lObjectIdVML;
	m_pXmlWriter->m_lObjectIdOle = oXmlWriter.m_lObjectIdOle;

	CString ret = oXmlWriter.GetXmlString();
	*bsXml = new CString();
	**bsXml = ret;

	m_pReader->Seek(_e);
	return S_OK;
}

HRESULT CDrawingConverter::SaveObjectEx(LONG lStart, LONG lLength, const CString& bsMainProps, LONG lDocType, CString** bsXml)
{
	if (XMLWRITER_DOC_TYPE_DOCX == lDocType)
	{
        m_pImageManager->m_bIsWord = true;
		// нужно писать всякие inline/anchor + word2007 format
		return SaveObject(lStart, lLength, bsMainProps, bsXml);
	}

	if (bsXml == NULL)
		return S_OK;

	m_pReader->Seek(lStart);

	if (lDocType != XMLWRITER_DOC_TYPE_DOCX)
        m_pImageManager->m_bIsWord = false;
	else
        m_pImageManager->m_bIsWord = true;
	
	++m_nCurrentIndexObject;

	BYTE typeRec1 = m_pReader->GetUChar(); // must be 0;
	LONG _e = m_pReader->GetPos() + m_pReader->GetLong() + 4;

	m_pReader->Skip(5); // type record (must be 1) + 4 byte - len record
	PPTX::Logic::SpTreeElem oElem;

	m_pReader->m_lDocumentType = lDocType;

	oElem.fromPPTY(m_pReader);
	bool bOle = false;
	if (oElem.is<PPTX::Logic::Pic>())
	{
		PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
		if(oPic.oleObject.IsInit())
		{
			bOle = oPic.oleObject->isValid();
		}
	}
	
	m_pReader->m_lDocumentType = XMLWRITER_DOC_TYPE_PPTX;

	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.m_lObjectIdVML = m_pXmlWriter->m_lObjectIdVML;
	oXmlWriter.m_lObjectIdOle = m_pXmlWriter->m_lObjectIdOle;
	oXmlWriter.m_lDocType = (BYTE)lDocType;
    oXmlWriter.m_bIsUseOffice2007 = false;

	oXmlWriter.m_bIsTop = (1 == m_nCurrentIndexObject) ? true : false;

#if defined(BUILD_CONFIG_FULL_VERSION) && defined(AVS_USE_CONVERT_PPTX_TOCUSTOM_VML)
	if (NULL == m_pOOXToVMLRenderer)
		m_pOOXToVMLRenderer = new COOXToVMLGeometry();
	oXmlWriter.m_pOOXToVMLRenderer = m_pOOXToVMLRenderer;
#endif

	if(bOle)
	{
		ConvertPicVML(oElem, bsMainProps, oXmlWriter);
	}
	else
	{
		oElem.toXmlWriter(&oXmlWriter);
	}

	--m_nCurrentIndexObject;

	m_pXmlWriter->m_lObjectIdVML = oXmlWriter.m_lObjectIdVML;
	m_pXmlWriter->m_lObjectIdOle = oXmlWriter.m_lObjectIdOle;
	if (XMLWRITER_DOC_TYPE_XLSX == lDocType)
	{
		m_pXmlWriter->m_strOleXlsx = oXmlWriter.m_strOleXlsx;
	}

	CString ret = oXmlWriter.GetXmlString();
	*bsXml = new CString();
	**bsXml = ret;

	m_pReader->Seek(_e);
	return S_OK;
}

void CDrawingConverter::ConvertPicVML(PPTX::Logic::SpTreeElem& oElem, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter)
{
	if (bsMainProps)
	{
		ConvertMainPropsToVML(bsMainProps, oWriter, oElem);
	}

	oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе
	PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
	oPic.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap);
}

void CDrawingConverter::ConvertShapeVML(PPTX::Logic::SpTreeElem& oElem, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter)
{
	if (bsMainProps)
	{
		ConvertMainPropsToVML(bsMainProps, oWriter, oElem);
	}

	oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе
	PPTX::Logic::Shape& oShape = oElem.as<PPTX::Logic::Shape>();
	oShape.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap);
}

void CDrawingConverter::ConvertGroupVML(PPTX::Logic::SpTreeElem& oElem, const CString& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter)
{
	if (bsMainProps)
	{
		// TODO: здесь нужно заполнить поля в oXmlWriter'e
		ConvertMainPropsToVML(bsMainProps, oWriter, oElem);
	}

	oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе (вместе с остальными параметрами)
	PPTX::Logic::SpTree& oGroup = oElem.as<PPTX::Logic::SpTree>();
	oGroup.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap);
}
void CDrawingConverter::ConvertTextVML(XmlUtils::CXmlNode &nodeTextBox, PPTX::Logic::Shape* pShape)
{
	if (pShape->txBody.IsInit() == false)
		pShape->txBody = new PPTX::Logic::TxBody();
	
	if (pShape->txBody.IsInit() == false) return;

	XmlUtils::CXmlNodes nodes = nodeTextBox.GetNodes(_T("*"));

	for (int i = 0 ; i < nodes.GetCount(); i++)
	{
		XmlUtils::CXmlNode node;
		if (nodes.GetAt(i, node))
		{
			CString name = node.GetName();
			if (name == _T("div"))
			{
				PPTX::Logic::Paragraph p;

				CString strStyle = node.GetAttribute(_T("style"));
				PPTX::CCSS oCSSParser;
				oCSSParser.LoadFromString2(strStyle);

				//todooo oCSSParser->pPr

				XmlUtils::CXmlNodes nodesDiv = node.GetNodes(_T("*"));
				for (int j = 0 ; j < nodesDiv.GetCount(); j++)
				{
					XmlUtils::CXmlNode node1;
					if (nodesDiv.GetAt(j, node1))
					{
						name = node1.GetName();
						if (name == _T("font"))
						{
							PPTX::Logic::Run  *run = new PPTX::Logic::Run();
							
							run->SetText(node1.GetText());
							
							std::vector<CString > attNames, attValues;
							node1.ReadAllAttributes(attNames,attValues);

							if (attNames.size() > 0)
							{
								run->rPr = new PPTX::Logic::RunProperties();							
									
								for (long r = 0; r < attNames.size(); r++)
								{
									if (attNames[r] == _T("color") && attValues[r].GetLength() == 7) 
									{
										attValues[r].Replace(_T("#"),_T(""));
										PPTX::Logic::SolidFill	*fill	= new PPTX::Logic::SolidFill();
										PPTX::Logic::SrgbClr	*color	= new PPTX::Logic::SrgbClr();
										
										color->SetHexString(attValues[r]);
										fill->Color.Color = color;
										
										run->rPr->Fill.Fill = fill;
										run->rPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
									}
									else if (attNames[r] == _T("size"))
									{
										run->rPr->sz = _wtoi(attValues[r].GetBuffer()) * 5;
									}
									else if (attNames[r] == _T("face"))
									{	
										run->rPr->latin = new PPTX::Logic::TextFont();
										run->rPr->latin->typeface = attValues[r];
									}
								}
							}
							PPTX::Logic::RunElem elm;
							p.RunElems.push_back(elm);
							
							p.RunElems.back().InitRun(run);
						}
					}
				}
				pShape->txBody->Paragrs.push_back(p);
			}
		}
	}
}
void CDrawingConverter::ConvertMainPropsToVML(const CString& bsMainProps, NSBinPptxRW::CXmlWriter& pWriter, PPTX::Logic::SpTreeElem& oElem)
{
	XmlUtils::CXmlNode oNode;
	if (!oNode.FromXmlString((CString)bsMainProps))
		return;

	NSBinPptxRW::CXmlWriter oWriter;

	double dKoef = 72.0 / (36000 * 25.4);
	if (_T("wp:inline") == oNode.GetName())
	{
		nullable_int margT; oNode.ReadAttributeBase(L"distT", margT);
		nullable_int margB; oNode.ReadAttributeBase(L"distB", margB);
		nullable_int margL; oNode.ReadAttributeBase(L"distL", margL);
		nullable_int margR; oNode.ReadAttributeBase(L"distR", margR);

		if (margL.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-left"), dKoef * (*margL));
		if (margT.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-top"), dKoef * (*margT));
		if (margR.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-right"), dKoef * (*margR));
		if (margB.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-bottom"), dKoef * (*margB));

		XmlUtils::CXmlNode oNodeS;
		if (oNode.GetNode(_T("wp:extent"), oNodeS))
		{
			int _width = oNodeS.ReadAttributeInt(_T("cx"));
			int _height = oNodeS.ReadAttributeInt(_T("cy"));

			oWriter.WriteAttributeCSS_double1_pt(_T("width"), dKoef * _width);
			oWriter.WriteAttributeCSS_double1_pt(_T("height"), dKoef * _height);
		}
	}
	else
	{
		oWriter.WriteAttributeCSS(_T("position"), _T("absolute"));
		nullable_int margT; oNode.ReadAttributeBase(L"distT", margT);
		nullable_int margB; oNode.ReadAttributeBase(L"distB", margB);
		nullable_int margL; oNode.ReadAttributeBase(L"distL", margL);
		nullable_int margR; oNode.ReadAttributeBase(L"distR", margR);

		if (margL.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-left"), dKoef * (*margL));
		if (margT.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-top"), dKoef * (*margT));
		if (margR.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-right"), dKoef * (*margR));
		if (margB.is_init())
			oWriter.WriteAttributeCSS_double1_pt(_T("mso-wrap-distance-bottom"), dKoef * (*margB));

		nullable_int zIndex; oNode.ReadAttributeBase(L"relativeHeight", zIndex);
		if (zIndex.is_init())
			oWriter.WriteAttributeCSS_int(_T("z-index"), *zIndex);

		XmlUtils::CXmlNode oNodeHorP;
		if (oNode.GetNode(_T("wp:positionH"), oNodeHorP))
		{
			CString strWriteRelFrom = oNodeHorP.GetAttribute(_T("relativeFrom"), _T("column"));
			if (strWriteRelFrom == _T("character"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("char"));
			else if (strWriteRelFrom == _T("page"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("page"));
			else if (strWriteRelFrom == _T("margin"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("margin"));
			else if (strWriteRelFrom == _T("leftMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("left-margin-area"));
			else if (strWriteRelFrom == _T("rightMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("right-margin-area"));
			else if (strWriteRelFrom == _T("insideMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("inner-margin-area"));
			else if (strWriteRelFrom == _T("outsideMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("outer-margin-area"));
			else if (strWriteRelFrom == _T("column"))
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal-relative"), _T("text"));

			XmlUtils::CXmlNode oNodeO;
			if (oNodeHorP.GetNode(_T("wp:posOffset"), oNodeO))
			{
				int nPos = oNodeHorP.ReadValueInt(_T("wp:posOffset"));
				oWriter.WriteAttributeCSS_double1_pt(_T("margin-left"), dKoef * nPos);
				oWriter.WriteAttributeCSS(_T("mso-position-horizontal"), _T("absolute"));
			}
			else
			{
				CString sA = oNodeHorP.ReadValueString(_T("wp:align"));
				if (_T("") != sA)
				{
					oWriter.WriteAttributeCSS(_T("mso-position-horizontal"), sA);
				}
			}
		}

		XmlUtils::CXmlNode oNodeVerP;
		if (oNode.GetNode(_T("wp:positionV"), oNodeVerP))
		{
			CString strWriteRelFrom = oNodeVerP.GetAttribute(_T("relativeFrom"), _T("paragraph"));
			if (strWriteRelFrom == _T("margin"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("margin"));
			else if (strWriteRelFrom == _T("paragraph"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("text"));
			else if (strWriteRelFrom == _T("page"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("page"));
			else if (strWriteRelFrom == _T("topMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("top-margin-area"));
			else if (strWriteRelFrom == _T("bottomMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("bottom-margin-area"));
			else if (strWriteRelFrom == _T("insideMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("inner-margin-area"));
			else if (strWriteRelFrom == _T("outsideMargin"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("outer-margin-area"));
			else if (strWriteRelFrom == _T("line"))
				oWriter.WriteAttributeCSS(_T("mso-position-vertical-relative"), _T("line"));

			XmlUtils::CXmlNode oNodeO;
			if (oNodeVerP.GetNode(_T("wp:posOffset"), oNodeO))
			{
				int nPos = oNodeVerP.ReadValueInt(_T("wp:posOffset"));
				oWriter.WriteAttributeCSS_double1_pt(_T("margin-top"), dKoef * nPos);
				oWriter.WriteAttributeCSS(_T("mso-position-vertical"), _T("absolute"));
			}
			else
			{
				CString sA = oNodeVerP.ReadValueString(_T("wp:align"));
				if (_T("") != sA)
				{
					oWriter.WriteAttributeCSS(_T("mso-position-vertical"), sA);
				}
			}
		}

		XmlUtils::CXmlNode oNodeS;
		if (oNode.GetNode(_T("wp:extent"), oNodeS))
		{
			int _width = oNodeS.ReadAttributeInt(_T("cx"));
			int _height = oNodeS.ReadAttributeInt(_T("cy"));

			oWriter.WriteAttributeCSS_double1_pt(_T("width"), dKoef * _width);
			oWriter.WriteAttributeCSS_double1_pt(_T("height"), dKoef * _height);
		}

		XmlUtils::CXmlNode oNodeWrap = oNode.ReadNode(_T("<wp:wrapNone/>"));
		XmlUtils::CXmlNode oNodeWrapPoints;
		if (oNodeWrap.IsValid())
		{
			// ok
		}
		else
		{
			oNodeWrap = oNode.ReadNode(_T("wp:wrapSquare"));
			if (oNodeWrap.IsValid())
			{
				pWriter.m_strNodes += _T("<w10:wrap type=\"square\"/>");
			}
			else
			{
				oNodeWrap = oNode.ReadNode(_T("wp:wrapTopAndBottom"));
				if (oNodeWrap.IsValid())
				{
					pWriter.m_strNodes += _T("<w10:wrap type=\"topAndBottom\"/>");
				}
				else
				{
					oNodeWrap = oNode.ReadNode(_T("wp:wrapTight"));
					if (oNodeWrap.IsValid())
					{
						pWriter.m_strNodes += _T("<w10:wrap type=\"tight\"/>");
						oNodeWrap.GetNode(_T("wp:wrapPolygon"), oNodeWrapPoints);
					}
					else
					{
						oNodeWrap = oNode.ReadNode(_T("wp:wrapThrough"));
						if (oNodeWrap.IsValid())
						{
							pWriter.m_strNodes += _T("<w10:wrap type=\"through\"/>");
							oNodeWrap.GetNode(_T("wp:wrapPolygon"), oNodeWrapPoints);
						}
					}
				}
			}
		}

		if (oNodeWrapPoints.IsValid())
		{
			double dKoefX = 100000.0 / 21600;
			double dKoefY = dKoefX;
			if (oElem.is<PPTX::Logic::SpTree>())
			{
				PPTX::Logic::SpTree& oSpTr = oElem.as<PPTX::Logic::SpTree>();
				if (oSpTr.grpSpPr.xfrm.is_init())
				{
					int nW = oSpTr.grpSpPr.xfrm->chExtX.get_value_or(21600);
					int nH = oSpTr.grpSpPr.xfrm->chExtY.get_value_or(21600);

					//dKoefX = ((double)(nW)) / 21600.0;
					//dKoefY = ((double)(nH)) / 21600.0;
					dKoefX = 1.0;
					dKoefY = 1.0;
				}
			}

			CString strAttr = _T(" wrapcoords=\"");
			XmlUtils::CXmlNodes oNodesP;
			if (oNodeWrapPoints.GetNodes(_T("*"), oNodesP))
			{
				int nCountP = oNodesP.GetCount();
				for (int i = 0; i < nCountP; ++i)
				{
					XmlUtils::CXmlNode oNodeT;
					oNodesP.GetAt(i, oNodeT);

					int nX = oNodeT.ReadAttributeInt(_T("x"));
					int nY = oNodeT.ReadAttributeInt(_T("y"));
					nX = (int)(dKoefX * nX);
					nY = (int)(dKoefY * nY);

					CString strFP = _T("");
					strFP.Format(_T("%d %d"), nX, nY);
					strAttr += strFP;

					if (i < (nCountP - 1))
						strAttr += _T(" ");
				}
			}
			strAttr += _T("\"");

			pWriter.m_strAttributesMain += strAttr;
		}
	}

	pWriter.m_strStyleMain = oWriter.GetXmlString();
}

HRESULT CDrawingConverter::GetTxBodyBinary(const CString& bsXml)
{
	XmlUtils::CXmlNode oNode;
	if (!oNode.FromXmlString((CString)bsXml))
		return S_FALSE;
	
	PPTX::Logic::TxBody oTxBody(oNode);

	//m_pBinaryWriter->ClearNoAttack();
	//ULONG lOldPos = m_pBinaryWriter->GetPosition();
	m_pBinaryWriter->m_pCommon->CheckFontPicker();
	//m_pBinaryWriter->m_pCommon->m_pNativePicker->Init(m_strFontDirectory);

	m_pBinaryWriter->WriteRecord1(0, oTxBody);

	//m_pBinaryWriter->SetPosition(lOldPos);

	//m_pBinaryWriter->ClearNoAttack();
	return S_OK;
}

HRESULT CDrawingConverter::GetTxBodyXml(LONG lStart, LONG lLength, CString** pbstrXml)
{
	m_pReader->Seek(lStart);

	BYTE type = m_pReader->GetUChar();
	if (0 != type || NULL == pbstrXml)
		return S_FALSE;

	PPTX::Logic::TxBody oTxBody;
	oTxBody.fromPPTY(m_pReader);

	NSBinPptxRW::CXmlWriter oWriter;
	oTxBody.toXmlWriterExcel(&oWriter);

	CString strXml = oWriter.GetXmlString();
	*pbstrXml = new CString();
	**pbstrXml = strXml;

	return S_OK;
}

HRESULT CDrawingConverter::SetFontDir(const CString& bsFontDir)
{
	m_strFontDirectory = (CString)bsFontDir;
	return S_OK;
}

HRESULT CDrawingConverter::GetRecordBinary(LONG lRecordType, const CString& bsXml)
{
	if (bsXml.IsEmpty())
		return S_FALSE;

	CString strXml = _T("<main ");

	strXml += _T("\
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" \
xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"");

	strXml += _T(">");
	strXml += (CString)bsXml;
	strXml += _T("</main>");

	XmlUtils::CXmlNode oNodeMain;
	if (!oNodeMain.FromXmlString(strXml))
		return S_FALSE;

	XmlUtils::CXmlNodes oNodes;
	if (!oNodeMain.GetNodes(_T("*"), oNodes))
		return S_FALSE;

	if (1 != oNodes.GetCount())
		return S_FALSE;

	XmlUtils::CXmlNode oNode;
	oNodes.GetAt(0, oNode);

	PPTX::WrapperWritingElement* pWritingElem = NULL;
	switch (lRecordType)
	{
	case XMLWRITER_RECORD_TYPE_SPPR:
		{
			PPTX::Logic::SpPr* pSpPr = new PPTX::Logic::SpPr();
			*pSpPr = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pSpPr;
			break;
		}
	case XMLWRITER_RECORD_TYPE_CLRMAPOVR:
		{
			PPTX::Logic::ClrMap* pClrMap = new PPTX::Logic::ClrMap();
			*pClrMap = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pClrMap;
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_OUTLINE:
		{
			PPTX::Logic::Ln* pLn = new PPTX::Logic::Ln();
			*pLn = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pLn;
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_FILL:
		{
			PPTX::Logic::UniFill* pUniFill = new PPTX::Logic::UniFill();
			pUniFill->GetFillFrom(oNode);

			pWritingElem = (PPTX::WrapperWritingElement*)pUniFill;
			break;
		}
	default:
		break;
	}

	if (NULL == pWritingElem)
		return S_FALSE;

	//m_pBinaryWriter->ClearNoAttack();
	m_pBinaryWriter->m_pCommon->CheckFontPicker();

	//ULONG lOldPos = m_pBinaryWriter->GetPosition();

	m_pBinaryWriter->WriteRecord1(0, *pWritingElem);
	
	
	RELEASEOBJECT(pWritingElem);

	//m_pBinaryWriter->SetPosition(lOldPos);

	return S_OK;	
}

HRESULT CDrawingConverter::GetRecordXml(LONG lStart, LONG lLength, LONG lRecType, LONG lDocType, CString** bsXml)
{
	if (bsXml == NULL)
		return S_FALSE;

	if (lDocType != XMLWRITER_DOC_TYPE_DOCX)
        m_pReader->m_pRels->m_pManager->m_bIsWord = false;
	else
        m_pReader->m_pRels->m_pManager->m_bIsWord = true;

	m_pReader->Seek(lStart);
	
	BYTE typeRec1 = m_pReader->GetUChar();
	
	PPTX::WrapperWritingElement* pWritingElem = NULL;

	switch (lRecType)
	{
	case XMLWRITER_RECORD_TYPE_SPPR:
		{
			pWritingElem = (PPTX::WrapperWritingElement*)(new PPTX::Logic::SpPr());
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_CLRMAPOVR:
		{
			PPTX::Logic::ClrMap* pClrMap = new  PPTX::Logic::ClrMap();
			pClrMap->m_name = _T("a:clrMapOvr");
			pWritingElem = (PPTX::WrapperWritingElement*)(pClrMap);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_OUTLINE:
		{
			PPTX::Logic::Ln* pLn = new  PPTX::Logic::Ln();
			pWritingElem = (PPTX::WrapperWritingElement*)(pLn);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_FILL:
		{
			PPTX::Logic::UniFill* pUniFill = new  PPTX::Logic::UniFill();
			pWritingElem = (PPTX::WrapperWritingElement*)(pUniFill);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	default:
		break;
	}

	if (NULL == pWritingElem)
		return S_FALSE;

	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.m_lDocType = (BYTE)lDocType;
    oXmlWriter.m_bIsUseOffice2007 = false;
	oXmlWriter.m_bIsTop = true;

	pWritingElem->toXmlWriter(&oXmlWriter);

	CString ret = oXmlWriter.GetXmlString();
	*bsXml = new CString();
	**bsXml = ret;

	RELEASEOBJECT(pWritingElem);

	return S_OK;
}

HRESULT CDrawingConverter::SetDstContentRels()
{
	++m_pReader->m_nCurrentRelsStack;

	//чистить текущий m_pRels хорошо при последовательной записи автофигур в word.
	//плохо в случае записи перезентаций, с момента перехода на единственный обьект m_pReader.
	//пример: презетации записали несколько Rels, записываем chart, вызывается SetDstContentRels и трутся Rels презентаций
	//if (0 == m_pReader->m_nCurrentRelsStack)
	//{
	//	m_pReader->m_pRels->Clear();
	//	m_pReader->m_pRels->StartRels();
	//}
	//else
	{
		m_pReader->m_stackRels.push_back(m_pReader->m_pRels);
		NSBinPptxRW::CRelsGenerator* pGenerator = new NSBinPptxRW::CRelsGenerator(m_pReader->m_pRels->m_pManager);
		m_pReader->m_pRels = pGenerator;
		m_pReader->m_pRels->StartRels();
	}
	return S_OK;
}
HRESULT CDrawingConverter::SaveDstContentRels(const CString& bsRelsPath)
{
	m_pReader->m_pRels->CloseRels();
	m_pReader->m_pRels->SaveRels((CString)bsRelsPath);

	--m_pReader->m_nCurrentRelsStack;
	if (-1 > m_pReader->m_nCurrentRelsStack)
		m_pReader->m_nCurrentRelsStack = -1;

	//if (-1 != m_pReader->m_nCurrentRelsStack)
	{
		int nIndex = m_pReader->m_stackRels.size() - 1;

		if (0 <= nIndex)
		{
			NSBinPptxRW::CRelsGenerator* pCur = m_pReader->m_pRels;
			m_pReader->m_pRels = m_pReader->m_stackRels[nIndex];
			m_pReader->m_stackRels.erase(m_pReader->m_stackRels.begin() + nIndex);
			RELEASEOBJECT(pCur);								
		}
	}

	return S_OK;
}
HRESULT CDrawingConverter::WriteRels(const CString& bsType, const CString& bsTarget, const CString& bsTargetMode, LONG* lId)
{
	if (NULL == lId)
		return S_FALSE;

	*lId = m_pReader->m_pRels->WriteRels(bsType, bsTarget, bsTargetMode);
	return S_OK;
}
HRESULT CDrawingConverter::SetFontPicker(COfficeFontPicker* pFontPicker)
{
	m_pBinaryWriter->m_pCommon->CreateFontPicker(pFontPicker);
	return S_OK;
}
HRESULT CDrawingConverter::SetAdditionalParam(const CString& ParamName, BYTE *pArray, size_t szCount)
{
    CString name = (CString)ParamName;
    if (name == _T("SerializeImageManager"))
    {
        NSBinPptxRW::CBinaryFileReader oReader;
        oReader.Deserialize(m_pBinaryWriter->m_pCommon->m_pImageManager, pArray, szCount);
    }
    else if (name == _T("SerializeImageManager2"))
    {
        NSBinPptxRW::CBinaryFileReader oReader;
        oReader.Deserialize(m_pImageManager, pArray, szCount);
    }
    if (name == _T("xfrm_override") && pArray)
    {
		PPTX::Logic::Xfrm *pXfrm = (PPTX::Logic::Xfrm*)pArray;
		
		m_oxfrm_override = new PPTX::Logic::Xfrm(*pXfrm);
	}
    return S_OK;

}
HRESULT CDrawingConverter::SetAdditionalParam(const CString& ParamName, VARIANT ParamValue)
{
	CString name = (CString)ParamName;
	if (name == _T("SourceFileDir"))
	{
		m_pReader->m_pRels->m_pManager = m_pImageManager;
        m_pImageManager->m_bIsWord = true;
		m_pReader->m_strFolder = CString(ParamValue.bstrVal);
	}
	else if (name == _T("SourceFileDir2"))
	{
		m_pReader->m_pRels->m_pManager = m_pImageManager;
        m_pImageManager->m_bIsWord = false;
		m_pReader->m_strFolder = CString(ParamValue.bstrVal);
	}
	else if (name == _T("UseConvertion2007"))
	{
		m_bIsUseConvertion2007 = (ParamValue.boolVal == VARIANT_TRUE) ? true : false;
	}
    else if (name == _T("SerializeImageManager"))
	{
        // moved to CDrawingConverter::SetAdditionalParam(const CString& ParamName, BYTE **pArray, size_t& szCount)
        return S_FALSE;
        //NSBinPptxRW::CBinaryFileReader oReader;
        //oReader.Deserialize(m_pBinaryWriter->m_pCommon->m_pImageManager, ParamValue.parray);
	}
	else if (name == _T("SerializeImageManager2"))
	{
        // moved to CDrawingConverter::SetAdditionalParam(const CString& ParamName, BYTE **pArray, size_t& szCount)
        return S_FALSE;
        //NSBinPptxRW::CBinaryFileReader oReader;
        //oReader.Deserialize(m_pImageManager, ParamValue.parray);
    }
	else if (name == _T("DocumentChartsCount") && ParamValue.vt == VT_I4)
	{
		m_pReader->m_lChartNumber = ParamValue.lVal + 1;
	}
	else if (name == _T("ObjectIdVML") && ParamValue.vt == VT_I4)
	{
		m_pXmlWriter->m_lObjectIdVML = ParamValue.lVal;
	}
	return S_OK;
}
HRESULT CDrawingConverter::GetAdditionalParam(const CString& ParamName, BYTE **pArray, size_t& szCount)
{
    CString name = (CString)ParamName;
    if (name == _T("SerializeImageManager"))
    {
        NSBinPptxRW::CBinaryFileWriter oWriter;

        return oWriter.Serialize(m_pBinaryWriter->m_pCommon->m_pImageManager, pArray, szCount) ? S_OK : S_FALSE;
    }
    else if (name == _T("SerializeImageManager2"))
    {
        NSBinPptxRW::CBinaryFileWriter oWriter;

        return oWriter.Serialize(m_pImageManager, pArray, szCount) ? S_OK : S_FALSE;
    }
    return S_OK;
}
HRESULT CDrawingConverter::GetAdditionalParam(const CString& ParamName, VARIANT* ParamValue)
{
	CString name = (CString)ParamName;

    if (name == _T("SerializeImageManager"))
	{
        // moved to GetAdditionalParam(const CString& ParamName, BYTE **pArray, size_t& szCount)
        return S_FALSE;
        /*
		NSBinPptxRW::CBinaryFileWriter oWriter;

		ParamValue->vt = VT_ARRAY;
		ParamValue->parray = oWriter.Serialize(m_pBinaryWriter->m_pCommon->m_pImageManager);
        */
	}
	else if (name == _T("SerializeImageManager2"))
	{
        // moved to GetAdditionalParam(const CString& ParamName, BYTE **pArray, size_t& szCount)
        return S_FALSE;
        /*
		NSBinPptxRW::CBinaryFileWriter oWriter;

		ParamValue->vt = VT_ARRAY;
		ParamValue->parray = oWriter.Serialize(m_pImageManager);
        */
	}
    else if (name == _T("DocumentChartsCount"))
	{
		ParamValue->vt = VT_I4;
		ParamValue->lVal = m_pReader->m_lChartNumber - 1;
	}
	else if (name == _T("ObjectIdVML"))
	{
		ParamValue->vt = VT_I4;
		ParamValue->lVal = m_pXmlWriter->m_lObjectIdVML;
	}
	else if (name == _T("OleXlsx"))
	{
		ParamValue->vt = VT_BSTR;
#if defined(_WIN32) || defined (_WIN64)
		ParamValue->bstrVal = m_pXmlWriter->m_strOleXlsx.AllocSysString();
#else
		ParamValue->bstrVal = m_pXmlWriter->m_strOleXlsx;
#endif
	}
	else if (name == _T("ContentTypes"))
	{
		ParamValue->vt = VT_BSTR;
#if defined(_WIN32) || defined (_WIN64)
		ParamValue->bstrVal = m_pReader->m_strContentTypes.AllocSysString();
#else
        ParamValue->bstrVal = m_pReader->m_strContentTypes;
#endif
	}
	return S_OK;
}
void CDrawingConverter::Clear()
{
	for (std::map<CString, CShape*>::iterator pPair = m_mapShapeTypes.begin(); pPair != m_mapShapeTypes.end(); ++pPair)
	{
		CShape* pMem = pPair->second;
		RELEASEOBJECT(pMem);
	}
	m_mapShapeTypes.clear();
	m_mapRels.clear();
}

HRESULT CDrawingConverter::SetCurrentRelsPath()
{
	std::map<CString, smart_ptr<PPTX::CCommonRels>>::iterator pPair = m_mapRels.find(m_strCurrentRelsPath);

	if (m_mapRels.end() == pPair)
	{
		smart_ptr<PPTX::CCommonRels> pCR = new PPTX::CCommonRels();
		m_mapRels.insert(std::pair<CString, NSCommon::smart_ptr<PPTX::CCommonRels>>(m_strCurrentRelsPath, pCR));

		pPair = m_mapRels.find(m_strCurrentRelsPath);
		
		OOX::CPath filename = m_strCurrentRelsPath;	
		pPair->second->_read(filename);
	}

	*m_pBinaryWriter->m_pCommonRels = pPair->second;

	return S_OK;
}
void CDrawingConverter::SetFontManager(CFontManager* pFontManager)
{
	if(NULL != m_pBinaryWriter && NULL != m_pBinaryWriter->m_pCommon && NULL != m_pBinaryWriter->m_pCommon->m_pImageManager)
		m_pBinaryWriter->m_pCommon->m_pImageManager->SetFontManager(pFontManager);
}
