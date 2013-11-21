#include "stdafx.h"
#include "Qsif.h"

namespace XLS
{;

Qsif::Qsif()
{
}


Qsif::~Qsif()
{
}


BaseObjectPtr Qsif::clone()
{
	return BaseObjectPtr(new Qsif(*this));
}


void Qsif::writeFields(CFRecord& record)
{
#pragma message("####################### Qsif record is not implemented")
	Log::error("Qsif record is not implemented.");
	//record << some_value;
}


void Qsif::readFields(CFRecord& record)
{
#pragma message("####################### Qsif record is not implemented")
	Log::error("Qsif record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

