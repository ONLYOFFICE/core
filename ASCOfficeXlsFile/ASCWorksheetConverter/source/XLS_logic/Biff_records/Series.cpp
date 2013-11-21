#include "stdafx.h"
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
	WORD sdtY = 0x0001;
	WORD sdtBSize = 0x0001;
	record << sdtX << sdtY << cValx << cValy << sdtBSize << cValBSize;
}


void Series::readFields(CFRecord& record)
{
	WORD sdtY;
	WORD sdtBSize;
	record >> sdtX >> sdtY >> cValx >> cValy >> sdtBSize >> cValBSize;
}

} // namespace XLS

