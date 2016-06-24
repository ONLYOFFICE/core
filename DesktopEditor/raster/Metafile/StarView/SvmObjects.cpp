/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../../../common/String.h"


namespace MetaFile
{
	void parseString(CDataStream &stream, std::wstring &string, unsigned short version, unsigned short charset)
	{

		if (charset == 0xffff)//RTL_UNICODE
		{
			unsigned int length;
			stream >> length;
			
			string = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)stream.GetCurPtr(), length);
	
			stream.Skip(length*2);
		}
		else
		{
			unsigned short  length;
			stream >> length;
		
			if (charset < 1)
			{
				std::string ansiString = std::string((char*)stream.GetCurPtr(),length);
				string = std::wstring(ansiString.begin(), ansiString.end());
			}else
				string = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)stream.GetCurPtr(), length,
																			(NSStringExt::CConverter::ESingleByteEncoding)charset);
			stream.Skip(length);
		}
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

	stream >> version;
	stream >> totalSize;

	parseString(stream, font->sFamilyName, version);
	parseString(stream, font->sStyle, version);

	stream >> font->SizeWidth;
	stream >> font->SizeHeight;
	
	stream >> font->CharSet;    
	stream >> font->Family;
	stream >> font->Pitch;
	stream >> font->Weight;    
	stream >> font->Underline;  
	stream >> font->StrikeOut; 
	stream >> font->Italic; 
	stream >> font->Language;
	stream >> font->Width;

	stream >> font->Orientation;
	stream >> font->bWordline;   
	stream >> font->bOutline;
	stream >> font->bShadow;
	stream >> font->Kerning;

	char   temp8;
	bool    tempbool;
	unsigned short tempu16;

	if (version > 1) 
	{
		stream >> temp8;        // relief
		stream >> tempu16;      // language
		stream >> tempbool;     // vertical
		if (tempbool)
			font->Orientation = 2;
		stream >> tempu16;      // emphasis
	}

	if (version > 2) 
	{
		stream >> tempu16;      // overline
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

