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
#ifndef _BUILD_COLOR_H_ 
#define _BUILD_COLOR_H_

#include "AggPlusEnums.h"
#include "../agg-2.4/include/agg_color_rgba.h"

/*
#define GetRValue(rgb)      (unsigned char(rgb))
#define GetGValue(rgb)      (unsigned char(rgb) >> 8))
#define GetBValue(rgb)      (unsigned char(rgb) >> 16))
*/

namespace Aggplus
{

class CColor  
{
public:
	CColor() { Argb = 0xFF000000; } //Black;
	//Color values are not premultiplied.
	CColor(BYTE r, BYTE g, BYTE b) { Argb = MakeARGB(255, r, g, b); }
    CColor(BYTE a, LONG lColor, bool bSwapRGB = false)
    {
        if (!bSwapRGB)
            Argb = ((a << AlphaShift) | lColor);
        else
        {
            Argb = MakeARGB(a, (BYTE)(lColor >> BlueShift), (BYTE)(lColor >> GreenShift), (BYTE)(lColor >> RedShift));
        }
    }
	CColor(BYTE a, BYTE r, BYTE g, BYTE b) { Argb = MakeARGB(a, r, g, b); }
	CColor(ARGB argb) { Argb = argb; }

	BYTE GetAlpha() const { return (BYTE) (Argb >> AlphaShift); }
	BYTE GetA() const { return GetAlpha(); }
	BYTE GetRed() const   { return (BYTE) (Argb >> RedShift); }
	BYTE GetR() const { return GetRed(); }
	BYTE GetGreen() const { return (BYTE) (Argb >> GreenShift); }
	BYTE GetG() const { return GetGreen(); }
	BYTE GetBlue() const { return (BYTE) (Argb >> BlueShift); }
	BYTE GetB() const { return GetBlue(); }

	ARGB GetValue() const { return Argb; }
	void SetValue(ARGB argb) { Argb = argb; }
		 

public:
// Shift count and bit mask for A, R, G, B components
	enum
	{
		AlphaShift  = 24,
		RedShift    = 16,
		GreenShift  = 8,
		BlueShift   = 0
	};
	enum
	{
		AlphaMask   = 0xff000000,
		RedMask     = 0x00ff0000,
		GreenMask   = 0x0000ff00,
		BlueMask    = 0x000000ff
	};

	// Assemble A, R, G, B values into a 32-bit integer
	static ARGB MakeARGB(BYTE a, BYTE r, BYTE g, BYTE b)
	{
		return (((ARGB) (b) <<  BlueShift) |
			((ARGB) (g) << GreenShift) |
			((ARGB) (r) <<   RedShift) |
			((ARGB) (a) << AlphaShift));
	}

	//AGG converter
	inline agg::rgba8 GetAggColor() const { return(agg::rgba8(GetBlue(), GetGreen(), GetRed(), GetAlpha())); }

protected:

	ARGB Argb;
};

}

#endif // _BUILD_COLOR_H_
