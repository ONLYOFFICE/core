#include "stdafx.h"
#include "Qsir.h"

namespace XLS
{;

Qsir::Qsir()
{
}


Qsir::~Qsir()
{
}


BaseObjectPtr Qsir::clone()
{
	return BaseObjectPtr(new Qsir(*this));
}


void Qsir::writeFields(CFRecord& record)
{
#pragma message("####################### Qsir record is not implemented")
	Log::error("Qsir record is not implemented.");
	//record << some_value;
}


void Qsir::readFields(CFRecord& record)
{
#pragma message("####################### Qsir record is not implemented")
	Log::error("Qsir record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

