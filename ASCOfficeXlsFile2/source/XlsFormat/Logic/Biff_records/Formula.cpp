
#include "Formula.h"

namespace XLS
{

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
	
	_UINT16 flags = 0;
	SETBIT(flags, 0, fAlwaysCalc);
	SETBIT(flags, 2, fFill);
	SETBIT(flags, 3, fShrFmla);
	SETBIT(flags, 5, fClearErrors);
	
	record << flags;

	_UINT32 chn = 0;
	record << chn;
	formula.store(record);
	//formula.store(record, cell.getLocation());
}


void Formula::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> cell >> val >> flags;
	
	fAlwaysCalc		= GETBIT(flags, 0);
	fFill			= GETBIT(flags, 2);
	fShrFmla		= GETBIT(flags, 3);
	fClearErrors	= GETBIT(flags, 5);
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{	
		//_UINT16 chn = 0;
		//record >> chn;
	}
	else
	{
		_UINT32 chn = 0;
		record >> chn;
	}
	formula.load(record);

}


const CellRef Formula::getLocation() const
{
	return cell.getLocation();
}



} // namespace XLS

