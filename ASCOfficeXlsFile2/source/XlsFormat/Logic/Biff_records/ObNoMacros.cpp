#include "precompiled_xls.h"
#include "ObNoMacros.h"

namespace XLS
{;

ObNoMacros::ObNoMacros()
{
}


ObNoMacros::~ObNoMacros()
{
}


BaseObjectPtr ObNoMacros::clone()
{
	return BaseObjectPtr(new ObNoMacros(*this));
}


void ObNoMacros::writeFields(CFRecord& record)
{
#pragma message("####################### ObNoMacros record is not implemented")
	Log::error("ObNoMacros record is not implemented.");
	//record << some_value;
}


void ObNoMacros::readFields(CFRecord& record)
{
	// no data in this tag
}

} // namespace XLS

