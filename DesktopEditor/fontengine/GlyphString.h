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
#ifndef _BUILD_GLYPH_STRING_H
#define _BUILD_GLYPH_STRING_H

#include "../common/Types.h"
#include "../common/Array.h"

#include <math.h>
#include <string>

#include "../graphics/pro/Fonts.h"

//-------------------------------------------------------------------------------------------------------------------------------
// TGlyphBitmap
//-------------------------------------------------------------------------------------------------------------------------------
class TGlyphBitmap 
{
public:
	int     nX;        // Сдвиг по X начальной точки для рисования символа
	int     nY;        // Сдвиг по Y начальной точки для рисования символа
	int     nWidth;    // Ширина символа
	int     nHeight;   // Высота символа
	INT    bAA;       // Anti-aliased: True означает, что Bitmap 8-битный(т.е. с альфой); False - Bitmap 1-битный
	BYTE*	pData;     // Bitmap data(картинка с символом)
	INT    bFreeData; // True, если память в pData нужно освободить

	TGlyphBitmap();
	~TGlyphBitmap();
};

enum EGlyphState
{
    glyphstateNormal = 0,  // символ отрисовался в нужном шрифте
    glyphstateDefault,     // символ отрисовался в дефолтовом шрифте
    glyphstateMiss         // символ не отрисовался
};

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
};

class TGlyph
{
public:
    unsigned int lUnicode; // Юникод
	float        fX;       // Позиция глифа
	float        fY;       // на BaseLine

	float        fLeft;    //
	float        fTop;     // BBox
	float        fRight;   //
	float        fBottom;  //

	TMetrics		oMetrics;

	EGlyphState		eState;

	bool			bBitmap;
	TGlyphBitmap	oBitmap;

public:
	TGlyph();
	~TGlyph();
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
}

class CGlyphString
{
public:
	CGlyphString();
	CGlyphString(const std::wstring& wsString, float fX = 0, float fY = 0);
	~CGlyphString();
	void SetString(const std::wstring& wsString, float fX = 0, float fY = 0);
    void SetString(const unsigned int* pGids, const unsigned int& nGidsCount, float fX = 0, float fY = 0);
    void SetStringGID(const unsigned int& gid, float fX = 0, float fY = 0);
	void Reset();
	int  GetLength();
	void SetBBox(int nIndex, float fLeft, float fTop, float fRight, float fBottom);
	void SetMetrics(int nIndex, float fWidth, float fHeight, float fHoriAdvance, float fHoriBearingX, float fHoriBearingY, float fVertAdvance, float fVertBearingX, float fVertBearingY);
	void SetStartPoint(int nIndex, float fX, float fY);
	void SetState(int nIndex, EGlyphState eState);
	void GetBBox(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom, int nIndex = -1, int nType = 0);
	void GetBBox2(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom);
	void SetCTM(float fA, float fB, float fC, float fD, float fE ,float fF);
	void ResetCTM();
	void Transform(float *pfX, float *pfY);
	void SetTrans(float fX, float fY);
	TGlyph* GetAt(int nIndex);
	INT GetNext(TGlyph*& pGlyph);
public:
	float   m_fTransX;
	float   m_fTransY;

	float   m_fX; // Координаты начальной точки для рисования
	float   m_fY; //

	float   m_fEndX; // Координаты конечной точки
	float   m_fEndY; //

	double  m_arrCTM[6];     // Глобальная матрица преобразования
	double  m_dIDet;         // (Детерминант матрицы преобразования)^(-1)
	
private:
	TGlyph* m_pGlyphsBuffer; // Символы в данной строке
	int     m_nGlyphsCount;  // Количество символов в строке
	int     m_nGlyphIndex;   // Номер текущего символа
};

#endif /* _BUILD_GLYPH_STRING_H */
