
#include "BuiltInStyle.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr BuiltInStyle::clone()
{
	return BiffStructurePtr(new BuiltInStyle(*this));
}


void BuiltInStyle::store(CFRecord& record)
{
	record << istyBuiltIn << iLevel;
}


void BuiltInStyle::load(CFRecord& record)
{
	record >> istyBuiltIn >> iLevel;
}


} // namespace XLS
