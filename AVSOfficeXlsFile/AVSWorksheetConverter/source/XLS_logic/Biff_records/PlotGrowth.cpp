#include "stdafx.h"
#include "PlotGrowth.h"

namespace XLS
{;

PlotGrowth::PlotGrowth()
{
}


PlotGrowth::~PlotGrowth()
{
}


BaseObjectPtr PlotGrowth::clone()
{
	return BaseObjectPtr(new PlotGrowth(*this));
}


void PlotGrowth::writeFields(CFRecord& record)
{
	record << dxPlotGrowth << dyPlotGrowth;
}


void PlotGrowth::readFields(CFRecord& record)
{
	record >> dxPlotGrowth >> dyPlotGrowth;
}

} // namespace XLS

