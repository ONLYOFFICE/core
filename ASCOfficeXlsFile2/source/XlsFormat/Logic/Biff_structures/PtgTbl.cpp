#include "precompiled_xls.h"
#include "PtgTbl.h"
#include <Binary/CFRecord.h>

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


//void PtgTbl::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ref", RgceLoc(row, col, true, true).toString().c_str());
//}
//
//
//void PtgTbl::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::wstring  cell_ref = getStructAttribute(xml_tag, L"ref");
//	RgceLoc location(cell_ref);
//	row = static_cast<unsigned __int16>(location.getRow());
//	col = static_cast<unsigned __int16>(location.getColumn());
//}


void PtgTbl::storeFields(CFRecord& record)
{
	record << row << col;
}


void PtgTbl::loadFields(CFRecord& record)
{
	record >> row >> col;
}


void PtgTbl::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// The reference coordinates shall be obtained from row/column values.
	// No textual form need but the empty line.
	ptg_stack.push(L""); 
}


} // namespace XLS

