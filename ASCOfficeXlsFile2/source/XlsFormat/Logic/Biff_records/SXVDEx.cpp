
#include "SXVDEx.h"

namespace XLS
{

SXVDEx::SXVDEx()
{
}


SXVDEx::~SXVDEx()
{
}


BaseObjectPtr SXVDEx::clone()
{
	return BaseObjectPtr(new SXVDEx(*this));
}


void SXVDEx::writeFields(CFRecord& record)
{
#pragma message("####################### SXVDEx record is not implemented")
	Log::error("SXVDEx record is not implemented.");
	//record << some_value;
}


void SXVDEx::readFields(CFRecord& record)
{
#pragma message("####################### SXVDEx record is not implemented")
	Log::error("SXVDEx record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

