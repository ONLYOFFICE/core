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

	struct TEmfHeader
	{
		TEmfHeader() : ulSignature(0), ulVersion(0), ulRecords(0), ushObjects(0), ulSize(0), ulPalEntries(0), ulOffsetDescription(0),ulSizeDescription(0) {}
		
		TRectL         oBounds;
		TRectL         oFrame;
		unsigned int   ulSignature;
		unsigned int   ulVersion;
		unsigned int   ulSize;
		unsigned int   ulRecords;
		unsigned short ushObjects;
		unsigned short ushReserved;
		unsigned int   ulSizeDescription;
		unsigned int   ulOffsetDescription;
		unsigned int   ulPalEntries;
		TSizeL         oDevice;
		TSizeL         oMillimeters;
		TRectL         oFrameToBounds;
		TRectL         oFramePx;
	};

	struct TEmfStretchDIBITS
	{
		TRectL       oBounds;
		int          nXDest;
		int          nYDest;
		int          nXSrc;
		int          nYSrc;
		int          nCxSrc;
		int          nCySrc;
		unsigned int unOffBmiSrc;
		unsigned int unCbBmiSrc;
		unsigned int unOffBitsSrc;
		unsigned int unCbBitsSrc;
		unsigned int unUsageSrc;
		unsigned int unBitBltRasterOperation;
		int          nCxDest;
		int          nCyDest;
	};

	struct TRegionDataHeader
	{
		unsigned int unSize;
		unsigned int unType;
		unsigned int unCountRects;
		unsigned int unRgnSize;
		TRectL       oBounds;
	};

	#define TEmfXForm TXForm

	struct TEmfStretchBLT
	{
		TRectL       oBounds;
		int          nXDest;
		int          nYDest;
		int          nCxDest;
		int          nCyDest;
		unsigned int unBitBltRasterOperation;
		int          nXSrc;
		int          nYSrc;
		TXForm       oXformSrc;
		TRGBA        oBkColorSrc;
		unsigned int unUsageSrc;
		unsigned int unOffBmiSrc;
		unsigned int unCbBmiSrc;
		unsigned int unOffBitsSrc;
		unsigned int unCbBitsSrc;
		int          nCxSrc;
		int          nCySrc;
	};

	template<typename T>
	struct TEmrText
	{
		TPointL       oReference;
		unsigned int  unChars;
		unsigned int  unOffString;
		unsigned int  unOptions;
		TRectL        oRectangle;
		unsigned int  unOffDx;
		T*            pOutputString; // unsinged short* либо unsigned char*
		unsigned int* pOutputDx;
		
		TEmrText() : pOutputString(NULL), pOutputDx(NULL){}
		void Free()
		{
			if (pOutputString)
			{
				T* pString = (T*)pOutputString;
				delete[] pString;
				pOutputString = NULL;
			}

			RELEASEOBJECT(pOutputDx);
		}
	};
	
	typedef TEmrText<unsigned char>  TEmrTextA;
	typedef TEmrText<unsigned short> TEmrTextW;
	
	template<typename T>
	struct TExtTextout
	{
		TRectL        oBounds;
		unsigned int  unIGraphicsMode;
		double        dExScale;
		double        dEyScale;
		TEmrText<T>   oEmrText;

		~TExtTextout()
		{
			Free();
		}

		void Free()
		{
			oEmrText.Free();
		}
	};
	
	typedef TExtTextout<unsigned char>  TEmfExtTextoutA;
	typedef TExtTextout<unsigned short> TEmfExtTextoutW;

	template<typename T>
	struct TPolyTextout
	{
		TRectL       oBounds;
		unsigned int unIGraphicsMode;
		double       dExScale;
		double       dEyScale;
		unsigned int unCStrings;
		TEmrText<T>* arEmrText;

		TPolyTextout() : arEmrText(NULL)
		{}

		~TPolyTextout()
		{
			if (NULL != arEmrText)
			{
				for (unsigned int unIndex = 0; unIndex < unCStrings; unIndex++)
					arEmrText[unIndex].Free();

				RELEASEARRAYOBJECTS(arEmrText);
			}
		}
	};
	
	typedef TPolyTextout<unsigned char>  TPolyTextoutA;
	typedef TPolyTextout<unsigned short> TPolyTextoutW;

	struct TEmfLogFont
	{
		int            nHeight;
		int            nWidth;
		int            nEscapement;
		int            nOrientation;
		int            nWeight;
		unsigned char  uchItalic;
		unsigned char  uchUnderline;
		unsigned char  uchStrikeOut;
		unsigned char  uchCharSet;
		unsigned char  uchOutPrecision;
		unsigned char  uchClipPrecision;
		unsigned char  uchQuality;
		unsigned char  uchPitchAndFamily;
		unsigned short ushFaceName[32];
	};

	struct TEmfLogFontEx
	{
		TEmfLogFont    oLogFont;
		unsigned short ushFullName[64];
		unsigned short ushStyle[32];
		unsigned short ushScript[32];
	};

	struct TEmfDesignVector
	{
		unsigned int unSignature;
		unsigned int unNumAxes;
		int*         pValues;
	};

	struct TEmfBitBlt
	{
		TRectL       oBounds;
		int          nXDest;
		int          nYDest;
		int          nCxDest;
		int          nCyDest;
		unsigned int unBitBltRasterOperation;
		int          nXSrc;
		int          nYSrc;
		TEmfXForm    oXfromSrc;
		TRGBA        oBkColorSrc;
		unsigned int unUsageSrc;
		unsigned int unOffBmiSrc;
		unsigned int unCbBmiSrc;
		unsigned int unOffBitsSrc;
		unsigned int unCbBitsSrc;
	};

	struct TEmfSetDiBitsToDevice
	{
		TRectL       oBounds;
		int          nXDest;
		int          nYDest;
		int          nXSrc;
		int          nYSrc;
		int          nCxSrc;
		int          nCySrc;
		unsigned int unOffBmiSrc;
		unsigned int unCbBmiSrc;
		unsigned int unOffBitsSrc;
		unsigned int unCbBitsSrc;
		unsigned int unUsageSrc;
		unsigned int unIStartScan;
		unsigned int unCScans;
	};

	struct TEmfDibPatternBrush
	{
		unsigned int unUsage;
		unsigned int unOffBmi;
		unsigned int unCbBmi;
		unsigned int unOffBits;
		unsigned int unCbBits;
	};

	struct TEmfLogPaletteEntry
	{
		unsigned char uchReserved;
		unsigned char uchBlue;
		unsigned char uchGreen;
		unsigned char uchRed;
	};

	struct TEmfSmallTextout
	{
		int             nX;
		int             nY;
		unsigned int    unCChars;
		unsigned int    unFuOptions;
		unsigned int    unIGraphicsMode;
		double          dExScale;
		double          dEyScale;
		TRectL          oBounds;
		unsigned short* pTextString;

		TEmfSmallTextout() : pTextString(NULL)
		{}
		
		~TEmfSmallTextout()
		{
			RELEASEOBJECT(pTextString);
		}

		unsigned int GetSize()
		{
			unsigned int unSize = 28;

			if (!(unFuOptions & ETO_NO_RECT))
				unSize += 16;

			if (unFuOptions & ETO_SMALL_CHARS)
				unSize += unCChars;
			else
				unSize += 2 * unCChars;

			return unSize;
		}
	};

	struct TEmfAlphaBlend
	{
		TRectL        oBounds;
		int           nXDest;
		int           nYDest;
		int           nCxDest;
		int           nCyDest;
		unsigned char uchBlendOperation;
		unsigned char uchBlendFlags;
		unsigned char uchSrcConstantAlpha;
		unsigned char uchAlphaFormat;
		int           nXSrc;
		int           nYSrc;
		TEmfXForm     oXformSrc;
		TRGBA         oBkColor;
		unsigned int  unUsageSrc;
		unsigned int  unOffBmiSrc;
		unsigned int  unCbBmiSrc;
		unsigned int  unOffBitsSrc;
		unsigned int  unCbBitsSrc;
		int           nCxSrc;
		int           nCySrc;
	};

	struct TTriVertex
	{
		int nX;
		int nY;
		unsigned short ushRed;
		unsigned short ushGreen;
		unsigned short ushBlue;
		unsigned short ushAlpha;
	};

	const unsigned int c_nTEmfAlphaBlendSize = 100;
};
#endif //_METAFILE_EMF_EMFTYPES_H
