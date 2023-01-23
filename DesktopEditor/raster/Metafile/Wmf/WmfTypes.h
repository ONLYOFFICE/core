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
#ifndef _METAFILE_WMF_WMFTYPES_H
#define _METAFILE_WMF_WMFTYPES_H

/* Metafile Functions */
#define META_SETBKCOLOR              0x0201
#define META_SETBKMODE               0x0102
#define META_SETMAPMODE              0x0103
#define META_SETROP2                 0x0104
#define META_SETRELABS               0x0105
#define META_SETPOLYFILLMODE         0x0106
#define META_SETSTRETCHBLTMODE       0x0107
#define META_SETTEXTCHAREXTRA        0x0108
#define META_SETTEXTCOLOR            0x0209
#define META_SETTEXTJUSTIFICATION    0x020A
#define META_SETWINDOWORG            0x020B
#define META_SETWINDOWEXT            0x020C
#define META_SETVIEWPORTORG          0x020D
#define META_SETVIEWPORTEXT          0x020E
#define META_OFFSETWINDOWORG         0x020F
#define META_SCALEWINDOWEXT          0x0410
#define META_OFFSETVIEWPORTORG       0x0211
#define META_SCALEVIEWPORTEXT        0x0412
#define META_LINETO                  0x0213
#define META_MOVETO                  0x0214
#define META_EXCLUDECLIPRECT         0x0415
#define META_INTERSECTCLIPRECT       0x0416
#define META_ARC                     0x0817
#define META_ELLIPSE                 0x0418
#define META_FLOODFILL               0x0419
#define META_PIE                     0x081A
#define META_RECTANGLE               0x041B
#define META_ROUNDRECT               0x061C
#define META_PATBLT                  0x061D
#define META_SAVEDC                  0x001E
#define META_SETPIXEL                0x041F
#define META_OFFSETCLIPRGN           0x0220
#define META_TEXTOUT                 0x0521
#define META_BITBLT                  0x0922
#define META_STRETCHBLT              0x0B23
#define META_POLYGON                 0x0324
#define META_POLYLINE                0x0325
#define META_ESCAPE                  0x0626
#define META_RESTOREDC               0x0127
#define META_FILLREGION              0x0228
#define META_FRAMEREGION             0x0429
#define META_INVERTREGION            0x012A
#define META_PAINTREGION             0x012B
#define META_SELECTCLIPREGION        0x012C
#define META_SELECTOBJECT            0x012D
#define META_SETTEXTALIGN            0x012E
#define META_CHORD                   0x0830
#define META_SETMAPPERFLAGS          0x0231
#define META_EXTTEXTOUT              0x0a32
#define META_SETDIBTODEV             0x0d33
#define META_SELECTPALETTE           0x0234
#define META_REALIZEPALETTE          0x0035
#define META_ANIMATEPALETTE          0x0436
#define META_SETPALENTRIES           0x0037
#define META_POLYPOLYGON             0x0538
#define META_RESIZEPALETTE           0x0139
#define META_DIBBITBLT               0x0940
#define META_DIBSTRETCHBLT           0x0b41
#define META_DIBCREATEPATTERNBRUSH   0x0142
#define META_STRETCHDIB              0x0f43
#define META_EXTFLOODFILL            0x0548
#define META_SETLAYOUT               0x0149
#define META_DELETEOBJECT            0x01f0
#define META_CREATEPALETTE           0x00f7
#define META_CREATEPATTERNBRUSH      0x01F9
#define META_CREATEPENINDIRECT       0x02FA
#define META_CREATEFONTINDIRECT      0x02FB
#define META_CREATEBRUSHINDIRECT     0x02FC
#define META_CREATEREGION            0x06FF

namespace MetaFile
{
#define META_EOF 0x0000

