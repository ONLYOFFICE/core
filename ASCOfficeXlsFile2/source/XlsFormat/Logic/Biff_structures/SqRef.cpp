
#include "SqRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr SqRef::clone()
{
	return BiffStructurePtr(new SqRef(*this));
}


void SqRef::store(CFRecord& record)
{
	unsigned short cref = rgrefs.size();
	record << cref;
	for(std::vector<Ref8U>::iterator it = rgrefs.begin(), itEnd = rgrefs.end(); it != itEnd; ++it)
	{
		it->store(record);
	}
}


void SqRef::load(CFRecord& record)
{
	unsigned short cref;
	record >> cref;
	for(int i = 0; i < cref; ++i)
	{
		Ref8U refu;
		record >> refu;
		rgrefs.push_back(refu);
	}
}


} // namespace XLS

