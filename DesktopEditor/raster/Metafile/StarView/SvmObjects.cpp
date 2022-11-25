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

#include "SvmObjects.h"
#include "../../../common/StringExt.h"
#include "../../../../UnicodeConverter/UnicodeConverter.h"

static const char* CodingCharsets[95] =
{
	"", 				// 	0
	"windows-1252",		// 	1	MS_1252
	"", 				// 	2	APPLE_ROMAN
	"IBM437", 			// 	3	IBM_437
	"IBM850", 			// 	4	IBM_850
	"IBM860",			// 	5	IBM_860
	"IBM861", 			// 	6	IBM_861
	"IBM863", 			// 	7	IBM_863
	"IBM865",			// 	8	IBM_865
	"", 				// 	9	Reserved
	"", 				// 	10	SYMBOL
	"", 				// 	11	ASCII_US
	"ISO-8859-1", 		// 	12	ISO_8859_1
	"ISO-8859-2", 		// 	13	ISO_8859_2
	"ISO-8859-3", 		// 	14	ISO_8859_3
	"ISO-8859-4", 		// 	15	ISO_8859_4
	"ISO-8859-5", 		// 	16	ISO_8859_5
	"ISO-8859-6", 		// 	17	ISO_8859_6
	"ISO-8859-7", 		// 	18	ISO_8859_7
	"ISO-8859-8", 		// 	19	ISO_8859_8
	"ISO-8859-9", 		// 	20	ISO_8859_9
	"ISO-8859-14", 		// 	21	ISO_8859_14
	"ISO-8859-15", 		// 	22	ISO_8859_15
	"IBM737", 			// 	23	IBM_737
	"IBM775", 			// 	24	IBM_775
	"IBM852", 			// 	25	IBM_852
	"IBM855", 			// 	26	IBM_855
	"IBM857", 			// 	27	IBM_857
	"IBM862", 			// 	28	IBM_862
	"IBM864", 			// 	29	IBM_864
	"IBM866", 			// 	30	IBM_866
	"IBM869", 			// 	31	IBM_869
	"windows-874", 		// 	32	MS_874
	"windows-1250", 	// 	33	MS_1250
	"windows-1251", 	// 	34	MS_1251
	"windows-1253", 	// 	35	MS_1253
	"windows-1254", 	// 	36	MS_1254
	"windows-1255", 	// 	37	MS_1255
	"windows-1256", 	// 	38	MS_1256
	"windows-1257", 	// 	39	MS_1257
	"windows-1258", 	// 	40	MS_1258
	"", 				// 	41	APPLE_ARABIC
	"", 				// 	42	APPLE_CENTEURO
	"", 				// 	43	APPLE_CROATIAN
	"x-mac-cyrillic", 	// 	44	APPLE_CYRILLIC
	"", 				// 	45	APPLE_DEVANAGARI
	"", 				// 	46	APPLE_FARSI
	"", 				// 	47	APPLE_GREEK
	"", 				// 	48	APPLE_GUJARATI
	"", 				// 	49	APPLE_GURMUKHI
	"", 				// 	50	APPLE_HEBREW
	"", 				// 	51	APPLE_ICELAND
	"", 				// 	52	APPLE_ROMANIAN
	"", 				// 	53	APPLE_THAI
	"", 				// 	54	APPLE_TURKISH
	"", 				// 	55	APPLE_UKRAINIAN
	"", 				// 	56	APPLE_CHINSIMP
	"", 				// 	57	APPLE_CHINTRAD
	"", 				// 	58	APPLE_JAPANESE
	"", 				// 	59	APPLE_KOREAN
	"windows-932", 		// 	60	MS_932
	"windows-936", 		// 	61	MS_936
	"windows-949", 		// 	62	MS_949
	"windows-950", 		// 	63	MS_950
	"", 				// 	64	SHIFT_JIS
	"GB2312", 			// 	65	GB_2312
	"", 				// 	66	GBT_12345
	"", 				// 	67	GBK
	"Big5", 			// 	68	BIG5
	"", 				// 	69	EUC_JP
	"", 				// 	70	EUC_CN
	"", 				// 	71	EUC_TW
	"", 				// 	72	ISO_2022_JP
	"", 				// 	73	ISO_2022_CN
	"", 				// 	74	KOI8_R
	"UTF-7", 			// 	75	UTF7
	"UTF-8", 			// 	76	UTF8
	"ISO-8859-10", 	// 	77	ISO_8859_10
	"ISO-8859-13", 	// 	78	ISO_8859_13
	"EUC-KR",			// 	79	EUC_KR
	"", 				// 	80	ISO_2022_KR
	"", 				// 	81	JIS_X_0201
	"", 				// 	82	JIS_X_0208
	"", 				// 	83	JIS_X_0212
	"windows-1361", 	// 	84	MS_1361
	"", 				// 	85	GB_18030
	"", 				// 	86	BIG5_HKSCS
	"", 				// 	87	TIS_620
	"KOI8-U", 			// 	88	KOI8_U
	"", 				// 	89	ISCII_DEVANAGARI
	"", 				// 	90	JAVA_UTF8
	"", 				// 	91	ADOBE_STANDARD
	"", 				// 	92	ADOBE_SYMBOL
	"", 				// 	93	PT154
	"" 					// 	94	ADOBE_DINGBATS
};
namespace MetaFile
{
	int parseString(CDataStream &stream, std::wstring &string, unsigned short version, unsigned short charset)
	{
		int nRead = 0;
		if (charset == 0xffff)//RTL_UNICODE
		{
			unsigned int length;
			stream >> length;
			
			string = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)stream.GetCurPtr(), length);
	
