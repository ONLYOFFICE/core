#include "stdafx.h"
#include "PlotArea.h"

namespace XLS
{;

PlotArea::PlotArea()
{
}


PlotArea::~PlotArea()
{
}


BaseObjectPtr PlotArea::clone()
{
	return BaseObjectPtr(new PlotArea(*this));
}


void PlotArea::writeFields(CFRecord& record)
{
	// no data
}


void PlotArea::readFields(CFRecord& record)
{
	// no data
}

} // namespace XLS

