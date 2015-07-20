
#include "BIGNAME.h"
#include <Logic/Biff_records/BigName.h>
#include <Logic/Biff_records/ContinueBigName.h>

namespace XLS
{;


BIGNAME::BIGNAME()
{
}


BIGNAME::~BIGNAME()
{
}


BaseObjectPtr BIGNAME::clone()
{
	return BaseObjectPtr(new BIGNAME(*this));
}


// BIGNAME = BigName *ContinueBigName
const bool BIGNAME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<BigName>())
	{
		return false;
	}
	proc.repeated<ContinueBigName>(0, 0);

	return true;
}

} // namespace XLS

