#include "stdafx.h"
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
	WORD cRuns = rgRuns.size();
	record << cRuns;
	for(BiffStructurePtrVector::const_iterator it = rgRuns.begin(), itEnd = rgRuns.end(); it != itEnd; ++it)
	{
		(*it)->store(record);
	}
}


void AlRuns::readFields(CFRecord& record)
{
	WORD cRuns;
	record >> cRuns;
	for(int i = 0; i < cRuns; ++i)
	{
		FormatRunPtr format(new FormatRun);
		record >> *format;
		rgRuns.push_back(format);
	}
}

} // namespace XLS

