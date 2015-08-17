
#include "LPr.h"
//#include <Exception/UnexpectedProgramPath.h>

namespace XLS
{


BaseObjectPtr LPr::clone()
{
	return BaseObjectPtr(new LPr(*this));
}


void LPr::writeFields(CFRecord& record)
{
	// The record is unused
	throw;// EXCEPT::LE::WhatIsTheFuck("LPr must appears in the intermediate XML file while it must not.", __FUNCTION__);
}


void LPr::readFields(CFRecord& record)
{
	// The record is unused
}

} // namespace XLS

