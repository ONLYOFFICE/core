
#include "ObProj.h"

namespace XLS
{;

ObProj::ObProj()
{
}


ObProj::~ObProj()
{
}


BaseObjectPtr ObProj::clone()
{
	return BaseObjectPtr(new ObProj(*this));
}


void ObProj::writeFields(CFRecord& record)
{
#pragma message("####################### ObProj record is not implemented")
	Log::error("ObProj record is not implemented.");
	//record << some_value;
}


void ObProj::readFields(CFRecord& record)
{
	// the tag doesn't contain data
}

} // namespace XLS

