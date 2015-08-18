
#include "SerNil.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr SerNil::clone()
{
	return BiffStructurePtr(new SerNil(*this));
}


void SerNil::store(CFRecord& record)
{
    record.storeAnyData(fixed_type);
	record.skipNunBytes(8); // reserved/unused
}


void SerNil::load(CFRecord& record)
{
	record.skipNunBytes(8); // reserved/unused
}

const std::wstring SerNil::toString() const
{
	return L"";
}


} // namespace XLS
