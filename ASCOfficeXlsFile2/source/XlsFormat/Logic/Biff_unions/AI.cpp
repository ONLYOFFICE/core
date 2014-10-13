#include "precompiled_xls.h"
#include "AI.h"
#include <Logic/Biff_records/BRAI.h>
#include <Logic/Biff_records/SeriesText.h>

//#include <Logic/Biff_unions/PAGESETUP.h>

namespace XLS
{;


AI::AI()
{
}


AI::~AI()
{
}


BaseObjectPtr AI::clone()
{
	return BaseObjectPtr(new AI(*this));
}


// AI = BRAI [SeriesText]
const bool AI::loadContent(BinProcessor& proc)
{
	/*if(!proc.mandatory<BRAI>())
	{
		return false;
	}*/

	// fix
	if(!proc.optional<BRAI>())
	{
		return false;
	}

	proc.optional<SeriesText>();

	return true;
}

} // namespace XLS

