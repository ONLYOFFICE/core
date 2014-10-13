#include "precompiled_xls.h"
#include "SXString.h"

namespace XLS
{;

SXString::SXString()
{
}


SXString::~SXString()
{
}


BaseObjectPtr SXString::clone()
{
	return BaseObjectPtr(new SXString(*this));
}


void SXString::writeFields(CFRecord& record)
{
#pragma message("####################### SXString record is not implemented")
	Log::error("SXString record is not implemented.");
	//record << some_value;
}


void SXString::readFields(CFRecord& record)
{
#pragma message("####################### SXString record is not implemented")
	Log::error("SXString record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

