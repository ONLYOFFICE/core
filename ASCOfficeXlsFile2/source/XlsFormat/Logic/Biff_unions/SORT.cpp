
#include "SORT.h"
#include <Logic/Biff_records/RRSort.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


SORT::SORT()
{
}


SORT::~SORT()
{
}


BaseObjectPtr SORT::clone()
{
	return BaseObjectPtr(new SORT(*this));
}


// SORT = RRSort *Continue
const bool SORT::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<RRSort>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

