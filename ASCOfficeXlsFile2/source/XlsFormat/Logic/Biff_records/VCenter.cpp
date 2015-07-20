
#include "VCenter.h"

namespace XLS
{;

VCenter::VCenter()
{
}


VCenter::~VCenter()
{
}


BaseObjectPtr VCenter::clone()
{
	return BaseObjectPtr(new VCenter(*this));
}


void VCenter::writeFields(CFRecord& record)
{
	record << vcenter;
}


void VCenter::readFields(CFRecord& record)
{
	record >> vcenter;
}

} // namespace XLS

