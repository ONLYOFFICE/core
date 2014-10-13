#include "precompiled_xls.h"
#include "SXViewLink.h"

namespace XLS
{;

SXViewLink::SXViewLink()
{
}


SXViewLink::~SXViewLink()
{
}


BaseObjectPtr SXViewLink::clone()
{
	return BaseObjectPtr(new SXViewLink(*this));
}


void SXViewLink::writeFields(CFRecord& record)
{
#pragma message("####################### SXViewLink record is not implemented")
	Log::error("SXViewLink record is not implemented.");
	//record << some_value;
}


void SXViewLink::readFields(CFRecord& record)
{
#pragma message("####################### SXViewLink record is not implemented")
	Log::error("SXViewLink record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

