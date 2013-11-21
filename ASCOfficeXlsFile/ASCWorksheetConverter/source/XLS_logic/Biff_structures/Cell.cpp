#include "stdafx.h"
#include "Cell.h"
#include "CellRef.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


Cell::Cell()
{
}


Cell::Cell(const WORD rw_init, const WORD col_init, const WORD ixfe_init)
{
	rw = rw_init;
	col = col_init;
	ixfe = ixfe_init;
}


BiffStructurePtr Cell::clone()
{
	return BiffStructurePtr(new Cell(*this));
}


void Cell::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
// 	static _bstr_t rw_str(L"rw");
// 	static _bstr_t col_str(L"col");
	static _bstr_t ixfe_str(L"ixfe");
	static _bstr_t ref_str(L"ref");
// 	xml_tag->setAttribute(rw_str, rw);
// "col" attribute is not used in XSLT
// 	xml_tag->setAttribute(col_str, col);
	xml_tag->setAttribute(ixfe_str, ixfe);
	xml_tag->setAttribute(ref_str, getLocation().toString().c_str());
}


void Cell::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	static _bstr_t rw_str(L"rw");
	static _bstr_t col_str(L"col");
	static _bstr_t ixfe_str(L"ixfe");
	rw = getStructAttribute(xml_tag, rw_str);
	col = getStructAttribute(xml_tag, col_str);
	ixfe = getStructAttribute(xml_tag, ixfe_str);
}


void Cell::store(CFRecord& record)
{
	record << rw << col << ixfe;
}


void Cell::load(CFRecord& record)
{
	record >> rw >> col >> ixfe;
}


const CellRef Cell::getLocation() const
{
	return RgceLoc(rw, col, true, true);
}


} // namespace XLS
