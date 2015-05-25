#pragma once

#include "../Common/MetaFileTypes.h"
#include "../Common/MetaFileObjects.h"

#include "../Common/MetaFileUtils.h"

#include "../../../common/File.h"

namespace MetaFile
{
	typedef enum
	{
		SVM_OBJECT_UNKNOWN			= 0x00,
		SVM_OBJECT_BRUSH			= 0x01,
		SVM_OBJECT_FONT				= 0x02,
		SVM_OBJECT_PEN				= 0x03
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
    unsigned int	length;
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
struct TSvmBitmapSize
{
	unsigned short cx;
	unsigned short cy;
};
struct TSvmBitmapPoint
{
	unsigned short x;
	unsigned short y;
};
enum ESvmMapUnit
{
MAP_100TH_MM, MAP_10TH_MM, MAP_MM, MAP_CM,
			   MAP_1000TH_INCH, MAP_100TH_INCH, MAP_10TH_INCH, MAP_INCH,
			   MAP_POINT, MAP_TWIP, MAP_PIXEL, MAP_SYSFONT, MAP_APPFONT,
			   MAP_RELATIVE, /*MAP_REALAPPFONT,*/ MAP_LASTENUMDUMMY
};

enum ESvnRasterOp
{
	ROP_OVERPAINT, ROP_XOR, ROP_0, ROP_1, ROP_INVERT 
};
enum ESvmGradientStyle
{
	GRADIENT_LINEAR		= 0,
	GRADIENT_AXIAL		= 1,
	GRADIENT_RADIAL		= 2,
	GRADIENT_ELLIPTICAL = 3,
	GRADIENT_SQUARE		= 4,
	GRADIENT_RECT		= 5
};
enum ESvmLineStyle
{
	LINE_NONE = 0,
	LINE_SOLID = 1,
	LINE_DASH = 2
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
	unsigned char a; 

	unsigned int color;

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
		r = oOther.r;		g = oOther.g;		b = oOther.b;		a = oOther.a; color = oOther.color;
	}
	TSvmColor& operator=(const TSvmColor& oColor)
	{
		r = oColor.r;		g = oColor.g;		b = oColor.b;		a = oColor.a; color = oColor.color;
		return *this;
	}
};

struct TSvmLineInfo
{
	ESvmLineStyle	style;
	int				width;
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
	int          GetColor2();
	unsigned int GetStyleEx();
	unsigned int GetStyle();
	unsigned int GetHatch();
	unsigned int GetAlpha();
	std::wstring GetDibPatterPath(){ return L""; }
	void GetBounds(double& left, double& top, double& width, double& height);

public:
	unsigned short	BrushStyleEx;  //angle, or ....
	unsigned short	BrushStyle;
	TSvmColor		Color;
	TSvmColor		Color2;
	unsigned short	BrushHatch;

	TSvmRect		BrushBounds;
};

struct TSvmBitmap
{
	unsigned int	nSize;
	unsigned int	nWidth;
	unsigned int	nHeight;
	unsigned short	nPlanes;
	unsigned short	nBitCount;
	unsigned int	nCompression;
	unsigned int	nSizeImage;
	int				nXPelsPerMeter;
	int				nYPelsPerMeter;
	unsigned int	nColsUsed;
	unsigned int	nColsImportant;
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
		return (int)SizeHeight;
	}
	std::wstring GetFaceName()
	{
		return sFamilyName;
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
		return 0;//(int)Escapement;
	}
	int          GetCharSet()
	{
		return (int)CharSet;
	}

public:

	unsigned int	SizeHeight;
	unsigned int	SizeWidth;
	
	unsigned short	Width;
	unsigned short	Orientation;
	unsigned short	Weight;
	unsigned short	Italic;
	unsigned short	Underline;
	unsigned short	StrikeOut;
	unsigned short	CharSet;
	unsigned short	Language;
	unsigned short	Pitch;
	unsigned short	Family;
	
	bool bWordline;
	bool bOutline;
	bool bShadow;
	char Kerning;

	std::wstring  sFamilyName;
	std::wstring  sStyle;
	// Согласно спецификации длина имени не должна превышать 32 знака с учетом нулевого символа в конце
};

class CSvmPen : public CSvmObjectBase, public IPen
{
public:
	CSvmPen()
	{
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
		return (unsigned int)Width;
	}

public:
	unsigned short	PenStyle;
	int				Width;
	TSvmColor		Color;
};

void parseString(CDataStream &stream, std::wstring &string, unsigned short version = 0, unsigned short charset = 0);

CDataStream& operator>>(CDataStream &stream, VersionCompat &compat);
CDataStream& operator>>(CDataStream &stream, Fraction &fract);
CDataStream& operator>>(CDataStream &stream, MapMode &mm);
CDataStream& operator>>(CDataStream &stream, SvmHeader &header);
CDataStream& operator>>(CDataStream &stream, TSvmBitmapSize &s);
CDataStream& operator>>(CDataStream &stream, TSvmBitmapPoint &s);
CDataStream& operator>>(CDataStream &stream, TSvmPoint &p);
CDataStream& operator>>(CDataStream &stream, TSvmRect &p);
CDataStream& operator>>(CDataStream &stream, TSvmPolygon &p);
CDataStream& operator>>(CDataStream &stream, TSvmColor &c);
CDataStream& operator>>(CDataStream &stream, TSvmBitmap &b);
CDataStream& operator>>(CDataStream &stream, CSvmFont *f);
CDataStream& operator>>(CDataStream &stream, TSvmLineInfo &l);

}


