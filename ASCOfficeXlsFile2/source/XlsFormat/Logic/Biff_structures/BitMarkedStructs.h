#pragma once

#include "BiffStructure.h"
#include "BiffAttribute.h"
//#include <Exception/WrongBiffRecord.h>
#include "Boolean.h"

#pragma pack(1)

namespace XLS
{;

typedef BiffAttributeSimple<unsigned int> RwLongU;
typedef BiffAttributeSimple<unsigned __int16> ColU;
typedef BiffAttributeSimple<unsigned __int16> Col256U;
typedef BiffAttributeSimple<unsigned __int16> IXFCell;
typedef BiffAttributeSimple<unsigned __int16> Rw;
typedef BiffAttributeSimple<unsigned __int16> Col;
typedef BiffAttributeSimple<unsigned __int16> Icv;
typedef BiffAttributeSimple<unsigned __int16> IcvChart;
typedef BiffAttributeSimple<unsigned __int16> TabId;
typedef BiffAttributeSimple<short> Col_NegativeOne;
typedef BiffAttributeSimple<unsigned __int16> R_RwU; // Special version for use as a record attribute
typedef BiffAttributeSimple<unsigned __int16> IFmt;
typedef BiffAttributeSimple<unsigned __int16> CrtLayout12Mode;
typedef unsigned __int16 RwU;
typedef unsigned __int16 Ilel;
typedef unsigned char DColunByteU;
typedef unsigned __int16 DRw;
typedef unsigned char ColunByte;
typedef unsigned char ColunByteU;
typedef long Rw12;
typedef long Col12;
typedef unsigned __int16 Rwx;
typedef unsigned __int16 Colx;
typedef unsigned int ColorICV;
typedef unsigned int ColorTheme;
typedef unsigned __int16 ObjId;
typedef unsigned char FillPattern;


typedef BiffAttributeSimple<unsigned char>		BIFF_BYTE;
typedef BiffAttributeSimple<unsigned __int16>	BIFF_WORD;
typedef BiffAttributeSimple<short>				BIFF_SHORT;
typedef BiffAttributeSimple<unsigned int>		BIFF_DWORD;
typedef BiffAttributeSimple<long>				BIFF_LONG;
typedef BiffAttributeSimple<bool>				BIFF_BOOL;
typedef BiffAttributeSimple<double>				BIFF_DOUBLE;
typedef BiffAttributeSimple<std::wstring>		BIFF_BSTR;



struct PtgAttrSpaceType : public BiffStructure_NoVtbl
{
	unsigned char type;
	unsigned char cch;
};


struct RkNumber : public BiffStructure_NoVtbl
{
	unsigned int fX100 : 1;
	unsigned int fInt : 1;
	unsigned int num : 30;
	operator _variant_t()
	{
		if(fInt)
		{
			//return fX100 ? num / 100 : num;
			return fX100 ? num / 100.0 : num;
		}
		else
		{
			return fX100 ? toDouble() / 100.0 : toDouble();
		}
	}
private:
	const double toDouble() const
	{
		union DOUBLE_MAKER
		{
			double full;
			struct 
			{
				UINT64 remainder : 34;
				UINT64 significant : 30;
			};
		} val;
		val.significant = num;
		val.remainder = 0;
		return val.full;
	}
};


struct BErr : public BiffStructure_NoVtbl
{
	unsigned char err;
	BErr() {};
	BErr(const std::wstring  str)
	{
		if(std::wstring (L"#NULL!") == str)
		{
			err = 0x00;
		}
		else if(std::wstring (L"#DIV/0!") == str)
		{
			err = 0x07;
		}
		else if(std::wstring (L"#VALUE!") == str)
		{
			err = 0x0F;
		}
		else if(std::wstring (L"#REF!") == str)
		{
			err = 0x17;
		}
		else if(std::wstring (L"#NAME?") == str)
		{
			err = 0x1D;
		}
		else if(std::wstring (L"#NUM!") == str)
		{
			err = 0x24;
		}
		else if(std::wstring (L"#N/A") == str)
		{
			err = 0x2A;
		}
		else
		{
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of BErr.", "unknown");
		}
	};
	operator const _variant_t()
	{
		return toString().c_str();
	}

