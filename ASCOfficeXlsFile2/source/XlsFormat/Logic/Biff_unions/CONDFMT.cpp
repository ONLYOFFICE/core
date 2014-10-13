#include "precompiled_xls.h"
#include "CONDFMT.h"
#include <Logic/Biff_records/CondFmt.h>
#include <Logic/Biff_records/CF.h>

namespace XLS
{;


CONDFMT::CONDFMT()
{
}


CONDFMT::~CONDFMT()
{
}


BaseObjectPtr CONDFMT::clone()
{
	return BaseObjectPtr(new CONDFMT(*this));
}


// CONDFMT = CondFmt 1*3CF
const bool CONDFMT::loadContent(BinProcessor& proc)
{
	CondFmt cond_fmt;
	if(!proc.mandatory(cond_fmt))
	{
		return false;
	}
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt>())
	{
		return false;
	}
	*/
	{
		CF cf1(cond_fmt.getLocation());
		proc.optional(cf1);

		CF cf2(cond_fmt.getLocation());
		if(proc.optional(cf2))
		{
			CF cf3(cond_fmt.getLocation());
			proc.optional(cf3);
		}
	}
	/*  The above is equal to:
		proc.repeated<CF>(1, 3);
	*/
	return true;
}

} // namespace XLS