			stream.Skip(length * 2);
			nRead = length * 2 + 4;
		}
		else
		{
			unsigned short  length;
			stream >> length;
		
			if (charset < 1)
			{
				std::string ansiString = std::string((char*)stream.GetCurPtr(),length);
				string = std::wstring(ansiString.begin(), ansiString.end());
			}
			else
			{
				std::string coding_name;
				if (charset < 95) 
					coding_name = std::string(CodingCharsets[charset]);

				if (!coding_name.empty())
				{
					NSUnicodeConverter::CUnicodeConverter converter;
					string = converter.toUnicode((char*)stream.GetCurPtr(), (unsigned int)length, coding_name.c_str());
				}
				else
				{
					string = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)stream.GetCurPtr(), length,
																			(NSStringExt::CConverter::ESingleByteEncoding)charset); //не все
				}
			}
			stream.Skip(length);
			
			nRead = length + 2;
		}
		return nRead;
	}

VersionCompat::VersionCompat()
    : version(0)
    , length(0)
{
}

VersionCompat::VersionCompat(CDataStream &stream)
{
    stream >> version;
    stream >> length;
}

Fraction::Fraction()
    : numerator(1)
    , denominator(1)
{
}

Fraction::Fraction(CDataStream &stream)
{
    stream >> numerator;
    stream >> denominator;
}

TSvmMapMode::TSvmMapMode()
    : version()
    , unit(0)
    , scaleX()
    , scaleY()
    , isSimple(false)
{
}

TSvmMapMode::TSvmMapMode(CDataStream &stream)
{
	stream >> *this;
}

CDataStream& operator>>(CDataStream &stream, VersionCompat &compat)
{
	stream >> compat.version;
	stream >> compat.length;

	return stream;
}
CDataStream& operator>>(CDataStream &stream, Fraction &fract)
{
	stream >> fract.numerator;
	stream >> fract.denominator;

	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmMapMode &mm)
{
    stream >> mm.version;
    stream >> mm.unit;
    stream >> mm.origin;
	stream >> mm.scaleX;
    stream >> mm.scaleY;
    stream >> mm.isSimple;         // FIXME: how many bytes?

    return stream;
}

SvmHeader::SvmHeader()
    : versionCompat()
    , compressionMode()
    , mapMode()
	, boundRect()
    , actionCount(0)
{
}

SvmHeader::SvmHeader(CDataStream &stream)
{
	stream >> *this;
}

CDataStream& operator>>(CDataStream &stream, SvmHeader &header)
{
	stream >> header.versionCompat;
	stream >> header.compressionMode;
	stream >> header.mapMode;

	unsigned int height, width;
	stream >> width;
	stream >> height;

	header.boundRect.nBottom	= height;
	header.boundRect.nRight		= width;

	stream >> header.actionCount;

	if (header.versionCompat.version > 1)
		stream.Skip(1);

	return stream;
}

TSvmPoint::TSvmPoint(CDataStream &stream)
{
	stream >> *this;
}

TSvmRect::TSvmRect()
{
	l = t = r = b = 0;
}
CDataStream& operator>>(CDataStream &stream, TSvmSize &s)
{
	stream >> s.cx;
	stream >> s.cy;

	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmPoint &p)
{
	stream >> p.x;
	stream >> p.y;

	return stream;
}

