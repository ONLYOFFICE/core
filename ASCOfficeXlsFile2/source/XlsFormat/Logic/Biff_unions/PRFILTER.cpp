#include "precompiled_xls.h"
#include "PRFILTER.h"
#include <Logic/Biff_records/SxFilt.h>
#include <Logic/Biff_records/SxItm.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


PRFILTER::PRFILTER()
{
}


PRFILTER::~PRFILTER()
{
}


BaseObjectPtr PRFILTER::clone()
{
	return BaseObjectPtr(new PRFILTER(*this));
}


// PRFILTER = SxFilt [SxItm *Continue]
const bool PRFILTER::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxFilt>())
	{
		return false;
	}
	if(proc.optional<SxItm>())
	{
		proc.repeated<Continue>(0, 0);
	}

	return true;
}

} // namespace XLS

