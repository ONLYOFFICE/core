#include "stdafx.h"
#include "PIVOTIVD.h"
#include <XLS_logic/Biff_records/SxIvd.h>
#include <XLS_logic/Biff_records/Continue.h>

namespace XLS
{;


PIVOTIVD::PIVOTIVD()
{
}


PIVOTIVD::~PIVOTIVD()
{
}


BaseObjectPtr PIVOTIVD::clone()
{
	return BaseObjectPtr(new PIVOTIVD(*this));
}


// PIVOTIVD = SxIvd *Continue
const bool PIVOTIVD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SxIvd>())
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);

	return true;
}

} // namespace XLS

