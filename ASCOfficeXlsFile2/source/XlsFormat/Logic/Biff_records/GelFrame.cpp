#include "precompiled_xls.h"
#include "GelFrame.h"

namespace XLS
{;

GelFrame::GelFrame()
{
}


GelFrame::~GelFrame()
{
}


BaseObjectPtr GelFrame::clone()
{
	return BaseObjectPtr(new GelFrame(*this));
}


void GelFrame::writeFields(CFRecord& record)
{
	record << OPT1 << OPT2;
}


void GelFrame::readFields(CFRecord& record)
{
	record >> OPT1 >> OPT2;
}

} // namespace XLS

