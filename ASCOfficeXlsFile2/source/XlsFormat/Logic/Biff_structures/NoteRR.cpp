
#include "NoteRR.h"
#include <Binary/CFRecord.h>

namespace XLS
{


NoteRR::NoteRR()
{
}


NoteRR::NoteRR(CFRecord& record)
{
	load(record);
}


BiffStructurePtr NoteRR::clone()
{
	return BiffStructurePtr(new NoteRR(*this));
}


//void NoteRR::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	rrd.toXML(own_tag);
//	own_tag->setAttribute(L"bitfDelNote", bitfDelNote);
//	own_tag->setAttribute(L"bitfAddNote", bitfAddNote);
//	own_tag->setAttribute(L"row", row);
//	own_tag->setAttribute(L"col", col);
//	own_tag->setAttribute(L"fShow", fShow);
//	own_tag->setAttribute(L"fRwHidden", fRwHidden);
//	own_tag->setAttribute(L"fColHidden", fColHidden);
//	own_tag->setAttribute(L"guid", STR::guid2bstr(guid));
//	own_tag->setAttribute(L"ichEnd", ichEnd);
//	own_tag->setAttribute(L"cchNote", cchNote);
//	own_tag->setAttribute(L"stAuthor", stAuthor);
//}
//
//
//const bool NoteRR::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### NoteRR record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! NoteRR record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void NoteRR::store(CFRecord& record)
{
#pragma message("####################### NoteRR record has no BiffStructure::store() implemented")
	Log::error(" Error!!! NoteRR record has no BiffStructure::store() implemented.");
	//record << something;
}


void NoteRR::load(CFRecord& record)
{
	record >> rrd;
	unsigned short flags;
	record >> flags;
	bitfDelNote = GETBIT(flags, 0);
	bitfAddNote = GETBIT(flags, 1);
	
	record >> row >> col >> flags;
	fShow = GETBIT(flags, 1);
	fRwHidden = GETBIT(flags, 7);
	fColHidden = GETBIT(flags, 8);
	
	record >> guid >> ichEnd >> cchNote >> stAuthor;
	
	record.skipNunBytes(2); // unused
}


} // namespace XLS

