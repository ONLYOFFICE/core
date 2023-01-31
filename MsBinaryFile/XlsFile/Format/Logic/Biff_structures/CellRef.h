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

namespace XLS
{

class CellRef : public BiffStructure
{
public:
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellRef)

	CellRef();
	CellRef(const std::wstring  str);
    CellRef(const int row_init, const int column_init, const bool row_relative_init, const bool col_relative_init);

	BiffStructurePtr clone();

	static const ElementType type = typeCellRef;	

	const std::wstring	toString() const;
	void				fromString(const std::wstring& str);
	operator std::wstring  () const;

	void operator+=(const CellRef& appended_ref);
	void operator-=(const CellRef& subtracted_ref);

    virtual void load(CFRecord& record) {}

    const int	getRow() const;
    const int	getColumn() const;
	const bool	getRowRelative() const;
	const bool	getColumnRelative() const;

    int		row;
    int		column;
	bool	rowRelative;
	bool	colRelative;
protected:
	bool	fQuoted;
	mutable std::wstring to_string_cache;

};
bool operator==(const CellRef& ref1, const CellRef& ref2);

class CellRangeRef;
typedef boost::shared_ptr<CellRangeRef> CellRangeRefPtr;

#define DEFINE_NAME_CLASS(name) \
class name ## _name \
{\
public:\
    const std::string & operator() ()\
{\
    static std::string  name_str(#name); \
	return name_str;\
};\
};

typedef enum
{
	rel_Present,
	rel_Absent,
	rel_PresentQuoted
} RELATIVE_INFO;


DEFINE_NAME_CLASS(RgceLoc)
DEFINE_NAME_CLASS(RgceLocRel)
DEFINE_NAME_CLASS(RgceElfLocExtra)
DEFINE_NAME_CLASS(RgceElfLoc)

template<class NameProducer, class RwType, class ColType, RELATIVE_INFO rel_info>
class CellRef_T : public CellRef
{
public:
    CellRef_T(const std::wstring & str_ref) : CellRef(str_ref) {}
    CellRef_T() {}
    CellRef_T(const int row_init, const int column_init, const bool row_relative_init, const bool col_relative_init)
        :	CellRef(row_init, column_init, row_relative_init, col_relative_init) {}

	template<class otherNameProducer, class otherRwType, class otherColType, RELATIVE_INFO otherRel_info>
	CellRef_T(CellRef_T<otherNameProducer, otherRwType, otherColType, otherRel_info>& other)
		:	CellRef(other)
	{
    }

	CellRef_T(CellRef& other)
		:	CellRef(other)
	{
    }

    virtual const std::string & getClassName() const { return NameProducer()();}

	CellRef_T operator+(const CellRef& appended_ref)
	{
		CellRef_T ret_val(*this);
		ret_val +=(appended_ref);
		return ret_val;
    }

	CellRef_T operator-(const CellRef& subtracted_ref)
	{
		CellRef_T ret_val(*this);
		ret_val -=(subtracted_ref);
		return ret_val;
    }

	BiffStructurePtr clone()
	{
		return BiffStructurePtr(new CellRef_T(*this));
	};

	virtual void load(CFRecord& record)
	{
		RwType rw;
		ColType col;
		record >> rw >> col;
		row = rw;
		fQuoted = false;
		switch(rel_info)
		{
			case rel_Present:
				column = (col << 2) >> 2;
				rowRelative = 0 != (col & (1 << (sizeof(ColType) * 8 - 1)));
				colRelative = 0 != (col & (1 << (sizeof(ColType) * 8 - 2)));
				break;
			case rel_Absent:
				column = col;
				rowRelative = true;
				colRelative = true;
				break;
			case rel_PresentQuoted:
				column = (col << 2) >> 2;
				colRelative = rowRelative = 0 != (col & (1 << (sizeof(ColType) * 8 - 1)));
				fQuoted = 0 != (col & (1 << (sizeof(ColType) * 8 - 2)));
				break;
		}
	};

};


typedef CellRef_T<RgceLoc_name, unsigned short, unsigned short, rel_Present>			RgceLoc;
typedef CellRef_T<RgceLocRel_name, unsigned short, short, rel_Present>					RgceLocRel;
typedef CellRef_T<RgceElfLocExtra_name, unsigned short, unsigned short, rel_Present>	RgceElfLocExtra;
typedef CellRef_T<RgceElfLoc_name, unsigned short, unsigned short, rel_PresentQuoted>	RgceElfLoc;




} // namespace XLS

