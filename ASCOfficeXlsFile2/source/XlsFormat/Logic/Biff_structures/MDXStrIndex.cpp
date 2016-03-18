
#include "MDXStrIndex.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr MDXStrIndex::clone()
{
	return BiffStructurePtr(new MDXStrIndex(*this));
}


void MDXStrIndex::store(CFRecord& record)
{
	record >> index;
}


void MDXStrIndex::load(CFRecord& record)
{
	record >> index;
}


} // namespace XLS

