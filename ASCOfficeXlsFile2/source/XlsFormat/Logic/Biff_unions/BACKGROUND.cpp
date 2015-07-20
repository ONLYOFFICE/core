
#include "BACKGROUND.h"
#include <Logic/Biff_records/BkHim.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


BACKGROUND::BACKGROUND()
{
}


BACKGROUND::~BACKGROUND()
{
}


BaseObjectPtr BACKGROUND::clone()
{
	return BaseObjectPtr(new BACKGROUND(*this));
}


// BACKGROUND = BkHim *Continue
const bool BACKGROUND::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<BkHim>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

