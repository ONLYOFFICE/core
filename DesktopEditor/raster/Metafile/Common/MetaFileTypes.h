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
#ifndef _METAFILE_COMMON_METAFILETYPES_H
#define _METAFILE_COMMON_METAFILETYPES_H

#include <string>
#include <cfloat>
#include "../../../common/StringExt.h"

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef NULL
#define NULL 0
#endif

#define METAFILE_RGBA(r, g, b, a) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) | ( (unsigned char)(a) << 24 ) ) )
#define INTCOLOR_TO_RGB(color) std::to_wstring(color >> 0 & 0xFF) + L", " + std::to_wstring(color >> 8 & 0xFF) + L", " + std::to_wstring(color >> 16 & 0xFF)

#if !defined (_WIN32) && !defined(_WIN64)
	#define BLACKONWHITE                 1
	#define WHITEONBLACK                 2
	#define COLORONCOLOR                 3
	#define HALFTONE                     4
	#define MAXSTRETCHBLTMODE            4

	#define PT_CLOSEFIGURE      0x01
	#define PT_LINETO           0x02
	#define PT_BEZIERTO         0x04
	#define PT_MOVETO           0x06

	#ifndef _MAC
		#define ENHMETA_SIGNATURE       0x464D4520
	#else
		#define ENHMETA_SIGNATURE       0x20454D46
	#endif
#endif

#ifndef MWT_IDENTITY
	#define MWT_IDENTITY	  0x01
#endif
#ifndef MWT_LEFTMULTIPLY
	#define MWT_LEFTMULTIPLY  0x02
#endif
#ifndef MWT_RIGHTMULTIPLY
	#define MWT_RIGHTMULTIPLY 0x03
#endif
#ifndef MWT_SET
	#define MWT_SET           0x04
#endif


//---------------------------------------------------------------------------------------------------
// Modes for CWmfFile.SetMapMode
//---------------------------------------------------------------------------------------------------
#define MM_TEXT        1
#define MM_LOMETRIC    2
#define MM_HIMETRIC    3
#define MM_LOENGLISH   4
#define MM_HIENGLISH   5
#define MM_TWIPS       6
#define MM_ISOTROPIC   7
#define MM_ANISOTROPIC 8
#define MM_DPI         9

#define RGN_AND  1
#define RGN_OR   2
#define RGN_XOR  3
#define RGN_DIFF 4
#define RGN_COPY 5

//--------------------------------------------------------------------------------------------------
// From wingdi.h
//--------------------------------------------------------------------------------------------------
/* PolyFill() Modes */
#define ALTERNATE                    1
#define WINDING                      2
#define POLYFILL_LAST                2

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
#define BKMODE_LAST         2

/* Brush Styles */
#define BS_SOLID            0
#define BS_NULL             1
#define BS_HOLLOW           BS_NULL
#define BS_HATCHED          2
#define BS_PATTERN          3
#define BS_INDEXED          4
#define BS_DIBPATTERN       5
#define BS_DIBPATTERNPT     6
#define BS_PATTERN8X8       7
#define BS_DIBPATTERN8X8    8
#define BS_MONOPATTERN      9
#define BS_LINEARGRADIENT	10
#define BS_RADIALGRADIENT	11
#define BS_AXIALGRADIENT	12
#define BS_RECTGRADIENT		13
#define BS_PATHGRADIENT		14
//square? ellips ??

/* Hatch Styles */
#define HS_HORIZONTAL       0       /* ----- */
#define HS_VERTICAL         1       /* ||||| */
#define HS_FDIAGONAL        2       /* \\\\\ */
#define HS_BDIAGONAL        3       /* ///// */
#define HS_CROSS            4       /* +++++ */
#define HS_DIAGCROSS        5       /* xxxxx */

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8
#define PS_STYLE_MASK       0x0000000F

#define PS_ENDCAP_ROUND     0x00000000
#define PS_ENDCAP_SQUARE    0x00000100
#define PS_ENDCAP_FLAT      0x00000200
#define PS_ENDCAP_MASK      0x00000F00

#define PS_JOIN_ROUND       0x00000000
#define PS_JOIN_BEVEL       0x00001000
#define PS_JOIN_MITER       0x00002000
#define PS_JOIN_MASK        0x0000F000

