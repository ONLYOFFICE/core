
#include "SerAuxTrend.h"

namespace XLS
{

SerAuxTrend::SerAuxTrend()
:	numIntercept(0x0100)
{
}


SerAuxTrend::~SerAuxTrend()
{
}


BaseObjectPtr SerAuxTrend::clone()
{
	return BaseObjectPtr(new SerAuxTrend(*this));
}


void SerAuxTrend::writeFields(CFRecord& record)
{
	record << regt << ordUser << numIntercept << fEquation << fRSquared << numForecast << numBackcast;
}


void SerAuxTrend::readFields(CFRecord& record)
{
	record >> regt >> ordUser >> numIntercept >> fEquation >> fRSquared >> numForecast >> numBackcast;
}

} // namespace XLS

