
#include "AUTOFILTER.h"
#include <Logic/Biff_records/AutoFilterInfo.h>
#include <Logic/Biff_records/AutoFilter12.h>
#include <Logic/Biff_records/AutoFilter.h>
#include <Logic/Biff_records/ContinueFrt12.h>
#include <Logic/Biff_unions/SORTDATA12.h>

namespace XLS
{;


AUTOFILTER::AUTOFILTER()
{
}


AUTOFILTER::~AUTOFILTER()
{
}


class Parenthesis_AUTOFILTER_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AUTOFILTER_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AUTOFILTER_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if (!proc.optional<AutoFilter>())
		{
			if(!proc.mandatory<AutoFilter12>())
			{
				return false;
			}
			proc.repeated<ContinueFrt12>(0, 0);
		}
		return true;
	};
};


BaseObjectPtr AUTOFILTER::clone()
{
	return BaseObjectPtr(new AUTOFILTER(*this));
}


// AUTOFILTER = AutoFilterInfo *(AutoFilter / (AutoFilter12 *ContinueFrt12)) *SORTDATA12
const bool AUTOFILTER::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<AutoFilterInfo>()) 
	{
		return false;
	}
	proc.repeated<Parenthesis_AUTOFILTER_1>(0, 0);
	proc.repeated<SORTDATA12>(0, 0);

	return true;
}

} // namespace XLS