CSvmBrush::CSvmBrush() : Color(255, 255, 255), Color2(255, 255, 255)
{
	BrushStyle = BS_SOLID;
	BrushHatch = HS_HORIZONTAL;
}
CSvmBrush::CSvmBrush(CSvmBrush& oBrush)
{
	BrushStyle = oBrush.BrushStyle;
	Color = oBrush.Color;
	Color2 = oBrush.Color2;
	BrushHatch = oBrush.BrushHatch;
	BrushStyleEx = oBrush.BrushStyleEx;
	BrushBounds = oBrush.BrushBounds;
}

int CSvmBrush::GetColor()
{
	return METAFILE_RGBA(Color.r, Color.g, Color.b);
}
int CSvmBrush::GetColor2()
{
	return METAFILE_RGBA(Color2.r, Color2.g, Color2.b);
}
unsigned int CSvmBrush::GetStyleEx()
{
	return BrushStyleEx;
}
unsigned int CSvmBrush::GetStyle()
{
	return BrushStyle;
}
unsigned int CSvmBrush::GetHatch()
{
	return BrushHatch;
}
unsigned int CSvmBrush::GetAlpha()
{
	return 0xff-Color.a;
}
unsigned int CSvmBrush::GetAlpha2()
{
	return 0xff-Color2.a;
}
void CSvmBrush::GetBounds(double& left, double& top, double& width, double& height)
{
	left	= BrushBounds.l;
	top		= BrushBounds.t;
	width	= BrushBounds.r - BrushBounds.l;
	height	= BrushBounds.b - BrushBounds.t;
}

void CSvmBrush::GetDibPattern(unsigned char **pBuffer, unsigned int &unWidth, unsigned int &unHeight)
{}

int CSvmPen::GetColor()
{
	return METAFILE_RGBA(Color.r, Color.g, Color.b);
}
TSvmRect::TSvmRect(CDataStream &stream)
{
	stream >> *this;
}
CDataStream& operator>>(CDataStream &stream, TSvmRect &p)
{
	stream >> p.l;
	stream >> p.t;
	stream >> p.r;
	stream >> p.b;

	return stream;
}
TSvmPolygon::TSvmPolygon(CDataStream &stream)
{
	stream >> *this;
}
CDataStream& operator>>(CDataStream &stream, TSvmPolygon &p)
{
	unsigned short count;
	stream >> count;

	for (int i = 0; i < count; i++)
	{
		TSvmPoint point;
		stream >> point;		
		p.points.push_back(point);
	}
	return stream;
}
#define METAFILE_RGBA(r, g, b, a) ((unsigned int)( ( (unsigned char)(r) )| ( ( (unsigned char)(g) ) << 8 ) | ( ( (unsigned char)(b) ) << 16 ) | ( (unsigned char)(a) << 24 ) ) )


CDataStream& operator>>(CDataStream &stream, TSvmColor &c)
{
    stream >> c.b;
    stream >> c.g;
    stream >> c.r;
    stream >> c.a;

	c.color = METAFILE_RGBA(c.r, c.g, c.b, c.a);

	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmColorEx &c)
{
	stream >> c.name;
	if ( c.name & 0x8000 )
	{
		//if(compression_mode) 
		stream >> c.r;
		stream >> c.g;
		stream >> c.b;
	}
	else
	{
		//из таблички
	}

	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmLineInfo &i)
{
	VersionCompat version;

	stream >> version;

	unsigned short style;
	stream >> style;
	
	i.style = (ESvmLineStyle) style;
	stream >> i.width;

	if (version.version >=2)
	{
		//counts  dot & dashes, size, distance
		stream.Skip(2 + 4 + 2 + 4 + 4);
	}

	return stream;
}
CDataStream& operator>>(CDataStream &stream, CSvmFont *font)
{
	unsigned short  version;
	unsigned int	totalSize;
	unsigned int	totalRead = 0;

	stream >> version;			
	stream >> totalSize;

	totalRead += parseString(stream, font->sFamilyName, version);
	totalRead += parseString(stream, font->sStyle, version);

	stream >> font->SizeWidth;		totalRead += 4;
	stream >> font->SizeHeight;		totalRead += 4;
	
	stream >> font->CharSet;		totalRead += 2;  
	stream >> font->Family;			totalRead += 2; 
	stream >> font->Pitch;			totalRead += 2; 
	stream >> font->Weight;  		totalRead += 2;   
	stream >> font->Underline;  	totalRead += 2; 
	stream >> font->StrikeOut; 		totalRead += 2; 
	stream >> font->Italic; 		totalRead += 2; 
	stream >> font->Language;		totalRead += 2; 
	stream >> font->Width;			totalRead += 2; 

	stream >> font->Orientation;	totalRead += 2; 
	stream >> font->bWordline;  	totalRead += 1; 
	stream >> font->bOutline;		totalRead += 1; 
	stream >> font->bShadow;		totalRead += 1; 
	stream >> font->Kerning;		totalRead += 1; 

	char   temp8;
	bool    tempbool;
	unsigned short tempu16;

	if (version > 1) 
	{
		stream >> temp8;        totalRead += 1;// relief
		stream >> tempu16;      totalRead += 2;// language
		stream >> tempbool;     totalRead += 1;// vertical
		if (tempbool)
			font->Orientation = 2;
		stream >> tempu16;      totalRead += 2;// emphasis
	}

	if (version > 2) 
	{
		stream >> tempu16;      totalRead += 2;// overline
	}
	return stream;
}
#define DIBCOREHEADERSIZE			12 

