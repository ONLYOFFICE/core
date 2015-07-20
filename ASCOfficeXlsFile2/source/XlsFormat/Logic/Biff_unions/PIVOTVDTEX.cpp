
#include "PIVOTVDTEX.h"
#include <Logic/Biff_records/SXVDTEx.h>
#include <Logic/Biff_records/ContinueFrt.h>

namespace XLS
{;


PIVOTVDTEX::PIVOTVDTEX()
{
}


PIVOTVDTEX::~PIVOTVDTEX()
{
}


BaseObjectPtr PIVOTVDTEX::clone()
{
	return BaseObjectPtr(new PIVOTVDTEX(*this));
}


// PIVOTVDTEX = SXVDTEx *ContinueFrt
const bool PIVOTVDTEX::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXVDTEx>())
	{
		return false;
	}
	proc.repeated<ContinueFrt>(0, 0);
	return true;
}

} // namespace XLS

