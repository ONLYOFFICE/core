#pragma once

//#include <qglobal.h>
#include "../Common/MetaFileTypes.h"
#include "../Common/MetaFileObjects.h"

#include "../Common/MetaFileUtils.h"

#include "../../../common/File.h"

namespace MetaFile
{
	typedef enum
	{
		SVM_OBJECT_UNKNOWN = 0x00,
		SVM_OBJECT_BRUSH = 0x01,
		SVM_OBJECT_FONT = 0x02,
		SVM_OBJECT_PEN = 0x03//,
		//EMF_OBJECT_PALETTE = 0x04
	} ESvmObjectType;

	class CSvmObjectBase
	{
	public:
		CSvmObjectBase(){}
		virtual ~CSvmObjectBase(){}
		virtual ESvmObjectType GetType()
		{
			return SVM_OBJECT_UNKNOWN;
		}
	};

struct VersionCompat 
{
    VersionCompat();
    VersionCompat(CDataStream &stream);

    unsigned short  version;
    unsigned int  length;//32
};

struct Fraction 
{
    Fraction();
    Fraction(CDataStream &stream);

	unsigned int  numerator;
    unsigned int  denominator;
};

struct TSvmPoint
{
	TSvmPoint(){ x = y = 0; }
	TSvmPoint(CDataStream &stream);

	int x;
	int y;

	void Set(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

struct TSvmSize
{
	unsigned int cx;
	unsigned int cy;
};
struct MapMode 
{
    MapMode();
    MapMode(CDataStream &stream);

    VersionCompat	version;
    unsigned short	unit;
	TSvmPoint		origin;
    Fraction		scaleX;
    Fraction		scaleY;
    bool			isSimple;
};

struct SvmHeader 
{
    SvmHeader();
    SvmHeader(CDataStream &stream);

    VersionCompat	versionCompat;
    unsigned int	compressionMode;
    MapMode			mapMode;
	TRect			boundRect;
    unsigned int	actionCount;
};

struct TSvmRect
{
	TSvmRect();
	TSvmRect(CDataStream &stream);

	int l;
	int r;
	int t;
	int b;

};

struct TSvmPolygon
{
	TSvmPolygon(){ count = 0; }
	TSvmPolygon(CDataStream &stream);
	std::vector<TSvmPoint>	points;
	unsigned short			count;
};
struct TSvmColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a; //Reserved Must be 0x00

	TSvmColor()
	{
		r = 0;		g = 0;		b = 0;	
	}
	TSvmColor(unsigned char _r, unsigned char _g, unsigned char _b)
	{
		r = _r;		g = _g;		b = _b;
	}
	void Set(unsigned char _r, unsigned char _g, unsigned char _b)
	{
		r = _r;		g = _g;		b = _b;
	}
	void Init()
	{
		r = 0;		g = 0;		b = 0;		a = 0;
	}
	void Copy(const TSvmColor& oOther)
	{
		r = oOther.r;		g = oOther.g;		b = oOther.b;		a = oOther.a;
	}
	TSvmColor& operator=(const TSvmColor& oColor)
	{
		r = oColor.r;		g = oColor.g;		b = oColor.b;		a = oColor.a;
		return *this;
	}
};
struct TSvmWindow
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

	void Copy(TSvmWindow* pOther)
	{
		lX = pOther->lX;
		lY = pOther->lY;
		ulW = pOther->ulW;
		ulH = pOther->ulH;
	}
};

class CSvmBrush : public CSvmObjectBase, public IBrush
{
public:
	CSvmBrush();
	CSvmBrush(CSvmBrush& oBrush);
	virtual ~CSvmBrush(){}
	virtual ESvmObjectType GetType()
	{
		return SVM_OBJECT_BRUSH;
	}
	// IBrush
	int          GetColor();
	unsigned int GetStyle();
	unsigned int GetHatch();
	unsigned int GetAlpha();
	std::wstring GetDibPatterPath(){ return L""; }

public:

	unsigned short BrushStyle;
	TSvmColor      Color;
	unsigned short BrushHatch;
};

class CSvmFont : public CSvmObjectBase, public IFont
{
public:

	CSvmFont()
	{
	}
	~CSvmFont()
	{
	}

	virtual ESvmObjectType GetType()
	{
		return SVM_OBJECT_FONT;
	}

	// IFont
	int          GetHeight()
	{
		return (int)Height;
	}
	std::wstring GetFaceName()
	{
		return sFacename;
	}
	int          GetWeight()
	{
		return (int)Weight;
	}
	bool         IsItalic()
	{
		return (0x01 == Italic ? true : false);
	}
	bool         IsStrikeOut()
	{
		return (0x01 == StrikeOut ? true : false);
	}
	bool         IsUnderline()
	{
		return (0x01 == Underline ? true : false);
	}
	int          GetEscapement()
	{
		return (int)Escapement;
	}
	int          GetCharSet()
	{
		return (int)CharSet;
	}

public:

	short         Height;
	short         Width;
	short         Escapement;
	short         Orientation;
	short         Weight;
	unsigned char Italic;
	unsigned char Underline;
	unsigned char StrikeOut;
	unsigned char CharSet;
	unsigned char OutPrecision;
	unsigned char ClipPrecision;
	unsigned char Quality;
	unsigned char PitchAndFamily;
	std::wstring  sFacename; 
	// Согласно спецификации длина имени не должна превышать 32 знака с учетом нулевого символа в конце
};

class CSvmPen : public CSvmObjectBase, public IPen
{
public:
	CSvmPen()
	{
		Width.x = 1;
	}
	~CSvmPen()
	{

	}
	virtual ESvmObjectType GetType()
	{
		return SVM_OBJECT_PEN;
	}

	// IPen
	int          GetColor();
	unsigned int GetStyle()
	{
		return (unsigned int)PenStyle;
	}
	unsigned int GetWidth()
	{
		return (unsigned int)Width.x;
	}

public:
	unsigned short PenStyle;
	TSvmPoint      Width;
	TSvmColor      Color;
};

void soakBytes(CDataStream &stream, int numBytes);
void parseString(CDataStream &stream, std::wstring &string);

CDataStream& operator>>(CDataStream &stream, VersionCompat &compat);
CDataStream& operator>>(CDataStream &stream, Fraction &fract);
CDataStream& operator>>(CDataStream &stream, MapMode &mm);
CDataStream& operator>>(CDataStream &stream, SvmHeader &header);
CDataStream& operator>>(CDataStream &stream, TSvmPoint &p);
CDataStream& operator>>(CDataStream &stream, TSvmRect &p);
CDataStream& operator>>(CDataStream &stream, TSvmPolygon &p);
CDataStream& operator>>(CDataStream &stream, TSvmColor &c);
CDataStream& operator>>(CDataStream &stream, CSvmBrush *b);
CDataStream& operator>>(CDataStream &stream, CSvmFont *f);

}


