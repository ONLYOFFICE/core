
#include "RkRec.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr RkRec::clone()
{
	return BiffStructurePtr(new RkRec(*this));
}

//
//void RkRec::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"RK", RK_);
//}
//
//
//const bool RkRec::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### RkRec record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! RkRec record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void RkRec::store(CFRecord& record)
{
	record << ixfe << RK_;
}


void RkRec::load(CFRecord& record)
{
	record >> ixfe >> RK_;
}


const unsigned short RkRec::get_ixfe() const
{
	return ixfe;
}

} // namespace XLS

