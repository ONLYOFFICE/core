#include "precompiled_xls.h"
#include "DConBin.h"

namespace XLS
{;

DConBin::DConBin()
{
}


DConBin::~DConBin()
{
}


BaseObjectPtr DConBin::clone()
{
	return BaseObjectPtr(new DConBin(*this));
}


void DConBin::writeFields(CFRecord& record)
{
#pragma message("####################### DConBin record is not implemented")
	Log::error("DConBin record is not implemented.");
	//record << some_value;
}


void DConBin::readFields(CFRecord& record)
{
#pragma message("####################### DConBin record is not implemented")
	Log::error("DConBin record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

