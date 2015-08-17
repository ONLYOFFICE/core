
#include "Chart3d.h"

namespace XLS
{

Chart3d::Chart3d()
{
}


Chart3d::~Chart3d()
{
}


BaseObjectPtr Chart3d::clone()
{
	return BaseObjectPtr(new Chart3d(*this));
}


void Chart3d::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fPerspective);
	SETBIT(flags, 1, fCluster);
	SETBIT(flags, 2, f3DScaling);
	SETBIT(flags, 4, fNotPieChart);
	SETBIT(flags, 5, fWalls2D);
	unsigned short pcHeight_raw = static_cast<unsigned short>(pcHeight);
	record << anRot << anElev << pcDist << pcHeight_raw << pcDepth << pcGap << flags;
}


void Chart3d::readFields(CFRecord& record)
{
	unsigned short flags;
	unsigned short pcHeight_raw;
	record >> anRot >> anElev >> pcDist >> pcHeight_raw >> pcDepth >> pcGap >> flags;
	fPerspective = GETBIT(flags, 0);
	fCluster = GETBIT(flags, 1);
	f3DScaling = GETBIT(flags, 2);
	fNotPieChart = GETBIT(flags, 4);
	fWalls2D = GETBIT(flags, 5);
	pcHeight = fNotPieChart ? static_cast<short>(pcHeight_raw) : pcHeight_raw;
}

} // namespace XLS

