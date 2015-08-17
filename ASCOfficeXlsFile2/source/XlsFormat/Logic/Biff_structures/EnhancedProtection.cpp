
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


//void EnhancedProtection::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"iprotObjects", iprotObjects);
//	own_tag->setAttribute(L"iprotScenarios", iprotScenarios);
//	own_tag->setAttribute(L"iprotFormatCells", iprotFormatCells);
//	own_tag->setAttribute(L"iprotFormatColumns", iprotFormatColumns);
//	own_tag->setAttribute(L"iprotFormatRows", iprotFormatRows);
//	own_tag->setAttribute(L"iprotInsertColumns", iprotInsertColumns);
//	own_tag->setAttribute(L"iprotInsertRows", iprotInsertRows);
//	own_tag->setAttribute(L"iprotInsertHyperlinks", iprotInsertHyperlinks);
//	own_tag->setAttribute(L"iprotDeleteColumns", iprotDeleteColumns);
//	own_tag->setAttribute(L"iprotDeleteRows", iprotDeleteRows);
//	own_tag->setAttribute(L"iprotSelLockedCells", iprotSelLockedCells);
//	own_tag->setAttribute(L"iprotSort", iprotSort);
//	own_tag->setAttribute(L"iprotAutoFilter", iprotAutoFilter);
//	own_tag->setAttribute(L"iprotPivotTables", iprotPivotTables);
//	own_tag->setAttribute(L"iprotSelUnlockedCells", iprotSelUnlockedCells);
//}
//
//
//const bool EnhancedProtection::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### EnhancedProtection record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! EnhancedProtection record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


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

