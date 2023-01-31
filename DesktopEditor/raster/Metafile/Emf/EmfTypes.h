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
#ifndef _METAFILE_EMF_EMFTYPES_H
#define _METAFILE_EMF_EMFTYPES_H

#include "../../../common/Types.h"
#include "../Common/MetaFileTypes.h"

#if !defined(_WIN32) && !defined(_WIN64)
//from wingdi.h

#define PT_CLOSEFIGURE      0x01
#define PT_LINETO           0x02
#define PT_BEZIERTO         0x04
#define PT_MOVETO           0x06

#define ENHMETA_SIGNATURE       0x464D4520

// Stock object flag used in the object handle index in the enhanced
// metafile records.
// E.g. The object handle index (META_STOCK_OBJECT | BLACK_BRUSH)
// represents the stock object BLACK_BRUSH.

#define ENHMETA_STOCK_OBJECT    0x80000000

// Enhanced metafile record types.

#define EMR_HEADER                      1
#define EMR_POLYBEZIER                  2
#define EMR_POLYGON                     3
#define EMR_POLYLINE                    4
#define EMR_POLYBEZIERTO                5
#define EMR_POLYLINETO                  6
#define EMR_POLYPOLYLINE                7
#define EMR_POLYPOLYGON                 8
#define EMR_SETWINDOWEXTEX              9
#define EMR_SETWINDOWORGEX              10
#define EMR_SETVIEWPORTEXTEX            11
#define EMR_SETVIEWPORTORGEX            12
#define EMR_SETBRUSHORGEX               13
#define EMR_EOF                         14
#define EMR_SETPIXELV                   15
#define EMR_SETMAPPERFLAGS              16
#define EMR_SETMAPMODE                  17
#define EMR_SETBKMODE                   18
#define EMR_SETPOLYFILLMODE             19
#define EMR_SETROP2                     20
#define EMR_SETSTRETCHBLTMODE           21
#define EMR_SETTEXTALIGN                22
#define EMR_SETCOLORADJUSTMENT          23
#define EMR_SETTEXTCOLOR                24
#define EMR_SETBKCOLOR                  25
#define EMR_OFFSETCLIPRGN               26
#define EMR_MOVETOEX                    27
#define EMR_SETMETARGN                  28
#define EMR_EXCLUDECLIPRECT             29
#define EMR_INTERSECTCLIPRECT           30
#define EMR_SCALEVIEWPORTEXTEX          31
#define EMR_SCALEWINDOWEXTEX            32
#define EMR_SAVEDC                      33
#define EMR_RESTOREDC                   34
#define EMR_SETWORLDTRANSFORM           35
#define EMR_MODIFYWORLDTRANSFORM        36
#define EMR_SELECTOBJECT                37
#define EMR_CREATEPEN                   38
#define EMR_CREATEBRUSHINDIRECT         39
#define EMR_DELETEOBJECT                40
#define EMR_ANGLEARC                    41
#define EMR_ELLIPSE                     42
#define EMR_RECTANGLE                   43
#define EMR_ROUNDRECT                   44
#define EMR_ARC                         45
#define EMR_CHORD                       46
#define EMR_PIE                         47
#define EMR_SELECTPALETTE               48
#define EMR_CREATEPALETTE               49
#define EMR_SETPALETTEENTRIES           50
#define EMR_RESIZEPALETTE               51
#define EMR_REALIZEPALETTE              52
#define EMR_EXTFLOODFILL                53
#define EMR_LINETO                      54
#define EMR_ARCTO                       55
#define EMR_POLYDRAW                    56
#define EMR_SETARCDIRECTION             57
#define EMR_SETMITERLIMIT               58
#define EMR_BEGINPATH                   59
#define EMR_ENDPATH                     60
#define EMR_CLOSEFIGURE                 61
#define EMR_FILLPATH                    62
#define EMR_STROKEANDFILLPATH           63
#define EMR_STROKEPATH                  64
#define EMR_FLATTENPATH                 65
#define EMR_WIDENPATH                   66
#define EMR_SELECTCLIPPATH              67
#define EMR_ABORTPATH                   68

