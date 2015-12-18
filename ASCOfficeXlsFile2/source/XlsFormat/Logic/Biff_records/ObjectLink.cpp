
#include "ObjectLink.h"

namespace XLS
{

ObjectLink::ObjectLink()
{
}


ObjectLink::~ObjectLink()
{
}


BaseObjectPtr ObjectLink::clone()
{
	return BaseObjectPtr(new ObjectLink(*this));
}


void ObjectLink::writeFields(CFRecord& record)
{
	record << wLinkObj << wLinkVar1 << wLinkVar2;
}


void ObjectLink::readFields(CFRecord& record)
{
	record >> wLinkObj >> wLinkVar1 >> wLinkVar2;

//--------------------------------------------------
	testLinkObj = wLinkObj;
	testLinkVar1= wLinkVar1;
	testLinkVar2= wLinkVar2;
}

} // namespace XLS

