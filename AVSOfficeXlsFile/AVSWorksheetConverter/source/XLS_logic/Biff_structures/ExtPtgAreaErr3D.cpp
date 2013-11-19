#include "stdafx.h"
#include "ExtPtgAreaErr3D.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExtPtgAreaErr3D::clone()
{
	return BiffStructurePtr(new ExtPtgAreaErr3D(*this));
}


void ExtPtgAreaErr3D::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"itabFirst", iTabs.itabFirst);
	own_tag->setAttribute(L"itabLast", iTabs.itabLast);
}


const bool ExtPtgAreaErr3D::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ExtPtgAreaErr3D record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! ExtPtgAreaErr3D record has no BiffStructure::fromXML() implemented.");
	return false;
}


void ExtPtgAreaErr3D::store(CFRecord& record)
{
#pragma message("####################### ExtPtgAreaErr3D record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtPtgAreaErr3D record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtPtgAreaErr3D::load(CFRecord& record)
{
	record >> iTabs;
	record.skipNBytes(8); // unused
}


void ExtPtgAreaErr3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ExtPtgAreaErr3D record is not implemented")
	Log::info("ExtPtgAreaErr3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

