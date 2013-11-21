#include "stdafx.h"
#include "RkRec.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr RkRec::clone()
{
	return BiffStructurePtr(new RkRec(*this));
}


void RkRec::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"RK", RK_);
}


const bool RkRec::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### RkRec record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! RkRec record has no BiffStructure::fromXML() implemented.");
	return false;
}


void RkRec::store(CFRecord& record)
{
	record << ixfe << RK_;
}


void RkRec::load(CFRecord& record)
{
	record >> ixfe >> RK_;
}


const WORD RkRec::get_ixfe() const
{
	return ixfe;
}

} // namespace XLS

