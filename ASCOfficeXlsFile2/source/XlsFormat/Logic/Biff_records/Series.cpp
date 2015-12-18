
#include "Series.h"

namespace XLS
{

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
	record << sdtX << sdtY << cValx << cValy << sdtBSize << cValBSize;
}


void Series::readFields(CFRecord& record)
{
	record >> sdtX >> sdtY >> cValx >> cValy >> sdtBSize >> cValBSize;
}

} // namespace XLS

