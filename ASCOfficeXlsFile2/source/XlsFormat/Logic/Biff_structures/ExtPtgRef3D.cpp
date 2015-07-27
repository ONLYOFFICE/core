
#include "ExtPtgRef3D.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


ExtPtgRef3D::ExtPtgRef3D(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr ExtPtgRef3D::clone()
{
	return BiffStructurePtr(new ExtPtgRef3D(*this));
}


//void ExtPtgRef3D::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"itabFirst", iTabs.itabFirst);
//	own_tag->setAttribute(L"itabLast", iTabs.itabLast);
//	own_tag->setAttribute(L"loc", loc + cell_base_ref);
//}
//
//
//const bool ExtPtgRef3D::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExtPtgRef3D record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExtPtgRef3D record has no BiffStructure::fromXML() implemented.");
//	return false;
//}
//

void ExtPtgRef3D::store(CFRecord& record)
{
	record << iTabs << loc;
}


void ExtPtgRef3D::load(CFRecord& record)
{
	record >> iTabs >> loc;
}


void ExtPtgRef3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
#pragma message("####################### ExtPtgRef3D record is not implemented")
	Log::info("ExtPtgRef3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

