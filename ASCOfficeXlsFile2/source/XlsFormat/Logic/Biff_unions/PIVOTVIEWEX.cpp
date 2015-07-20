
#include "PIVOTVIEWEX.h"
#include <Logic/Biff_records/SXViewEx.h>
#include <Logic/Biff_records/SXPIEx.h>
#include <Logic/Biff_unions/PIVOTTH.h>
#include <Logic/Biff_unions/PIVOTVDTEX.h>

namespace XLS
{;


PIVOTVIEWEX::PIVOTVIEWEX()
{
}


PIVOTVIEWEX::~PIVOTVIEWEX()
{
}


BaseObjectPtr PIVOTVIEWEX::clone()
{
	return BaseObjectPtr(new PIVOTVIEWEX(*this));
}


// PIVOTVIEWEX = SXViewEx *PIVOTTH *SXPIEx *PIVOTVDTEX
const bool PIVOTVIEWEX::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXViewEx>())
	{
		return false;
	}
	proc.repeated<PIVOTTH>(0, 0);
	proc.repeated<SXPIEx>(0, 0);
	proc.repeated<PIVOTVDTEX>(0, 0);

	return true;
}

} // namespace XLS

