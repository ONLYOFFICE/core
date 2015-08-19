
#include "Run.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Run::clone()
{
	return BiffStructurePtr(new Run(*this));
}

Run::Run()
{		
}

Run::~Run()
{
}
void Run::store(CFRecord& record)
{	
}

void Run::load(CFRecord& record)
{
	formatRun.load(record);
	record.skipNunBytes(4);	// unused1 + unused2
}


} // namespace XLS