	const std::wstring toString() const
	{
		switch(err)
		{
		case 0x00:
			return L"#NULL!";
		case 0x07:
			return L"#DIV/0!";
		case 0x0F:
			return L"#VALUE!";
		case 0x17:
			return L"#REF!";
		case 0x1D:
			return L"#NAME?";
		case 0x24:
			return L"#NUM!";
		case 0x2A:
			return L"#N/A";
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of BErr.", "unknown");
		}
	}
};


struct XColorType : public BiffStructure_NoVtbl
{
	unsigned int type;
	enum
	{
		XCLRAUTO = 0x00000000, // Automatic color
		XCLRINDEXED = 0x00000001, // Indexed color
		XCLRRGB = 0x00000002, // RGB color
		XCLRTHEMED = 0x00000003, // Theme color
		XCLRNINCHED = 0x00000004 //Color not set
	};
};


struct RevisionType : public BiffStructure_NoVtbl
{
	unsigned __int16 type;
	enum
	{
		REVTINSRW = 0x0000,		// Insert Row.
		REVTINSCOL = 0x0001,	// Insert Column.
		REVTDELRW = 0x0002,		// Delete Row.
		REVTDELCOL = 0x0003,	// Delete Column.
		REVTMOVE = 0x0004,		// Cell Move.
		REVTINSERTSH = 0x0005,	// Insert Sheet.
		REVTSORT = 0x0007,		// Sort.
		REVTCHANGECELL = 0x0008,// Cell Change.
		REVTRENSHEET = 0x0009,	// Rename Sheet.
		REVTDEFNAME = 0x000A,	// Defined Name Change.
		REVTFORMAT = 0x000B,	// Format Revision.
		REVTAUTOFMT = 0x000C,	// AutoFormat Revision.
		REVTNOTE = 0x000D,		// Comment Revision.
		REVTHEADER = 0x0020,	// Header (meta-data) Revision.
		REVTCONFLICT = 0x0025,	// Conflict.
		REVTADDVIEW = 0x002B,	// Custom view Add.
		REVTDELVIEW = 0x002C,	// Custom view Delete.
		REVTTRASHQTFIELD = 0x002E // Query table field Removal.
	};
};



struct Ts : public BiffStructure_NoVtbl
{
private:
	unsigned char unused1 : 1;
public:
	unsigned char ftsItalic : 1;
private:
	unsigned char unused2 : 5;
public:
	unsigned char ftsStrikeout : 1;
private:
	unsigned char unused3_1;
	unsigned __int16 unused3_2;
};


struct DXFNumIFmt : public BiffStructure_NoVtbl
{
private:
	unsigned char unused;
public:
	unsigned char ifmt;
};


class SharedFeatureType : public BiffAttributeSimple<unsigned __int16>
{
public:

	enum
	{
		ISFPROTECTION = 0x0002, // Specifies the enhanced protection type. 
		ISFFEC2 = 0x0003, // Specifies the ignored formula errors type.
		ISFFACTOID = 0x0004, // Specifies the smart tag type.
		ISFLIST = 0x0005, // Specifies the list type.
	};

};


struct FFErrorCheck : public BiffStructure_NoVtbl
{
	unsigned int ffecCalcError : 1;
	unsigned int ffecEmptyCellRef : 1;
	unsigned int ffecNumStoredAsText : 1;
	unsigned int ffecInconsistRange : 1;
	unsigned int ffecInconsistFmla : 1;
	unsigned int ffecTextDateInsuff : 1;
	unsigned int ffecUnprotFmla : 1;
	unsigned int ffecDataValidation : 1;
	unsigned int reserved : 24;
};


struct ExtSheetPair : public BiffStructure_NoVtbl
{
	short itabFirst;
	short itabLast;
};


struct CondDataValue : public BiffStructure_NoVtbl
{
	unsigned int condDataValue;
private:
	unsigned int reserved;
};


struct KPISets : public BiffStructure_NoVtbl
{
	unsigned int set;
	enum {
		KPINIL = 0xFFFFFFFF, // Sort by no-icon
		KPI3ARROWS = 0x00000000, // Kpi3 Arrows set
		KPI3ARROWSGRAY = 0x00000001, // Kpi3 Arrows Gray set
		KPI3FLAGS = 0x00000002, // Kpi3 Flags set
		KPI3TRAFFICLIGHTS1 = 0x00000003, // Kpi3 Traffic Lights 1 set
		KPI3TRAFFICLIGHTS2 = 0x00000004, // Kpi3 Traffic Lights 2 set
		KPI3SIGNS = 0x00000005, // Kpi3 Signs set
		KPI3SYMBOLS = 0x00000006, // Kpi3 Symbols set
		KPI3SYMBOLS2 = 0x00000007, // Kpi3 Symbols 2 set
		KPI4ARROWS = 0x00000008, // Kpi4 Arrows set
		KPI4ARROWSGRAY = 0x00000009, // Kpi4 Arrows Gray set
		KPI4REDTOBLACK = 0x0000000A, // Kpi4 Red To Black set
		KPI4RATING = 0x0000000B, // Kpi4 Rating set
		KPI4TRAFFICLIGHTS = 0x0000000C, // Kpi4 Traffic Lights set
		KPI5ARROWS = 0x0000000D, // Kpi5 Arrows set
		KPI5ARROWSGRAY = 0x0000000E, // Kpi5 Arrows Gray set
		KPI5RATING = 0x0000000F, // Kpi5 Rating set
		KPI5QUARTERS = 0x00000010 // Kpi5 Quarters set
	};
};


struct CFFlag : public BiffStructure_NoVtbl
{
	KPISets iIconSet;
	long iIcon;
};


struct FrtFlags : public BiffStructure_NoVtbl
{
	FrtFlags() : fFrtRef(false), fFrtAlert(false), reserved2(0), reserved(0) {};
	bool fFrtRef : 1;
	bool fFrtAlert : 1;
private:
	unsigned char reserved2 : 6;
	unsigned char reserved;
};


struct CFrtId : public BiffStructure_NoVtbl
{
	unsigned __int16 rtFirst;
	unsigned __int16 rtLast;
};


} // namespace XLS


namespace CRYPTO
{;

struct Version : public XLS::BiffStructure_NoVtbl
{
	unsigned __int16 vMajor;
	unsigned __int16 vMinor;
};


} // namespace CRYPTO

#pragma pack()


