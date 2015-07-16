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
	unsigned short sdtY = 0x0001;
	unsigned short sdtBSize = 0x0001;
	record << sdtX << sdtY << cValx << cValy << sdtBSize << cValBSize;
}


void Series::readFields(CFRecord& record)
{
	unsigned short sdtY;
	unsigned short sdtBSize;
	record >> sdtX >> sdtY >> cValx >> cValy >> sdtBSize >> cValBSize;
}

} // namespace XLS

