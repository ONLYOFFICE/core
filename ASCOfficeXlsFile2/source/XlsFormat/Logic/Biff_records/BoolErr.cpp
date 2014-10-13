#include "precompiled_xls.h"
#include "BoolErr.h"

namespace XLS
{;

BoolErr::BoolErr()
{
}


BoolErr::~BoolErr()
{
}


BaseObjectPtr BoolErr::clone()
{
	return BaseObjectPtr(new BoolErr(*this));
}


void BoolErr::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell << bes;
}


void BoolErr::readFields(CFRecord& record)
{
	record >> cell >> bes;
}

const CellRef BoolErr::getLocation() const
{
	return cell.getLocation();
}

} // namespace XLS

