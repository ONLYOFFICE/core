
#include "Axis.h"

namespace XLS
{;

Axis::Axis()
{
}


Axis::~Axis()
{
}


BaseObjectPtr Axis::clone()
{
	return BaseObjectPtr(new Axis(*this));
}


void Axis::writeFields(CFRecord& record)
{
	record << wType;
	record.reserveNunBytes(16); // reserved
}


void Axis::readFields(CFRecord& record)
{
	record >> wType;
	record.skipNunBytes(16); // reserved
}

} // namespace XLS

