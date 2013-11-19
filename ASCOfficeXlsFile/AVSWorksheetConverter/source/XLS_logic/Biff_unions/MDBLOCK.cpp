#include "stdafx.h"
#include "MDBLOCK.h"
#include <XLS_logic/Biff_records/MDB.h>
#include <XLS_logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


MDBLOCK::MDBLOCK()
{
}


MDBLOCK::~MDBLOCK()
{
}


BaseObjectPtr MDBLOCK::clone()
{
	return BaseObjectPtr(new MDBLOCK(*this));
}


// MDBLOCK = MDB *ContinueFrt12
const bool MDBLOCK::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDB>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

} // namespace XLS

