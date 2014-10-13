#include "precompiled_xls.h"
#include "SCENARIOS.h"
#include <Logic/Biff_records/ScenMan.h>
#include <Logic/Biff_records/SCENARIO.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


SCENARIOS::SCENARIOS()
{
}


SCENARIOS::~SCENARIOS()
{
}


class Parenthesis_SCENARIOS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SCENARIOS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SCENARIOS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SCENARIO>())
		{
			return false;
		}
		proc.repeated<Continue>(0, 0);
		return true;
	};
};


BaseObjectPtr SCENARIOS::clone()
{
	return BaseObjectPtr(new SCENARIOS(*this));
}


// SCENARIOS = ScenMan *(SCENARIO *Continue)
const bool SCENARIOS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<ScenMan>())
	{
		return false;
	}
	proc.repeated<Parenthesis_SCENARIOS_1>(0, 0);

	return true;
}

} // namespace XLS

