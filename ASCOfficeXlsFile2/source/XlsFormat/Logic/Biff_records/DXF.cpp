
#include "DXF.h"

namespace XLS
{;

DXF::DXF()
{
}


DXF::~DXF()
{
}


BaseObjectPtr DXF::clone()
{
	return BaseObjectPtr(new DXF(*this));
}


void DXF::writeFields(CFRecord& record)
{
#pragma message("####################### DXF record is not implemented")
	Log::error("DXF record is not implemented.");
	//record << some_value;
}


void DXF::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	unsigned short flags;
	record >> flags >> xfprops;
	fNewBorder = GETBIT(flags, 1);
}

} // namespace XLS