#define EMR_GDICOMMENT                  70
#define EMR_FILLRGN                     71
#define EMR_FRAMERGN                    72
#define EMR_INVERTRGN                   73
#define EMR_PAINTRGN                    74
#define EMR_EXTSELECTCLIPRGN            75
#define EMR_BITBLT                      76
#define EMR_STRETCHBLT                  77
#define EMR_MASKBLT                     78
#define EMR_PLGBLT                      79
#define EMR_SETDIBITSTODEVICE           80
#define EMR_STRETCHDIBITS               81
#define EMR_EXTCREATEFONTINDIRECTW      82
#define EMR_EXTTEXTOUTA                 83
#define EMR_EXTTEXTOUTW                 84
#define EMR_POLYBEZIER16                85
#define EMR_POLYGON16                   86
#define EMR_POLYLINE16                  87
#define EMR_POLYBEZIERTO16              88
#define EMR_POLYLINETO16                89
#define EMR_POLYPOLYLINE16              90
#define EMR_POLYPOLYGON16               91
#define EMR_POLYDRAW16                  92
#define EMR_CREATEMONOBRUSH             93
#define EMR_CREATEDIBPATTERNBRUSHPT     94
#define EMR_EXTCREATEPEN                95
#define EMR_POLYTEXTOUTA                96
#define EMR_POLYTEXTOUTW                97

#define EMR_SETICMMODE                  98
#define EMR_CREATECOLORSPACE            99
#define EMR_SETCOLORSPACE              100
#define EMR_DELETECOLORSPACE           101
#define EMR_GLSRECORD                  102
#define EMR_GLSBOUNDEDRECORD           103
#define EMR_PIXELFORMAT                104

#define EMR_RESERVED_105               105
#define EMR_RESERVED_106               106
#define EMR_RESERVED_107               107
#define EMR_RESERVED_108               108
#define EMR_RESERVED_109               109
#define EMR_RESERVED_110               110
#define EMR_COLORCORRECTPALETTE        111
#define EMR_SETICMPROFILEA             112
#define EMR_SETICMPROFILEW             113
#define EMR_ALPHABLEND                 114
#define EMR_SETLAYOUT                  115
#define EMR_TRANSPARENTBLT             116
#define EMR_RESERVED_117               117
#define EMR_GRADIENTFILL               118
#define EMR_RESERVED_119               119
#define EMR_RESERVED_120               120
#define EMR_COLORMATCHTOTARGETW        121
#define EMR_CREATECOLORSPACEW          122


#define EMR_MIN                          1

#define EMR_MAX                        122

#else

#include <windows.h>

#endif

#ifndef EMR_SMALLTEXTOUT
#define EMR_SMALLTEXTOUT EMR_RESERVED_108
#endif

#ifndef ETO_OPAQUE
	#define ETO_OPAQUE             0x00000002
#endif
#ifndef ETO_CLIPPED
	#define ETO_CLIPPED            0x00000004
#endif
#ifndef ETO_GLYPH_INDEX
	#define ETO_GLYPH_INDEX        0x00000010
#endif
#ifndef ETO_RTLREADING
	#define ETO_RTLREADING         0x00000080
#endif
#ifndef ETO_NO_RECT
	#define ETO_NO_RECT            0x00000100
#endif
#ifndef ETO_SMALL_CHARS
	#define ETO_SMALL_CHARS        0x00000200
#endif
#ifndef ETO_NUMERICSLOCAL 
	#define ETO_NUMERICSLOCAL      0x00000400
#endif
#ifndef ETO_NUMERICSLATIN
	#define ETO_NUMERICSLATIN      0x00000800
#endif
#ifndef ETO_IGNORELANGUAGE
	#define ETO_IGNORELANGUAGE     0x00001000
#endif
#ifndef ETO_PDY
	#define ETO_PDY                0x00002000
#endif
#ifndef ETO_REVERSE_INDEX_MAP
	#define ETO_REVERSE_INDEX_MAP  0x00010000
#endif

namespace MetaFile
{
	struct TEmfColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a; //Reserved Must be 0x00

		TEmfColor()
		{
			r = 0;
			g = 0;
			b = 0;
		}

