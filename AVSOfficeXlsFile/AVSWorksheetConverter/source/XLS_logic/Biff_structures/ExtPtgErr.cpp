#include "stdafx.h"
#include "ExtPtgErr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExtPtgErr::clone()
{
	return BiffStructurePtr(new ExtPtgErr(*this));
}


void ExtPtgErr::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"err", err);
}


const bool ExtPtgErr::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ExtPtgErr record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! ExtPtgErr record has no BiffStructure::fromXML() implemented.");
	return false;
}


void ExtPtgErr::store(CFRecord& record)
{
	record << err;
}


void ExtPtgErr::load(CFRecord& record)
{
	record >> err;
}


void ExtPtgErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(err.toString());
}


} // namespace XLS

