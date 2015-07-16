#include "precompiled_xls.h"
#include "NoteSh.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


NoteSh::NoteSh()
{
}


NoteSh::NoteSh(CFRecord& record)
{
	load(record);
}


BiffStructurePtr NoteSh::clone()
{
	return BiffStructurePtr(new NoteSh(*this));
}

//
//void NoteSh::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"row", row);
//	own_tag->setAttribute(L"col", col);
//	own_tag->setAttribute(L"ref", CellRef(row, col, true, true));
//	own_tag->setAttribute(L"fShow", fShow);
//	own_tag->setAttribute(L"fRwHidden", fRwHidden);
//	own_tag->setAttribute(L"fColHidden", fColHidden);
//	own_tag->setAttribute(L"idObj", idObj);
//	own_tag->setAttribute(L"stAuthor", stAuthor);
//}


//const bool NoteSh::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### NoteSh record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! NoteSh record has no BiffStructure::fromXML() implemented.");
//	return false;
//}
//

void NoteSh::store(CFRecord& record)
{
#pragma message("####################### NoteSh record has no BiffStructure::store() implemented")
	Log::error(" Error!!! NoteSh record has no BiffStructure::store() implemented.");
	//record << something;
}


void NoteSh::load(CFRecord& record)
{
	unsigned short flags;
	record >> row >> col >> flags;
	fShow = GETBIT(flags, 1);
	fRwHidden = GETBIT(flags, 7);
	fColHidden = GETBIT(flags, 8);

	record >> idObj >> stAuthor;
	record.skipNunBytes(1); // unused
}


} // namespace XLS

