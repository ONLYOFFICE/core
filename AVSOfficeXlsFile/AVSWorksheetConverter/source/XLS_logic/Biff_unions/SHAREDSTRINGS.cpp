#include "stdafx.h"
#include "SHAREDSTRINGS.h"
#include <XLS_logic/Biff_records/SST.h>
#include <XLS_logic/Biff_records/Continue.h>

namespace XLS
{;


SHAREDSTRINGS::SHAREDSTRINGS(const WORD code_page)
: code_page_(code_page)
{
}


SHAREDSTRINGS::~SHAREDSTRINGS()
{
}


BaseObjectPtr SHAREDSTRINGS::clone()
{
	return BaseObjectPtr(new SHAREDSTRINGS(*this));
}


// SHAREDSTRINGS = SST *Continue
const bool SHAREDSTRINGS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory(SST(code_page_)))
	{
		return false;
	}
	proc.repeated<Continue>(0, 0);
	return true;
}

} // namespace XLS

