
#include "SvmObjects.h"


namespace MetaFile
{
	void soakBytes(CDataStream &stream, int numBytes)
	{
		unsigned char scratch;
		for (int i = 0; i < numBytes; ++i) 
		{
			stream >> scratch;
		}
	}
	void parseString(CDataStream &stream, std::wstring &string)
	{
		unsigned short  length;

		stream >> length;
		for (unsigned int i = 0; i < length; ++i)
		{
			unsigned char  ch;
			stream >> ch;
			string += char(ch);
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

MapMode::MapMode()
    : version()
    , unit(0)
    , scaleX()
    , scaleY()
    , isSimple(true)
{
}

MapMode::MapMode(CDataStream &stream)
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
CDataStream& operator>>(CDataStream &stream, MapMode &mm)
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
		soakBytes(stream, 1);

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
CDataStream& operator>>(CDataStream &stream, TSvmPoint &p)
{
	stream >> p.x;
	stream >> p.y;

	return stream;
}
CSvmBrush::CSvmBrush() : Color(255, 255, 255)
{
	BrushStyle = BS_SOLID;
	BrushHatch = HS_HORIZONTAL;
}
CSvmBrush::CSvmBrush(CSvmBrush& oBrush)
{
	BrushStyle = oBrush.BrushStyle;
	Color = oBrush.Color;
	BrushHatch = oBrush.BrushHatch;
}

int CSvmBrush::GetColor()
{
	return METAFILE_RGBA(Color.r, Color.g, Color.b);
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
	return 255;
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
	stream >> p.r;
	stream >> p.t;
	stream >> p.b;

	return stream;
}
TSvmPolygon::TSvmPolygon(CDataStream &stream)
{
	stream >> *this;
}
CDataStream& operator>>(CDataStream &stream, TSvmPolygon &p)
{
	stream >> p.count;

	for (int i = 0; i < p.count; i++)
	{
		TSvmPoint point;
		stream >> point;		
		p.points.push_back(point);
	}
	return stream;
}
CDataStream& operator>>(CDataStream &stream, TSvmColor &c)
{
	stream >> c.r;
	stream >> c.g;
	stream >> c.b;
	unsigned char reserv;
	stream >> reserv;
	return stream;

}
CDataStream& operator>>(CDataStream &stream, CSvmBrush *b)
{
	return stream;
}
CDataStream& operator>>(CDataStream &stream, CSvmFont *font)
{
	unsigned short  version;
	unsigned int  totalSize;

	// the VersionCompat struct
	stream >> version;
	stream >> totalSize;

	// Name and style
	std::wstring  family;
	std::wstring  style;

	parseString(stream, family);
	parseString(stream, style);

	font->sFacename = family;

	// Font size
	unsigned int  width;
	unsigned int  height;
	stream >> width;
	stream >> height;
	
	font->Width = width;
	font->Height = height;

	char   temp8;
	bool    tempbool;
	unsigned short tempu16;
	stream >> tempu16;          // charset
	stream >> tempu16;          // family
	stream >> tempu16;          // pitch
	stream >> tempu16;          // weight
	if (tempu16 > 0)
		font->Weight = tempu16;
	stream >> tempu16;          // underline
	if (tempu16 > 0)
		font->Underline= 1;
	stream >> tempu16;          // strikeout
	if (tempu16 > 0)
		font->StrikeOut = 1;
	stream >> tempu16;          // italic
	if (tempu16 > 0)
		font->Italic = 1;
	stream >> tempu16;          // language
	stream >> tempu16;          // width

	if (tempu16 > 0)
		font->Width = tempu16; //??? todo

	stream >> tempu16;          // orientation
	stream >> tempbool;         // wordline
	stream >> tempbool;         // outline
	stream >> tempbool;         // shadow
	stream >> temp8;            // kerning

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
	// FIXME: Read away the rest of font here to allow for higher versions than 3.
	return stream;
}

}

