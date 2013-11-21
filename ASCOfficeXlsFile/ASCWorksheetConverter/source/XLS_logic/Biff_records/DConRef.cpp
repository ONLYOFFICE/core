#include "stdafx.h"
#include "DConRef.h"

namespace XLS
{;

DConRef::DConRef()
{
}


DConRef::~DConRef()
{
}


BaseObjectPtr DConRef::clone()
{
	return BaseObjectPtr(new DConRef(*this));
}


void DConRef::writeFields(CFRecord& record)
{
#pragma message("####################### DConRef record is not implemented")
	Log::error("DConRef record is not implemented.");
	//record << some_value;
}


void DConRef::readFields(CFRecord& record)
{
#pragma message("####################### DConRef record is not implemented")
	Log::error("DConRef record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

