
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
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		LPAnsiString s;
		record >> s;
		string = s;
	}
	else
		record >> string;
}

} // namespace XLS

