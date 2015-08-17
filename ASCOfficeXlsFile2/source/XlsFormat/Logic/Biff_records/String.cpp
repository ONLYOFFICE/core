
#include "String.h"

namespace XLS
{

String::String()
{
}


String::~String()
{
}


BaseObjectPtr String::clone()
{
	return BaseObjectPtr(new String(*this));
}


void String::writeFields(CFRecord& record)
{
	record << string;
}


void String::readFields(CFRecord& record)
{
	record >> string;
}

} // namespace XLS

