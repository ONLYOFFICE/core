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

#include "BiffStructure.h"
#include "CellRef.h"

namespace XLS
{

class CellRangeRef : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellRangeRef)
public:
	typedef enum {
		stRange,
		stSingleCell,
		stColumn,
		stRow,
		stColumnBelow
	} SourceType;

	CellRangeRef();
	CellRangeRef(const std::wstring & str_ref);
	CellRangeRef(const CellRef& cell_ref, const SourceType type);
	CellRangeRef(const CellRef& cell_ref1, const CellRef& cell_ref2);

	BiffStructurePtr clone();

	static const ElementType	type = typeCellRangeRef;

	const std::wstring	toString(const bool useShortForm = true) const;
	void				fromString(const std::wstring& str);
	operator std::wstring  () const;

	const CellRef getTopLeftCell() const;

    const int	getRowFirst() const { return rowFirst; }
    const int	getRowLast() const { return rowLast; }
	const bool	getRowFirstRelative() const { return rowFirstRelative; }
	const bool	getRowLastRelative() const { return rowLastRelative; }
    const int	getColumnFirst() const { return columnFirst; }
    const int	getColumnLast() const { return columnLast; }
	const bool	getColumnFirstRelative() const { return columnFirstRelative; }
	const bool	getColumnLastRelative() const { return columnLastRelative; }
	void		setColumnRelativity(const bool is_relative);
	void		setRowRelativity(const bool is_relative);

	bool inRange(const CellRef& cell);

	void operator+=(const CellRef& appended_ref);
	void operator-=(const CellRef& subtracted_ref);

    virtual void load(CFRecord& record) {}

    int		rowFirst;
    int		rowLast;
	bool	rowFirstRelative;
	bool	rowLastRelative;
    int		columnFirst;
    int		columnLast;
	bool	columnFirstRelative;
	bool	columnLastRelative;

	mutable std::wstring to_string_cache;

};


DEFINE_NAME_CLASS(Ref)
DEFINE_NAME_CLASS(Ref8)
DEFINE_NAME_CLASS(Ref8U)
DEFINE_NAME_CLASS(RefU)
DEFINE_NAME_CLASS(Ref8U2007)
DEFINE_NAME_CLASS(RFX)
DEFINE_NAME_CLASS(RgceArea)
DEFINE_NAME_CLASS(RgceAreaRel)

template<class NameProducer, class RwType, class ColType, RELATIVE_INFO rel_info>
class CellRangeRef_T : public CellRangeRef
{
public:
    CellRangeRef_T(const std::wstring & str_ref) : CellRangeRef(str_ref) {}
    CellRangeRef_T() {}

	template<class otherNameProducer, class otherRwType, class otherColType, RELATIVE_INFO otherRel_info>
	CellRangeRef_T(CellRangeRef_T<otherNameProducer, otherRwType, otherColType, otherRel_info>& other)
	:	CellRangeRef(other)
	{
    }

	CellRangeRef_T(CellRangeRef& other)
	:	CellRangeRef(other)
	{
    }

    virtual const std::string & getClassName() const { return NameProducer()();}

	CellRangeRef_T operator+(const CellRef& appended_ref)
	{
		CellRangeRef_T ret_val(*this);
		ret_val +=(appended_ref);
		return ret_val;
    }

	CellRangeRef_T operator-(const CellRef& subtracted_ref)
	{
		CellRangeRef_T ret_val(*this);
		ret_val -=(subtracted_ref);
		return ret_val;
    }

	BiffStructurePtr clone()
	{
		return BiffStructurePtr(new CellRangeRef_T(*this));
    }

	virtual void load(CFRecord& record)
	{
		RwType rwFirst;
		RwType rwLast;
		ColType colFirst;
		ColType colLast;
		
		record >> rwFirst >> rwLast >> colFirst >> colLast;
		
		rowFirst = rwFirst;
		rowLast = rwLast;
		
		switch(rel_info)
		{
			case rel_Present:
				columnFirst = (colFirst << 2) >> 2;
				columnLast = (colLast << 2) >> 2;
				rowFirstRelative = 0 != (colFirst & (1 << (sizeof(ColType) * 8 - 1)));
				columnFirstRelative = 0 != (colFirst & (1 << (sizeof(ColType) * 8 - 2)));
				rowLastRelative = 0 != (colLast & (1 << (sizeof(ColType) * 8 - 1)));
				columnLastRelative = 0 != (colLast & (1 << (sizeof(ColType) * 8 - 2)));
				break;
			case rel_Absent:
				columnFirst = colFirst;
				columnLast = colLast;
				rowFirstRelative = true;
				columnFirstRelative = true;
				rowLastRelative = true;
				columnLastRelative = true;
				break;
		}
	};

};


typedef CellRangeRef_T<Ref_name, unsigned short, unsigned char, rel_Absent> Ref;
typedef CellRangeRef_T<Ref8_name, unsigned short, unsigned short, rel_Absent> Ref8;
typedef CellRangeRef_T<Ref8U_name, unsigned short, unsigned short, rel_Absent> Ref8U;
typedef CellRangeRef_T<RefU_name, unsigned short, unsigned char, rel_Absent> RefU;
typedef CellRangeRef_T<Ref8U2007_name, unsigned int, unsigned int, rel_Absent> Ref8U2007;
typedef CellRangeRef_T<RFX_name, int, int, rel_Absent> RFX;
typedef CellRangeRef_T<RgceArea_name, unsigned short, unsigned short, rel_Present> RgceArea;
typedef CellRangeRef_T<RgceAreaRel_name, short, short, rel_Present> RgceAreaRel;










} // namespace XLS

