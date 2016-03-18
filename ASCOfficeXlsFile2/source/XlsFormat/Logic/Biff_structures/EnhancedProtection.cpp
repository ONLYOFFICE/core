
#include "EnhancedProtection.h"
#include <Binary/CFRecord.h>

namespace XLS
{


EnhancedProtection::EnhancedProtection()
{
}


EnhancedProtection::EnhancedProtection(CFRecord& record)
{
	load(record);
}


BiffStructurePtr EnhancedProtection::clone()
{
	return BiffStructurePtr(new EnhancedProtection(*this));
}


void EnhancedProtection::store(CFRecord& record)
{
#pragma message("####################### EnhancedProtection record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EnhancedProtection record has no BiffStructure::store() implemented.");
	//record << something;
}


void EnhancedProtection::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	iprotObjects = GETBIT(flags, 0);
	iprotScenarios = GETBIT(flags, 1);
	iprotFormatCells = GETBIT(flags, 2);
	iprotFormatColumns = GETBIT(flags, 3);
	iprotFormatRows = GETBIT(flags, 4);
	iprotInsertColumns = GETBIT(flags, 5);
	iprotInsertRows = GETBIT(flags, 6);
	iprotInsertHyperlinks = GETBIT(flags, 7);
	iprotDeleteColumns = GETBIT(flags, 8);
	iprotDeleteRows = GETBIT(flags, 9);
	iprotSelLockedCells = GETBIT(flags, 10);
	iprotSort = GETBIT(flags, 11);
	iprotAutoFilter = GETBIT(flags, 12);
	iprotPivotTables = GETBIT(flags, 13);
	iprotSelUnlockedCells = GETBIT(flags, 14);
	record.skipNunBytes(2); // reserved
}


} // namespace XLS

