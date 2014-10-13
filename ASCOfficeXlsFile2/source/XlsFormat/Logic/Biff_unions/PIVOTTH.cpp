#include "precompiled_xls.h"
#include "PIVOTTH.h"
#include <Logic/Biff_records/SXTH.h>
#include <Logic/Biff_records/ContinueFrt.h>

namespace XLS
{;


PIVOTTH::PIVOTTH()
{
}


PIVOTTH::~PIVOTTH()
{
}


BaseObjectPtr PIVOTTH::clone()
{
	return BaseObjectPtr(new PIVOTTH(*this));
}


// PIVOTTH = SXTH *ContinueFrt
const bool PIVOTTH::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXTH>())
	{
		return false;
	}
	proc.repeated<ContinueFrt>(0, 0);

	return true;
}

} // namespace XLS

