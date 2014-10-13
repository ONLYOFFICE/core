#include "precompiled_xls.h"
#include "SHAREDSTRINGS.h"
#include <Logic/Biff_records/SST.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


SHAREDSTRINGS::SHAREDSTRINGS(const unsigned __int16 code_page)
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

