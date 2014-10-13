#pragma once

#include "BiffStructure.h"
#include "CellRef.h"

namespace XLS
{;

class CellRangeRef : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CellRangeRef)
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

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);

	const std::wstring toString(const bool useShortForm = true) const;
	void fromString(const std::wstring& str);
	operator const _variant_t() const;
	operator const std::wstring  () const;

	const CellRef getTopLeftCell() const;

	const long getRowFirst() const { return rowFirst; }
	const long getRowLast() const { return rowLast; }
	const bool getRowFirstRelative() const { return rowFirstRelative; }
	const bool getRowLastRelative() const { return rowLastRelative; }
	const long getColumnFirst() const { return columnFirst; }
	const long getColumnLast() const { return columnLast; }
	const bool getColumnFirstRelative() const { return columnFirstRelative; }
	const bool getColumnLastRelative() const { return columnLastRelative; }
	void setColumnRelativity(const bool is_relative);
	void setRowRelativity(const bool is_relative);

	void operator+=(const CellRef& appended_ref);
	void operator-=(const CellRef& subtracted_ref);

private:
	virtual void load(CFRecord& record) {};
	virtual void store(CFRecord& record) {};

protected:
	long rowFirst;
	long rowLast;
	bool rowFirstRelative;
	bool rowLastRelative;
	long columnFirst;
	long columnLast;
	bool columnFirstRelative;
	bool columnLastRelative;

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
	CellRangeRef_T(const std::wstring & str_ref) : CellRangeRef(str_ref) {};
	CellRangeRef_T() {};

	template<class otherNameProducer, class otherRwType, class otherColType, RELATIVE_INFO otherRel_info>
	CellRangeRef_T(CellRangeRef_T<otherNameProducer, otherRwType, otherColType, otherRel_info>& other)
	:	CellRangeRef(other)
	{
	};

	CellRangeRef_T(CellRangeRef& other)
	:	CellRangeRef(other)
	{
	};

	virtual const std::wstring & getClassName() const { return NameProducer()();}

	CellRangeRef_T operator+(const CellRef& appended_ref)
	{
		CellRangeRef_T ret_val(*this);
		ret_val +=(appended_ref);
		return ret_val;
	};

	CellRangeRef_T operator-(const CellRef& subtracted_ref)
	{
		CellRangeRef_T ret_val(*this);
		ret_val -=(subtracted_ref);
		return ret_val;
	};

	BiffStructurePtr clone()
	{
		return BiffStructurePtr(new CellRangeRef_T(*this));
	};

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

	virtual void store(CFRecord& record)
	{
		RwType rwFirst(static_cast<RwType>(rowFirst));
		RwType rwLast(static_cast<RwType>(rowLast));
		ColType colFirst;
		ColType colLast;
		switch(rel_info)
		{
		case rel_Present:
			colFirst = static_cast<ColType>(columnFirst) & ((1 << (sizeof(ColType) * 8 - 2)) - 1);
			colLast = static_cast<ColType>(columnLast) & ((1 << (sizeof(ColType) * 8 - 2)) - 1);
			colFirst |= rowFirstRelative ? (1 << (sizeof(ColType) * 8 - 1)) : 0;
			colFirst |= columnFirstRelative ? (1 << (sizeof(ColType) * 8 - 2)) : 0;
			colLast |= rowLastRelative ? (1 << (sizeof(ColType) * 8 - 1)) : 0;
			colLast |= columnLastRelative ? (1 << (sizeof(ColType) * 8 - 2)) : 0;
			break;
		case rel_Absent:
			colFirst = static_cast<ColType>(columnFirst);
			colLast = static_cast<ColType>(columnLast);
			break;
		}
		record << rwFirst << rwLast << colFirst << colLast;
	};


};


typedef CellRangeRef_T<Ref_name, unsigned __int16, unsigned char, rel_Absent> Ref;
typedef CellRangeRef_T<Ref8_name, unsigned __int16, unsigned __int16, rel_Absent> Ref8;
typedef CellRangeRef_T<Ref8U_name, unsigned __int16, unsigned __int16, rel_Absent> Ref8U;
typedef CellRangeRef_T<RefU_name, unsigned __int16, unsigned char, rel_Absent> RefU;
typedef CellRangeRef_T<Ref8U2007_name, unsigned int, unsigned int, rel_Absent> Ref8U2007;
typedef CellRangeRef_T<RFX_name, long, long, rel_Absent> RFX;
typedef CellRangeRef_T<RgceArea_name, unsigned __int16, unsigned __int16, rel_Present> RgceArea;
typedef CellRangeRef_T<RgceAreaRel_name, short, short, rel_Present> RgceAreaRel;










} // namespace XLS

