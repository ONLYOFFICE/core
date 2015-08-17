
#include "Qsi.h"

namespace XLS
{

Qsi::Qsi()
{
}


Qsi::~Qsi()
{
}


BaseObjectPtr Qsi::clone()
{
	return BaseObjectPtr(new Qsi(*this));
}


void Qsi::writeFields(CFRecord& record)
{
#pragma message("####################### Qsi record is not implemented")
	Log::error("Qsi record is not implemented.");
	//record << some_value;
}


void Qsi::readFields(CFRecord& record)
{
#pragma message("####################### Qsi record is not implemented")
	Log::error("Qsi record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

