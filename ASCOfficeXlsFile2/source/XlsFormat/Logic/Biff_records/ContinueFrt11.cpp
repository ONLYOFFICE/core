#include "precompiled_xls.h"
#include "ContinueFrt11.h"

namespace XLS
{;

ContinueFrt11::ContinueFrt11()
{
}


ContinueFrt11::~ContinueFrt11()
{
}


BaseObjectPtr ContinueFrt11::clone()
{
	return BaseObjectPtr(new ContinueFrt11(*this));
}


void ContinueFrt11::writeFields(CFRecord& record)
{
}


void ContinueFrt11::readFields(CFRecord& record)
{
}

} // namespace XLS