		TEmfColor(unsigned char _r, unsigned char _g, unsigned char _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		void Set(unsigned char _r, unsigned char _g, unsigned char _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		void Init()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		void InitWhite()
		{
			r = 255;
			g = 255;
			b = 255;
			a = 0;
		}

		void Copy(TEmfColor* pOther)
		{
			r = pOther->r;
			g = pOther->g;
			b = pOther->b;
			a = pOther->a;
		}
	};

	struct TEmfRect
	{
		short shLeft;
		short shTop;
		short shRight;
		short shBottom;
	};

	struct TEmfWindow
	{
		int lX;
		int lY;
		int ulW;
		int ulH;

		void Init()
		{
			lX = 0;
			lY = 0;
			ulW = 1;
			ulH = 1;
		}

		void Copy(TEmfWindow* pOther)
		{
			lX  = pOther->lX;
			lY  = pOther->lY;
			ulW = pOther->ulW;
			ulH = pOther->ulH;
		}
	};

	struct TEmfRectL
	{
		TEmfRectL()
		{
			lLeft   = 0;
			lTop    = 0;
			lRight  = 0;
			lBottom = 0;
		}
		TEmfRectL(const TRect& oRect)
		{
			lLeft   = oRect.nLeft;
			lTop    = oRect.nTop;
			lRight  = oRect.nRight;
			lBottom = oRect.nBottom;
		}

		void Update(bool bFlipedX, bool bFlipedY)
		{
			if (lTop > lBottom && bFlipedY)
			{
				int nTemp = lBottom;
				lBottom = lTop;
				lTop = nTemp;
			}

			if (lLeft < lRight && bFlipedX)
			{
				int nTemp = lRight;
				lRight = lLeft;
				lLeft = nTemp;
			}
		}

		int lLeft;
		int lTop;
		int lRight;
		int lBottom;		
	};

	struct TEmfPointL
	{
		int x;
		int y;

		TEmfPointL& operator=(const TEmfPointL& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;

			return *this;
		}

		bool operator==(const TEmfPointL& oPoint)
		{
			return ((x == oPoint.x) && (y == oPoint.y));
		}
	};

	struct TEmfPointS
	{
		short x;
		short y;
	};

	struct TEmfPointD
	{
		double x;
		double y;
	};

	struct TEmfSizeL
	{
		int cx;
		int cy;
	};

	struct TEmfScale
	{
		double dX;
		double dY;

		TEmfScale(double dXScale, double dYScale)
			: dX(dXScale), dY(dYScale){}
	};

	struct TEmfHeader
	{
		TEmfHeader() : ulSignature(0), ulVersion(0), ulRecords(0), ushObjects(0), ulSize(0), ulPalEntries(0), ulOffsetDescription(0),ulSizeDescription(0) {}
		
		TEmfRectL      oBounds;
		TEmfRectL      oFrame;
		unsigned int   ulSignature;
		unsigned int   ulVersion;
		unsigned int   ulSize;
		unsigned int   ulRecords;
		unsigned short ushObjects;
		unsigned short ushReserved;
		unsigned int   ulSizeDescription;
		unsigned int   ulOffsetDescription;
		unsigned int   ulPalEntries;
		TEmfSizeL      oDevice;
		TEmfSizeL      oMillimeters;
		TRect          oFrameToBounds;
		TEmfRectL      oFramePx;
	};

	struct TEmfStretchDIBITS
	{
		TEmfRectL    Bounds;
		int          xDest;
		int          yDest;
		int          xSrc;
		int          ySrc;
		int          cxSrc;
		int          cySrc;
		unsigned int offBmiSrc;
		unsigned int cbBmiSrc;
		unsigned int offBitsSrc;
		unsigned int cbBitsSrc;
		unsigned int UsageSrc;
		unsigned int BitBltRasterOperation;
		int          cxDest;
		int          cyDest;
	};

	struct TRegionDataHeader
	{
		unsigned int unSize;
		unsigned int unType;
		unsigned int unCountRects;
		unsigned int unRgnSize;
		TEmfRectL    oBounds;
	};

#define TEmfXForm TXForm
	//struct TEmfXForm
	//{
	//	double M11;
	//	double M12;
	//	double M21;
	//	double M22;
	//	double Dx;
	//	double Dy;

	//	void Init()
	//	{
	//		M11 = 1;
	//		M12 = 0;
	//		M21 = 0;
	//		M22 = 1;
	//		Dx  = 0;
	//		Dy  = 0;
	//	}

	//	void Copy(TEmfXForm* pOther)
	//	{
	//		M11 = pOther->M11;
	//		M12	= pOther->M12;
	//		M21	= pOther->M21;
	//		M22	= pOther->M22;
	//		Dx	= pOther->Dx;
	//		Dy	= pOther->Dy;
	//	}

	//	void Multiply(TEmfXForm &oOther, unsigned int ulMode)
	//	{
	//		if (MWT_IDENTITY == ulMode)
	//			Init();
	//		else if (MWT_LEFTMULTIPLY == ulMode)
	//		{
	//			// oOther слева, текущая матрица справа
	//			double dM11 = oOther.M11 * M11 + oOther.M12 * M21;
	//			double dM12 = oOther.M11 * M21 + oOther.M12 * M22;
	//			double dM21 = oOther.M21 * M11 + oOther.M22 * M21;
	//			double dM22 = oOther.M21 * M21 + oOther.M22 * M22;

	//			double dDx = oOther.Dx * M11 + oOther.Dy * M21 + Dx;
	//			double dDy = oOther.Dx * M21 + oOther.Dy * M22 + Dy;

	//			M11 = dM11;
	//			M12	= dM12;
	//			M21	= dM21;
	//			M22	= dM22;
	//			Dx	= dDx;
	//			Dy	= dDy;
	//		}
	//		else if (MWT_RIGHTMULTIPLY == ulMode)
	//		{
	//			// oOther справа, текущая матрица слева
	//			double dM11 = M11 * oOther.M11 + M12 * oOther.M21;
	//			double dM12 = M11 * oOther.M21 + M12 * oOther.M22;
	//			double dM21 = M21 * oOther.M11 + M22 * oOther.M21;
	//			double dM22 = M21 * oOther.M21 + M22 * oOther.M22;

	//			double dDx = Dx * oOther.M11 + Dy * oOther.M21 + oOther.Dx;
	//			double dDy = Dx * oOther.M21 + Dy * oOther.M22 + oOther.Dy;

	//			M11 = dM11;
	//			M12	= dM12;
	//			M21	= dM21;
	//			M22	= dM22;
	//			Dx	= dDx;
	//			Dy	= dDy;
	//		}
	//		else //if (MWT_SET == ulMode)
	//		{
	//			Copy(&oOther);
	//		}
	//	}
	//	void Apply(double& dX, double& dY)
	//	{
	//		double _dX = dX;
	//		double _dY = dY;

	//		dX = _dX * M11 + _dY * M21 + Dx;
	//		dY = _dX * M12 + _dY * M22 + Dy;
	//	}
	//};

	struct TEmfStretchBLT
	{
		TEmfRectL    Bounds;
		int          xDest;
		int          yDest;
		int          cxDest;
		int          cyDest;
		unsigned int BitBltRasterOperation;
		int          xSrc;
		int          ySrc;
		TXForm       XformSrc;
		TEmfColor    BkColorSrc;
		unsigned int UsageSrc;
		unsigned int offBmiSrc;
		unsigned int cbBmiSrc;
		unsigned int offBitsSrc;
		unsigned int cbBitsSrc;
		int          cxSrc;
		int          cySrc;
	};

	struct TEmfEmrText
	{
		TEmfPointL    Reference;
		unsigned int  Chars;
		unsigned int  offString;
		unsigned int  Options;
		TEmfRectL     Rectangle;
		unsigned int  offDx;
		void*         OutputString; // unsinged short* либо unsigned char*
		unsigned int* OutputDx;		

		TEmfEmrText()
		{
			OutputString = NULL;
			OutputDx     = NULL;
		}

		void FreeA()
		{
			if (OutputString)
			{
				unsigned char* pString = (unsigned char*)OutputString;
				delete[] pString;
				OutputString = NULL;
			}

			if (OutputDx)
			{
				delete[] OutputDx;
				OutputDx = NULL;
			}
		}

		void FreeW()
		{
			if (OutputString)
			{
				unsigned short* pString = (unsigned short*)OutputString;
				delete[] pString;
				OutputString = NULL;
			}

			if (OutputDx)
			{
				delete[] OutputDx;
				OutputDx = NULL;
			}
		}
	};

	struct TEmfExtTextoutW
	{
		TEmfRectL     Bounds;
		unsigned int  iGraphicsMode;
		double        exScale;
		double        eyScale;
		TEmfEmrText   wEmrText;

		~TEmfExtTextoutW()
		{
			Free();
		}

		void Free()
		{
			wEmrText.FreeW();
		}
	};

	struct TEmfExtTextoutA
	{
		TEmfRectL     Bounds;
		unsigned int  iGraphicsMode;
		double        exScale;
		double        eyScale;
		TEmfEmrText   aEmrText;

		~TEmfExtTextoutA()
		{
			Free();
		}

		void Free()
		{
			aEmrText.FreeA();
		}
	};

	struct TEmfPolyTextoutA
	{
		TEmfRectL    Bounds;
		unsigned int iGraphicsMode;
		double       exScale;
		double       eyScale;
		unsigned int cStrings;
		TEmfEmrText* aEmrText;

		TEmfPolyTextoutA()
		{
			aEmrText = NULL;
		}
		~TEmfPolyTextoutA()
		{
			if (aEmrText)
			{
				for (unsigned int unIndex = 0; unIndex < cStrings; unIndex++)
				{
					aEmrText[unIndex].FreeA();
				}
				delete[] aEmrText;
				aEmrText = NULL;
			}
		}
	};

	struct TEmfPolyTextoutW
	{
		TEmfRectL    Bounds;
		unsigned int iGraphicsMode;
		double       exScale;
		double       eyScale;
		unsigned int cStrings;
		TEmfEmrText* wEmrText;

		TEmfPolyTextoutW()
		{
			wEmrText = NULL;
		}
		~TEmfPolyTextoutW()
		{
			if (wEmrText)
			{
				for (unsigned int unIndex = 0; unIndex < cStrings; unIndex++)
				{
					wEmrText[unIndex].FreeW();
				}
				delete[] wEmrText;
				wEmrText = NULL;
			}
		}
	};

	struct TEmfLogFont
	{				   
		int            Height;
		int            Width;
		int            Escapement;
		int            Orientation;
		int            Weight;
		unsigned char  Italic;
		unsigned char  Underline;
		unsigned char  StrikeOut;
		unsigned char  CharSet;
		unsigned char  OutPrecision;
		unsigned char  ClipPrecision;
		unsigned char  Quality;
		unsigned char  PitchAndFamily;
		unsigned short FaceName[32];
	};

	struct TEmfLogFontEx
	{
		TEmfLogFont    LogFont;
		unsigned short FullName[64];
		unsigned short Style[32];
		unsigned short Script[32];
	};

	struct TEmfDesignVector
	{
		unsigned int Signature;
		unsigned int NumAxes;
		int*         Values;
	};

	struct TEmfBitBlt
	{
		TEmfRectL    Bounds;
		int          xDest;
		int          yDest;
		int          cxDest;
		int          cyDest;
		unsigned int BitBltRasterOperation;
		int          xSrc;
		int          ySrc;
		TEmfXForm    XfromSrc;
		TEmfColor    BkColorSrc;
		unsigned int UsageSrc;
		unsigned int offBmiSrc;
		unsigned int cbBmiSrc;
		unsigned int offBitsSrc;
		unsigned int cbBitsSrc;
	};

	struct TEmfSetDiBitsToDevice
	{
		TEmfRectL    Bounds;
		int          xDest;
		int          yDest;
		int          xSrc;
		int          ySrc;
		int          cxSrc;
		int          cySrc;
		unsigned int offBmiSrc;
		unsigned int cbBmiSrc;
		unsigned int offBitsSrc;
		unsigned int cbBitsSrc;
		unsigned int UsageSrc;
		unsigned int iStartScan;
		unsigned int cScans;
	};

	struct TEmfDibPatternBrush
	{
		unsigned int Usage;
		unsigned int offBmi;
		unsigned int cbBmi;
		unsigned int offBits;
		unsigned int cbBits;
	};

	struct TEmfLogPaletteEntry
	{
		unsigned char Reserved;
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
	};

	struct TEmfSmallTextout
	{
		int             x;
		int             y;
		unsigned int    cChars;
		unsigned int    fuOptions;
		unsigned int    iGraphicsMode;
		double          exScale;
		double          eyScale;
		TEmfRectL       Bounds;
		unsigned short* TextString;

		TEmfSmallTextout()
		{
			TextString = NULL;
		}
		~TEmfSmallTextout()
		{
			if (TextString)
				delete[] TextString;
		}

		unsigned int GetSize()
		{
			unsigned int unSize = 28;

			if (!(fuOptions & ETO_NO_RECT))
				unSize += 16;

			if (fuOptions & ETO_SMALL_CHARS)
				unSize += cChars;
			else
				unSize += 2 * cChars;

			return unSize;
		}
	};

	struct TEmfAlphaBlend
	{
		TEmfRectL     Bounds;
		int           xDest;
		int           yDest;
		int           cxDest;
		int           cyDest;
		unsigned char BlendOperation;
		unsigned char BlendFlags;
		unsigned char SrcConstantAlpha;
		unsigned char AlphaFormat;
		int           xSrc;
		int           ySrc;
		TEmfXForm     XformSrc;
		TEmfColor     BkColor;
		unsigned int  UsageSrc;
		unsigned int  offBmiSrc;
		unsigned int  cbBmiSrc;
		unsigned int  offBitsSrc;
		unsigned int  cbBitsSrc;
		int           cxSrc;
		int           cySrc;
	};

	const unsigned int c_nTEmfAlphaBlendSize = 100;
};
#endif //_METAFILE_EMF_EMFTYPES_H
