
#include "List12.h"

namespace XLS
{

List12::List12()
{
}


List12::~List12()
{
}


BaseObjectPtr List12::clone()
{
	return BaseObjectPtr(new List12(*this));
}


void List12::writeFields(CFRecord& record)
{
}


void List12::readFields(CFRecord& record)
{
	record >> frtHeader;
	record >> lsd;
	record >> idList;

	unsigned short _lsd = lsd;

	switch (lsd)
	{
		case 0:
			record >> rgbList12BlockLevel;
			break;
		case 1:
			record >> rgbList12TableStyleClientInfo;
			break;
		case 2:
			record >> rgbList12DisplayName;
			break;
	}
}

} // namespace XLS

