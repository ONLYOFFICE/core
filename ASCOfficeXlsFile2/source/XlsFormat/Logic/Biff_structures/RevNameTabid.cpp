
#include "RevNameTabid.h"
#include <Binary/CFRecord.h>

namespace XLS
{


RevNameTabid::RevNameTabid()
{
}


RevNameTabid::RevNameTabid(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevNameTabid::clone()
{
	return BiffStructurePtr(new RevNameTabid(*this));
}

void RevNameTabid::store(CFRecord& record)
{
	record << tabid << name;
}


void RevNameTabid::load(CFRecord& record)
{
	record >> tabid >> name;
}


const std::wstring RevNameTabid::toString() const
{
	return name.toString();
}

} // namespace XLS

