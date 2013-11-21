#include "stdafx.h"
#include "ContinueBigName.h"

namespace XLS
{;

ContinueBigName::ContinueBigName()
{
}


ContinueBigName::~ContinueBigName()
{
}


BaseObjectPtr ContinueBigName::clone()
{
	return BaseObjectPtr(new ContinueBigName(*this));
}


void ContinueBigName::writeFields(CFRecord& record)
{
#pragma message("####################### ContinueBigName record is not implemented")
	Log::error("ContinueBigName record is not implemented.");
	//record << some_value;
}


void ContinueBigName::readFields(CFRecord& record)
{
#pragma message("####################### ContinueBigName record is not implemented")
	Log::error("ContinueBigName record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

