
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

