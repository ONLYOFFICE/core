
#include "FeatFormulaErr2.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FeatFormulaErr2::FeatFormulaErr2()
{
}


FeatFormulaErr2::FeatFormulaErr2(CFRecord& record)
{
	load(record);
}


BiffStructurePtr FeatFormulaErr2::clone()
{
	return BiffStructurePtr(new FeatFormulaErr2(*this));
}


void FeatFormulaErr2::store(CFRecord& record)
{
	record << grffecIgnore;
}


void FeatFormulaErr2::load(CFRecord& record)
{
	record >> grffecIgnore;
}


} // namespace XLS

