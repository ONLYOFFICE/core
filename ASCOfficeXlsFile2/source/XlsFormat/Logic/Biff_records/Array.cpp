//
#include "Array.h"

namespace XLS
{

Array::Array(const CellRef& cell_base_ref_init)
:	formula(false, cell_base_ref_init)
{
}

Array::~Array()
{
}


BaseObjectPtr Array::clone()
{
	return BaseObjectPtr(new Array(*this));
}


void Array::writeFields(CFRecord& record)
{
	record << ref_;
	unsigned short flags = 0;
	SETBIT(flags, 0, fAlwaysCalc);
	record << flags;
	record.reserveNunBytes(4); // unused
	formula.store(record);
}


void Array::readFields(CFRecord& record)
{
	record >> ref_;
	unsigned short flags;
	record >> flags;
	fAlwaysCalc = GETBIT(flags, 0);
	record.skipNunBytes(4); // unused
	formula.load(record);
}

} // namespace XLS

