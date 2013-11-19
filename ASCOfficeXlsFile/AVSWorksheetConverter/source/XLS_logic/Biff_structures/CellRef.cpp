#include "stdafx.h"
#include "CellRef.h"

namespace XLS
{;


CellRef::CellRef()
:	row(0),
	column(0),
	rowRelative(true),
	colRelative(true),
	fQuoted(false)
{
}


CellRef::CellRef(const _bstr_t str)
:	fQuoted(false)
{
	fromString(static_cast<wchar_t*>(str));
}


CellRef::CellRef(const std::wstring& str_ref)
:	fQuoted(false)
{
	fromString(str_ref);
}


CellRef::CellRef(const long row_init, const long column_init, const bool row_relative_init, const bool col_relative_init)
:	row(row_init),
	column(column_init),
	rowRelative(row_relative_init),
	colRelative(col_relative_init),
	fQuoted(false)
{
}


BiffStructurePtr CellRef::clone()
{
	return BiffStructurePtr(new CellRef(*this));
}


void CellRef::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	_bstr_t name = getStructAttribute(xml_tag, L"name");
	fromString(static_cast<wchar_t*>(name));
}


void CellRef::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"name", toString().c_str());
}


const std::wstring CellRef::toString() const
{
	if(to_string_cache.empty())
	{
		to_string_cache = AUX::loc2str(row, rowRelative, column, colRelative);
	}
	return to_string_cache;
}


void CellRef::fromString(const std::wstring& str)
{
	AUX::str2loc(str, row, rowRelative, column, colRelative);
	to_string_cache.clear();
}


CellRef::operator const _variant_t () const 
{
	return toString().c_str();
}


CellRef::operator const _bstr_t () const 
{
	return toString().c_str();
}


const long CellRef::getRow() const
{
	return row;
}
const long CellRef::getColumn() const
{
	return column;
}
const bool CellRef::getRowRelative() const
{
	return rowRelative;
}
const bool CellRef::getColumnRelative() const
{
	return colRelative;
}


void CellRef::operator+=(const CellRef& appended_ref)
{
	row += rowRelative ? appended_ref.getRow() : 0;
	column += colRelative ? appended_ref.getColumn() : 0;
	to_string_cache.clear();
}


void CellRef::operator-=(const CellRef& subtracted_ref)
{
	row -= rowRelative ? subtracted_ref.getRow() : 0;
	column -= colRelative ? subtracted_ref.getColumn() : 0;
	to_string_cache.clear();
}


} // namespace XLS

