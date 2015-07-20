
#include "MDXSTR.h"
#include <Logic/Biff_records/MDXStr.h>
#include <Logic/Biff_records/ContinueFrt12.h>

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

