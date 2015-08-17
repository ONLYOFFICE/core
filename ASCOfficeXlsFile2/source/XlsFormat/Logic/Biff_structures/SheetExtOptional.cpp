
#include "SheetExtOptional.h"
#include <Binary/CFRecord.h>

namespace XLS
{


SheetExtOptional::SheetExtOptional()
{
}


SheetExtOptional::SheetExtOptional(CFRecord& record)
{
	load(record);
}


BiffStructurePtr SheetExtOptional::clone()
{
	return BiffStructurePtr(new SheetExtOptional(*this));
}


void SheetExtOptional::store(CFRecord& record)
{
#pragma message("####################### SheetExtOptional record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SheetExtOptional record has no BiffStructure::store() implemented.");
	//record << something;
}


void SheetExtOptional::load(CFRecord& record)
{
	unsigned int flags;
	record >> flags;
	icvPlain12 = static_cast<unsigned char>(GETBITS(flags, 0, 6));
	fCondFmtCalc = GETBIT(flags, 7);
	fNotPublished = GETBIT(flags, 8);
	record >> color;
}


} // namespace XLS

