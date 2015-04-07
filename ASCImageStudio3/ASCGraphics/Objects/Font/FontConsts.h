#ifndef _FONT_CONSTS_H
#define _FONT_CONSTS_H

#include "GlyphBitmap.h"
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

//---------------------------------------------------------------------------------------------------

#define UNKNOWN_CHARSET 3 // для случаев, когда задано значение DEFAULT_CHARSET, но 
                          // на самом деле charset не учитывается

//---------------------------------------------------------------------------------------------------

#define MAX_FONT_CACHE_SIZE 16
#define MAX_FONT_NAME_LEN   50
#define MAX_FONT_STYLE_LEN  40

//---------------------------------------------------------------------------------------------------
// Некоторые константы, затрагивающие рисование глифов
//---------------------------------------------------------------------------------------------------

#define FONT_FRACTION_BITS  2
#define FONT_FRACTION      (1 << FONT_FRACTION_BITS)
#define FONT_FRACTION_MULT ((double)1 / (double)FONT_FRACTION)

#define FONT_ITALIC_ANGLE  0.3090169943749 // Синус 18 градусов (подбиралось под Word 2007)  

//---------------------------------------------------------------------------------------------------
// Структура для хранения информации о символе в строке
//---------------------------------------------------------------------------------------------------

enum EGlyphState
{
	glyphstateNormal = 0,  // символ отрисовался в нужном шрифте
	glyphstateDeafault,    // символ отрисовался в дефолтовом шрифте
	glyphstateMiss         // символ не отрисовался
};

struct TGlyph
{
	long         lUnicode; // Юникод
	float        fX;       // Позиция глифа
	float        fY;       // на BaseLine

	float        fLeft;    //
	float        fTop;     // BBox
	float        fRight;   //
	float        fBottom;  //

	struct TMetrics
	{
		float fWidth;
		float fHeight;

		float fHoriBearingX;
		float fHoriBearingY;
		float fHoriAdvance;

		float fVertBearingX;
		float fVertBearingY;
		float fVertAdvance;
	} oMetrics;

	EGlyphState  eState;

	bool         bBitmap;
	TGlyphBitmap oBitmap;
};

namespace FontConstants
{

//---------------------------------------------------------------------------------------------------
// Константы связанные с CharMap: Platform, Encoding ID
//---------------------------------------------------------------------------------------------------

	const long c_lUniPlatform = 0;
	const long c_lMacPlatform = 1;
	const long c_lWinPlatform = 3;

	const long c_lWinEncSymbol   = 0;
	const long c_lWinEncUSC2     = 1;
	const long c_lWinEncShiftJIS = 2;
	const long c_lWinEncPRC      = 3;
	const long c_lWinEncBig5     = 4;
	const long c_lWinEncWansung  = 5;
	const long c_lWinEncJohab    = 6;
	const long c_lWinEncUCS4     = 10;

//--------------------------------------------------------------------------
// FontStyle: face types and common styles
//--------------------------------------------------------------------------

	enum FontStyle
	{
		FontStyleRegular    = 0,
		FontStyleBold       = 1,
		FontStyleItalic     = 2,
		FontStyleBoldItalic = 3,
		FontStyleUnderline  = 4,
		FontStyleStrikeout  = 8
	};

//--------------------------------------------------------------------------
// Unit constants
//--------------------------------------------------------------------------

	enum Unit
	{
		UnitWorld,      // 0 -- World coordinate (non-physical unit)
		UnitDisplay,    // 1 -- Variable -- for PageTransform only
		UnitPixel,      // 2 -- Each unit is one device pixel.
		UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
		UnitInch,       // 4 -- Each unit is 1 inch.
		UnitDocument,   // 5 -- Each unit is 1/300 inch.
		UnitMillimeter  // 6 -- Each unit is 1 millimeter.
	};


};

#endif /* _FONT_CONSTS_H */