#include "stdafx.h"
#include "SORTDATA12.h"
#include <XLS_logic/Biff_records/SortData.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


SORTDATA12::SORTDATA12()
{
}


SORTDATA12::~SORTDATA12()
{
}


BaseObjectPtr SORTDATA12::clone()
{
	return BaseObjectPtr(new SORTDATA12(*this));
}


// SORTDATA12 = SortData *ContinueFrt12
const bool SORTDATA12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SortData>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0); // processed inside of SortData

	return true;
}

} // namespace XLS

