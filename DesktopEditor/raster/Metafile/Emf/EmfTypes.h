#ifndef _EMF_TYPES_H
#define _EMF_TYPES_H

#if !defined(_WIN32) && !defined(_WIN64)
//from wingdi.h

#ifndef _MAC
    #define ENHMETA_SIGNATURE       0x464D4520
#else
    #define ENHMETA_SIGNATURE       0x20454D46
#endif

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
			ulW = 1024;
			ulH = 1024;
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
		int lLeft;
		int lTop;
		int lRight;
		int lBottom;
	};

	struct TEmfPointL
	{
		int x;
		int y;

		TEmfPointL& operator=(TEmfPointL& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;

			return *this;
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
		unsigned int cx;
		unsigned int cy;
	};

	struct TEmfHeader
	{
		TEmfRectL      oBounds;
		TEmfRectL      oFrame;
		unsigned int  ulSignature;
		unsigned int  ulVersion;
		unsigned int  ulSize;
		unsigned int  ulRecords;
		unsigned short ushObjects;
		unsigned short ushReserved;
		unsigned int  ulSizeDescription;
		unsigned int  ulOffsetDescription;
		unsigned int  ulPalEntries;
		TEmfSizeL      oDevice;
		TEmfSizeL      oMillimeters;
		TEmfRectL      oFrameToBounds;
	};

	struct TEmfStretchDIBITS
	{
		TEmfRectL     Bounds;
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

#define MWT_IDENTITY	  0x01
#define MWT_LEFTMULTIPLY  0x02
#define MWT_RIGHTMULTIPLY 0x03
#define MWT_SET           0x04

	struct TEmfXForm
	{
		double M11;
		double M12;
		double M21;
		double M22;
		double Dx;
		double Dy;

		void Init()
		{
			M11 = 1;
			M12 = 0;
			M21 = 0;
			M22 = 1;
			Dx  = 0;
			Dy  = 0;
		}

		void Copy(TEmfXForm* pOther)
		{
			M11 = pOther->M11;
			M12	= pOther->M12;
			M21	= pOther->M21;
			M22	= pOther->M22;
			Dx	= pOther->Dx;
			Dy	= pOther->Dy;
		}

		void Multiply(TEmfXForm &oOther, unsigned int ulMode)
		{
			if (MWT_IDENTITY == ulMode)
				Init();
			else if (MWT_LEFTMULTIPLY == ulMode)
			{
				// oOther слева, текущая матрица справа
				double dM11 = oOther.M11 * M11 + oOther.M12 * M21;
				double dM12 = oOther.M11 * M21 + oOther.M12 * M22;
				double dM21 = oOther.M21 * M11 + oOther.M22 * M21;
				double dM22 = oOther.M21 * M21 + oOther.M22 * M22;

				double dDx = oOther.Dx * M11 + oOther.Dy * M21 + Dx;
				double dDy = oOther.Dx * M21 + oOther.Dy * M22 + Dy;

				M11 = dM11;
				M12	= dM12;
				M21	= dM21;
				M22	= dM22;
				Dx	= dDx;
				Dy	= dDy;
			}
			else if (MWT_RIGHTMULTIPLY == ulMode)
			{
				// oOther справа, текущая матрица слева
				double dM11 = M11 * oOther.M11 + M12 * oOther.M21;
				double dM12 = M11 * oOther.M21 + M12 * oOther.M22;
				double dM21 = M21 * oOther.M11 + M22 * oOther.M21;
				double dM22 = M21 * oOther.M21 + M22 * oOther.M22;

				double dDx = Dx * oOther.M11 + Dy * oOther.M21 + oOther.Dx;
				double dDy = Dx * oOther.M21 + Dy * oOther.M22 + oOther.Dy;

				M11 = dM11;
				M12	= dM12;
				M21	= dM21;
				M22	= dM22;
				Dx	= dDx;
				Dy	= dDy;
			}
			else //if (MWT_SET == ulMode)
			{
				Copy(&oOther);
			}
		}
		void Apply(double& dX, double& dY)
		{
			double _dX = dX;
			double _dY = dY;

			dX = _dX * M11 + _dY * M21 + Dx;
			dY = _dX * M12 + _dY * M22 + Dy;
		}
	};

	struct TEmfEmrText
	{
		TEmfPointL     Reference;
		unsigned int  Chars;
		unsigned int  offString;
		unsigned int  Options;
		TEmfRectL      Rectangle;
		unsigned int  offDx;
		void*          OutputString; // unsinged short* либо unsigned char*
		unsigned int* OutputDx;
	};

	struct TEmfExtTextoutW
	{
		TEmfRectL     Bounds;
		unsigned int iGraphicsMode;
		double        exScale;
		double        eyScale;
		TEmfEmrText   wEmrText;
	};

	struct TEmfLogFont
	{
		int           Height;
		int           Width;
		int           Escapement;
		int           Orientation;
		int           Weight;
		unsigned char  Italic;
		unsigned char  Underline;
		unsigned char  StrikOut;
		unsigned char  CharSet;
		unsigned char  OutPrecision;
		unsigned char  ClipPrecision;
		unsigned char  Quality;
		unsigned char  PitchAndFamily;
		unsigned short FaceName[32];
	};

	struct TEmfLogFontEx
	{
		TEmfLogFont   LogFont;
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
		TEmfRectL     Bounds;
		int          xDest;
		int          yDest;
		int          cxDest;
		int          cyDest;
		unsigned int BitBltRasterOperation;
		int          xSrc;
		int          ySrc;
		TEmfXForm     XfromSrc;
		TEmfColor     BkColorSrc;
		unsigned int UsageSrc;
		unsigned int offBmiSrc;
		unsigned int cbBmiSrc;
		unsigned int offBitsSrc;
		unsigned int cbBitsSrc;
	};

	struct TEmfSetDiBitsToDevice
	{
		TEmfRectL     Bounds;
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
};
#endif //_EMF_TYPES_H
