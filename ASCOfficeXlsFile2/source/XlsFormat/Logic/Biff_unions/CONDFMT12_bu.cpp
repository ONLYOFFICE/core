
#include "CONDFMT12.h"
#include <Logic/Biff_records/CondFmt12.h>
#include <Logic/Biff_records/CF12.h>

namespace XLS
{


CONDFMT12::CONDFMT12()
{
}


CONDFMT12::~CONDFMT12()
{
}


BaseObjectPtr CONDFMT12::clone()
{
	return BaseObjectPtr(new CONDFMT12(*this));
}


// CONDFMT12 = CondFmt12 1*CF12
const bool CONDFMT12::loadContent(BinProcessor& proc)
{
	CondFmt12 cond_fmt12;
	if(!proc.mandatory(cond_fmt12))
	{
		return false;
	}
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt12>())
	{
		return false;
	}
	*/
	{
		CF12 cf12(cond_fmt12.getLocation());
		proc.optional(cf12);

		for(;/*infinity*/;)
		{
			CF12 cf12(cond_fmt12.getLocation());
			if(!proc.optional(cf12))
			{
				break;
			}

		}
	}
	/*  The above is equal to:
	proc.repeated<CF12>(1, 0);
	*/
	return true;

}

} // namespace XLS

