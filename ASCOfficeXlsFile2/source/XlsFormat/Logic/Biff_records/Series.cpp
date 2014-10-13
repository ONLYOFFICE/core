#include "precompiled_xls.h"
#include "Series.h"

namespace XLS
{;

Series::Series()
{
}


Series::~Series()
{
}


BaseObjectPtr Series::clone()
{
	return BaseObjectPtr(new Series(*this));
}


void Series::writeFields(CFRecord& record)
{
	unsigned __int16 sdtY = 0x0001;
	unsigned __int16 sdtBSize = 0x0001;
	record << sdtX << sdtY << cValx << cValy << sdtBSize << cValBSize;
}


void Series::readFields(CFRecord& record)
{
	unsigned __int16 sdtY;
	unsigned __int16 sdtBSize;
	record >> sdtX >> sdtY >> cValx >> cValy >> sdtBSize >> cValBSize;
}

} // namespace XLS