#define PS_COSMETIC         0x00000000
#define PS_GEOMETRIC        0x00010000
#define PS_TYPE_MASK        0x000F0000

#define AD_COUNTERCLOCKWISE 1
#define AD_CLOCKWISE        2

/* Text Alignment Options */
#define TA_NOUPDATECP                0
#define TA_UPDATECP                  1

#define TA_LEFT                      0
#define TA_RIGHT                     2
#define TA_CENTER                    6

#define TA_TOP                       0
#define TA_BOTTOM                    8
#define TA_BASELINE                  24

#if (WINVER >= 0x0400)
#define TA_RTLREADING                256
#define TA_MASK       (TA_BASELINE+TA_CENTER+TA_UPDATECP+TA_RTLREADING)
#else
#define TA_MASK       (TA_BASELINE+TA_CENTER+TA_UPDATECP)
#endif

#define VTA_BASELINE TA_BASELINE
#define VTA_LEFT     TA_BOTTOM
#define VTA_RIGHT    TA_TOP
#define VTA_CENTER   TA_CENTER
#define VTA_BOTTOM   TA_RIGHT
#define VTA_TOP      TA_LEFT

/* Binary raster ops */
#define R2_BLACK            1   /*  0       */
#define R2_NOTMERGEPEN      2   /* DPon     */
#define R2_MASKNOTPEN       3   /* DPna     */
#define R2_NOTCOPYPEN       4   /* PN       */
#define R2_MASKPENNOT       5   /* PDna     */
#define R2_NOT              6   /* Dn       */
#define R2_XORPEN           7   /* DPx      */
#define R2_NOTMASKPEN       8   /* DPan     */
#define R2_MASKPEN          9   /* DPa      */
#define R2_NOTXORPEN        10  /* DPxn     */
#define R2_NOP              11  /* D        */
#define R2_MERGENOTPEN      12  /* DPno     */
#define R2_COPYPEN          13  /* P        */
#define R2_MERGEPENNOT      14  /* PDno     */
#define R2_MERGEPEN         15  /* DPo      */
#define R2_WHITE            16  /*  1       */
#define R2_LAST             16

/* Ternary raster operations */
#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (DWORD)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (DWORD)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
#define NOTSRCERASE         (DWORD)0x001100A6 /* dest = (NOT src) AND (NOT dest) */
#define MERGECOPY           (DWORD)0x00C000CA /* dest = (source AND pattern)     */
#define MERGEPAINT          (DWORD)0x00BB0226 /* dest = (NOT source) OR dest     */
#define PATCOPY             (DWORD)0x00F00021 /* dest = pattern                  */
#define PATPAINT            (DWORD)0x00FB0A09 /* dest = DPSnoo                   */
#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
#define DSTINVERT           (DWORD)0x00550009 /* dest = (NOT dest)               */
#define BLACKNESS           (DWORD)0x00000042 /* dest = BLACK                    */
#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE                    */

/* Object Definitions for EnumObjects() */
#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13
#define OBJ_COLORSPACE      14

#define ANSI_CHARSET            0
#define DEFAULT_CHARSET         1
#define SYMBOL_CHARSET          2
#define SHIFTJIS_CHARSET        128
#define HANGEUL_CHARSET         129
#define HANGUL_CHARSET          129
#define GB2312_CHARSET          134
#define CHINESEBIG5_CHARSET     136
#define OEM_CHARSET             255

#define JOHAB_CHARSET           130
#define HEBREW_CHARSET          177
#define ARABIC_CHARSET          178
#define GREEK_CHARSET           161
#define TURKISH_CHARSET         162
#define VIETNAMESE_CHARSET      163
#define THAI_CHARSET            222
#define EASTEUROPE_CHARSET      238
#define RUSSIAN_CHARSET         204

#define MAC_CHARSET             77
#define BALTIC_CHARSET          186

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

#ifndef LAYOUT_LTR
	#define LAYOUT_LTR							0x0000
#endif
#ifndef LAYOUT_RTL
	#define LAYOUT_RTL							0x0001
#endif
#ifndef LAYOUT_BITMAPORIENTATIONPRESERVED
	#define LAYOUT_BITMAPORIENTATIONPRESERVED	0x0008
