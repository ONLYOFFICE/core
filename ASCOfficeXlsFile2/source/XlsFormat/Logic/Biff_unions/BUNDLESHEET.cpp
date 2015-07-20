
#include "BUNDLESHEET.h"
#include <Logic/Biff_records/BoundSheet8.h>

namespace XLS
{;


BUNDLESHEET::BUNDLESHEET()
{
}


BUNDLESHEET::~BUNDLESHEET()
{
}


BaseObjectPtr BUNDLESHEET::clone()
{
	return BaseObjectPtr(new BUNDLESHEET(*this));
}


// BUNDLESHEET = BoundSheet8
const bool BUNDLESHEET::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<BoundSheet8>())
	{
		return false;
	}
	return true;
}

} // namespace XLS

