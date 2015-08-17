
#include "ExtPtgArea3D.h"
#include <Binary/CFRecord.h>
#include "CellRangeRef.h"

namespace XLS
{


ExtPtgArea3D::ExtPtgArea3D(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr ExtPtgArea3D::clone()
{
	return BiffStructurePtr(new ExtPtgArea3D(*this));
}

//
//void ExtPtgArea3D::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"itabFirst", iTabs.itabFirst);
//	own_tag->setAttribute(L"itabLast", iTabs.itabLast);
//	own_tag->setAttribute(L"loc", area + cell_base_ref);
//}
//

//const bool ExtPtgArea3D::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExtPtgArea3D record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExtPtgArea3D record has no BiffStructure::fromXML() implemented.");
//	return false;
//	area -= cell_base_ref;
//}


void ExtPtgArea3D::store(CFRecord& record)
{
	record << iTabs << area;
}


void ExtPtgArea3D::load(CFRecord& record)
{
	record >> iTabs >> area;
}


void ExtPtgArea3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
#pragma message("####################### ExtPtgArea3D record is not implemented")
	Log::info("ExtPtgArea3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

