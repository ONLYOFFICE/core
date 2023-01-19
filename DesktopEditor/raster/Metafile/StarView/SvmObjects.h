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

struct TSvmMapMode 
{
    TSvmMapMode();
    TSvmMapMode(CDataStream &stream);

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
    TSvmMapMode		mapMode;
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
	TSvmPolygon(){}
	TSvmPolygon(CDataStream &stream);

	std::vector<TSvmPoint>	points;
};

struct TSvmColorEx
{
	unsigned short name;

	unsigned short r;
	unsigned short g;
	unsigned short b;
};

struct TSvmGradient
{
	VersionCompat version;
	
	unsigned short style;
	
	TSvmColorEx color1;
	TSvmColorEx color2;

	unsigned short angle;
    unsigned short border;
    unsigned short offX;
    unsigned short offY;
	unsigned short intensityStart;
    unsigned short intensityEnd;
    unsigned short stepCount;	
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
		r = 0;		g = 0;		b = 0;		a = 0;
	}
	TSvmColor(unsigned char _r, unsigned char _g, unsigned char _b)
	{
		r = _r;		g = _g;		b = _b;
	}
	void Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 0)
	{
		r = _r;		g = _g;		b = _b;		a = _a;
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
	unsigned int GetAlpha2();
	std::wstring GetDibPatterPath(){ return L""; }
	void GetCenterPoint(double& dX, double& dY){}
	void GetBounds(double& left, double& top, double& width, double& height);
	void GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight);

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
	double          GetHeight()
	{
		return (double)SizeHeight;
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
	int          GetOrientation()
	{
		return (int)Orientation;
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
		Width = 1;
		PenStyle = PS_NULL;
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
	double GetWidth()
	{
		return (double)Width;
	}
	unsigned int GetAlpha()
	{
		return 255;
	}
	double GetMiterLimit()
	{
		return 0;
	}
	double GetDashOffset()
	{
		return 0;
	}
	void GetDashData(double*& arDatas, unsigned int& unSize)
	{
		arDatas = NULL;
		unSize  = 0;
	}
public:
	unsigned short	PenStyle;
	int				Width;
	TSvmColor		Color;
};

int parseString(CDataStream &stream, std::wstring &string, unsigned short version = 0, unsigned short charset = 0);

CDataStream& operator>>(CDataStream &stream, VersionCompat &compat);
CDataStream& operator>>(CDataStream &stream, Fraction &fract);
CDataStream& operator>>(CDataStream &stream, TSvmMapMode &mm);
CDataStream& operator>>(CDataStream &stream, SvmHeader &header);
CDataStream& operator>>(CDataStream &stream, TSvmSize &s);
CDataStream& operator>>(CDataStream &stream, TSvmPoint &p);
CDataStream& operator>>(CDataStream &stream, TSvmRect &p);
CDataStream& operator>>(CDataStream &stream, TSvmPolygon &p);
CDataStream& operator>>(CDataStream &stream, TSvmColor &c);
CDataStream& operator>>(CDataStream &stream, TSvmColorEx &c);
CDataStream& operator>>(CDataStream &stream, TSvmBitmap &b);
CDataStream& operator>>(CDataStream &stream, CSvmFont *f);
CDataStream& operator>>(CDataStream &stream, TSvmLineInfo &l);
CDataStream& operator>>(CDataStream &stream, TSvmGradient &g);


}


