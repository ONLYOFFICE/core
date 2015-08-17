
#include "FNGROUPS.h"
#include <Logic/Biff_records/BuiltInFnGroupCount.h>
#include <Logic/Biff_records/FnGroupName.h>
#include <Logic/Biff_records/FnGrp12.h>

namespace XLS
{


FNGROUPS::FNGROUPS()
{
}


FNGROUPS::~FNGROUPS()
{
}


BaseObjectPtr FNGROUPS::clone()
{
	return BaseObjectPtr(new FNGROUPS(*this));
}


// FNGROUPS = BuiltInFnGroupCount *FnGroupName *FnGrp12
const bool FNGROUPS::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<BuiltInFnGroupCount>())
	{
		return false;
	}
	proc.repeated<FnGroupName>(0, 0);
	proc.repeated<FnGrp12>(0, 0);

	return true;
}

} // namespace XLS

