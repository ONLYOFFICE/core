
#include "SIIndex.h"

namespace XLS
{

SIIndex::SIIndex()
{
}


SIIndex::~SIIndex()
{
}


BaseObjectPtr SIIndex::clone()
{
	return BaseObjectPtr(new SIIndex(*this));
}


void SIIndex::writeFields(CFRecord& record)
{
	record << numIndex;
}


void SIIndex::readFields(CFRecord& record)
{
	record >> numIndex;
}

} // namespace XLS

