
#include "HCenter.h"

namespace XLS
{;

HCenter::HCenter()
{
}


HCenter::~HCenter()
{
}


BaseObjectPtr HCenter::clone()
{
	return BaseObjectPtr(new HCenter(*this));
}


void HCenter::writeFields(CFRecord& record)
{
	record << hcenter;
}


void HCenter::readFields(CFRecord& record)
{
	record >> hcenter;
}

} // namespace XLS

