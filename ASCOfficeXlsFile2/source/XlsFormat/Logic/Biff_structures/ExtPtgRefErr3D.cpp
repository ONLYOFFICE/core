#include "precompiled_xls.h"
#include "ExtPtgRefErr3D.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExtPtgRefErr3D::clone()
{
	return BiffStructurePtr(new ExtPtgRefErr3D(*this));
}

//
//void ExtPtgRefErr3D::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"itabFirst", iTabs.itabFirst);
//	own_tag->setAttribute(L"itabLast", iTabs.itabLast);
//}
//
//
//const bool ExtPtgRefErr3D::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExtPtgRefErr3D record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExtPtgRefErr3D record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void ExtPtgRefErr3D::store(CFRecord& record)
{
#pragma message("####################### ExtPtgRefErr3D record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtPtgRefErr3D record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtPtgRefErr3D::load(CFRecord& record)
{
	record >> iTabs;
	record.skipNunBytes(4); // unused
}


void ExtPtgRefErr3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
#pragma message("####################### ExtPtgRefErr3D record is not implemented")
	Log::info("ExtPtgRefErr3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