	typedef enum
	{
		WMF_NEWFRAME = 0x0001,
		WMF_ABORTDOC = 0x0002,
		WMF_NEXTBAND = 0x0003,
		WMF_SETCOLORTABLE = 0x0004,
		WMF_GETCOLORTABLE = 0x0005,
		WMF_FLUSHOUT = 0x0006,
		WMF_DRAFTMODE = 0x0007,
		WMF_QUERYESCSUPPORT = 0x0008,
		WMF_SETABORTPROC = 0x0009,
		WMF_STARTDOC = 0x000A,
		WMF_ENDDOC = 0x000B,
		WMF_GETPHYSPAGESIZE = 0x000C,
		WMF_GETPRINTINGOFFSET = 0x000D,
		WMF_GETSCALINGFACTOR = 0x000E,
		WMF_META_ESCAPE_ENHANCED_METAFILE = 0x000F,
		WMF_SETPENWIDTH = 0x0010,
		WMF_SETCOPYCOUNT = 0x0011,
		WMF_SETPAPERSOURCE = 0x0012,
		WMF_PASSTHROUGH = 0x0013,
		WMF_GETTECHNOLOGY = 0x0014,
		WMF_SETLINECAP = 0x0015,
		WMF_SETLINEJOIN = 0x0016,
		WMF_SETMITERLIMIT = 0x0017,
		WMF_BANDINFO = 0x0018,
		WMF_DRAWPATTERNRECT = 0x0019,
		WMF_GETVECTORPENSIZE = 0x001A,
		WMF_GETVECTORBRUSHSIZE = 0x001B,
		WMF_ENABLEDUPLEX = 0x001C,
		WMF_GETSETPAPERBINS = 0x001D,
		WMF_GETSETPRINTORIENT = 0x001E,
		WMF_ENUMPAPERBINS = 0x001F,
		WMF_SETDIBSCALING = 0x0020,
		WMF_EPSPRINTING = 0x0021,
		WMF_ENUMPAPERMETRICS = 0x0022,
		WMF_GETSETPAPERMETRICS = 0x0023,
		WMF_POSTSCRIPT_DATA = 0x0025,
		WMF_POSTSCRIPT_IGNORE = 0x0026,
		WMF_GETDEVICEUNITS = 0x002A,
		WMF_GETEXTENDEDTEXTMETRICS = 0x0100,
		WMF_GETPAIRKERNTABLE = 0x0102,
		WMF_EXTTEXTOUT = 0x0200,
		WMF_GETFACENAME = 0x0201,
		WMF_DOWNLOADFACE = 0x0202,
		WMF_METAFILE_DRIVER = 0x0801,
		WMF_QUERYDIBSUPPORT = 0x0C01,
		WMF_BEGIN_PATH = 0x1000,
		WMF_CLIP_TO_PATH = 0x1001,
		WMF_END_PATH = 0x1002,
		WMF_OPENCHANNEL = 0x100E,
		WMF_DOWNLOADHEADER = 0x100F,
		WMF_CLOSECHANNEL = 0x1010,
		WMF_POSTSCRIPT_PASSTHROUGH = 0x1013,
		WMF_ENCAPSULATED_POSTSCRIPT = 0x1014,
		WMF_POSTSCRIPT_IDENTIFY = 0x1015,
		WMF_vPOSTSCRIPT_INJECTION = 0x1016,
		WMF_CHECKJPEGFORMAT = 0x1017,
		WMF_CHECKPNGFORMAT = 0x1018,
		WMF_GET_PS_FEATURESETTING = 0x1019,
		WMF_MXDC_ESCAPE = 0x101A,
		WMF_SPCLPASSTHROUGH2 = 0x11D8
	} MetafileEscapes;

	struct TWmfColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a; //Reserved Must be 0x00

		TWmfColor()
		{
			r = 0;
			g = 0;
			b = 0;
		}

		TWmfColor(unsigned char _r, unsigned char _g, unsigned char _b)
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

		void Copy(TWmfColor& oOther)
		{
			r = oOther.r;
			g = oOther.g;
			b = oOther.b;
			a = oOther.a;
		}

