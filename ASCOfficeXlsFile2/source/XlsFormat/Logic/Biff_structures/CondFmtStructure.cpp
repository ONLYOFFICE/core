
#include "CondFmtStructure.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr CondFmtStructure::clone()
{
	return BiffStructurePtr(new CondFmtStructure(*this));
}


void CondFmtStructure::store(CFRecord& record)
{
	record << ccf;
	unsigned short flags = 0;
	SETBIT(flags, 0, fToughRecalc);
	SETBITS(flags, 1, 15, nID);
	record << flags;
	record << refBound << sqref;
}


void CondFmtStructure::load(CFRecord& record)
{
	record >> ccf;
	unsigned short flags;
	record >> flags;
	fToughRecalc = GETBIT(flags, 0);
	nID = GETBITS(flags, 1, 15);
	record >> refBound >> sqref;
}


const CellRef CondFmtStructure::getLocation() const
{
	return sqref.getLocationFirstCell();
}

} // namespace XLS

