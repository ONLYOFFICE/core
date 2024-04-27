/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

	const std::wstring	toString(const bool xlsb = false) const;
	void				fromString(const std::wstring& str);
	operator std::wstring  () const;

	void operator+=(const CellRef& appended_ref);
	void operator-=(const CellRef& subtracted_ref);

    void load(CFRecord& record) override {}
	void save(CFRecord& record) override {}

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
    CellRef_T(const std::wstring & str_ref) : CellRef(str_ref)
    {
        if(sizeof(RwType) < 4)
        {
            column = AUX::normalizeColumn(column);
            row = AUX::normalizeRow(row);
        }
    }
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

	void load(CFRecord& record) override
	{
		RwType rw;
		ColType col;
		record >> rw >> col;
		row = rw;
		
		fQuoted = false;
		switch(rel_info)
		{
			case rel_Present:
			{
				column = GETBITS(col, 0, sizeof(ColType) * 8 - 3);
				
				colRelative = GETBIT(col, sizeof(ColType) * 8 - 2);
				rowRelative = GETBIT(col, sizeof(ColType) * 8 - 1);
			}break;
			case rel_Absent:
			{
				column = col;
				
				rowRelative = true;
				colRelative = true;
			}break;
			case rel_PresentQuoted:
			{
				column = GETBITS(col, 0, sizeof(ColType) * 8 - 3);
				
				fQuoted = GETBIT(col, sizeof(ColType) * 8 - 2);
				colRelative = rowRelative = GETBIT(col, sizeof(ColType) * 8 - 1);
			}break;
		}
	}

	void save(CFRecord& record) override
	{
		RwType rw;
        ColType col = 0;
		rw = row;
		auto version = record.getGlobalWorkbookInfo()->Version;
		
		if (version < 0x0800)
		{
			col = column;
		}
		else
		{
        	SETBITS(col, 0, 13, column);
        	SETBIT(col, 14, colRelative);
        	SETBIT(col, 15, rowRelative);
		}

		record << rw << col;
	}

};


typedef CellRef_T<RgceLoc_name, unsigned short, unsigned short, rel_Present>			RgceLoc;
typedef CellRef_T<RgceLocRel_name, unsigned short, short, rel_Present>					RgceLocRel;
typedef CellRef_T<RgceElfLocExtra_name, unsigned short, unsigned short, rel_Present>	RgceElfLocExtra;
typedef CellRef_T<RgceElfLoc_name, unsigned short, unsigned short, rel_PresentQuoted>	RgceElfLoc;




} // namespace XLS

