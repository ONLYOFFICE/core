#include "stdafx.h"
#include "DOCROUTE.h"
#include <XLS_logic/Biff_records/DocRoute.h>
#include <XLS_logic/Biff_records/RecipName.h>

namespace XLS
{;


DOCROUTE::DOCROUTE()
{
}


DOCROUTE::~DOCROUTE()
{
}


BaseObjectPtr DOCROUTE::clone()
{
	return BaseObjectPtr(new DOCROUTE(*this));
}


// DOCROUTE = DocRoute 1*65535RecipName
const bool DOCROUTE::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<DocRoute>())
	{
		return false;
	}
	proc.repeated<RecipName>(1, 65535);

	return true;
}

} // namespace XLS

