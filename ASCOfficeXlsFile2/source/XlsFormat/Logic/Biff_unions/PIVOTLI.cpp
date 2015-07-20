
#include "PIVOTLI.h"
#include <Logic/Biff_records/SXLI.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


PIVOTLI::PIVOTLI()
{
}


PIVOTLI::~PIVOTLI()
{
}


BaseObjectPtr PIVOTLI::clone()
{
	return BaseObjectPtr(new PIVOTLI(*this));
}


// PIVOTLI = SXLI *Continue
const bool PIVOTLI::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXLI>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

