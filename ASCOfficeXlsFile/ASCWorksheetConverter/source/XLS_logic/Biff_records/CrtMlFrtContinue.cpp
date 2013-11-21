#include "stdafx.h"
#include "CrtMlFrtContinue.h"

namespace XLS
{;

CrtMlFrtContinue::CrtMlFrtContinue()
{
}


CrtMlFrtContinue::~CrtMlFrtContinue()
{
}


BaseObjectPtr CrtMlFrtContinue::clone()
{
	return BaseObjectPtr(new CrtMlFrtContinue(*this));
}


void CrtMlFrtContinue::writeFields(CFRecord& record)
{
#pragma message("####################### CrtMlFrtContinue record is not implemented")
	Log::error("CrtMlFrtContinue record is not implemented.");
	//record << some_value;
}


void CrtMlFrtContinue::readFields(CFRecord& record)
{
#pragma message("####################### CrtMlFrtContinue record is not implemented")
	Log::error("CrtMlFrtContinue record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

