#include "precompiled_xls.h"
#include "Number.h"

namespace XLS
{;

Number::Number()
{
}


Number::~Number()
{
}


BaseObjectPtr Number::clone()
{
	return BaseObjectPtr(new Number(*this));
}


void Number::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell << num;
}


void Number::readFields(CFRecord& record)
{
	record >> cell >> num;
}

const CellRef Number::getLocation() const
{
	return cell.getLocation();
}

} // namespace XLS

