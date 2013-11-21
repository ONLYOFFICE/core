#include "stdafx.h"
#include "PtgTbl.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgTbl::PtgTbl()
:	Ptg(fixed_id)
{
}


BiffStructurePtr PtgTbl::clone()
{
	return BiffStructurePtr(new PtgTbl(*this));
}


void PtgTbl::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"ref", RgceLoc(row, col, true, true).toString().c_str());
}


void PtgTbl::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	_bstr_t cell_ref = getStructAttribute(xml_tag, L"ref");
	RgceLoc location(cell_ref);
	row = static_cast<WORD>(location.getRow());
	col = static_cast<WORD>(location.getColumn());
}


void PtgTbl::storeFields(CFRecord& record)
{
	record << row << col;
}


void PtgTbl::loadFields(CFRecord& record)
{
	record >> row >> col;
}


void PtgTbl::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// The reference coordinates shall be obtained from row/column values.
	// No textual form need but the empty line.
	ptg_stack.push(L""); 
}


} // namespace XLS

