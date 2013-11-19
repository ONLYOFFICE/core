#include "stdafx.h"
#include "PIVOTEX.h"
#include <XLS_logic/Biff_records/SXEx.h>
#include <XLS_logic/Biff_unions/PIVOTSELECT.h>
#include <XLS_logic/Biff_unions/PIVOTFORMAT.h>

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

