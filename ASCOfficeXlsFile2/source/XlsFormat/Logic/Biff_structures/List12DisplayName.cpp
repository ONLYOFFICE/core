
#include "List12DisplayName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr List12DisplayName::clone()
{
	return BiffStructurePtr(new List12DisplayName(*this));
}

List12DisplayName::List12DisplayName()
{		
}


List12DisplayName::~List12DisplayName()
{
}


void List12DisplayName::store(CFRecord& record)
{	
}


void List12DisplayName::load(CFRecord& record)
{	
	record >> stListName;
	record >> stListComment;
}


} // namespace XLS

