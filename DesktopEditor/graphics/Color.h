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
