#include "precompiled_xls.h"
#include "MDTINFO.h"
#include <Logic/Biff_records/MDTInfo.h>
#include <Logic/Biff_records/ContinueFrt12.h>

namespace XLS
{;


MDTINFO::MDTINFO()
{
}


MDTINFO::~MDTINFO()
{
}


BaseObjectPtr MDTINFO::clone()
{
	return BaseObjectPtr(new MDTINFO(*this));
}


// MDTINFO = MDTInfo *ContinueFrt12
const bool MDTINFO::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDTInfo>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

} // namespace XLS

