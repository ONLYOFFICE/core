/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
	int     nX;        // X offset of starting point for drawing character
	int     nY;        // Y offset of starting point for drawing character
	int     nWidth;    // Character width
	int     nHeight;   // Character height
	INT    bAA;       // Anti-aliased: True means Bitmap is 8-bit (i.e. with alpha); False - Bitmap is 1-bit
	BYTE*	pData;     // Bitmap data (image with character)
	INT    bFreeData; // True if memory in pData needs to be freed

	TGlyphBitmap();
	~TGlyphBitmap();
};

enum EGlyphState
{
	glyphstateNormal = 0,  // character rendered in required font
	glyphstateDefault,     // character rendered in default font
	glyphstateMiss         // character was not rendered
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
	unsigned int lUnicode; // Unicode
	float        fX;       // Glyph position
	float        fY;       // on BaseLine

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
	// Constants related to CharMap: Platform, Encoding ID
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

	float   m_fX; // Starting point coordinates for drawing
	float   m_fY; //

	float   m_fEndX; // End point coordinates
	float   m_fEndY; //

	double  m_arrCTM[6];     // Global transformation matrix
	double  m_dIDet;         // (Transformation matrix determinant)^(-1)

private:
	TGlyph* m_pGlyphsBuffer; // Characters in this string
	int     m_nGlyphsCount;  // Number of characters in string
	int     m_nGlyphIndex;   // Current character index
};

#endif /* _BUILD_GLYPH_STRING_H */
