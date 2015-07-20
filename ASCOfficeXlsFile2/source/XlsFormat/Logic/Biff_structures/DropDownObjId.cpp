
#include "DropDownObjId.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DropDownObjId::clone()
{
	return BiffStructurePtr(new DropDownObjId(*this));
}


//void DropDownObjId::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"id", id);
//}
//
//
//const bool DropDownObjId::fromXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = parent->selectSingleNode(getClassName());
//	if(!own_tag)
//	{
//		return false;
//	}
//	id = getStructAttribute(own_tag, L"id");
//	return true;
//}


void DropDownObjId::store(CFRecord& record)
{
	record << id;
}


void DropDownObjId::load(CFRecord& record)
{
	record >> id;
}


} // namespace XLS

