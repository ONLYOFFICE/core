#include "stdafx.h"
#include "DVAL.h"
#include <XLS_logic/Biff_records/DVal.h>
#include <XLS_logic/Biff_records/Dv.h>

namespace XLS
{;


DVAL::DVAL()
{
}


DVAL::~DVAL()
{
}


BaseObjectPtr DVAL::clone()
{
	return BaseObjectPtr(new DVAL(*this));
}


// DVAL = DVal *65534Dv
const bool DVAL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DVal>())
	{
		return false;
	}
	proc.repeated<Dv>(0, 65534);

	return true;
}

} // namespace XLS