#endif

#define GM_COMPATIBLE       1
#define GM_ADVANCED         2

// BRUSH STYLES
#define HS_05Percent        6
#define HS_10Percent        7
#define HS_20Percent        8
#define HS_25Percent        9
#define HS_30Percent        10
#define HS_40Percent        11
#define HS_50Percent        12
#define HS_60Percent        13
#define HS_70Percent        14
#define HS_75Percent        15
#define HS_80Percent        16
#define HS_90Percent        17

#define HS_LTDOWNWARDDIAG   18
#define HS_LTUPWARDDIAG     19
#define HS_DNDOWNWARDDIAG   20
#define HS_DNUPWARDDIAG     21
#define HS_WDOWNWARDDIAG    22
#define HS_WUPWARDDIAG      23

#define HS_LTVERTICAL       24
#define HS_LTHORIZONTAL     25
#define HS_NVERTICAL        26
#define HS_NHORIZONTAL      27
#define HS_DNVERTICAL       28
#define HS_DNHORIZONTAL     29

#define HS_DASHDOWNWARDDIAG 30
#define HS_DASHUPWARDDIAG   31
#define HS_DASHHORIZONTAL   32
#define HS_DASHVERTICAL     33

#define HS_SMALLCONFETTI    34
#define HS_LARGECONFETTI    35
#define HS_ZIGZAG           36
#define HS_WAVE             37
#define HS_DIAGBRICK        38
#define HS_HORIZBRICK       39
#define HS_WEAVE            40
#define HS_PLAID            41
#define HS_DIVOT            42
#define HS_DOTGRID          43
#define HS_DOTDIAMOND       44
#define HS_SHINGLE          45
#define HS_TRELLIS          46
#define HS_SPHERE           47
#define HS_SGRID            48
#define HS_SCHECHERBOARD    49
#define HS_LCHECHERBOARD    50
#define HS_OUTLINEDDIAMOND  51
#define HS_SOLIDDIAMOND     52

namespace MetaFile
{
	#define DEFAULT_FONT_SIZE 14

	enum InterpretatorType
	{
			Emf,
			Wmf,
			Render,
			XML,
			Svg,
			Array
	};

	enum EMetaFileBitCount
	{
		BI_BITCOUNT_0 = 0x0000,
		BI_BITCOUNT_1 = 0x0001,
		BI_BITCOUNT_2 = 0x0004,
		BI_BITCOUNT_3 = 0x0008,
		BI_BITCOUNT_4 = 0x0010,
		BI_BITCOUNT_5 = 0x0018,
		BI_BITCOUNT_6 = 0x0020
	};

	template<typename T>
	struct TPoint
	{
		T X;
		T Y;

		TPoint() : X(0), Y(0) {}

		template<typename U>
		TPoint(U _X, U _Y) : X(static_cast<T>(_X)), Y(static_cast<T>(_Y)) {}
		template<typename U>
		TPoint(const TPoint<U>& oPoint) :  X(static_cast<T>(oPoint.X)), Y(static_cast<T>(oPoint.Y)) {}

		template<typename U>
		TPoint<T> operator=(const TPoint<U>& oPoint)
		{
			X = static_cast<T>(oPoint.X);
			Y = static_cast<T>(oPoint.Y);

			return *this;
		}

		TPoint<T> operator *= (T scale)
		{
			X *= scale;
			Y *= scale;
		}
		
		bool operator==(const TPoint<T>& oPoint) const
		{
			return X == oPoint.X && Y == oPoint.Y;
		}
		
		bool operator!=(const TPoint<T>& oPoint) const
		{
			return X != oPoint.X || Y != oPoint.Y;
		}
	};

	template<>
	bool TPoint<double>::operator==(const TPoint<double>& oPoint) const;

	template<>
	bool TPoint<double>::operator!=(const TPoint<double>& oPoint) const;

	typedef TPoint<int>    TPointL;
	typedef TPoint<short>  TPointS;
	typedef TPoint<double> TPointD;

	typedef TPoint<int>    TSizeL;
	typedef TPoint<double> TScale;

	template<typename T>
	struct TRect
	{
		T Left;
		T Top;
		T Right;
		T Bottom;

