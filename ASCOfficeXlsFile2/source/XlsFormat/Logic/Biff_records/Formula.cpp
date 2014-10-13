#include "precompiled_xls.h"
#include "Formula.h"

namespace XLS
{;

Formula::Formula()
:	formula(false)
{
}


Formula::~Formula()
{
}


BaseObjectPtr Formula::clone()
{
	return BaseObjectPtr(new Formula(*this));
}


void Formula::writeFields(CFRecord& record)
{
	record << cell << val;
	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fAlwaysCalc);
	SETBIT(flags, 2, fFill);
	SETBIT(flags, 3, fShrFmla);
	SETBIT(flags, 5, fClearErrors);
	record << flags;

	unsigned int chn = 0;
	record << chn;
	formula.store(record);
	//formula.store(record, cell.getLocation());
}


void Formula::readFields(CFRecord& record)
{
	unsigned __int16 flags;
	record >> cell >> val >> flags;
	fAlwaysCalc = GETBIT(flags, 0);
	fFill = GETBIT(flags, 2);
	fShrFmla = GETBIT(flags, 3);
	fClearErrors = GETBIT(flags, 5);
	
	unsigned int chn = 0;
	record >> chn;
	formula.load(record);

}


const CellRef Formula::getLocation() const
{
	return cell.getLocation();
}



} // namespace XLS

