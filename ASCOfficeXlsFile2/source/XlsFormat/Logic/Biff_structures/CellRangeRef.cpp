
#include "CellRangeRef.h"
#include "Auxiliary/HelpFunc.h"

namespace XLS
{


CellRangeRef::CellRangeRef()
:	rowFirst(0),
	rowLast(0),
	columnFirst(0),
	columnLast(0),
	rowFirstRelative(true),
	columnFirstRelative(true),
	rowLastRelative(true),
	columnLastRelative(true)
{
}


CellRangeRef::CellRangeRef(const std::wstring& str_ref)
{
	fromString(str_ref);
}


CellRangeRef::CellRangeRef(const CellRef& cell_ref, const SourceType type)
{
	switch(type)
	{
		case stColumnBelow:
			rowFirst = cell_ref.getRow() + 1;
			rowLast = 65535;
			columnFirst = cell_ref.getColumn();
			columnLast = cell_ref.getColumn();
			rowFirstRelative = false;
			rowLastRelative = false;
			columnFirstRelative = cell_ref.getColumnRelative();
			columnLastRelative = cell_ref.getColumnRelative();
			break;
		case stRow:
			rowFirst = cell_ref.getRow();
			rowLast = cell_ref.getRow();
			columnFirst = 0;
			columnLast = 255;
			rowFirstRelative = cell_ref.getRowRelative();
			rowLastRelative = cell_ref.getRowRelative();
			columnFirstRelative = false;
			columnLastRelative = false;
			break;
		default:
			throw;// EXCEPT::LE::WhatIsTheFuck("Unexpected value.", __FUNCTION__);
	}

}


CellRangeRef::CellRangeRef(const CellRef& cell_ref1, const CellRef& cell_ref2)
:	rowFirst(cell_ref1.getRow()),
	rowLast(cell_ref2.getRow()),
	columnFirst(cell_ref1.getColumn()),
	columnLast(cell_ref2.getColumn()),
	rowFirstRelative(cell_ref1.getRowRelative()),
	columnFirstRelative(cell_ref1.getColumnRelative()),
	rowLastRelative(cell_ref2.getRowRelative()),
	columnLastRelative(cell_ref2.getColumnRelative())
{
}


BiffStructurePtr CellRangeRef::clone()
{
	return BiffStructurePtr(new CellRangeRef(*this));
}

//
//void CellRangeRef::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::wstring  name = getStructAttribute(xml_tag, L"name");
//	fromString(static_cast<wchar_t*>(name));
//}
//
//
//void CellRangeRef::(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"name", toString().c_str());
//	xml_tag->setAttribute(L"top_left_cell", getTopLeftCell().toString().c_str());
//	xml_tag->setAttribute(L"bottom_right_cell", CellRef(rowLast, columnLast, rowLastRelative, columnLastRelative).toString().c_str());
//}


const std::wstring CellRangeRef::toString(const bool useShortForm) const
{
	if(to_string_cache.empty())
	{
		long rowLast_norm = AUX::normalizeRow(rowLast);
		long rowFirst_norm = AUX::normalizeRow(rowFirst);
		long columnFirst_norm = AUX::normalizeColumn(columnFirst);
		long columnLast_norm = AUX::normalizeColumn(columnLast);
		if(0 == rowFirst_norm && 65535 == rowLast_norm ) // whole column or range of columns
		{
			if(useShortForm)
			{
				return to_string_cache = AUX::column2str(columnFirst_norm) + L':' + AUX::column2str(columnLast_norm);
			}
			else
			{
				rowLast_norm = 1048575;
			}
		}
		if(0 == columnFirst_norm && 255 == columnLast_norm) // whole row or range of rows
		{
			if(useShortForm)
			{
				return to_string_cache = AUX::row2str(rowFirst_norm) + L':' + AUX::row2str(rowLast_norm);
			}
			else
			{
				columnLast_norm = 16383;
			}
		}
		if(columnLast_norm == columnFirst_norm && rowFirst_norm == rowLast_norm) // single cell
		{
			return to_string_cache = AUX::loc2str(rowFirst_norm, rowFirstRelative, columnFirst_norm, columnFirstRelative);
		}
		else
		{
			return to_string_cache = AUX::loc2str(rowFirst_norm, rowFirstRelative, columnFirst_norm, columnFirstRelative) + 
				L':' + AUX::loc2str(rowLast_norm, rowLastRelative, columnLast_norm, columnLastRelative);
		}
	}
	return to_string_cache;
}


void CellRangeRef::fromString(const std::wstring& str)
{
	std::wstring::const_iterator str_begin = str.begin();
	std::wstring::const_iterator str_end = str.end();

	AUX::str2loc(str_begin, str_end, rowFirst, rowFirstRelative, columnFirst, columnFirstRelative);
	if(str_begin == str_end || L':' != *str_begin) // single cell
	{
		rowLast = rowFirst;
		rowLastRelative = rowFirstRelative;
		columnLast = columnFirst;
		columnLastRelative = columnFirstRelative;
		return;
	}
	++str_begin; // skip ':'
	AUX::str2loc(str_begin, str_end, rowLast, rowLastRelative, columnLast, columnLastRelative);
	if(-1 == rowFirst || -1 == rowLast) // no row specified - means whole column or range of columns
	{
		if(-1 == columnFirst || -1 == columnLast) // no correct data found in the supplied string
		{
			throw;// EXCEPT::LE::WrongIntermediateXMLFormat(std::string("CellRangeRef failed to initialize from \"") + 
															//static_cast<char*>(std::wstring (str.c_str())) + "\".", __FUNCTION__);
		}

		rowFirst = 0;
		rowLast = 65535;
	}
	if(-1 == columnFirst || -1 == columnLast) // no column specified - means whole row or range of rows
	{
		columnFirst = 0;
		columnLast = 255;
	}
	to_string_cache.clear();
}



CellRangeRef::operator const std::wstring  () const
{
	return toString().c_str();
}


const CellRef CellRangeRef::getTopLeftCell() const
{
	return CellRef(rowFirst, columnFirst, rowFirstRelative, columnFirstRelative);
}


void CellRangeRef::operator+=(const CellRef& appended_ref)
{
	rowFirst += rowFirstRelative ? appended_ref.getRow() : 0;
	rowLast += rowLastRelative ? appended_ref.getRow() : 0;
	columnFirst += columnFirstRelative ? appended_ref.getColumn() : 0;
	columnLast += columnLastRelative ? appended_ref.getColumn() : 0;
	to_string_cache.clear();
}


void CellRangeRef::operator-=(const CellRef& subtracted_ref)
{
	rowFirst -= rowFirstRelative ? subtracted_ref.getRow() : 0;
	rowLast -= rowLastRelative ? subtracted_ref.getRow() : 0;
	columnFirst -= columnFirstRelative ? subtracted_ref.getColumn() : 0;
	columnLast -= columnLastRelative ? subtracted_ref.getColumn() : 0;
	to_string_cache.clear();
}


void CellRangeRef::setColumnRelativity(const bool is_relative)
{
	rowFirstRelative = rowLastRelative = is_relative;
	to_string_cache.clear();
}


void CellRangeRef::setRowRelativity(const bool is_relative)
{
	columnFirstRelative = columnLastRelative = is_relative;
	to_string_cache.clear();
}


} // namespace XLS

