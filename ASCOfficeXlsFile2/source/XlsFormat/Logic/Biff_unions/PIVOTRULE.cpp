
#include "PIVOTRULE.h"
#include <Logic/Biff_records/SxRule.h>
#include <Logic/Biff_unions/PRFILTER.h>

namespace XLS
{


PIVOTRULE::PIVOTRULE()
{
}


PIVOTRULE::~PIVOTRULE()
{
}


BaseObjectPtr PIVOTRULE::clone()
{
	return BaseObjectPtr(new PIVOTRULE(*this));
}


// PIVOTRULE = SxRule *PRFILTER
const bool PIVOTRULE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxRule>())
	{
		return false;
	}
	proc.repeated<PRFILTER>(0, 0);

	return true;
}

} // namespace XLS

