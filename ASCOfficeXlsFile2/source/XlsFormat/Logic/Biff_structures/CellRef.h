#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CellRef : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(CellRef)
public:

	CellRef();
	CellRef(const std::wstring  str);
	CellRef(const long row_init, const long column_init, const bool row_relative_init, const bool col_relative_init);

	BiffStructurePtr clone();

	static const ElementType	type = typeCellRef;
	

	const std::wstring toString() const;
	void fromString(const std::wstring& str);
	operator const _variant_t () const;
	operator const std::wstring  () const;

	void operator+=(const CellRef& appended_ref);
	void operator-=(const CellRef& subtracted_ref);

private:
	virtual void load(CFRecord& record) {};
	virtual void store(CFRecord& record) {};

public:
	const long getRow() const;
	const long getColumn() const;
	const bool getRowRelative() const;
	const bool getColumnRelative() const;

protected:
	long row;
	long column;
	bool rowRelative;
	bool colRelative;
	bool fQuoted;

	mutable std::wstring to_string_cache;

};

class CellRangeRef;
typedef boost::shared_ptr<CellRangeRef> CellRangeRefPtr;

#define DEFINE_NAME_CLASS(name) \
class name ## _name \
{\
public:\
	const std::wstring & operator() ()\
{\
	static std::wstring  name_str(L#name); \
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
	CellRef_T(const std::wstring & str_ref) : CellRef(str_ref) {};
	CellRef_T() {};
	CellRef_T(const long row_init, const long column_init, const bool row_relative_init, const bool col_relative_init)
		:	CellRef(row_init, column_init, row_relative_init, col_relative_init) {};

	template<class otherNameProducer, class otherRwType, class otherColType, RELATIVE_INFO otherRel_info>
	CellRef_T(CellRef_T<otherNameProducer, otherRwType, otherColType, otherRel_info>& other)
		:	CellRef(other)
	{
	};

	CellRef_T(CellRef& other)
		:	CellRef(other)
	{
	};

	virtual const std::wstring & getClassName() const { return NameProducer()();}

	CellRef_T operator+(const CellRef& appended_ref)
	{
		CellRef_T ret_val(*this);
		ret_val +=(appended_ref);
		return ret_val;
	};

	CellRef_T operator-(const CellRef& subtracted_ref)
	{
		CellRef_T ret_val(*this);
		ret_val -=(subtracted_ref);
		return ret_val;
	};

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

	virtual void store(CFRecord& record)
	{
		RwType rw(static_cast<RwType>(row));
		ColType col;
		switch(rel_info)
		{
			case rel_Present:
				col = static_cast<ColType>(column) & ((1 << (sizeof(ColType) * 8 - 2)) - 1);
				col |= rowRelative ? (1 << (sizeof(ColType) * 8 - 1)) : 0;
				col |= colRelative ? (1 << (sizeof(ColType) * 8 - 2)) : 0;
				break;
			case rel_Absent:
				col = static_cast<ColType>(column);
				break;
			case rel_PresentQuoted:
				col = static_cast<ColType>(column) & ((1 << (sizeof(ColType) * 8 - 2)) - 1);
				col |= rowRelative ? (1 << (sizeof(ColType) * 8 - 1)) : 0;
				col |= fQuoted ? (1 << (sizeof(ColType) * 8 - 2)) : 0;
				break;
		}
		record << rw << col;
	};


};


typedef CellRef_T<RgceLoc_name, unsigned short, unsigned short, rel_Present> RgceLoc;
typedef CellRef_T<RgceLocRel_name, unsigned short, short, rel_Present> RgceLocRel;
typedef CellRef_T<RgceElfLocExtra_name, unsigned short, unsigned short, rel_Present> RgceElfLocExtra;
typedef CellRef_T<RgceElfLoc_name, unsigned short, unsigned short, rel_PresentQuoted> RgceElfLoc;




} // namespace XLS

