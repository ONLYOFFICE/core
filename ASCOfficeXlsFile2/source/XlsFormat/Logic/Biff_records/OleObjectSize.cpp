
#include "OleObjectSize.h"

namespace XLS
{;

OleObjectSize::OleObjectSize()
{
}


OleObjectSize::~OleObjectSize()
{
}


BaseObjectPtr OleObjectSize::clone()
{
	return BaseObjectPtr(new OleObjectSize(*this));
}


void OleObjectSize::writeFields(CFRecord& record)
{
	record << ref_;
}


void OleObjectSize::readFields(CFRecord& record)
{
	record >> ref_;
}

} // namespace XLS

