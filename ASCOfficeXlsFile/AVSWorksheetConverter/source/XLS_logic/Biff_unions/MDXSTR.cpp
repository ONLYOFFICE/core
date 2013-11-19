#include "stdafx.h"
#include "MDXSTR.h"
#include <XLS_logic/Biff_records/MDXStr.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


MDXSTR::MDXSTR()
{
}


MDXSTR::~MDXSTR()
{
}


BaseObjectPtr MDXSTR::clone()
{
	return BaseObjectPtr(new MDXSTR(*this));
}


// MDXSTR = MDXStr *ContinueFrt12
const bool MDXSTR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDXStr>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

} // namespace XLS

