#include "precompiled_xls.h"
#include "FORMULA.h"
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Formula.h>
#include <Logic/Biff_records/Array.h>
#include <Logic/Biff_records/Table.h>
#include <Logic/Biff_records/ShrFmla.h>
#include <Logic/Biff_unions/SUB.h>
#include <Logic/Biff_records/String.h>
#include <Logic/Biff_records/Continue.h>

namespace XLS
{;


FORMULA::FORMULA(std::vector<CellRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


FORMULA::~FORMULA()
{
}


BaseObjectPtr FORMULA::clone()
{
	return BaseObjectPtr(new FORMULA(*this));
}


// FORMULA = [Uncalced] Formula [Array / Table / ShrFmla / SUB] [String *Continue]
const bool FORMULA::loadContent(BinProcessor& proc)
{
	proc.optional<Uncalced>();


	Formula formula;
	if(!proc.mandatory(formula))
	{
		return false;
	}
/*  The above is equal to:
	
	if(!proc.mandatory<Formula>())
	{
		return false;
	}
*/
	location = formula.getLocation();

	Array array_formula(location);
	bool array_present = proc.optional(array_formula);
	if(!array_present &&
		!proc.optional<Table>())
	{
		//proc.optional<ShrFmla>(); // replaced with below:
		ShrFmla shr_formula(location);
		if(proc.optional(shr_formula))
		{
			shared_formulas_locations_ref_.push_back(location);
		}
		//proc.optional<SUB>(); // I haven't found any mention about SUB in the docs
	}

	if(proc.optional<String>())
	{
		proc.repeated<Continue>(0, 0);
	}

	return true;
}

const CellRef FORMULA::getLocation() const
{
	return location;
}


} // namespace XLS

