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

#ifdef __linux__
    #include <inttypes.h>
#endif

#include "../../../../../OOXML/Base/Base.h"
#include "BiffStructure.h"
#include "BiffAttribute.h"
#include "../../Auxiliary/HelpFunc.h"
#include "Boolean.h"

#pragma pack(1)

namespace XLS
{

typedef _UINT32			RwLongU;
typedef unsigned short	IXFCell;
typedef unsigned short	Rw;
typedef unsigned short	Col;
typedef unsigned short	Icv;
typedef unsigned short	IcvChart;
typedef unsigned short	TabId;
typedef _INT16			Col_NegativeOne;
typedef unsigned short	R_RwU; // Special version for use as a record attribute
typedef unsigned short	IFmt;
typedef unsigned short	CrtLayout12Mode;
typedef unsigned short	ColU;
typedef unsigned short	Col256U;
typedef unsigned short	RwU;
typedef unsigned short	Ilel;
typedef unsigned char	DColunByteU;
typedef unsigned short	DRw;
typedef unsigned char	ColunByte;
typedef unsigned char	ColunByteU;

typedef _INT32			Rw12;
typedef _INT32			UncheckedRw; // in biff12
typedef _INT32			UncheckedCol; // in biff12
typedef _INT32			Col12;
typedef unsigned short	Rwx;
typedef unsigned short	Colx;
typedef _UINT32			ColorICV;
typedef _UINT32			ColorTheme;
typedef _UINT16			ObjId;
typedef unsigned char	FillPattern;

typedef BiffAttributeSimple<unsigned char>		BIFF_BYTE;
typedef BiffAttributeSimple<_UINT16>			BIFF_WORD;
typedef BiffAttributeSimple<_UINT32>			BIFF_DWORD;

struct PtgAttrSpaceType : public BiffStructure_NoVtbl
{
	unsigned char type;
	unsigned char cch;
};

struct RkNumber : public BiffStructure_NoVtbl
{
	unsigned int	fX100	: 1;
	unsigned int	fInt	: 1;
	int				num		: 30;

	std::wstring value()
	{
		if(fInt)
		{
			return fX100 ? STR::double2str( num / 100.0) : STR::int2wstr(num);
		}
		else
		{
			return STR::double2str( fX100 ? toDouble() / 100.0 : toDouble() );
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
                _UINT64 remainder : 34;
                _UINT64 significant : 30;
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
    BErr() {}
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
        else if(std::wstring (L"#GETTING_DATA") == str) // in biff12
        {
            err = 0x2B;
        }
		else
		{
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of BErr.", "unknown");
		}
	};

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
        case 0x2B:
            return L"#GETTING_DATA"; // in biff12
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of BErr.", "unknown");
			break;
		}
        
        return L""; // TODO: need fixed
	}
};


struct XColorType : public BiffStructure_NoVtbl
{
	_UINT32 type;
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
	_UINT16 type;
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
	unsigned short unused3_2;
};


struct DXFNumIFmt : public BiffStructure_NoVtbl
{
private:
	unsigned char unused;
public:
	unsigned char ifmt;
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
	_UINT32 condDataValue;
	_UINT32 reserved;
};


struct KPISets : public BiffStructure_NoVtbl
{
	_UINT32 set;
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

struct KPISets14 : public BiffStructure_NoVtbl // in biff12
{
    _UINT32 set;
    enum {
        KPINIL_14 = 0xFFFFFFFF, // Sort by no-icon
        KPI3ARROWS_14 = 0x00000000, // Kpi3 Arrows set
        KPI3ARROWSGRAY_14 = 0x00000001, // Kpi3 Arrows Gray set
        KPI3FLAGS_14 = 0x00000002, // Kpi3 Flags set
        KPI3TRAFFICLIGHTS1_14 = 0x00000003, // Kpi3 Traffic Lights 1 set
        KPI3TRAFFICLIGHTS2_14 = 0x00000004, // Kpi3 Traffic Lights 2 set
        KPI3SIGNS_14 = 0x00000005, // Kpi3 Signs set
        KPI3SYMBOLS_14 = 0x00000006, // Kpi3 Symbols set
        KPI3SYMBOLS2_14 = 0x00000007, // Kpi3 Symbols 2 set
        KPI4ARROWS_14 = 0x00000008, // Kpi4 Arrows set
        KPI4ARROWSGRAY_14 = 0x00000009, // Kpi4 Arrows Gray set
        KPI4REDTOBLACK_14 = 0x0000000A, // Kpi4 Red To Black set
        KPI4RATING_14 = 0x0000000B, // Kpi4 Rating set
        KPI4TRAFFICLIGHTS_14 = 0x0000000C, // Kpi4 Traffic Lights set
        KPI5ARROWS_14 = 0x0000000D, // Kpi5 Arrows set
        KPI5ARROWSGRAY_14 = 0x0000000E, // Kpi5 Arrows Gray set
        KPI5RATING_14 = 0x0000000F, // Kpi5 Rating set
        KPI5QUARTERS_14 = 0x00000010, // Kpi5 Quarters set
        KPI3STARS_14 = 0x00000011, // Kpi3 Stars set
        KPI3TRIANGLES_14 = 0x00000012, // Kpi3 Triangles set
        KPI5BOXES_14 = 0x00000013 // Kpi5 Boxes set
    };
};


struct CFFlag : public BiffStructure_NoVtbl
{
	KPISets iIconSet;
	long iIcon;
};

struct CFFlag14 : public BiffStructure_NoVtbl // in biff12
{
    KPISets14 iIconSet;
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
	unsigned short rtFirst;
	unsigned short rtLast;
};

} // namespace XLS

#pragma pack()


