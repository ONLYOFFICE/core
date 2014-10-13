//#include "precompiled_xls.h"
#include "AlRuns.h"

namespace XLS
{;

AlRuns::AlRuns()
{
}


AlRuns::~AlRuns()
{
}


BaseObjectPtr AlRuns::clone()
{
	return BaseObjectPtr(new AlRuns(*this));
}


void AlRuns::writeFields(CFRecord& record)
{
	unsigned __int16 cRuns = rgRuns.size();
	record << cRuns;
	for(BiffStructurePtrVector::iterator it = rgRuns.begin(), itEnd = rgRuns.end(); it != itEnd; ++it)
	{
		(*it)->store(record);
	}
}


void AlRuns::readFields(CFRecord& record)
{
	unsigned __int16 cRuns;
	record >> cRuns;
	for(int i = 0; i < cRuns; ++i)
	{
		FormatRunPtr format(new FormatRun);
		record >> *format;
		rgRuns.push_back(format);
	}
}

} // namespace XLS

