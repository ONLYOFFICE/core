
#include "PrintSize.h"

namespace XLS
{

PrintSize::PrintSize()
{
}


PrintSize::~PrintSize()
{
}


BaseObjectPtr PrintSize::clone()
{
	return BaseObjectPtr(new PrintSize(*this));
}


void PrintSize::writeFields(CFRecord& record)
{
	record << printSize;
}


void PrintSize::readFields(CFRecord& record)
{
	record >> printSize;
}

} // namespace XLS