		TWmfColor& operator=(TWmfColor& oColor)
		{
			r = oColor.r;
			g = oColor.g;
			b = oColor.b;
			a = oColor.a;
			return *this;
		}
	};
	struct TWmfPaletteEntry
	{
		unsigned char Values;
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
	};
	struct TWmfPointS
	{
		short x;
		short y;

		void Set(short _x, short _y)
		{
			x = _x;
			y = _y;
		}
	};
	struct TWmfRect
	{
		short Left;
		short Top;
		short Right;
		short Bottom;

		TWmfRect()
		{
			Left = Top = Right = Bottom = 0;
		}

		bool Empty() const
		{
			return (0 == Left) && (0 == Top) && (0 == Right) && (0 == Bottom);
		}
	};
	struct TWmfPlaceable
	{
		unsigned int   Key;
		unsigned short HWmf;
		TWmfRect       BoundingBox;
		unsigned short Inch;
		unsigned int   Reserved;
		unsigned short Checksum;
	};
	struct TWmfHeader
	{
		unsigned short Type;
		unsigned short HeaderSize;
		unsigned short Version;
		unsigned int   Size;
		unsigned short NumberOfObjects;
		unsigned int   MaxRecord;
		unsigned short NumberOfMembers;
	};
	struct TWmfScanLine
	{
		unsigned short Left;
		unsigned short Right;
	};
	struct TWmfScanObject
	{
		unsigned short  Count;
		unsigned short  Top;
		unsigned short  Bottom;
		TWmfScanLine*   ScanLines;
		unsigned short  Count2;
	};
	struct TWmfWindow
	{
		short x;
		short y;
		short w;
		short h;

		bool bUnchangedExt = true;
		bool bUnchangedOrg = true;

		void Init()
		{
			x = 0;
			y = 0;
			w = 1;
			h = 1;
		}

		void Copy(TWmfWindow& oOther)
		{
			x = oOther.x;
			y = oOther.y;
			w = oOther.w;
			h = oOther.h;

			bUnchangedExt = oOther.bUnchangedExt;
			bUnchangedOrg = oOther.bUnchangedOrg;
		}
	};
	struct TWmfLogBrush
	{
		unsigned short BrushStyle;
		TWmfColor      Color;
		unsigned short BurshHatch;
	};
	struct TWmfBitmap16
	{
		short          Type;
		short          Width;
		short          Height;
		short          WidthBytes;
		unsigned char  Planes;
		unsigned char  BitsPixel;
		unsigned char* Bits;
	};
	struct TWmfBitBlt
	{
		unsigned int RasterOperation;
		short        YSrc;
		short        XSrc;
		short        Height;
		short        Width;
		short        YDest;
		short        XDest;
	};
	struct TWmfSetDibToDev
	{
		unsigned short ColorUsage;
		unsigned short ScanCount;
		unsigned short StartScan;
		unsigned short yDib;
		unsigned short xDib;
		unsigned short Height;
		unsigned short Width;
		unsigned short yDest;
		unsigned short xDest;
	};
	struct TWmfStretchBlt
	{
		unsigned int RasterOperation;
		short        SrcHeight;
		short        SrcWidth;
		short        YSrc;
		short        XSrc;
		short        DestHeight;
		short        DestWidth;
		short        YDest;
		short        XDest;
	};
	struct TWmfStretchDib
	{
		unsigned int   RasterOperation;
		unsigned short ColorUsage;
		short          SrcHeight;
		short          SrcWidth;
		short          YSrc;
		short          XSrc;
		short          DestHeight;
		short          DestWidth;
		short          yDst;
		short          xDst;
	};
	class CWmfEscapeBuffer
	{
		public:
			CWmfEscapeBuffer() : m_pBytes(NULL), m_unSize(0), m_unPosition(0) {};
			~CWmfEscapeBuffer()
			{
				Clear();
			}

			bool Empty() const
			{
				return 0 == m_unSize;
			}

			void SetSize(unsigned int unSize)
			{
				Clear();
				m_unSize = unSize;
				m_pBytes = new unsigned char[m_unSize];
			};

			void IncreasePosition(unsigned int unValue)
			{
				m_unPosition += unValue;

				if (m_unPosition > m_unSize)
					m_unPosition = m_unSize;
			}

			unsigned char* GetBuffer() const
			{
				return m_pBytes;
			}

			unsigned char* GetCurPtr() const
			{
				if (NULL == m_pBytes || 0 == m_unSize)
					return NULL;

				return m_pBytes + m_unPosition;
			}

			unsigned int GetSize() const
			{
				return m_unSize;
			}

		private:

			void Clear()
			{
				if (NULL != m_pBytes)
				{
					delete m_pBytes;
					m_pBytes = NULL;
				}

				m_unSize = 0;
				m_unPosition = 0;
			}

			unsigned char* m_pBytes;
			unsigned int   m_unSize;
			unsigned int   m_unPosition;
	};
}

#endif // _METAFILE_WMF_WMFTYPES_H