		TRect() : Left(0), Top(0), Right(0), Bottom(0) {}
		
		template <typename U>
		TRect(U _Left, U _Top, U _Right, U _Bottom) :
			Left  (static_cast<T>(_Left))  ,
			Top   (static_cast<T>(_Top))   ,
			Right (static_cast<T>(_Right)) ,
			Bottom(static_cast<T>(_Bottom))
		{}
		
		template <typename U1, typename U2, typename U3, typename U4>
		TRect(U1 _Left, U2 _Top, U3 _Right, U4 _Bottom) :
			Left  (static_cast<T>(_Left))  ,
			Top   (static_cast<T>(_Top))   ,
			Right (static_cast<T>(_Right)) ,
			Bottom(static_cast<T>(_Bottom))
		{}
		

		template <typename U>
		TRect(const TRect<U>& oRect) : 
			Left  (static_cast<T>(oRect.Left))  ,
			Top   (static_cast<T>(oRect.Top))   ,
			Right (static_cast<T>(oRect.Right)) ,
			Bottom(static_cast<T>(oRect.Bottom))
		{}

		template <typename U>
		TRect<T>& operator=(const TRect<U>& oRect)
		{
			Left   = static_cast<T>(oRect.Left);
			Top    = static_cast<T>(oRect.Top);
			Right  = static_cast<T>(oRect.Right);
			Bottom = static_cast<T>(oRect.Bottom);

			return *this;
		}

		TRect<T>& operator*=(T scale)
		{
			Left   *= scale;
			Top    *= scale;
			Right  *= scale;
			Bottom *= scale;

			return *this;
		}

		bool Empty() const
		{
			return Left == Top && Top == Right && Right == Bottom;
		}

		void Update(bool bFlipedX, bool bFlipedY)
		{
			if ((Top > Bottom && !bFlipedY) || (Top < Bottom && bFlipedY))
			{
				T Temp = Bottom;
				Bottom = Top;
				Top = Temp;
			}

			if ((Left > Right && !bFlipedX) || (Left < Right && bFlipedX))
			{
				T Temp = Right;
				Right = Left;
				Left = Temp;
			}
		}

		void Copy(const TRect<T>* pOther) const
		{
			if (NULL == pOther)
				return;
			
			Left   = pOther->Left;
			Top    = pOther->Top;
			Right  = pOther->Right;
			Bottom = pOther->Bottom;
		}
		
		bool operator==(const TRect<T>& oRect) const
		{
			return Left  == oRect.Left  && Top    == oRect.Top    &&
			       Right == oRect.Right && Bottom == oRect.Bottom;
		}
		
		bool operator!=(const TRect<T>& oRect) const
		{
			return Left  != oRect.Left  || Top    != oRect.Top    ||
			       Right != oRect.Right || Bottom != oRect.Bottom;
		}
	};

	template<>
	bool TRect<double>::operator==(const TRect<double>& oRect) const;

	template<>
	bool TRect<double>::operator!=(const TRect<double>& oRect) const;
	
	typedef TRect <int>    TRectL;
	typedef TRect <short>  TRectS;
	typedef TRect <double> TRectD;

	struct TRGBA
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		TRGBA();
		TRGBA(const TRGBA& oRGB);
		TRGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0);
		TRGBA(int nValue);

		void Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0);
		void Copy(const TRGBA &oRGBA);

		int ToInt() const;
		void SwapToBGR();

		unsigned char GetRed()   const;
		unsigned char GetGreen() const;
		unsigned char GetBlue()  const;
		unsigned char GetAlpha() const;

		TRGBA& operator=(const TRGBA& oRGBA);
	};

	struct TXForm
	{
		double M11;
		double M12;
		double M21;
		double M22;
		double Dx;
		double Dy;

		TXForm();
		TXForm(const TXForm& oXForm);
		TXForm(double m11, double m12, double m21, double m22, double dx, double dy);

		void Init();
		void Copy(const TXForm* pOther);
		void Copy(const TXForm& oOther);
		void Multiply(const TXForm& oOther, unsigned int ulMode);
		void Apply(double& dX, double& dY) const;
	};
}

#endif //_METAFILE_COMMON_METAFILETYPES_H
