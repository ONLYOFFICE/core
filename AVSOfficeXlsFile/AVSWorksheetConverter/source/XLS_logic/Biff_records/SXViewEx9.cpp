#include "stdafx.h"
#include "SXViewEx9.h"

namespace XLS
{;

SXViewEx9::SXViewEx9()
{
}


SXViewEx9::~SXViewEx9()
{
}


BaseObjectPtr SXViewEx9::clone()
{
	return BaseObjectPtr(new SXViewEx9(*this));
}


void SXViewEx9::writeFields(CFRecord& record)
{
#pragma message("####################### SXViewEx9 record is not implemented")
	Log::error("SXViewEx9 record is not implemented.");
	//record << some_value;
}


void SXViewEx9::readFields(CFRecord& record)
{
#pragma message("####################### SXViewEx9 record is not implemented")
	Log::error("SXViewEx9 record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

