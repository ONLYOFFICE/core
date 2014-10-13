#include "precompiled_xls.h"
#include "PIVOTEX.h"
#include <Logic/Biff_records/SXEx.h>
#include <Logic/Biff_unions/PIVOTSELECT.h>
#include <Logic/Biff_unions/PIVOTFORMAT.h>

namespace XLS
{;


PIVOTEX::PIVOTEX()
{
}


PIVOTEX::~PIVOTEX()
{
}


BaseObjectPtr PIVOTEX::clone()
{
	return BaseObjectPtr(new PIVOTEX(*this));
}


// PIVOTEX = SXEx *PIVOTSELECT *PIVOTFORMAT
const bool PIVOTEX::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXEx>())
	{
		return false;
	}
	proc.repeated<PIVOTSELECT>(0, 0);
	proc.repeated<PIVOTFORMAT>(0, 0);

	return true;
}

} // namespace XLS