CDataStream& operator>>(CDataStream &stream, TSvmBitmap &b)
{
	// BITMAPINFOHEADER or BITMAPCOREHEADER
	stream >> b.nSize;

	// BITMAPCOREHEADER
	if ( b.nSize == DIBCOREHEADERSIZE )
	{
		short nTmp16;

		stream >> nTmp16;	b.nWidth = nTmp16;
		stream >> nTmp16;	b.nHeight = nTmp16;
		stream >> b.nPlanes;
		stream >> b.nBitCount;
	}
	else
	{
		// unknown Header
		if( b.nSize < sizeof( TSvmBitmap ) )
		{
			unsigned int nUnknownSize = sizeof( b.nSize );

			stream >> b.nWidth;		nUnknownSize += sizeof( b.nWidth );
			stream >> b.nHeight;	nUnknownSize += sizeof( b.nHeight );
			stream >> b.nPlanes;	nUnknownSize += sizeof( b.nPlanes );
			stream >> b.nBitCount;	nUnknownSize += sizeof( b.nBitCount );

			if( nUnknownSize < b.nSize )
			{
				stream >> b.nCompression;
				nUnknownSize += sizeof( b.nCompression );

				if( nUnknownSize < b.nSize )
				{
					stream >> b.nSizeImage;
					nUnknownSize += sizeof( b.nSizeImage );

					if( nUnknownSize < b.nSize )
					{
						stream >> b.nXPelsPerMeter;
						nUnknownSize += sizeof( b.nXPelsPerMeter );

						if( nUnknownSize < b.nSize )
						{
							stream >> b.nYPelsPerMeter;
							nUnknownSize += sizeof( b.nYPelsPerMeter );
						}

						if( nUnknownSize < b.nSize )
						{
							stream >> b.nColsUsed;
							nUnknownSize += sizeof( b.nColsUsed );

							if( nUnknownSize < b.nSize )
							{
								stream >> b.nColsImportant;
								nUnknownSize += sizeof( b.nColsImportant );
							}
						}
					}
				}
			}
		}
		else
		{
			stream >> b.nWidth;
			stream >> b.nHeight;
			stream >> b.nPlanes;
			stream >> b.nBitCount;
			stream >> b.nCompression;
			stream >> b.nSizeImage;
			stream >> b.nXPelsPerMeter;
			stream >> b.nYPelsPerMeter;
			stream >> b.nColsUsed;
			stream >> b.nColsImportant;
		}

		// Eventuell bis zur Palette ueberlesen
		if ( b.nSize > sizeof( TSvmBitmap ) ) // ???
			stream.Skip( b.nSize - sizeof( TSvmBitmap ) );
	}
	bool bTopDown;
	if ( b.nHeight < 0 )
	{
		bTopDown = true;
		b.nHeight *= -1;
	}
	else
		bTopDown = false;

   
    // #144105# protect a little against damaged files
    if( b.nSizeImage > ( 16 * static_cast< unsigned int >( b.nWidth * b.nHeight ) ) )
        b.nSizeImage = 0;
	
	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmGradient &g)
{
	stream >> g.version;	
	stream >> g.style;

	stream >> g.color1;
	stream >> g.color2;

	stream >> g.angle;

    stream >> g.border;
    stream >> g.offX;
    stream >> g.offY;
	stream >> g.intensityStart;
    stream >> g.intensityEnd;
    stream >> g.stepCount;	

	return stream;
}
}

