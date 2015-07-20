
#include "SerToCrt.h"

namespace XLS
{;

SerToCrt::SerToCrt()
{
}


SerToCrt::~SerToCrt()
{
}


BaseObjectPtr SerToCrt::clone()
{
	return BaseObjectPtr(new SerToCrt(*this));
}


void SerToCrt::writeFields(CFRecord& record)
{
	record << id;
}


void SerToCrt::readFields(CFRecord& record)
{
	record >> id;
}

} // namespace XLS

