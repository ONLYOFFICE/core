#include "precompiled_xls.h"
#include "PtgExp.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgExp::PtgExp()
:	Ptg(fixed_id)
{

}


BiffStructurePtr PtgExp::clone()
{
	return BiffStructurePtr(new PtgExp(*this));
}

//
//void PtgExp::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ref", RgceLoc(row, col, true, true).toString().c_str());
//}
//
//
//void PtgExp::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::wstring  cell_ref = getStructAttribute(xml_tag, L"ref");
//	RgceLoc location(cell_ref);
//	row = static_cast<unsigned __int16>(location.getRow());
//	col = static_cast<unsigned __int16>(location.getColumn());
//}


void PtgExp::storeFields(CFRecord& record)
{
	record << row << col;
}


void PtgExp::loadFields(CFRecord& record)
{
	record >> row >> col;
}


void PtgExp::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// The reference coordinates shall be obtained from row/column values.
	// No textual form need but the empty line.
	ptg_stack.push(L""); 
}


} // namespace XLS

