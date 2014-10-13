#include "precompiled_xls.h"
#include "CrtMlFrt.h"

namespace XLS
{;

CrtMlFrt::CrtMlFrt()
{
}


CrtMlFrt::~CrtMlFrt()
{
}


BaseObjectPtr CrtMlFrt::clone()
{
	return BaseObjectPtr(new CrtMlFrt(*this));
}


void CrtMlFrt::writeFields(CFRecord& record)
{
#pragma message("####################### CrtMlFrt record is not implemented")
	Log::error("CrtMlFrt record is not implemented.");
	//record << some_value;
}


void CrtMlFrt::readFields(CFRecord& record)
{
#pragma message("####################### CrtMlFrt record is not implemented")
	Log::error("CrtMlFrt record is not implemented.");
	//record >> some_value;	
}

} // namespace XLS

