#include "precompiled_xls.h"
#include "Fbi2.h"

namespace XLS
{;

Fbi2::Fbi2()
{
}


Fbi2::~Fbi2()
{
}


BaseObjectPtr Fbi2::clone()
{
	return BaseObjectPtr(new Fbi2(*this));
}


void Fbi2::writeFields(CFRecord& record)
{
#pragma message("####################### Fbi2 record is not implemented")
	Log::error("Fbi2 record is not implemented.");
	//record << some_value;
}


void Fbi2::readFields(CFRecord& record)
{
#pragma message("####################### Fbi2 record is not implemented")
	Log::error("Fbi2 record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

