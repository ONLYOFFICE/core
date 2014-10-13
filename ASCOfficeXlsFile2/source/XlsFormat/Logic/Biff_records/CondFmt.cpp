#include "precompiled_xls.h"
#include "CondFmt.h"

namespace XLS
{;

CondFmt::CondFmt()
{
}


CondFmt::~CondFmt()
{
}


BaseObjectPtr CondFmt::clone()
{
	return BaseObjectPtr(new CondFmt(*this));
}


void CondFmt::writeFields(CFRecord& record)
{
	unsigned __int16 flags;
	record << ccf;
	SETBIT(flags, 0, fToughRecalc);
	SETBITS(flags, 1, 15, nID);
	record << flags;

	Ref8U refBound_ref(refBound);
	record << refBound_ref << sqref;

}


void CondFmt::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> ccf >> flags;
	fToughRecalc = GETBIT(flags, 0);
	nID = GETBITS(flags, 1, 15);
	Ref8U refBound_ref;
	record >> refBound_ref >> sqref;
	refBound = static_cast<std::wstring >(refBound_ref);
}


const CellRef CondFmt::getLocation() const
{
	return sqref.getLocationFirstCell();
}


} // namespace XLS

