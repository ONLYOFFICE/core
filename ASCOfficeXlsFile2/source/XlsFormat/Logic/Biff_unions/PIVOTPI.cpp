
#include "PIVOTPI.h"
#include <Logic/Biff_records/SXPI.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{


PIVOTPI::PIVOTPI()
{
}


PIVOTPI::~PIVOTPI()
{
}


BaseObjectPtr PIVOTPI::clone()
{
	return BaseObjectPtr(new PIVOTPI(*this));
}


// PIVOTPI = SXPI *Continue
const bool PIVOTPI::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXPI>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

