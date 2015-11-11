
#include "SHFMLA_SET.h"
#include <Logic/Biff_structures/Cell.h>

namespace XLS
{


SHFMLA_SET::SHFMLA_SET(std::vector<CellRangeRef>& shared_formulas_locations_ref) : 
				shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


SHFMLA_SET::~SHFMLA_SET()
{
}


BaseObjectPtr SHFMLA_SET::clone()
{
	return BaseObjectPtr(new SHFMLA_SET(*this));
}


const bool SHFMLA_SET::loadContent(BinProcessor& proc)
{
	return true;
}



} // namespace XLS

