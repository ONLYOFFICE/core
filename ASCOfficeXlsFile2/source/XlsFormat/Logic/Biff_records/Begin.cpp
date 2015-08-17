
#include "Begin.h"

namespace XLS
{

Begin::Begin()
{
}


Begin::~Begin()
{
}


BaseObjectPtr Begin::clone()
{
	return BaseObjectPtr(new Begin(*this));
}


void Begin::writeFields(CFRecord& record)
{
	// no data
}


void Begin::readFields(CFRecord& record)
{
	// no data
}

} // namespace XLS

