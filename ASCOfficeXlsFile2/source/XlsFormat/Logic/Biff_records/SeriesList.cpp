
#include "SeriesList.h"

namespace XLS
{

SeriesList::SeriesList()
{
}


SeriesList::~SeriesList()
{
}


BaseObjectPtr SeriesList::clone()
{
	return BaseObjectPtr(new SeriesList(*this));
}


void SeriesList::writeFields(CFRecord& record)
{
	rgiser.store(record);
}


void SeriesList::readFields(CFRecord& record)
{
	rgiser.load(record);
}


BiffStructurePtr RGISeriesListSpecial::clone()
{
	return BiffStructurePtr(new RGISeriesListSpecial(*this));
}



void RGISeriesListSpecial::load(CFRecord& record)
{
	unsigned short cser;
	record >> cser;
	for (size_t i = 0; i < cser; ++i)
	{
		unsigned short rgi;
		record >> rgi;
		series.push_back(rgi);
	}
}


void RGISeriesListSpecial::store(CFRecord& record)
{
	unsigned short cser = series.size();
	record >> cser >> series;
}



} // namespace XLS

